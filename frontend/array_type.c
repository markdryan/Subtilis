/*
 * Copyright (c) 2019 Mark Ryan
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>
#include <string.h>

#include "../common/error_codes.h"
#include "array_type.h"
#include "builtins_helper.h"
#include "expression.h"
#include "reference_type.h"
#include "type_if.h"

/*
 * Each array variable occupies  a fixed amount of space either on the stack or
 * in global memory.  This memory is laid out as follows.
 *
 *  ----------------------------------
 * | Size in Bytes     |           0 |
 *  ----------------------------------
 * | Pointer to Data   |           4 |
 *  ----------------------------------
 * | Destructor        |           8 |
 *  ----------------------------------
 * | Size of DIM 1     |          12 |
 *  ----------------------------------
 * | Size of DIM n     | 4 * n * 4   |
 *  ----------------------------------
 */

#define SUBTIILIS_ARRAY_SIZE_OFF SUBTIILIS_REFERENCE_SIZE_OFF
#define SUBTIILIS_ARRAY_DATA_OFF SUBTIILIS_REFERENCE_DATA_OFF
#define SUBTIILIS_ARRAY_DESTRUCTOR_OFF SUBTIILIS_REFERENCE_DESTRUCTOR_OFF
#define SUBTIILIS_ARRAY_DIMS_OFF (SUBTIILIS_REFERENCE_DESTRUCTOR_OFF + 4)

size_t subtilis_array_type_size(const subtilis_type_t *type)
{
	size_t size;

	/*
	 * TODO: Size appropriately for 64 bit builds.
	 */

	/*
	 * We need, on 32 bit builds,
	 * 4 bytes for the pointer
	 * 4 bytes for the size
	 * 4 bytes for the destructor
	 * 4 bytes for each dimension unknown at compile time.
	 */

	size = SUBTIILIS_REFERENCE_SIZE + type->params.array.num_dims * 4;

	return size;
}

/* Does not consume elements in e */

