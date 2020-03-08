/*
 * Copyright (c) 2020 Mark Ryan
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

#include "expression.h"
#include "reference_type.h"

void subtilis_reference_type_init_ref(subtilis_parser_t *p, size_t dest_mem_reg,
				      size_t dest_loc, size_t source_reg,
				      bool check_size, subtilis_error_t *err)
{
	subtilis_ir_operand_t op0;
	subtilis_ir_operand_t op1;
	subtilis_ir_operand_t op2;
	subtilis_ir_operand_t copy;
	subtilis_ir_operand_t zero;
	subtilis_ir_operand_t gtzero;

	op0.reg = source_reg;

	op1.integer = SUBTIILIS_REFERENCE_SIZE_OFF;
	copy.reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_LOADO_I32, op0, op1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	op1.reg = dest_mem_reg;
	op2.integer = dest_loc + SUBTIILIS_REFERENCE_SIZE_OFF;
	subtilis_ir_section_add_instr_reg(
	    p->current, SUBTILIS_OP_INSTR_STOREO_I32, copy, op1, op2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	if (check_size) {
		zero.label = subtilis_ir_section_new_label(p->current);
		gtzero.label = subtilis_ir_section_new_label(p->current);
		subtilis_ir_section_add_instr_reg(p->current,
						  SUBTILIS_OP_INSTR_JMPC, copy,
						  gtzero, zero, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;
		subtilis_ir_section_add_label(p->current, gtzero.label, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;
	}

	op1.integer = SUBTIILIS_REFERENCE_DATA_OFF;
	copy.reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_LOADO_I32, op0, op1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_instr_no_reg(p->current, SUBTILIS_OP_INSTR_REF,
					     copy, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	op1.reg = dest_mem_reg;
	op2.integer = dest_loc + SUBTIILIS_REFERENCE_DATA_OFF;
	subtilis_ir_section_add_instr_reg(
	    p->current, SUBTILIS_OP_INSTR_STOREO_I32, copy, op1, op2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;
	if (check_size)
		subtilis_ir_section_add_label(p->current, zero.label, err);
}

void subtilis_reference_type_push_reference(subtilis_parser_t *p, size_t reg,
					    size_t loc, subtilis_error_t *err)
{
	subtilis_ir_operand_t store_op;
	subtilis_ir_operand_t op2;

	store_op.reg = reg;

	if (loc > 0) {
		op2.integer = loc;
		op2.reg = subtilis_ir_section_add_instr(
		    p->current, SUBTILIS_OP_INSTR_ADDI_I32, store_op, op2, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;
	} else {
		op2.reg = reg;
	}

	subtilis_ir_section_add_instr_no_reg(
	    p->current, SUBTILIS_OP_INSTR_PUSH_I32, op2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_reference_inc_cleanup_stack(p, err);
}

void subtilis_reference_type_new_ref(subtilis_parser_t *p, size_t dest_mem_reg,
				     size_t dest_loc, size_t source_reg,
				     bool check_size, subtilis_error_t *err)
{
	subtilis_reference_type_init_ref(p, dest_mem_reg, dest_loc, source_reg,
					 check_size, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_reference_type_push_reference(p, dest_mem_reg, dest_loc, err);
}

void subtilis_reference_type_assign_ref(subtilis_parser_t *p,
					size_t dest_mem_reg, size_t dest_loc,
					size_t source_reg,
					subtilis_error_t *err)
{
	subtilis_ir_operand_t op0;
	subtilis_ir_operand_t op1;
	subtilis_ir_operand_t copy;

	op0.reg = dest_mem_reg;
	op1.integer = dest_loc + SUBTIILIS_REFERENCE_DATA_OFF;

	copy.reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_LOADO_I32, op0, op1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_instr_no_reg(
	    p->current, SUBTILIS_OP_INSTR_DEREF, copy, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_reference_type_init_ref(p, dest_mem_reg, dest_loc, source_reg,
					 false, err);
}

size_t subtilis_reference_get_pointer(subtilis_parser_t *p, size_t reg,
				      size_t offset, subtilis_error_t *err)
{
	subtilis_ir_operand_t op0;
	subtilis_ir_operand_t op1;
	size_t dest;

	if (offset == 0)
		return reg;

	op0.reg = reg;
	op1.integer = (int32_t)offset;
	dest = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_ADDI_I32, op0, op1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return SIZE_MAX;

	return dest;
}

void subtilis_reference_type_memcpy(subtilis_parser_t *p, size_t mem_reg,
				    size_t loc, size_t src_reg, size_t size_reg,
				    subtilis_error_t *err)
{
	size_t dest_reg;
	subtilis_ir_operand_t op0;
	subtilis_ir_operand_t op1;

	op0.reg = mem_reg;
	op1.integer = loc + SUBTIILIS_REFERENCE_DATA_OFF;

	dest_reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_LOADO_I32, op0, op1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_reference_type_memcpy_dest(p, dest_reg, src_reg, size_reg,
					    err);
}

void subtilis_reference_type_memcpy_dest(subtilis_parser_t *p, size_t dest_reg,
					 size_t src_reg, size_t size_reg,
					 subtilis_error_t *err)
{
	subtilis_ir_arg_t *args;
	char *name = NULL;
	static const char memcpy[] = "_memcpy";

	name = malloc(sizeof(memcpy));
	if (!name) {
		subtilis_error_set_oom(err);
		return;
	}
	strcpy(name, memcpy);

	args = malloc(sizeof(*args) * 3);
	if (!args) {
		free(name);
		subtilis_error_set_oom(err);
		return;
	}

	args[0].type = SUBTILIS_IR_REG_TYPE_INTEGER;
	args[0].reg = dest_reg;
	args[1].type = SUBTILIS_IR_REG_TYPE_INTEGER;
	args[1].reg = src_reg;
	args[2].type = SUBTILIS_IR_REG_TYPE_INTEGER;
	args[2].reg = size_reg;

	(void)subtilis_exp_add_call(p, name, SUBTILIS_BUILTINS_MEMCPY, NULL,
				    args, &subtilis_type_void, 3, err);
}

static void prv_ensure_cleanup_stack(subtilis_parser_t *p,
				     subtilis_error_t *err)
{
	subtilis_ir_inst_t *instr;

	/*
	 * This piece is a bit weird.  At the start of the procedure we insert
	 * a NOP.  If we encounter at least one array (local or global) we
	 * replace this nop with a mov instruction that initialises the
	 * cleanup_stack counter to 0.  If there are no arrays delcared in the
	 * procedure, the nop will be removed when we generate code for the
	 * target architecture.
	 */

	if (p->current->cleanup_stack == SIZE_MAX) {
		instr =
		    &p->current->ops[p->current->cleanup_stack_nop]->op.instr;
		p->current->cleanup_stack = p->current->cleanup_stack_reg;
		instr->type = SUBTILIS_OP_INSTR_MOVI_I32;
		instr->operands[0].reg = p->current->cleanup_stack;
		instr->operands[1].integer = 0;
	}
}

