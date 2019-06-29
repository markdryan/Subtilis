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

#include "float64_type.h"

static subtilis_exp_t *prv_returne(subtilis_parser_t *p, subtilis_exp_t *e,
				   subtilis_error_t *err)
{
	return e;
}

static subtilis_exp_t *prv_exp_to_var_const(subtilis_parser_t *p,
					    subtilis_exp_t *e,
					    subtilis_error_t *err)
{
	size_t reg;
	subtilis_exp_t *e2;

	reg = subtilis_ir_section_add_instr2(
	    p->current, SUBTILIS_OP_INSTR_MOVI_REAL, e->exp.ir_op, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto on_error;
	e2 = subtilis_exp_new_var(SUBTILIS_TYPE_REAL, reg, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto on_error;
	subtilis_exp_delete(e);

	return e2;

on_error:
	subtilis_exp_delete(e);
	return NULL;
}

static void prv_dup(subtilis_exp_t *e1, subtilis_exp_t *e2,
		    subtilis_error_t *err)
{
	e2->exp.ir_op = e1->exp.ir_op;
}

static subtilis_exp_t *prv_to_int32_const(subtilis_parser_t *p,
					  subtilis_exp_t *e,
					  subtilis_error_t *err)
{
	e->type = SUBTILIS_TYPE_CONST_INTEGER;
	e->exp.ir_op.integer = (int32_t)e->exp.ir_op.real;
	return e;
}

static subtilis_exp_t *prv_unary_minus_const(subtilis_parser_t *p,
					     subtilis_exp_t *e,
					     subtilis_error_t *err)
{
	e->exp.ir_op.real = -e->exp.ir_op.real;
	return e;
}

static subtilis_exp_t *prv_add_const(subtilis_parser_t *p, subtilis_exp_t *a1,
				     subtilis_exp_t *a2, subtilis_error_t *err)
{
	/* a2 must be const */

	switch (a2->type) {
	case SUBTILIS_TYPE_CONST_INTEGER:
		a1->exp.ir_op.real =
		    ((double)a2->exp.ir_op.integer) + a1->exp.ir_op.real;
		a1->type = SUBTILIS_TYPE_CONST_REAL;
		break;
	case SUBTILIS_TYPE_CONST_REAL:
		a1->exp.ir_op.real += a2->exp.ir_op.real;
		break;
	default:
		subtilis_exp_delete(a1);
		subtilis_exp_delete(a2);
		subtilis_error_set_bad_expression(err, p->l->stream->name,
						  p->l->line);
		return NULL;
	}

	subtilis_exp_delete(a2);
	return a1;
}

static subtilis_exp_t *prv_mul_const(subtilis_parser_t *p, subtilis_exp_t *a1,
				     subtilis_exp_t *a2, subtilis_error_t *err)
{
	/* a2 must be const */

	switch (a2->type) {
	case SUBTILIS_TYPE_CONST_INTEGER:
		a1->exp.ir_op.real =
		    ((double)a2->exp.ir_op.integer) * a1->exp.ir_op.real;
		a1->type = SUBTILIS_TYPE_CONST_REAL;
		break;
	case SUBTILIS_TYPE_CONST_REAL:
		a1->exp.ir_op.real *= a2->exp.ir_op.real;
		break;
	default:
		subtilis_exp_delete(a1);
		subtilis_exp_delete(a2);
		subtilis_error_set_bad_expression(err, p->l->stream->name,
						  p->l->line);
		return NULL;
	}

	subtilis_exp_delete(a2);
	return a1;
}

static void prv_args_to_int(subtilis_parser_t *p, subtilis_exp_t *a1,
			    subtilis_exp_t *a2, subtilis_error_t *err)
{
	a1 = subtilis_type_if_to_int(p, a1, err);
	if (err->type != SUBTILIS_ERROR_OK) {
		subtilis_exp_delete(a2);
		return;
	}

	a2 = subtilis_type_if_to_int(p, a2, err);
	if (err->type != SUBTILIS_ERROR_OK) {
		subtilis_exp_delete(a1);
		return;
	}
}

static subtilis_exp_t *prv_and_const(subtilis_parser_t *p, subtilis_exp_t *a1,
				     subtilis_exp_t *a2, subtilis_error_t *err)
{
	/* a2 must be const */

	prv_args_to_int(p, a1, a2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	a1->exp.ir_op.integer &= a2->exp.ir_op.integer;
	subtilis_exp_delete(a2);
	return a1;
}

static subtilis_exp_t *prv_or_const(subtilis_parser_t *p, subtilis_exp_t *a1,
				    subtilis_exp_t *a2, subtilis_error_t *err)
{
	/* a2 must be const */

	prv_args_to_int(p, a1, a2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	a1->exp.ir_op.integer |= a2->exp.ir_op.integer;
	subtilis_exp_delete(a2);
	return a1;
}

static subtilis_exp_t *prv_eor_const(subtilis_parser_t *p, subtilis_exp_t *a1,
				     subtilis_exp_t *a2, subtilis_error_t *err)
{
	/* a2 must be const */

	prv_args_to_int(p, a1, a2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	a1->exp.ir_op.integer ^= a2->exp.ir_op.integer;
	subtilis_exp_delete(a2);
	return a1;
}

static subtilis_exp_t *prv_not_const(subtilis_parser_t *p, subtilis_exp_t *e,
				     subtilis_error_t *err)
{
	e->type = SUBTILIS_TYPE_CONST_INTEGER;
	e->exp.ir_op.integer = ~(int32_t)e->exp.ir_op.real;
	return e;
}

static subtilis_exp_t *prv_eq_const(subtilis_parser_t *p, subtilis_exp_t *a1,
				    subtilis_exp_t *a2, subtilis_error_t *err)
{
	/* a2 must be const */

	switch (a2->type) {
	case SUBTILIS_TYPE_CONST_INTEGER:
		a1->exp.ir_op.integer =
		    ((double)a1->exp.ir_op.integer) == a2->exp.ir_op.real ? -1
									  : 0;
		a1->type = SUBTILIS_TYPE_CONST_INTEGER;
		break;
	case SUBTILIS_TYPE_CONST_REAL:
		a1->exp.ir_op.integer =
		    a1->exp.ir_op.real == a2->exp.ir_op.real ? -1 : 0;
		a1->type = SUBTILIS_TYPE_CONST_INTEGER;
		break;
	default:
		subtilis_exp_delete(a1);
		subtilis_exp_delete(a2);
		subtilis_error_set_bad_expression(err, p->l->stream->name,
						  p->l->line);
		return NULL;
	}

	subtilis_exp_delete(a2);
	return a1;
}

static subtilis_exp_t *prv_neq_const(subtilis_parser_t *p, subtilis_exp_t *a1,
				     subtilis_exp_t *a2, subtilis_error_t *err)
{

	/* a2 must be const */

	switch (a2->type) {
	case SUBTILIS_TYPE_CONST_INTEGER:
		a1->exp.ir_op.integer =
		    ((double)a1->exp.ir_op.integer) != a2->exp.ir_op.real ? -1
									  : 0;
		a1->type = SUBTILIS_TYPE_CONST_INTEGER;
		break;
	case SUBTILIS_TYPE_CONST_REAL:
		a1->exp.ir_op.integer =
		    a1->exp.ir_op.real != a2->exp.ir_op.real ? -1 : 0;
		a1->type = SUBTILIS_TYPE_CONST_INTEGER;
		break;
	default:
		subtilis_exp_delete(a1);
		subtilis_exp_delete(a2);
		subtilis_error_set_bad_expression(err, p->l->stream->name,
						  p->l->line);
		return NULL;
	}

	subtilis_exp_delete(a2);
	return a1;
}

static subtilis_exp_t *prv_sub_const(subtilis_parser_t *p, subtilis_exp_t *a1,
				     subtilis_exp_t *a2, bool swapped,
				     subtilis_error_t *err)
{
	/* a2 must be const */

	switch (a2->type) {
	case SUBTILIS_TYPE_CONST_INTEGER:
		a1->exp.ir_op.real =
		    a1->exp.ir_op.real - ((double)a2->exp.ir_op.integer);
		break;
	case SUBTILIS_TYPE_CONST_REAL:
		a1->exp.ir_op.real -= a2->exp.ir_op.real;
		break;
	default:
		subtilis_exp_delete(a1);
		subtilis_exp_delete(a2);
		subtilis_error_set_bad_expression(err, p->l->stream->name,
						  p->l->line);
		return NULL;
	}

	subtilis_exp_delete(a2);
	return a1;
}

static subtilis_exp_t *prv_divr_const(subtilis_parser_t *p, subtilis_exp_t *a1,
				      subtilis_exp_t *a2, bool swapped,
				      subtilis_error_t *err)
{
	/* a2 must be const and a float64*/

	a1->exp.ir_op.real /= a2->exp.ir_op.real;
	subtilis_exp_delete(a2);
	return a1;
}

static subtilis_exp_t *prv_gt_const(subtilis_parser_t *p, subtilis_exp_t *a1,
				    subtilis_exp_t *a2, bool swapped,
				    subtilis_error_t *err)
{
	/* a2 must be const */

	switch (a2->type) {
	case SUBTILIS_TYPE_CONST_INTEGER:
		a1->exp.ir_op.integer =
		    a1->exp.ir_op.real > ((double)a2->exp.ir_op.integer) ? -1
									 : 0;
		a1->type = SUBTILIS_TYPE_CONST_INTEGER;
		break;
	case SUBTILIS_TYPE_CONST_REAL:
		a1->exp.ir_op.integer =
		    a1->exp.ir_op.real > a2->exp.ir_op.real ? -1 : 0;
		a1->type = SUBTILIS_TYPE_CONST_INTEGER;
		break;
	default:
		subtilis_exp_delete(a1);
		subtilis_exp_delete(a2);
		subtilis_error_set_bad_expression(err, p->l->stream->name,
						  p->l->line);
		return NULL;
	}

	subtilis_exp_delete(a2);
	return a1;
}

static subtilis_exp_t *prv_lte_const(subtilis_parser_t *p, subtilis_exp_t *a1,
				     subtilis_exp_t *a2, bool swapped,
				     subtilis_error_t *err)
{
	/* a2 must be const */

	switch (a2->type) {
	case SUBTILIS_TYPE_CONST_INTEGER:
		a1->exp.ir_op.integer =
		    a1->exp.ir_op.real <= ((double)a2->exp.ir_op.integer) ? -1
									  : 0;
		a1->type = SUBTILIS_TYPE_CONST_INTEGER;
		break;
	case SUBTILIS_TYPE_CONST_REAL:
		a1->exp.ir_op.integer =
		    a1->exp.ir_op.real <= a2->exp.ir_op.real ? -1 : 0;
		a1->type = SUBTILIS_TYPE_CONST_INTEGER;
		break;
	default:
		subtilis_exp_delete(a1);
		subtilis_exp_delete(a2);
		subtilis_error_set_bad_expression(err, p->l->stream->name,
						  p->l->line);
		return NULL;
	}

	subtilis_exp_delete(a2);
	return a1;
}

static subtilis_exp_t *prv_lt_const(subtilis_parser_t *p, subtilis_exp_t *a1,
				    subtilis_exp_t *a2, bool swapped,
				    subtilis_error_t *err)
{
	/* a2 must be const */

	switch (a2->type) {
	case SUBTILIS_TYPE_CONST_INTEGER:
		a1->exp.ir_op.integer =
		    a1->exp.ir_op.real < ((double)a2->exp.ir_op.integer) ? -1
									 : 0;
		a1->type = SUBTILIS_TYPE_CONST_INTEGER;
		break;
	case SUBTILIS_TYPE_CONST_REAL:
		a1->exp.ir_op.integer =
		    a1->exp.ir_op.real < a2->exp.ir_op.real ? -1 : 0;
		a1->type = SUBTILIS_TYPE_CONST_INTEGER;
		break;
	default:
		subtilis_exp_delete(a1);
		subtilis_exp_delete(a2);
		subtilis_error_set_bad_expression(err, p->l->stream->name,
						  p->l->line);
		return NULL;
	}

	subtilis_exp_delete(a2);
	return a1;
}

static subtilis_exp_t *prv_gte_const(subtilis_parser_t *p, subtilis_exp_t *a1,
				     subtilis_exp_t *a2, bool swapped,
				     subtilis_error_t *err)
{
	/* a2 must be const */

	switch (a2->type) {
	case SUBTILIS_TYPE_CONST_INTEGER:
		a1->exp.ir_op.integer =
		    a1->exp.ir_op.real >= ((double)a2->exp.ir_op.integer) ? -1
									  : 0;
		a1->type = SUBTILIS_TYPE_CONST_INTEGER;
		break;
	case SUBTILIS_TYPE_CONST_REAL:
		a1->exp.ir_op.integer =
		    a1->exp.ir_op.real >= a2->exp.ir_op.real ? -1 : 0;
		a1->type = SUBTILIS_TYPE_CONST_INTEGER;
		break;
	default:
		subtilis_exp_delete(a1);
		subtilis_exp_delete(a2);
		subtilis_error_set_bad_expression(err, p->l->stream->name,
						  p->l->line);
		return NULL;
	}

	subtilis_exp_delete(a2);
	return a1;
}

subtilis_type_if subtilis_type_const_float64 = {
    .is_const = true,
    .exp_to_var = prv_exp_to_var_const,
    .copy_var = NULL,
    .dup = prv_dup,
    .to_int32 = prv_to_int32_const,
    .to_float64 = prv_returne,
    .unary_minus = prv_unary_minus_const,
    .add = prv_add_const,
    .mul = prv_mul_const,
    .and = prv_and_const,
    .or = prv_or_const,
    .eor = prv_eor_const,
    .not = prv_not_const,
    .eq = prv_eq_const,
    .neq = prv_neq_const,
    .sub = prv_sub_const,
    .div = NULL,
    .mod = NULL,
    .divr = prv_divr_const,
    .gt = prv_gt_const,
    .lte = prv_lte_const,
    .lt = prv_lt_const,
    .gte = prv_gte_const,
    .lsl = NULL,
    .lsr = NULL,
    .asr = NULL,
};

static subtilis_exp_t *prv_copy_var(subtilis_parser_t *p, subtilis_exp_t *e,
				    subtilis_error_t *err)
{
	size_t reg;
	subtilis_exp_t *e2;

	reg = subtilis_ir_section_add_instr2(
	    p->current, SUBTILIS_OP_INSTR_MOVFP, e->exp.ir_op, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto on_error;
	e2 = subtilis_exp_new_var(SUBTILIS_TYPE_REAL, reg, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto on_error;
	subtilis_exp_delete(e);
	return e2;

on_error:
	subtilis_exp_delete(e);
	return NULL;
}

static subtilis_exp_t *prv_to_int32(subtilis_parser_t *p, subtilis_exp_t *e,
				    subtilis_error_t *err)
{
	size_t reg;

	reg = subtilis_ir_section_add_instr2(
	    p->current, SUBTILIS_OP_INSTR_MOV_FP_I32, e->exp.ir_op, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto on_error;
	e->type = SUBTILIS_TYPE_INTEGER;
	e->exp.ir_op.reg = reg;

	return e;

on_error:
	subtilis_exp_delete(e);
	return NULL;
}

static subtilis_exp_t *prv_unary_minus(subtilis_parser_t *p, subtilis_exp_t *e,
				       subtilis_error_t *err)
{
	size_t reg;
	subtilis_ir_operand_t operand;

	operand.real = 0;
	reg = subtilis_ir_section_add_instr(p->current,
					    SUBTILIS_OP_INSTR_RSUBI_REAL,
					    e->exp.ir_op, operand, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto on_error;
	e->exp.ir_op.reg = reg;

	return e;

on_error:

	subtilis_exp_delete(e);
	return NULL;
}

static subtilis_exp_t *prv_commutative(subtilis_parser_t *p, subtilis_exp_t *a1,
				       subtilis_exp_t *a2,
				       subtilis_op_instr_type_t real_var_imm,
				       subtilis_op_instr_type_t real_var_var,
				       subtilis_error_t *err)
{
	size_t reg;

	switch (a2->type) {
	case SUBTILIS_TYPE_CONST_INTEGER:
		a2->exp.ir_op.real = (double)a2->exp.ir_op.integer;
	case SUBTILIS_TYPE_CONST_REAL:
		reg = subtilis_ir_section_add_instr(p->current, real_var_imm,
						    a1->exp.ir_op,
						    a2->exp.ir_op, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto on_error;
		a1->exp.ir_op.reg = reg;
		break;
	case SUBTILIS_TYPE_INTEGER:
		reg = subtilis_ir_section_add_instr2(
		    p->current, SUBTILIS_OP_INSTR_MOV_I32_FP, a2->exp.ir_op,
		    err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto on_error;
		a2->type = SUBTILIS_TYPE_REAL;
		a2->exp.ir_op.reg = reg;
	case SUBTILIS_TYPE_REAL:
		reg = subtilis_ir_section_add_instr(p->current, real_var_var,
						    a1->exp.ir_op,
						    a2->exp.ir_op, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto on_error;
		a1->exp.ir_op.reg = reg;
		break;
	default:
		subtilis_error_set_bad_expression(err, p->l->stream->name,
						  p->l->line);
		break;
	}

	subtilis_exp_delete(a2);
	return a1;

on_error:
	subtilis_exp_delete(a1);
	subtilis_exp_delete(a2);
	return NULL;
}

static subtilis_exp_t *prv_commutative_logical(
    subtilis_parser_t *p, subtilis_exp_t *a1, subtilis_exp_t *a2,
    subtilis_op_instr_type_t real_var_imm,
    subtilis_op_instr_type_t real_var_var, subtilis_error_t *err)
{
	size_t reg;

	switch (a2->type) {
	case SUBTILIS_TYPE_CONST_INTEGER:
		a2->exp.ir_op.real = (double)a2->exp.ir_op.integer;
	case SUBTILIS_TYPE_CONST_REAL:
		reg = subtilis_ir_section_add_instr(p->current, real_var_imm,
						    a1->exp.ir_op,
						    a2->exp.ir_op, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto on_error;
		a1->exp.ir_op.reg = reg;
		a1->type = SUBTILIS_TYPE_INTEGER;
		break;
	case SUBTILIS_TYPE_INTEGER:
		reg = subtilis_ir_section_add_instr2(
		    p->current, SUBTILIS_OP_INSTR_MOV_I32_FP, a2->exp.ir_op,
		    err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto on_error;
		a2->exp.ir_op.reg = reg;
	case SUBTILIS_TYPE_REAL:
		reg = subtilis_ir_section_add_instr(p->current, real_var_var,
						    a1->exp.ir_op,
						    a2->exp.ir_op, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto on_error;
		a1->exp.ir_op.reg = reg;
		a1->type = SUBTILIS_TYPE_INTEGER;
		break;
	default:
		subtilis_error_set_bad_expression(err, p->l->stream->name,
						  p->l->line);
		break;
	}

	subtilis_exp_delete(a2);
	return a1;

on_error:
	subtilis_exp_delete(a1);
	subtilis_exp_delete(a2);
	return NULL;
}

static subtilis_exp_t *prv_add(subtilis_parser_t *p, subtilis_exp_t *a1,
			       subtilis_exp_t *a2, subtilis_error_t *err)
{
	return prv_commutative(p, a1, a2, SUBTILIS_OP_INSTR_ADDI_REAL,
			       SUBTILIS_OP_INSTR_ADD_REAL, err);
}

static subtilis_exp_t *prv_mul(subtilis_parser_t *p, subtilis_exp_t *a1,
			       subtilis_exp_t *a2, subtilis_error_t *err)
{
	return prv_commutative(p, a1, a2, SUBTILIS_OP_INSTR_MULI_REAL,
			       SUBTILIS_OP_INSTR_MUL_REAL, err);
}

static subtilis_exp_t *prv_and(subtilis_parser_t *p, subtilis_exp_t *a1,
			       subtilis_exp_t *a2, subtilis_error_t *err)
{
	prv_args_to_int(p, a1, a2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	return subtilis_type_if_and(p, a1, a2, err);
}

static subtilis_exp_t *prv_or(subtilis_parser_t *p, subtilis_exp_t *a1,
			      subtilis_exp_t *a2, subtilis_error_t *err)
{
	prv_args_to_int(p, a1, a2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	return subtilis_type_if_or(p, a1, a2, err);
}

static subtilis_exp_t *prv_eor(subtilis_parser_t *p, subtilis_exp_t *a1,
			       subtilis_exp_t *a2, subtilis_error_t *err)
{
	prv_args_to_int(p, a1, a2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	return subtilis_type_if_eor(p, a1, a2, err);
}

static subtilis_exp_t *prv_not(subtilis_parser_t *p, subtilis_exp_t *e,
			       subtilis_error_t *err)
{
	e = subtilis_type_if_to_int(p, e, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	return subtilis_type_if_not(p, e, err);
}

static subtilis_exp_t *prv_eq(subtilis_parser_t *p, subtilis_exp_t *a1,
			      subtilis_exp_t *a2, subtilis_error_t *err)
{
	return prv_commutative_logical(p, a1, a2, SUBTILIS_OP_INSTR_EQI_REAL,
				       SUBTILIS_OP_INSTR_EQ_REAL, err);
}

static subtilis_exp_t *prv_neq(subtilis_parser_t *p, subtilis_exp_t *a1,
			       subtilis_exp_t *a2, subtilis_error_t *err)
{
	return prv_commutative_logical(p, a1, a2, SUBTILIS_OP_INSTR_NEQI_REAL,
				       SUBTILIS_OP_INSTR_NEQ_REAL, err);
}

static subtilis_exp_t *prv_sub(subtilis_parser_t *p, subtilis_exp_t *a1,
			       subtilis_exp_t *a2, bool swapped,
			       subtilis_error_t *err)
{
	subtilis_op_instr_type_t real_var_imm;

	real_var_imm = (swapped) ? SUBTILIS_OP_INSTR_RSUBI_REAL
				 : SUBTILIS_OP_INSTR_SUBI_REAL;

	return prv_commutative(p, a1, a2, real_var_imm,
			       SUBTILIS_OP_INSTR_SUB_REAL, err);
}

static subtilis_exp_t *prv_divr(subtilis_parser_t *p, subtilis_exp_t *a1,
				subtilis_exp_t *a2, bool swapped,
				subtilis_error_t *err)
{
	subtilis_op_instr_type_t real_var_imm;
	bool div_by_const = false;
	subtilis_exp_t *e;

	if (swapped) {
		real_var_imm = SUBTILIS_OP_INSTR_RDIVI_REAL;
	} else {
		div_by_const = a2->type == SUBTILIS_TYPE_CONST_REAL;
		if (div_by_const && (a2->exp.ir_op.real == 0.0)) {
			subtilis_error_set_divide_by_zero(
			    err, p->l->stream->name, p->l->line);
			subtilis_exp_delete(a1);
			subtilis_exp_delete(a2);
			return NULL;
		}
		real_var_imm = SUBTILIS_OP_INSTR_DIVI_REAL;
	}

	e = prv_commutative(p, a1, a2, real_var_imm, SUBTILIS_OP_INSTR_DIV_REAL,
			    err);

	if (!div_by_const) {
		subtilis_exp_handle_errors(p, err);
		if (err->type != SUBTILIS_ERROR_OK) {
			subtilis_exp_delete(e);
			return NULL;
		}
	}

	return e;
}

static subtilis_exp_t *prv_gt(subtilis_parser_t *p, subtilis_exp_t *a1,
			      subtilis_exp_t *a2, bool swapped,
			      subtilis_error_t *err)
{
	subtilis_op_instr_type_t real_var_imm;

	real_var_imm = (swapped) ? SUBTILIS_OP_INSTR_LTEI_REAL
				 : SUBTILIS_OP_INSTR_GTI_REAL;

	return prv_commutative(p, a1, a2, real_var_imm,
			       SUBTILIS_OP_INSTR_GT_REAL, err);
}

static subtilis_exp_t *prv_lte(subtilis_parser_t *p, subtilis_exp_t *a1,
			       subtilis_exp_t *a2, bool swapped,
			       subtilis_error_t *err)
{
	subtilis_op_instr_type_t real_var_imm;

	real_var_imm = (swapped) ? SUBTILIS_OP_INSTR_GTEI_REAL
				 : SUBTILIS_OP_INSTR_LTEI_REAL;

	return prv_commutative(p, a1, a2, real_var_imm,
			       SUBTILIS_OP_INSTR_LTE_REAL, err);
}

static subtilis_exp_t *prv_lt(subtilis_parser_t *p, subtilis_exp_t *a1,
			      subtilis_exp_t *a2, bool swapped,
			      subtilis_error_t *err)
{
	subtilis_op_instr_type_t real_var_imm;

	real_var_imm = (swapped) ? SUBTILIS_OP_INSTR_GTEI_REAL
				 : SUBTILIS_OP_INSTR_LTI_REAL;

	return prv_commutative(p, a1, a2, real_var_imm,
			       SUBTILIS_OP_INSTR_LT_REAL, err);
}

static subtilis_exp_t *prv_gte(subtilis_parser_t *p, subtilis_exp_t *a1,
			       subtilis_exp_t *a2, bool swapped,
			       subtilis_error_t *err)
{
	subtilis_op_instr_type_t real_var_imm;

	real_var_imm = (swapped) ? SUBTILIS_OP_INSTR_LTEI_REAL
				 : SUBTILIS_OP_INSTR_GTEI_REAL;

	return prv_commutative(p, a1, a2, real_var_imm,
			       SUBTILIS_OP_INSTR_GTE_REAL, err);
}

subtilis_type_if subtilis_type_float64 = {
    .is_const = false,
    .exp_to_var = prv_returne,
    .copy_var = prv_copy_var,
    .dup = prv_dup,
    .to_int32 = prv_to_int32,
    .to_float64 = prv_returne,
    .unary_minus = prv_unary_minus,
    .add = prv_add,
    .mul = prv_mul,
    .and = prv_and,
    .or = prv_or,
    .eor = prv_eor,
    .not = prv_not,
    .eq = prv_eq,
    .neq = prv_neq,
    .sub = prv_sub,
    .div = NULL,
    .mod = NULL,
    .divr = prv_divr,
    .gt = prv_gt,
    .lte = prv_lte,
    .lt = prv_lt,
    .gte = prv_gte,
    .lsl = NULL,
    .lsr = NULL,
    .asr = NULL,
};