void subtilis_array_type_init(subtilis_parser_t *p,
			      const subtilis_type_t *element_type,
			      subtilis_type_t *type, subtilis_exp_t **e,
			      size_t dims, subtilis_error_t *err)
{
	size_t i;

	if (dims > SUBTILIS_MAX_DIMENSIONS) {
		subtilis_error_set_assertion_failed(err);
		return;
	}

	subtilis_type_if_array_of(p, element_type, type, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	type->params.array.num_dims = dims;
	for (i = 0; i < dims; i++) {
		if (e[i]->type.type == SUBTILIS_TYPE_CONST_INTEGER)
			type->params.array.dims[i] = e[i]->exp.ir_op.integer;
		else
			type->params.array.dims[i] = SUBTILIS_DYNAMIC_DIMENSION;
	}
}

void subtlis_array_type_allocate(subtilis_parser_t *p, const char *var_name,
				 subtilis_type_t *type, size_t loc,
				 subtilis_exp_t **e,
				 subtilis_ir_operand_t store_reg,
				 subtilis_error_t *err)
{
	subtilis_ir_operand_t op;
	subtilis_ir_operand_t op1;
	size_t i;
	size_t el_size;
	subtilis_type_t element_type;
	int32_t offset;
	subtilis_exp_t *sizee = NULL;
	subtilis_exp_t *zero = NULL;

	if (loc + subtilis_array_type_size(type) > 0x7fffffff) {
		subtilis_error_set_assertion_failed(err);
		return;
	}

	offset = ((int32_t)loc) + SUBTIILIS_ARRAY_DIMS_OFF;

	sizee = subtilis_array_size_calc(p, var_name, e,
					 type->params.array.num_dims, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	for (i = 0; i < type->params.array.num_dims; i++) {
		op1.integer = offset;
		e[i] = subtilis_type_if_exp_to_var(p, e[i], err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;

		subtilis_ir_section_add_instr_reg(
		    p->current, SUBTILIS_OP_INSTR_STOREO_I32, e[i]->exp.ir_op,
		    store_reg, op1, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;

		offset += sizeof(int32_t);
	}

	zero = subtilis_exp_new_int32(0, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	zero = subtilis_type_if_exp_to_var(p, zero, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	sizee = subtilis_type_if_data_size(p, type, sizee, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	sizee = subtilis_type_if_exp_to_var(p, sizee, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	op.reg = subtilis_reference_type_alloc(p, type, loc, store_reg.reg,
					       sizee->exp.ir_op.reg, true, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	subtilis_type_if_element_type(p, type, &element_type, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	el_size = subtilis_type_if_size(&element_type, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	if ((el_size & 3) == 0)
		subtilis_builtin_memset_i32(p, op.reg, sizee->exp.ir_op.reg,
					    zero->exp.ir_op.reg, err);
	else
		subtilis_builtin_memset_i8(p, op.reg, sizee->exp.ir_op.reg,
					   zero->exp.ir_op.reg, err);

cleanup:

	subtilis_exp_delete(zero);
	subtilis_exp_delete(sizee);
}

static void prv_copy_ref_base(subtilis_parser_t *p, const subtilis_type_t *t,
			      subtilis_ir_operand_t dest_reg,
			      size_t dest_offset,
			      subtilis_ir_operand_t source_reg,
			      size_t source_offset, bool ref,
			      subtilis_error_t *err)
{
	subtilis_ir_operand_t soffset;
	subtilis_ir_operand_t doffset;
	subtilis_ir_operand_t ref_start;
	subtilis_ir_operand_t data;
	size_t i;

	/* TODO: This is platform specific */

	size_t ints_to_copy = 3 + (t->params.array.num_dims);

	soffset.integer = SUBTIILIS_ARRAY_SIZE_OFF + source_offset;
	doffset.integer = SUBTIILIS_ARRAY_SIZE_OFF + dest_offset;

	if (doffset.integer > 0) {
		ref_start.reg = subtilis_ir_section_add_instr(
		    p->current, SUBTILIS_OP_INSTR_ADDI_I32, dest_reg, doffset,
		    err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;
	} else {
		ref_start = dest_reg;
	}

	for (i = 0; i < ints_to_copy; i++) {
		data.reg = subtilis_ir_section_add_instr(
		    p->current, SUBTILIS_OP_INSTR_LOADO_I32, source_reg,
		    soffset, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;
		subtilis_ir_section_add_instr_reg(p->current,
						  SUBTILIS_OP_INSTR_STOREO_I32,
						  data, dest_reg, doffset, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;

		if (ref && (soffset.integer ==
			    source_offset + SUBTIILIS_ARRAY_DATA_OFF)) {
			subtilis_ir_section_add_instr_no_reg(
			    p->current, SUBTILIS_OP_INSTR_REF, data, err);
			if (err->type != SUBTILIS_ERROR_OK)
				return;
		}
		soffset.integer += sizeof(int32_t);
		doffset.integer += sizeof(int32_t);
	}

	/*
	 * It's important that this comes last as the reference we're
	 * copying might be left on the stack of a previous function.
	 * We don't want to overwrite the data before we've copied it.
	 */

	subtilis_ir_section_add_instr_no_reg(
	    p->current, SUBTILIS_OP_INSTR_PUSH_I32, ref_start, err);
}

void subtlis_array_type_copy_param_ref(subtilis_parser_t *p,
				       const subtilis_type_t *t,
				       subtilis_ir_operand_t dest_reg,
				       size_t dest_offset,
				       subtilis_ir_operand_t source_reg,
				       size_t source_offset,
				       subtilis_error_t *err)
{
	prv_copy_ref_base(p, t, dest_reg, dest_offset, source_reg,
			  source_offset, true, err);
}

void subtilis_array_type_create_ref(subtilis_parser_t *p, const char *var_name,
				    const subtilis_symbol_t *s, size_t mem_reg,
				    subtilis_exp_t *e, subtilis_error_t *err)
{
	subtilis_ir_operand_t dest_op;
	subtilis_ir_operand_t source_op;

	subtilis_array_type_match(p, &s->t, &e->type, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	dest_op.reg = mem_reg;
	source_op.reg = e->exp.ir_op.reg;
	prv_copy_ref_base(p, &s->t, dest_op, s->loc, source_op, 0, true, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	subtilis_reference_inc_cleanup_stack(p, &s->t, err);

cleanup:

	subtilis_exp_delete(e);
}

/*
 * Initialise a temporary reference on the caller's stack with the contents
 * of an array variable on the callee's stack.  This is called directly after
 * the callee's ret has executed so his stack is still presevered.
 *
 * TODO: This is a bit risky though.  Might be better, and faster, to allocate
 * space for this variable on the caller's stack.
 */

void subtlis_array_type_copy_ret(subtilis_parser_t *p, const subtilis_type_t *t,
				 size_t dest_reg, size_t source_reg,
				 subtilis_error_t *err)
{
	subtilis_ir_operand_t d;
	subtilis_ir_operand_t s;

	d.reg = dest_reg;
	s.reg = source_reg;

	prv_copy_ref_base(p, t, d, 0, s, 0, false, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_reference_inc_cleanup_stack(p, t, err);
}

/*
 * t1 is the target array.  If it has a dynamic dim that's treated as a match.
 */

void subtilis_array_type_match(subtilis_parser_t *p, const subtilis_type_t *t1,
			       const subtilis_type_t *t2, subtilis_error_t *err)
{
	size_t i;

	if (t1->type != t2->type) {
		subtilis_error_set_array_type_mismatch(err, p->l->stream->name,
						       p->l->line);
		return;
	}

	if (t1->params.array.num_dims != t2->params.array.num_dims) {
		subtilis_error_set_array_type_mismatch(err, p->l->stream->name,
						       p->l->line);
		return;
	}

	for (i = 0; i < t1->params.array.num_dims; i++) {
		if (t1->params.array.dims[i] == -1)
			continue;
		if (t1->params.array.dims[i] != t2->params.array.dims[i]) {
			subtilis_error_set_array_type_mismatch(
			    err, p->l->stream->name, p->l->line);
			return;
		}
	}
}

void subtilis_array_type_assign_ref(subtilis_parser_t *p,
				    const subtilis_type_array_t *dest_type,
				    size_t dest_mem_reg, size_t dest_loc,
				    size_t source_reg, subtilis_error_t *err)
{
	size_t i;
	subtilis_ir_operand_t op0;
	subtilis_ir_operand_t op1;
	subtilis_ir_operand_t op2;
	size_t copy_reg;

	subtilis_reference_type_assign_ref(p, dest_mem_reg, dest_loc,
					   source_reg, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	for (i = 0; i < dest_type->num_dims; i++) {
		if (dest_type->dims[i] != SUBTILIS_DYNAMIC_DIMENSION)
			continue;

		op0.reg = source_reg;
		op1.integer = SUBTIILIS_ARRAY_DIMS_OFF + (i * sizeof(int32_t));
		copy_reg = subtilis_ir_section_add_instr(
		    p->current, SUBTILIS_OP_INSTR_LOADO_I32, op0, op1, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;

		op0.reg = copy_reg;
		op1.reg = dest_mem_reg;
		op2.integer =
		    dest_loc + SUBTIILIS_ARRAY_DIMS_OFF + (i * sizeof(int32_t));
		subtilis_ir_section_add_instr_reg(p->current,
						  SUBTILIS_OP_INSTR_STOREO_I32,
						  op0, op1, op2, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;
	}
}

void subtilis_array_type_assign_to_reg(subtilis_parser_t *p, size_t reg,
				       subtilis_exp_t *e, subtilis_error_t *err)
{
	subtilis_reference_type_assign_to_reg(p, reg, e, false, err);
}

/* Does not consume e.  Does consume  max_dim */

static void prv_check_dynamic_dim(subtilis_parser_t *p, subtilis_exp_t *e,
				  int32_t lower_bound, subtilis_exp_t *max_dim,
				  subtilis_error_t *err)
{
	subtilis_ir_operand_t error_label;
	subtilis_ir_operand_t ok_label;
	subtilis_exp_t *zero = NULL;
	subtilis_exp_t *edup = NULL;
	subtilis_exp_t *maxe = NULL;

	error_label = subtilis_array_type_error_label(p);
	ok_label.label = subtilis_ir_section_new_label(p->current);

	edup = subtilis_type_if_dup(e, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	if (max_dim) {
		maxe = subtilis_type_if_dup(e, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;
	}

	zero = subtilis_exp_new_int32(lower_bound, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	edup = subtilis_type_if_gte(p, edup, zero, err);
	zero = NULL;
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	if (max_dim) {
		maxe = subtilis_type_if_lte(p, maxe, max_dim, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
		max_dim = NULL;
		edup = subtilis_type_if_and(p, edup, maxe, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
		maxe = NULL;
	}

	subtilis_ir_section_add_instr_reg(p->current, SUBTILIS_OP_INSTR_JMPC,
					  edup->exp.ir_op, ok_label,
					  error_label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	subtilis_ir_section_add_label(p->current, ok_label.label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

cleanup:

	subtilis_exp_delete(max_dim);
	subtilis_exp_delete(maxe);
	subtilis_exp_delete(edup);
}

subtilis_exp_t *subtilis_array_size_calc(subtilis_parser_t *p,
					 const char *var_name,
					 subtilis_exp_t **e, size_t index_count,
					 subtilis_error_t *err)
{
	size_t i;
	bool non_const_dim = false;
	subtilis_exp_t *sizee = NULL;
	subtilis_exp_t *edup = NULL;
	subtilis_exp_t *one = NULL;
	int64_t array_size = 1;

	/*
	 * TODO: This whole thing is 32 bit specific.  At least the pointer
	 * needs to be platform specific although we may still keep maximum
	 * array sizes to 32 bits.
	 */

	/* TODO:
	 * I don't see any way to detect whether the array dimensions overflow
	 * 32 bits. For constants we can use 64 bit integers to store the size.
	 * This would then limit us to using 32 bits for the array size, even on
	 * 64 bit builds (unless we resort to instrinsics) as there
	 * are no 128 bit integers in C.  For the ARM2 backend, there is
	 * no way to detect overflow of dynamic vars.  This could be done on
	 * StrongARM using UMULL.  Perhaps we could execute different code on
	 * SA here.  For now, be careful with those array bounds!
	 */

	for (i = 0; i < index_count; i++) {
		if (e[i]->type.type != SUBTILIS_TYPE_CONST_INTEGER) {
			non_const_dim = true;
			continue;
		}
		if (e[i]->exp.ir_op.integer <= 0) {
			subtilis_error_bad_dim(err, var_name,
					       p->l->stream->name, p->l->line);
			return NULL;
		}
		array_size = array_size * (e[i]->exp.ir_op.integer + 1);
	}

	if (array_size > (int64_t)0x7ffffff) {
		subtilis_error_bad_dim(err, var_name, p->l->stream->name,
				       p->l->line);
		return NULL;
	}

	sizee = subtilis_exp_new_int32((int32_t)array_size, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	if (non_const_dim) {
		sizee = subtilis_type_if_exp_to_var(p, sizee, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;

		for (i = 0; i < index_count; i++) {
			if (e[i]->type.type != SUBTILIS_TYPE_CONST_INTEGER) {
				prv_check_dynamic_dim(p, e[i], 0, NULL, err);
				if (err->type != SUBTILIS_ERROR_OK)
					goto cleanup;
				edup = subtilis_type_if_dup(e[i], err);
				if (err->type != SUBTILIS_ERROR_OK)
					goto cleanup;
				one = subtilis_exp_new_int32(1, err);
				if (err->type != SUBTILIS_ERROR_OK)
					goto cleanup;
				edup = subtilis_type_if_add(p, edup, one, err);
				if (err->type != SUBTILIS_ERROR_OK)
					goto cleanup;
				sizee =
				    subtilis_type_if_mul(p, sizee, edup, err);
				if (err->type != SUBTILIS_ERROR_OK)
					goto cleanup;
			}
		}
	}

	return sizee;

cleanup:

	subtilis_exp_delete(sizee);

	return NULL;
}

static subtilis_exp_t *prv_get_dynamic_dim_size(subtilis_parser_t *p,
						int32_t dim_size,
						int32_t offset, size_t mem_reg,
						subtilis_error_t *err)
{
	subtilis_ir_operand_t op0;
	subtilis_ir_operand_t op1;
	size_t reg;
	subtilis_exp_t *maxe = NULL;

	if (dim_size == SUBTILIS_DYNAMIC_DIMENSION) {
		op0.reg = mem_reg;
		op1.integer = offset;
		reg = subtilis_ir_section_add_instr(
		    p->current, SUBTILIS_OP_INSTR_LOADO_I32, op0, op1, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return NULL;
		maxe = subtilis_exp_new_int32_var(reg, err);
	} else {
		maxe = subtilis_exp_new_int32(dim_size, err);
	}

	return maxe;
}

/* Does not consume e. */

static subtilis_exp_t *prv_check_dynamic_index(subtilis_parser_t *p,
					       subtilis_exp_t *e,
					       int32_t dim_size, int32_t offset,
					       size_t mem_reg,
					       subtilis_error_t *err)
{
	subtilis_exp_t *maxe;
	subtilis_exp_t *maxe_dup = NULL;

	maxe = prv_get_dynamic_dim_size(p, dim_size, offset, mem_reg, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	maxe_dup = subtilis_type_if_dup(maxe, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;
	prv_check_dynamic_dim(p, e, 0, maxe_dup, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	return maxe;

cleanup:

	subtilis_exp_delete(maxe);

	return NULL;
}

/*
 * Functions that use arrays will typically contain a lot of error checks and
 * each of these checks can lead to code being called that generates an error.
 * We don't want to duplicate that code for each error, so we create the error
 * generation code once per function when it's first needed and retain the label
 * in the section so it can jumped to when needed.
 */

subtilis_ir_operand_t subtilis_array_type_error_label(subtilis_parser_t *p)
{
	subtilis_ir_operand_t error_label;

	if (p->current->array_access == SIZE_MAX) {
		error_label.label = subtilis_ir_section_new_label(p->current);
		p->current->array_access = error_label.label;
	} else {
		error_label.label = p->current->array_access;
	}

	return error_label;
}

void subtilis_array_gen_index_error_code(subtilis_parser_t *p,
					 subtilis_error_t *err)
{
	subtilis_ir_operand_t error_label;
	subtilis_exp_t *ecode = NULL;

	if (p->current->array_access == SIZE_MAX)
		return;

	error_label.label = p->current->array_access;

	subtilis_ir_section_add_label(p->current, error_label.label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	ecode = subtilis_exp_new_int32(SUBTILIS_ERROR_CODE_BAD_DIM, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_exp_generate_error(p, ecode, err);
}

static void prv_check_last_index(subtilis_parser_t *p, const char *var_name,
				 subtilis_exp_t **e,
				 const subtilis_type_t *type, int32_t offset,
				 size_t mem_reg, subtilis_error_t *err)
{
	int32_t dim_size;
	size_t i = type->params.array.num_dims - 1;
	subtilis_exp_t *maxe = NULL;

	dim_size = type->params.array.dims[i];
	if ((dim_size != SUBTILIS_DYNAMIC_DIMENSION) &&
	    (e[i]->type.type == SUBTILIS_TYPE_CONST_INTEGER)) {
		if ((e[i]->exp.ir_op.integer < 0) ||
		    (e[i]->exp.ir_op.integer > dim_size)) {
			subtilis_error_bad_index(
			    err, var_name, p->l->stream->name, p->l->line);
			return;
		}
	} else {
		maxe = prv_check_dynamic_index(p, e[i], dim_size, offset,
					       mem_reg, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;
		subtilis_exp_delete(maxe);
	}
}

static subtilis_exp_t *prv_compute_element_address(subtilis_parser_t *p,
						   const subtilis_type_t *type,
						   size_t mem_reg, size_t loc,
						   subtilis_exp_t *sizee,
						   subtilis_error_t *err)
{
	subtilis_exp_t *base;

	base =
	    subtilis_type_if_load_from_mem(p, &subtilis_type_integer, mem_reg,
					   loc + SUBTIILIS_ARRAY_DATA_OFF, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	if ((sizee->type.type == SUBTILIS_TYPE_CONST_INTEGER) &&
	    (sizee->exp.ir_op.integer == 0)) {
		subtilis_exp_delete(sizee);
		return base;
	}

	sizee = subtilis_type_if_data_size(p, type, sizee, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	return subtilis_type_if_add(p, base, sizee, err);

cleanup:

	subtilis_exp_delete(base);

	return NULL;
}

static subtilis_exp_t *
prv_1d_index_calc(subtilis_parser_t *p, const char *var_name,
		  const subtilis_type_t *type, size_t mem_reg, size_t loc,
		  subtilis_exp_t **e, size_t index_count, subtilis_error_t *err)
{
	subtilis_exp_t *sizee = NULL;
	int32_t offset = loc + SUBTIILIS_ARRAY_DIMS_OFF;

	prv_check_last_index(p, var_name, e, type, offset, mem_reg, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	sizee = subtilis_type_if_dup(e[0], err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	sizee = subtilis_type_if_exp_to_var(p, sizee, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	return prv_compute_element_address(p, type, mem_reg, loc, sizee, err);
}

static int64_t prv_check_constant_dims(subtilis_parser_t *p, subtilis_exp_t **e,
				       size_t index_count, const char *var_name,
				       const subtilis_type_t *type,
				       int *dyn_start, int32_t *block_size,
				       subtilis_error_t *err)
{
	int i;
	int j;
	int32_t dim_size;
	int64_t prod;
	int32_t bsize = 1;
	int dstart = index_count - 1;
	int64_t array_size = 0;

	for (i = index_count - 1; i >= 0; i--) {
		if (e[i]->type.type != SUBTILIS_TYPE_CONST_INTEGER)
			break;
		dim_size = type->params.array.dims[i];
		if (dim_size == SUBTILIS_DYNAMIC_DIMENSION)
			break;

		if ((e[i]->exp.ir_op.integer < 0) ||
		    (e[i]->exp.ir_op.integer > dim_size)) {
			subtilis_error_bad_index(
			    err, var_name, p->l->stream->name, p->l->line);
			return 0;
		}
		dstart--;
	}

	*dyn_start = dstart;

	/*
	 * i now stores the last dimension (working backwards) where the
	 * index and the dimension are both known at compile time.
	 */

	i++;
	for (j = i; j < index_count; j++) {
		dim_size = type->params.array.dims[j] + 1;
		bsize *= dim_size;
	}

	*block_size = bsize;

	for (; i < index_count - 1; i++) {
		prod = e[i]->exp.ir_op.integer;
		for (j = i + 1; j < index_count; j++) {
			dim_size = type->params.array.dims[j] + 1;
			prod *= dim_size;
		}

		array_size += prod;
	}

	return array_size;
}

subtilis_exp_t *
subtilis_array_index_calc(subtilis_parser_t *p, const char *var_name,
			  const subtilis_type_t *type, size_t mem_reg,
			  size_t loc, subtilis_exp_t **e, size_t index_count,
			  subtilis_error_t *err)
{
	int i;
	int32_t dim_size;
	size_t last_index;
	subtilis_exp_t *one;
	int32_t offset;
	int32_t last_offset;
	int32_t block_size = 0;
	int dyn_start = 0;
	subtilis_exp_t *blocke = NULL;
	subtilis_exp_t *sizee = NULL;
	subtilis_exp_t *edup = NULL;
	subtilis_exp_t *maxe = NULL;
	subtilis_exp_t *blocke_dup = NULL;
	int64_t array_size;

	if (index_count != type->params.array.num_dims) {
		subtilis_error_bad_index_count(err, var_name,
					       p->l->stream->name, p->l->line);
		return NULL;
	}

	if (index_count == 1)
		return prv_1d_index_calc(p, var_name, type, mem_reg, loc, e,
					 index_count, err);

	offset = loc + SUBTIILIS_ARRAY_DIMS_OFF;

	/*
	 * First we compute the constant part of the array indexing.  This can
	 * all be done at compile time if both the indexes and the dimensions
	 * are known to the compiler.  We start with the final index and work
	 * our way toward the first index.  Two sizes are returned here.
	 *
	 * array_size - the offset in elements from the beginning of the
	 * array we know we need to skip.  This does not include the offset
	 * of the last index, which is added later.
	 *
	 * block_size - the cummulative product elements of all the trailing
	 * dimensions that we've processed.
	 * dyn_start - contains the first index that could not be processed
	 * at compile time.  If dyn_start == indxex_count -1 none of the indices
	 * could be processed at compile time.
	 */

	array_size = prv_check_constant_dims(p, e, index_count, var_name, type,
					     &dyn_start, &block_size, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	sizee = subtilis_exp_new_int32((int32_t)array_size, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	sizee = subtilis_type_if_exp_to_var(p, sizee, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	last_index = index_count - 1;
	last_offset = offset + (sizeof(int32_t) * last_index);
	if (dyn_start == index_count - 1) {
		/*
		 * none of the indices could be processed at compile
		 * time, which means we don't know the block_size for
		 * the last element which the loop below needs.  So we
		 * need to compute it.
		 */

		dim_size = type->params.array.dims[last_index];
		maxe = prv_get_dynamic_dim_size(p, dim_size, last_offset,
						mem_reg, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;

		one = subtilis_exp_new_int32(1, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;

		blocke = subtilis_type_if_add(p, maxe, one, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;

		dyn_start--;
	} else {
		blocke = subtilis_exp_new_int32(block_size, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
	}

	offset += (sizeof(int32_t) * dyn_start);

	/* Need to compute  the dynamic portion of the offset array */

	for (i = dyn_start; i >= 0; i--) {
		dim_size = type->params.array.dims[i];

		maxe = prv_check_dynamic_index(p, e[i], dim_size, offset,
					       mem_reg, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
		edup = subtilis_type_if_dup(e[i], err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
		one = subtilis_exp_new_int32(1, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
		maxe = subtilis_type_if_add(p, maxe, one, err);
		one = NULL;
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
		blocke_dup = subtilis_type_if_dup(blocke, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
		edup = subtilis_type_if_mul(p, blocke_dup, edup, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
		blocke = subtilis_type_if_mul(p, blocke, maxe, err);
		maxe = NULL;
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
		sizee = subtilis_type_if_add(p, sizee, edup, err);
		edup = NULL;
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;

		offset -= sizeof(int32_t);
	}

	subtilis_exp_delete(blocke);

	/*
	 * Check and add the offset for the final index.  If it's constant and
	 * the last dimension is constant we'll have already checked it but hey
	 * hum.
	 */

	prv_check_last_index(p, var_name, e, type, last_offset, mem_reg, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	edup = subtilis_type_if_dup(e[index_count - 1], err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	sizee = subtilis_type_if_add(p, edup, sizee, err);
	edup = NULL;
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	return prv_compute_element_address(p, type, mem_reg, loc, sizee, err);

cleanup:

	subtilis_exp_delete(blocke);
	subtilis_exp_delete(maxe);
	subtilis_exp_delete(edup);
	subtilis_exp_delete(sizee);

	return NULL;
}

void subtilis_array_write(subtilis_parser_t *p, const char *var_name,
			  const subtilis_type_t *type,
			  const subtilis_type_t *el_type, size_t mem_reg,
			  size_t loc, subtilis_exp_t *e,
			  subtilis_exp_t **indices, size_t index_count,
			  subtilis_error_t *err)
{
	subtilis_exp_t *offset;

	offset = subtilis_array_index_calc(p, var_name, type, mem_reg, loc,
					   indices, index_count, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	e = subtilis_type_if_coerce_type(p, e, el_type, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	subtilis_type_if_assign_to_mem(p, offset->exp.ir_op.reg, 0, e, err);
	e = NULL;

cleanup:

	subtilis_exp_delete(offset);
	subtilis_exp_delete(e);
}

subtilis_exp_t *subtilis_array_read(subtilis_parser_t *p, const char *var_name,
				    const subtilis_type_t *type,
				    const subtilis_type_t *el_type,
				    size_t mem_reg, size_t loc,
				    subtilis_exp_t **indices,
				    size_t index_count, subtilis_error_t *err)
{
	subtilis_exp_t *offset;
	subtilis_exp_t *e;

	offset = subtilis_array_index_calc(p, var_name, type, mem_reg, loc,
					   indices, index_count, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	e = subtilis_type_if_load_from_mem(p, el_type, offset->exp.ir_op.reg, 0,
					   err);
	subtilis_exp_delete(offset);
	return e;
}

void subtilis_array_add(subtilis_parser_t *p, const char *var_name,
			const subtilis_type_t *type,
			const subtilis_type_t *el_type, size_t mem_reg,
			size_t loc, subtilis_exp_t *e, subtilis_exp_t **indices,
			size_t index_count, subtilis_error_t *err)
{
	subtilis_exp_t *offset;
	subtilis_exp_t *cur_val;

	offset = subtilis_array_index_calc(p, var_name, type, mem_reg, loc,
					   indices, index_count, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	cur_val = subtilis_type_if_load_from_mem(p, el_type,
						 offset->exp.ir_op.reg, 0, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	e = subtilis_type_if_add(p, cur_val, e, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	e = subtilis_type_if_coerce_type(p, e, el_type, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	subtilis_type_if_assign_to_mem(p, offset->exp.ir_op.reg, 0, e, err);

cleanup:
	subtilis_exp_delete(offset);
}

void subtilis_array_sub(subtilis_parser_t *p, const char *var_name,
			const subtilis_type_t *type,
			const subtilis_type_t *el_type, size_t mem_reg,
			size_t loc, subtilis_exp_t *e, subtilis_exp_t **indices,
			size_t index_count, subtilis_error_t *err)
{
	subtilis_exp_t *offset;
	subtilis_exp_t *cur_val;

	offset = subtilis_array_index_calc(p, var_name, type, mem_reg, loc,
					   indices, index_count, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	cur_val = subtilis_type_if_load_from_mem(p, el_type,
						 offset->exp.ir_op.reg, 0, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	e = subtilis_type_if_sub(p, cur_val, e, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	e = subtilis_type_if_coerce_type(p, e, el_type, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	subtilis_type_if_assign_to_mem(p, offset->exp.ir_op.reg, 0, e, err);

cleanup:
	subtilis_exp_delete(offset);
}

static void prv_check_dimension_num(subtilis_parser_t *p, subtilis_exp_t *e,
				    int32_t max_dims, subtilis_error_t *err)
{
	subtilis_exp_t *limit = NULL;

	limit = subtilis_exp_new_int32(max_dims, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	prv_check_dynamic_dim(p, e, 1, limit, err);
}

subtilis_exp_t *subtilis_array_get_dim(subtilis_parser_t *p, subtilis_exp_t *ar,
				       subtilis_exp_t *dim,
				       subtilis_error_t *err)
{
	int32_t requested_dim;
	int32_t dim_index;
	size_t num_dims;
	subtilis_ir_operand_t op;
	subtilis_exp_t *e = NULL;

	/*
	 * This is unfortunate as DIM(a()) may generate one add instruction
	 * for the array reference even though we don't actually need it.
	 * Hopefully, the optimiser will be able to get rid of it.
	 */

	num_dims = ar->type.params.array.num_dims;
	if (!dim) {
		e = subtilis_exp_new_int32(num_dims, err);
	} else {
		if (dim->type.type == SUBTILIS_TYPE_CONST_INTEGER) {
			if ((dim->exp.ir_op.integer <= 0) ||
			    (dim->exp.ir_op.integer > num_dims)) {
				subtilis_error_bad_index(
				    err, "dim", p->l->stream->name, p->l->line);
				goto cleanup;
			}
			dim_index = dim->exp.ir_op.integer - 1;
			requested_dim = ar->type.params.array.dims[dim_index];
			if (requested_dim != SUBTILIS_DYNAMIC_DIMENSION) {
				e = subtilis_exp_new_int32(requested_dim, err);
			} else {
				e = subtilis_type_if_load_from_mem(
				    p, &subtilis_type_integer,
				    ar->exp.ir_op.reg,
				    SUBTIILIS_ARRAY_DIMS_OFF +
					(sizeof(int32_t) * dim_index),
				    err);
			}
		} else {
			prv_check_dimension_num(p, dim, num_dims, err);
			if (err->type != SUBTILIS_ERROR_OK)
				goto cleanup;

			op.integer = 1;
			dim->exp.ir_op.reg = subtilis_ir_section_add_instr(
			    p->current, SUBTILIS_OP_INSTR_SUBI_I32,
			    dim->exp.ir_op, op, err);
			if (err->type != SUBTILIS_ERROR_OK)
				goto cleanup;
			/* TODO This is int32 specific */
			op.integer = 2;
			dim->exp.ir_op.reg = subtilis_ir_section_add_instr(
			    p->current, SUBTILIS_OP_INSTR_LSLI_I32,
			    dim->exp.ir_op, op, err);
			if (err->type != SUBTILIS_ERROR_OK)
				goto cleanup;
			op.reg = subtilis_ir_section_add_instr(
			    p->current, SUBTILIS_OP_INSTR_ADD_I32,
			    ar->exp.ir_op, dim->exp.ir_op, err);
			if (err->type != SUBTILIS_ERROR_OK)
				goto cleanup;
			e = subtilis_type_if_load_from_mem(
			    p, &subtilis_type_integer, op.reg,
			    SUBTIILIS_ARRAY_DIMS_OFF, err);
		}
	}

cleanup:

	subtilis_exp_delete(ar);
	subtilis_exp_delete(dim);

	return e;
}

subtilis_exp_t *subtilis_array_type_dynamic_size(subtilis_parser_t *p,
						 size_t mem_reg, size_t loc,
						 subtilis_error_t *err)
{
	return subtilis_type_if_load_from_mem(
	    p, &subtilis_type_integer, mem_reg, loc + SUBTIILIS_ARRAY_SIZE_OFF,
	    err);
}

void subtilis_array_type_deref_els(subtilis_parser_t *p, size_t data_reg,
				   size_t size_reg, subtilis_error_t *err)
{
	subtilis_ir_operand_t op0;
	subtilis_ir_operand_t op1;
	subtilis_ir_operand_t op2;
	subtilis_ir_operand_t data_end;
	subtilis_ir_operand_t counter;
	subtilis_ir_operand_t conde;
	subtilis_ir_operand_t start;
	subtilis_ir_operand_t end;

	start.label = subtilis_ir_section_new_label(p->current);
	end.label = subtilis_ir_section_new_label(p->current);

	/* size_reg must be > 0 */

	op0.reg = data_reg;
	counter.reg = subtilis_ir_section_add_instr2(
	    p->current, SUBTILIS_OP_INSTR_MOV, op0, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	op1.reg = size_reg;
	data_end.reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_ADD_I32, counter, op1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_label(p->current, start.label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_reference_type_deref(p, counter.reg, 0, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	op2.integer = subtilis_type_if_size(&subtilis_type_string, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_instr_reg(
	    p->current, SUBTILIS_OP_INSTR_ADDI_I32, counter, counter, op2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	conde.reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_LT_I32, counter, data_end, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_instr_reg(p->current, SUBTILIS_OP_INSTR_JMPC,
					  conde, start, end, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_label(p->current, end.label, err);
}

void subtilis_array_type_copy_els(subtilis_parser_t *p,
				  const subtilis_type_t *el_type,
				  size_t data_reg, size_t size_reg,
				  size_t src_reg, subtilis_error_t *err)
{
	subtilis_ir_operand_t op0;
	subtilis_ir_operand_t op1;
	subtilis_ir_operand_t op2;
	subtilis_ir_operand_t data_end;
	subtilis_ir_operand_t counter;
	subtilis_ir_operand_t counter2;
	subtilis_ir_operand_t conde;
	subtilis_ir_operand_t start;
	subtilis_ir_operand_t end;

	start.label = subtilis_ir_section_new_label(p->current);
	end.label = subtilis_ir_section_new_label(p->current);

	/* size_reg must be > 0 */

	op0.reg = data_reg;
	counter.reg = subtilis_ir_section_add_instr2(
	    p->current, SUBTILIS_OP_INSTR_MOV, op0, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	op0.reg = src_reg;
	counter2.reg = subtilis_ir_section_add_instr2(
	    p->current, SUBTILIS_OP_INSTR_MOV, op0, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	op1.reg = size_reg;
	data_end.reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_ADD_I32, counter, op1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_label(p->current, start.label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_reference_type_deref(p, counter.reg, 0, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_reference_type_init_ref(p, counter.reg, 0, counter2.reg, true,
					 err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	op2.integer = subtilis_type_if_size(el_type, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_instr_reg(
	    p->current, SUBTILIS_OP_INSTR_ADDI_I32, counter, counter, op2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_instr_reg(p->current,
					  SUBTILIS_OP_INSTR_ADDI_I32, counter2,
					  counter2, op2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	conde.reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_LT_I32, counter, data_end, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_instr_reg(p->current, SUBTILIS_OP_INSTR_JMPC,
					  conde, start, end, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_label(p->current, end.label, err);
}