void subtilis_reference_inc_cleanup_stack(subtilis_parser_t *p,
					  subtilis_error_t *err)
{
	subtilis_ir_operand_t op2;
	subtilis_ir_operand_t dest;

	prv_ensure_cleanup_stack(p, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	/*
	 * TODO: The part of the register allocator that preserves
	 * live registers between basic blocks can't handle the case
	 * when an instruction uses the same register for both source
	 * and destination.  Ultimately, all that code is going to
	 * dissapear when we have a global register allocator, so for
	 * now we're just going to live with it.
	 */

	op2.integer = 1;
	dest.reg = p->current->cleanup_stack;
	op2.reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_ADDI_I32, dest, op2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_instr_no_reg2(p->current, SUBTILIS_OP_INSTR_MOV,
					      dest, op2, err);
}

size_t subtilis_reference_type_alloc(subtilis_parser_t *p, size_t loc,
				     size_t store_reg, size_t size_reg,
				     bool push, subtilis_error_t *err)
{
	subtilis_ir_operand_t op;
	subtilis_ir_operand_t op1;
	subtilis_ir_operand_t size_op;
	subtilis_ir_operand_t store_op;

	size_op.reg = size_reg;
	store_op.reg = store_reg;

	op1.integer = loc + SUBTIILIS_REFERENCE_SIZE_OFF;
	subtilis_ir_section_add_instr_reg(p->current,
					  SUBTILIS_OP_INSTR_STOREO_I32, size_op,
					  store_op, op1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return SIZE_MAX;

	op1.integer = loc + SUBTIILIS_REFERENCE_DATA_OFF;
	op.reg = subtilis_ir_section_add_instr2(
	    p->current, SUBTILIS_OP_INSTR_ALLOC, size_op, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return SIZE_MAX;

	subtilis_exp_handle_errors(p, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return SIZE_MAX;

	if (push) {
		subtilis_reference_type_push_reference(p, store_reg, loc, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return SIZE_MAX;
	}

	subtilis_ir_section_add_instr_reg(
	    p->current, SUBTILIS_OP_INSTR_STOREO_I32, op, store_op, op1, err);

	return op.reg;
}

void subtilis_reference_type_pop_and_deref(subtilis_parser_t *p,
					   subtilis_error_t *err)
{
	size_t reg;
	subtilis_ir_operand_t op2;
	subtilis_ir_operand_t dest;

	reg = subtilis_ir_section_add_instr1(p->current,
					     SUBTILIS_OP_INSTR_POP_I32, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_reference_type_deref(p, reg, 0, true, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	if (p->current->cleanup_stack == SIZE_MAX) {
		subtilis_error_set_assertion_failed(err);
		return;
	}

	/*
	 * TODO: The part of the register allocator that preserves
	 * live registers between basic blocks can't handle the case
	 * when an instruction uses the same register for both source
	 * and destination.  Ultimately, all that code is going to dissapear
	 * when we have a global register allocator, so for now we're just
	 * going to live with it.
	 */

	op2.integer = 1;
	dest.reg = p->current->cleanup_stack;
	op2.reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_SUBI_I32, dest, op2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_instr_no_reg2(p->current, SUBTILIS_OP_INSTR_MOV,
					      dest, op2, err);
}

void subtilis_reference_type_deref(subtilis_parser_t *p, size_t mem_reg,
				   size_t loc, bool check,
				   subtilis_error_t *err)
{
	subtilis_ir_operand_t offset;
	subtilis_ir_operand_t size;
	subtilis_ir_operand_t op2;
	subtilis_ir_operand_t non_zero;
	subtilis_ir_operand_t zero;

	non_zero.label = subtilis_ir_section_new_label(p->current);
	zero.label = subtilis_ir_section_new_label(p->current);

	offset.reg = mem_reg;
	op2.integer = SUBTIILIS_REFERENCE_SIZE_OFF + loc;
	size.reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_LOADO_I32, offset, op2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_instr_reg(p->current, SUBTILIS_OP_INSTR_JMPC,
					  size, non_zero, zero, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;
	subtilis_ir_section_add_label(p->current, non_zero.label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	op2.integer = loc + SUBTIILIS_REFERENCE_DATA_OFF;
	offset.reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_LOADO_I32, offset, op2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_ir_section_add_instr_no_reg(
	    p->current, SUBTILIS_OP_INSTR_DEREF, offset, err);

	subtilis_ir_section_add_label(p->current, zero.label, err);
}
