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

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../common/error_codes.h"
#include "builtins_ir.h"
#include "parser_exp.h"
#include "parser_string.h"
#include "reference_type.h"
#include "string_type.h"
#include "type_if.h"

subtilis_exp_t *subtilis_parser_chrstr(subtilis_parser_t *p,
				       subtilis_token_t *t,
				       subtilis_error_t *err)
{
	subtilis_exp_t *e;
	subtilis_exp_t *retval;
	subtilis_buffer_t buf;
	char c_str[2];

	subtilis_buffer_init(&buf, 2);

	e = subtilis_parser_integer_bracketed_exp(p, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	if (subtilis_type_if_is_const(&e->type)) {
		c_str[0] = e->exp.ir_op.integer & 255;
		c_str[1] = 0;
		subtilis_buffer_append(&buf, c_str, 2, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
		retval = subtilis_exp_new_str(&buf, err);
		subtilis_buffer_free(&buf);
		subtilis_exp_delete(e);
		return retval;
	}

	return subtilis_string_type_new_tmp_from_char(p, e, err);

cleanup:
	subtilis_buffer_free(&buf);
	subtilis_exp_delete(e);

	return NULL;
}

subtilis_exp_t *subtilis_parser_asc(subtilis_parser_t *p, subtilis_token_t *t,
				    subtilis_error_t *err)
{
	subtilis_exp_t *e;

	e = subtilis_parser_bracketed_exp(p, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	return subtilis_string_type_asc(p, e, err);
}

subtilis_exp_t *subtilis_parser_len(subtilis_parser_t *p, subtilis_token_t *t,
				    subtilis_error_t *err)
{
	subtilis_exp_t *e;

	e = subtilis_parser_bracketed_exp(p, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	return subtilis_string_type_len(p, e, err);
}

static void prv_left_right_args(subtilis_parser_t *p, subtilis_token_t *t,
				subtilis_exp_t **e, int32_t default_val,
				subtilis_error_t *err)
{
	size_t args;
	size_t i;
	const char *tbuf;

	subtilis_lexer_get(p->l, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(t);
	if (strcmp(tbuf, "(")) {
		subtilis_error_set_exp_expected(err, "( ", p->l->stream->name,
						p->l->line);
		return;
	}

	args = subtilis_var_bracketed_args_have_b(p, t, e, 2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_lexer_get(p->l, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	if (args == 0) {
		subtilis_error_set_string_expected(err, p->l->stream->name,
						   p->l->line);
		goto cleanup;
	}

	if (args == 1) {
		e[1] = subtilis_exp_new_int32(default_val, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
	}

	return;

cleanup:

	for (i = 0; i < args; i++)
		subtilis_exp_delete(e[i]);
}

subtilis_exp_t *subtilis_parser_left_str_exp(subtilis_parser_t *p,
					     subtilis_token_t *t,
					     subtilis_error_t *err)
{
	subtilis_exp_t *e[2];

	prv_left_right_args(p, t, e, 1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	return subtilis_string_type_left_exp(p, e[0], e[1], err);
}

subtilis_exp_t *subtilis_parser_right_str_exp(subtilis_parser_t *p,
					      subtilis_token_t *t,
					      subtilis_error_t *err)
{
	subtilis_exp_t *e[2];

	prv_left_right_args(p, t, e, 1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	return subtilis_string_type_right_exp(p, e[0], e[1], err);
}

subtilis_exp_t *subtilis_parser_mid_str_exp(subtilis_parser_t *p,
					    subtilis_token_t *t,
					    subtilis_error_t *err)
{
	subtilis_exp_t *e[3];
	size_t args;
	size_t i;
	const char *tbuf;

	subtilis_lexer_get(p->l, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	tbuf = subtilis_token_get_text(t);
	if (strcmp(tbuf, "(")) {
		subtilis_error_set_exp_expected(err, "( ", p->l->stream->name,
						p->l->line);
		return NULL;
	}

	args = subtilis_var_bracketed_args_have_b(p, t, e, 3, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_lexer_get(p->l, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	if (args == 0) {
		subtilis_error_set_string_expected(err, p->l->stream->name,
						   p->l->line);
		return NULL;
	}

	if (args == 1) {
		subtilis_error_set_expected(err, ",", ")", p->l->stream->name,
					    p->l->line);
		goto cleanup;
	}

	return subtilis_string_type_mid_exp(p, e[0], e[1], e[2], err);

cleanup:

	for (i = 0; i < args; i++)
		subtilis_exp_delete(e[i]);

	return NULL;
}

subtilis_exp_t *subtilis_parser_string_str(subtilis_parser_t *p,
					   subtilis_token_t *t,
					   subtilis_error_t *err)
{
	subtilis_exp_t *e[2];
	const char *tbuf;
	size_t args;
	size_t i;

	e[0] = NULL;
	e[1] = NULL;

	subtilis_lexer_get(p->l, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	tbuf = subtilis_token_get_text(t);
	if (strcmp(tbuf, "(")) {
		subtilis_error_set_exp_expected(err, "( ", p->l->stream->name,
						p->l->line);
		return NULL;
	}

	args = subtilis_var_bracketed_args_have_b(p, t, e, 2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	if (args != 2 || ((e[1]->type.type != SUBTILIS_TYPE_CONST_STRING) &&
			  (e[1]->type.type != SUBTILIS_TYPE_STRING))) {
		subtilis_error_set_string_expected(err, p->l->stream->name,
						   p->l->line);
		goto cleanup;
	}

	e[0] = subtilis_type_if_to_int(p, e[0], err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	e[0] = subtilis_string_type_string(p, e[0], e[1], err);

	subtilis_lexer_get(p->l, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	return e[0];

cleanup:

	for (i = 0; i < args; i++)
		subtilis_exp_delete(e[i]);

	return NULL;
}

subtilis_exp_t *subtilis_parser_str_str(subtilis_parser_t *p,
					subtilis_token_t *t,
					subtilis_error_t *err)
{
	subtilis_exp_t *val;
	const char *tbuf;
	bool hex = false;

	subtilis_lexer_get(p->l, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	tbuf = subtilis_token_get_text(t);
	if (t->type != SUBTILIS_TOKEN_OPERATOR) {
		subtilis_error_set_expected(err, "( or ~", tbuf,
					    p->l->stream->name, p->l->line);
		return NULL;
	}

	if (!strcmp(tbuf, "~")) {
		hex = true;
		subtilis_lexer_get(p->l, t, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return NULL;
		tbuf = subtilis_token_get_text(t);
	}

	if (strcmp(tbuf, "(")) {
		subtilis_error_set_expected(err, "(", tbuf, p->l->stream->name,
					    p->l->line);
		return NULL;
	}

	val = subtilis_parser_bracketed_exp_internal(p, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_lexer_get(p->l, t, err);
	if (err->type != SUBTILIS_ERROR_OK) {
		subtilis_exp_delete(val);
		return NULL;
	}

	if (hex) {
		if (!subtilis_type_if_is_integer(&val->type)) {
			val = subtilis_type_if_to_int(p, val, err);
			if (err->type != SUBTILIS_ERROR_OK)
				return NULL;
		}
		return subtilis_type_if_to_hex_string(p, val, err);
	}

	return subtilis_type_if_to_string(p, val, err);
}

void subtilis_parser_left_str(subtilis_parser_t *p, subtilis_token_t *t,
			      subtilis_error_t *err)
{
	const char *tbuf;
	subtilis_exp_t *e[2];
	subtilis_exp_t *value;

	prv_left_right_args(p, t, e, -1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(t);
	if ((t->type != SUBTILIS_TOKEN_OPERATOR) || strcmp(tbuf, "=")) {
		subtilis_error_set_assignment_op_expected(
		    err, tbuf, p->l->stream->name, p->l->line);
		goto cleanup;
	}

	value = subtilis_parser_expression(p, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	subtilis_string_type_left(p, e[0], e[1], value, err);
	return;

cleanup:
	subtilis_exp_delete(e[0]);
	subtilis_exp_delete(e[1]);
}

void subtilis_parser_right_str(subtilis_parser_t *p, subtilis_token_t *t,
			       subtilis_error_t *err)
{
	const char *tbuf;
	subtilis_exp_t *e[2];
	subtilis_exp_t *value;

	prv_left_right_args(p, t, e, -1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(t);
	if ((t->type != SUBTILIS_TOKEN_OPERATOR) || strcmp(tbuf, "=")) {
		subtilis_error_set_assignment_op_expected(
		    err, tbuf, p->l->stream->name, p->l->line);
		goto cleanup;
	}

	value = subtilis_parser_expression(p, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	subtilis_string_type_right(p, e[0], e[1], value, err);
	return;

cleanup:
	subtilis_exp_delete(e[0]);
	subtilis_exp_delete(e[1]);
}

void subtilis_parser_mid_str(subtilis_parser_t *p, subtilis_token_t *t,
			     subtilis_error_t *err)
{
	size_t args;
	const char *tbuf;
	subtilis_exp_t *e[3];
	subtilis_exp_t *value;

	subtilis_lexer_get(p->l, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(t);
	if (strcmp(tbuf, "(")) {
		subtilis_error_set_exp_expected(err, "( ", p->l->stream->name,
						p->l->line);
		return;
	}

	args = subtilis_var_bracketed_args_have_b(p, t, e, 3, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_lexer_get(p->l, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	if (args == 0) {
		subtilis_error_set_string_expected(err, p->l->stream->name,
						   p->l->line);
		goto cleanup;
	}

	if (args == 1) {
		subtilis_error_set_numeric_exp_expected(err, p->l->stream->name,
							p->l->line);
		goto cleanup;
	}

	if (args == 2) {
		e[2] = subtilis_exp_new_int32(-1, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
	}

	tbuf = subtilis_token_get_text(t);
	if ((t->type != SUBTILIS_TOKEN_OPERATOR) || strcmp(tbuf, "=")) {
		subtilis_error_set_assignment_op_expected(
		    err, tbuf, p->l->stream->name, p->l->line);
		goto cleanup;
	}

	value = subtilis_parser_expression(p, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	if ((e[1]->type.type == SUBTILIS_TYPE_CONST_INTEGER) &&
	    (e[1]->exp.ir_op.integer == 0)) {
		subtilis_exp_delete(e[1]);
		subtilis_string_type_left(p, e[0], e[2], value, err);
		return;
	}

	subtilis_string_type_mid(p, e[0], e[1], e[2], value, err);

	return;

cleanup:
	subtilis_exp_delete(e[0]);
	subtilis_exp_delete(e[1]);
	subtilis_exp_delete(e[2]);
}

static subtilis_exp_t *prv_val_real(subtilis_parser_t *p, subtilis_token_t *t,
				    subtilis_exp_t *e, subtilis_error_t *err)
{
	const char *str;
	char *copy;
	char *ptr;
	size_t str_size;
	size_t str_reg;
	double ret_val = 0.0;

	if (e->type.type == SUBTILIS_TYPE_CONST_STRING) {
		str = subtilis_buffer_get_string(&e->exp.str);
		str_size = subtilis_buffer_get_size(&e->exp.str) - 1;
		if (str_size < 2 || (strcmp(str, "0x") && strcmp(str, "0X"))) {
			copy = malloc(str_size + 1);
			if (!copy) {
				subtilis_error_set_oom(err);
				subtilis_exp_delete(e);
				return NULL;
			}
			strcpy(copy, str);
			ptr = strchr(copy, 'e');
			if (ptr) {
				*ptr = 0;
			} else {
				ptr = strchr(copy, 'E');
				if (ptr)
					*ptr = 0;
			}
			ret_val = strtod(copy, NULL);
			if (isinf(ret_val)) {
				subtilis_error_set_number_too_long(
				    err, copy, p->l->stream->name, p->l->line);
				free(copy);
				subtilis_exp_delete(e);
				return NULL;
			}
			free(copy);
		}
		subtilis_exp_delete(e);
		return subtilis_exp_new_real(ret_val, err);
	} else if (e->type.type == SUBTILIS_TYPE_STRING) {
		str_reg = e->exp.ir_op.reg;
		subtilis_exp_delete(e);
		return subtilis_builtin_ir_call_str_to_fp(p, str_reg, err);
	}

	subtilis_error_set_string_expected(err, p->l->stream->name, p->l->line);
	return NULL;
}

static subtilis_exp_t *prv_call_val_int(subtilis_parser_t *p, size_t str_reg,
					size_t base_reg, subtilis_error_t *err)
{
	subtilis_ir_operand_t error_label;
	subtilis_ir_operand_t gt1_label;
	subtilis_ir_operand_t lt10_label;
	subtilis_ir_operand_t eq16_label;
	subtilis_ir_operand_t neq16_label;
	subtilis_ir_operand_t end_label;
	subtilis_ir_operand_t condee;
	subtilis_ir_operand_t op0;
	subtilis_ir_operand_t op1;
	subtilis_exp_t *e;
	subtilis_ir_operand_t ret_val;
	subtilis_exp_t *tmp;

	ret_val.reg = p->current->reg_counter++;

	error_label.label = subtilis_ir_section_new_label(p->current);
	gt1_label.label = subtilis_ir_section_new_label(p->current);
	lt10_label.label = subtilis_ir_section_new_label(p->current);
	eq16_label.label = subtilis_ir_section_new_label(p->current);
	neq16_label.label = subtilis_ir_section_new_label(p->current);
	end_label.label = subtilis_ir_section_new_label(p->current);

	op0.reg = base_reg;
	op1.integer = 16;
	condee.reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_NEQI_I32, op0, op1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_ir_section_add_instr_reg(p->current, SUBTILIS_OP_INSTR_JMPC,
					  condee, neq16_label, eq16_label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_ir_section_add_label(p->current, neq16_label.label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	op1.integer = 1;
	condee.reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_GTI_I32, op0, op1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_ir_section_add_instr_reg(p->current, SUBTILIS_OP_INSTR_JMPC,
					  condee, gt1_label, error_label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_ir_section_add_label(p->current, gt1_label.label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	op1.integer = 10;
	condee.reg = subtilis_ir_section_add_instr(
	    p->current, SUBTILIS_OP_INSTR_LTEI_I32, op0, op1, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_ir_section_add_instr_reg(p->current, SUBTILIS_OP_INSTR_JMPC,
					  condee, lt10_label, error_label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_ir_section_add_label(p->current, lt10_label.label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	tmp = subtilis_builtin_ir_call_str_to_int32(p, str_reg, base_reg, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_ir_section_add_instr_no_reg2(p->current, SUBTILIS_OP_INSTR_MOV,
					      ret_val, tmp->exp.ir_op, err);
	subtilis_exp_delete(tmp);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_ir_section_add_instr_no_reg(p->current, SUBTILIS_OP_INSTR_JMP,
					     end_label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_ir_section_add_label(p->current, eq16_label.label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	tmp = subtilis_builtin_ir_call_hexstr_to_int32(p, str_reg, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_ir_section_add_instr_no_reg2(p->current, SUBTILIS_OP_INSTR_MOV,
					      ret_val, tmp->exp.ir_op, err);
	subtilis_exp_delete(tmp);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_ir_section_add_instr_no_reg(p->current, SUBTILIS_OP_INSTR_JMP,
					     end_label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_ir_section_add_label(p->current, error_label.label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	e = subtilis_exp_new_int32(SUBTILIS_ERROR_CODE_BAD_ARGS, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_exp_generate_error(p, e, err);
	e = NULL;
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_ir_section_add_label(p->current, end_label.label, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	return subtilis_exp_new_int32_var(ret_val.reg, err);
}

static subtilis_exp_t *prv_val_int(subtilis_parser_t *p, subtilis_token_t *t,
				   subtilis_exp_t **e, subtilis_error_t *err)
{
	const char *str;
	size_t str_size;
	int32_t retval;
	long val;
	bool hex;
	subtilis_exp_t *res = NULL;

	if (e[0]->type.type != SUBTILIS_TYPE_CONST_STRING &&
	    e[0]->type.type != SUBTILIS_TYPE_STRING) {
		subtilis_error_set_string_expected(err, p->l->stream->name,
						   p->l->line);
		goto cleanup;
	}

	e[1] = subtilis_type_if_to_int(p, e[1], err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	if (e[1]->type.type == SUBTILIS_TYPE_CONST_INTEGER) {
		if ((e[1]->exp.ir_op.integer != 16) &&
		    (e[1]->exp.ir_op.integer < 2 ||
		     e[1]->exp.ir_op.integer > 10)) {
			subtilis_error_set_bad_val_arg(err, p->l->stream->name,
						       p->l->line);
			goto cleanup;
		}

		retval = 0;
		if (e[0]->type.type == SUBTILIS_TYPE_CONST_STRING) {
			str = subtilis_buffer_get_string(&e[0]->exp.str);
			str_size = subtilis_buffer_get_size(&e[0]->exp.str) - 1;
			if (str_size < 2 ||
			    (!isspace(str[0]) && strcmp(str, "0x") &&
			     strcmp(str, "0X"))) {
				val =
				    strtol(str, NULL, e[1]->exp.ir_op.integer);
				if (errno == ERANGE || val < INT32_MIN ||
				    val > INT32_MAX) {
					subtilis_error_set_number_too_long(
					    err, str, p->l->stream->name,
					    p->l->line);
					goto cleanup;
				}
				retval = (int32_t)val;
			}
			res = subtilis_exp_new_int32(retval, err);
			goto cleanup;
		}

		hex = e[1]->exp.ir_op.integer == 16;

		if (hex) {
			res = subtilis_builtin_ir_call_hexstr_to_int32(
			    p, e[0]->exp.ir_op.reg, err);
		} else {
			e[1] = subtilis_type_if_exp_to_var(p, e[1], err);
			if (err->type != SUBTILIS_ERROR_OK)
				goto cleanup;
			res = subtilis_builtin_ir_call_str_to_int32(
			    p, e[0]->exp.ir_op.reg, e[1]->exp.ir_op.reg, err);
		}
	} else {
		if (e[0]->type.type == SUBTILIS_TYPE_CONST_STRING) {
			str = subtilis_buffer_get_string(&e[0]->exp.str);
			str_size = subtilis_buffer_get_size(&e[0]->exp.str) - 1;
			if (str_size == 0) {
				res = subtilis_exp_new_int32(0, err);
				goto cleanup;
			}
			e[0] = subtilis_type_if_exp_to_var(p, e[0], err);
			if (err->type != SUBTILIS_ERROR_OK)
				goto cleanup;
		}
		res = prv_call_val_int(p, e[0]->exp.ir_op.reg,
				       e[1]->exp.ir_op.reg, err);
	}

cleanup:

	subtilis_exp_delete(e[0]);
	subtilis_exp_delete(e[1]);

	return res;
}

subtilis_exp_t *subtilis_parser_val(subtilis_parser_t *p, subtilis_token_t *t,
				    subtilis_error_t *err)
{
	size_t args;
	const char *tbuf;
	subtilis_exp_t *e[2];
	size_t i;

	subtilis_lexer_get(p->l, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	tbuf = subtilis_token_get_text(t);
	if (strcmp(tbuf, "(")) {
		subtilis_error_set_exp_expected(err, "( ", p->l->stream->name,
						p->l->line);
		return NULL;
	}

	args = subtilis_var_bracketed_args_have_b(p, t, e, 2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	subtilis_lexer_get(p->l, t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	if (args == 0) {
		subtilis_error_set_string_expected(err, p->l->stream->name,
						   p->l->line);
		goto cleanup;
	}

	if (args == 1)
		return prv_val_real(p, t, e[0], err);

	if (args == 2)
		return prv_val_int(p, t, e, err);

	subtilis_error_set_bad_arg_count(err, 2, args, p->l->stream->name,
					 p->l->line);
cleanup:

	for (i = 0; i < args; i++)
		subtilis_exp_delete(e[i]);

	return NULL;
}
