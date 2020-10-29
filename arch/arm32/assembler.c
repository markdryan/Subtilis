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

#include "arm_core.h"
#include "arm_expression.h"
#include "arm_keywords.h"
#include "assembler.h"

typedef void (*subtilis_arm_ass_fn_t)(void *);

struct subtilis_arm_ass_mnemomic_t_ {
	const char *name;
	subtilis_arm_instr_type_t type;
	subtilis_arm_ass_fn_t asm_fn;
	bool status_valid;
};

typedef struct subtilis_arm_ass_mnemomic_t_ subtilis_arm_ass_mnemomic_t;

/*
 * Must match order of codes in subtilis_arm_ccode_type_t
 */

/* clang-format off */
static const char *const condition_codes[] = {
	"EQ",
	"NE",
	"CS",
	"CC",
	"MI",
	"PL",
	"VS",
	"VC",
	"HI",
	"LS",
	"GE",
	"LT",
	"GT",
	"LE",
	"AL",
	"NV",
};

/*
 * Must match order of codes in subtilis_arm_mtran_type_t
 */

static const char *const mtran_types[] = {
	"IA",
	"IB",
	"DA",
	"DB",
	"FA",
	"FD",
	"EA",
	"ED",
};

static const subtilis_arm_ass_mnemomic_t a_mnem[] = {
	{ "ABS", SUBTILIS_FPA_INSTR_ABS, NULL, false },
	{ "ACS", SUBTILIS_FPA_INSTR_ACS, NULL, false },
	{ "ADC", SUBTILIS_ARM_INSTR_ADC, NULL, true },
	{ "ADD", SUBTILIS_ARM_INSTR_ADD, NULL, true },
	{ "ADF", SUBTILIS_FPA_INSTR_ADF, NULL, false },
	{ "ADR", SUBTILIS_ARM_INSTR_ADR, NULL, false },
	{ "AND", SUBTILIS_ARM_INSTR_AND, NULL, true },
	{ "ASN", SUBTILIS_FPA_INSTR_ASN, NULL, false },
	{ "ATN", SUBTILIS_FPA_INSTR_ATN, NULL, false },
};

static const subtilis_arm_ass_mnemomic_t b_mnem[] = {
	{ "B", SUBTILIS_ARM_INSTR_B, NULL, false },
	{ "BL", SUBTILIS_ARM_INSTR_B, NULL, false },
	{ "BIC", SUBTILIS_ARM_INSTR_BIC, NULL, true },
};

static const subtilis_arm_ass_mnemomic_t c_mnem[] = {
	{ "CMF", SUBTILIS_FPA_INSTR_CMF, NULL, false },
	{ "CMFE", SUBTILIS_FPA_INSTR_CMFE, NULL, false },
	{ "CMN", SUBTILIS_ARM_INSTR_CMN, NULL, false },
	{ "CMP", SUBTILIS_ARM_INSTR_CMP, NULL, false },
	{ "CNF", SUBTILIS_FPA_INSTR_CNF, NULL, false },
	{ "CNFE", SUBTILIS_FPA_INSTR_CNFE, NULL, false },
	{ "COS", SUBTILIS_FPA_INSTR_COS, NULL, false },
};

static const subtilis_arm_ass_mnemomic_t d_mnem[] = {
	{ "DVF", SUBTILIS_FPA_INSTR_DVF, NULL, false },
};

static const subtilis_arm_ass_mnemomic_t e_mnem[] = {
	{ "EOR", SUBTILIS_ARM_INSTR_EOR, NULL, true },
	{ "EXP", SUBTILIS_FPA_INSTR_EXP, NULL, false },
};

static const subtilis_arm_ass_mnemomic_t f_mnem[] = {
	{ "FDV", SUBTILIS_FPA_INSTR_FDV, NULL, false },
	{ "FIX", SUBTILIS_FPA_INSTR_FIX, NULL, false },
	{ "FLT", SUBTILIS_FPA_INSTR_FLT, NULL, false },
	{ "FML", SUBTILIS_FPA_INSTR_FML, NULL, false },
	{ "FRD", SUBTILIS_FPA_INSTR_FRD, NULL, false },
};

static const subtilis_arm_ass_mnemomic_t l_mnem[] = {
	{ "LDF", SUBTILIS_FPA_INSTR_LDF, NULL, false },
	{ "LDM", SUBTILIS_ARM_INSTR_LDM, NULL, false },
	{ "LDR", SUBTILIS_ARM_INSTR_LDR, NULL, false },
	{ "LGN", SUBTILIS_FPA_INSTR_LGN, NULL, false },
	{ "LOG", SUBTILIS_FPA_INSTR_LOG, NULL, false },
};

static const subtilis_arm_ass_mnemomic_t m_mnem[] = {
	{ "MLA", SUBTILIS_ARM_INSTR_MLA, NULL, true },
	{ "MNF", SUBTILIS_FPA_INSTR_MNF, NULL, false },
	{ "MOV", SUBTILIS_ARM_INSTR_MOV, NULL, true },
	{ "MUF", SUBTILIS_FPA_INSTR_MUF, NULL, false },
	{ "MUL", SUBTILIS_ARM_INSTR_MUL, NULL, true },
	{ "MVF", SUBTILIS_FPA_INSTR_MVF, NULL, false },
	{ "MVN", SUBTILIS_ARM_INSTR_MVN, NULL, true },
};

static const subtilis_arm_ass_mnemomic_t n_mnem[] = {
	{ "NRM", SUBTILIS_FPA_INSTR_NRM, NULL, false },
};

static const subtilis_arm_ass_mnemomic_t o_mnem[] = {
	{ "ORR", SUBTILIS_ARM_INSTR_ORR, NULL, true },
};

static const subtilis_arm_ass_mnemomic_t p_mnem[] = {
	{ "POL", SUBTILIS_FPA_INSTR_POL, NULL, false },
	{ "POW", SUBTILIS_FPA_INSTR_POW, NULL, false },
};

static const subtilis_arm_ass_mnemomic_t r_mnem[] = {
	{ "RDF", SUBTILIS_FPA_INSTR_RDF, NULL, false },
	{ "RFS", SUBTILIS_FPA_INSTR_RFS, NULL, false },
	{ "RMF", SUBTILIS_FPA_INSTR_RMF, NULL, false },
	{ "RND", SUBTILIS_FPA_INSTR_RND, NULL, false },
	{ "RPW", SUBTILIS_FPA_INSTR_RPW, NULL, false },
	{ "RSB", SUBTILIS_ARM_INSTR_RSB, NULL, true },
	{ "RSC", SUBTILIS_ARM_INSTR_RSC, NULL, true },
	{ "RSF", SUBTILIS_FPA_INSTR_RSF, NULL, false },
};

static const subtilis_arm_ass_mnemomic_t s_mnem[] = {
	{ "SBC", SUBTILIS_ARM_INSTR_SBC, NULL, true },
	{ "SIN", SUBTILIS_FPA_INSTR_SIN, NULL, false },
	{ "SQT", SUBTILIS_FPA_INSTR_SQT, NULL, false },
	{ "STF", SUBTILIS_FPA_INSTR_STF, NULL, false },
	{ "STM", SUBTILIS_ARM_INSTR_STM, NULL, false },
	{ "STR", SUBTILIS_ARM_INSTR_STR, NULL, false },
	{ "SUB", SUBTILIS_ARM_INSTR_SUB, NULL, true },
	{ "SUF", SUBTILIS_FPA_INSTR_SUF, NULL, false },
	{ "SWI", SUBTILIS_ARM_INSTR_SWI, NULL, false },
};

static const subtilis_arm_ass_mnemomic_t t_mnem[] = {
	{ "TAN", SUBTILIS_FPA_INSTR_TAN, NULL, false },
	{ "TEQ", SUBTILIS_ARM_INSTR_TEQ, NULL, false },
	{ "TST", SUBTILIS_ARM_INSTR_TST, NULL, false },
};

static const subtilis_arm_ass_mnemomic_t u_mnem[] = {
	{ "URD", SUBTILIS_FPA_INSTR_URD, NULL, false },
};

static const subtilis_arm_ass_mnemomic_t w_mnem[] = {
	{ "WFS", SUBTILIS_FPA_INSTR_WFS, NULL, false },
};

/* clang-format on */

struct subtilis_arm_ass_mnem_key_t_ {
	const subtilis_arm_ass_mnemomic_t *mnems;
	size_t count;
	size_t max_length;
};

typedef struct subtilis_arm_ass_mnem_key_t_ subtilis_arm_ass_mnem_key_t;

/* clang-format off */

static const subtilis_arm_ass_mnem_key_t keyword_map[] = {
	{ a_mnem, sizeof(a_mnem) / sizeof(a_mnem[0]), 3},
	{ b_mnem, sizeof(b_mnem) / sizeof(b_mnem[0]), 3},
	{ c_mnem, sizeof(c_mnem) / sizeof(c_mnem[0]), 4},
	{ d_mnem, sizeof(d_mnem) / sizeof(d_mnem[0]), 3},
	{ e_mnem, sizeof(e_mnem) / sizeof(e_mnem[0]), 3},
	{ f_mnem, sizeof(f_mnem) / sizeof(f_mnem[0]), 3},
	{ NULL, 0, 0 }, /* G */
	{ NULL, 0, 0 }, /* H */
	{ NULL, 0, 0 }, /* I */
	{ NULL, 0, 0 }, /* J */
	{ NULL, 0, 0 }, /* K */
	{ l_mnem, sizeof(l_mnem) / sizeof(l_mnem[0]), 3},
	{ m_mnem, sizeof(m_mnem) / sizeof(m_mnem[0]), 3},
	{ n_mnem, sizeof(n_mnem) / sizeof(n_mnem[0]), 3},
	{ o_mnem, sizeof(o_mnem) / sizeof(o_mnem[0]), 3},
	{ p_mnem, sizeof(p_mnem) / sizeof(p_mnem[0]), 3},
	{ NULL, 0, 0 }, /* Q */
	{ r_mnem, sizeof(r_mnem) / sizeof(r_mnem[0]), 3},
	{ s_mnem, sizeof(s_mnem) / sizeof(s_mnem[0]), 3},
	{ t_mnem, sizeof(t_mnem) / sizeof(t_mnem[0]), 3},
	{ u_mnem, sizeof(u_mnem) / sizeof(u_mnem[0]), 3},
	{ NULL, 0, 0 }, /* V */
	{ w_mnem, sizeof(w_mnem) / sizeof(w_mnem[0]), 3},
	{ NULL, 0, 0 }, /* X */
	{ NULL, 0, 0 }, /* Y */
	{ NULL, 0, 0 }, /* Z */
};

/* clang-format on */

static void prv_parse_label(subtilis_arm_ass_context_t *c, const char *name,
			    subtilis_error_t *err)
{
	size_t index;

	if (subtilis_string_pool_find(c->label_pool, name, &index)) {
		if (subtilis_bitset_isset(&c->pending_labels, index)) {
			subtilis_bitset_clear(&c->pending_labels, index);
		} else {
			subtilis_error_set_already_defined(
			    err, name, c->l->stream->name, c->l->line);
			return;
		}
	} else {
		index = subtilis_string_pool_register(c->label_pool, name, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;
	}

	subtilis_arm_section_add_label(c->arm_s, index, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_lexer_get(c->l, c->t, err);
}

static void prv_get_op2(subtilis_arm_ass_context_t *c, subtilis_arm_op2_t *op2,
			subtilis_error_t *err)
{
	uint32_t encoded;
	int32_t num;
	subtilis_arm_exp_val_t *val;
	const char *tbuf;

	val = subtilis_arm_exp_val_get(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	if ((val->type == SUBTILIS_ARM_EXP_TYPE_INT) ||
	    (val->type == SUBTILIS_ARM_EXP_TYPE_REAL)) {
		if (val->type == SUBTILIS_ARM_EXP_TYPE_REAL)
			num = (int32_t)val->val.real;
		else
			num = val->val.integer;
		if (!subtilis_arm_encode_imm(num, &encoded)) {
			subtilis_error_set_ass_integer_encode(
			    err, num, c->l->stream->name, c->l->line);
			goto cleanup;
		}
		op2->type = SUBTILIS_ARM_OP2_I32;
		op2->op.integer = encoded;
		goto cleanup;
	}

	if (val->type != SUBTILIS_ARM_EXP_TYPE_REG) {
		subtilis_error_set_expected(err, "register",
					    subtilis_arm_exp_type_name(val),
					    c->l->stream->name, c->l->line);
		goto cleanup;
	}

	op2->op.reg = val->val.reg;

	tbuf = subtilis_token_get_text(c->t);
	if (c->t->type != SUBTILIS_TOKEN_OPERATOR) {
		op2->type = SUBTILIS_ARM_OP2_REG;
		goto cleanup;
	}

	op2->type = SUBTILIS_ARM_OP2_SHIFTED;

	tbuf = subtilis_token_get_text(c->t);
	if (strcmp(tbuf, ",")) {
		op2->type = SUBTILIS_ARM_OP2_REG;
		goto cleanup;
	}

	subtilis_lexer_get(c->l, c->t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	tbuf = subtilis_token_get_text(c->t);
	if (c->t->type != SUBTILIS_TOKEN_KEYWORD) {
		subtilis_error_set_keyword_expected(
		    err, tbuf, c->l->stream->name, c->l->line);
		goto cleanup;
	}

	switch (c->t->tok.keyword.type) {
	case SUBTILIS_ARM_KEYWORD_ASR:
		op2->op.shift.type = SUBTILIS_ARM_SHIFT_ASR;
		break;
	case SUBTILIS_ARM_KEYWORD_LSL:
		op2->op.shift.type = SUBTILIS_ARM_SHIFT_LSL;
		break;
	case SUBTILIS_ARM_KEYWORD_LSR:
		op2->op.shift.type = SUBTILIS_ARM_SHIFT_LSR;
		break;
	case SUBTILIS_ARM_KEYWORD_ROR:
		op2->op.shift.type = SUBTILIS_ARM_SHIFT_ROR;
		break;
	case SUBTILIS_ARM_KEYWORD_RRX:
		op2->op.shift.type = SUBTILIS_ARM_SHIFT_RRX;
		break;
	default:
		subtilis_error_set_expected(
		    err, "ASR or LSL or LSR or ROR or RRX", tbuf,
		    c->l->stream->name, c->l->line);
		goto cleanup;
	}

	subtilis_arm_exp_val_free(val);
	val = subtilis_arm_exp_val_get(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	if (val->type == SUBTILIS_ARM_EXP_TYPE_INT) {
		op2->op.shift.shift.integer = val->val.integer;
		op2->op.shift.shift_reg = false;
	} else if (val->type == SUBTILIS_ARM_EXP_TYPE_REG) {
		op2->op.shift.shift.reg = val->val.reg;
		op2->op.shift.shift_reg = true;
	} else {
		subtilis_error_set_expected(err, "integer or register",
					    subtilis_arm_exp_type_name(val),
					    c->l->stream->name, c->l->line);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
	}

cleanup:

	subtilis_arm_exp_val_free(val);
}

static subtilis_arm_reg_t prv_get_reg(subtilis_arm_ass_context_t *c,
				      subtilis_error_t *err)
{
	subtilis_arm_exp_val_t *val;
	subtilis_arm_reg_t reg;

	val = subtilis_arm_exp_val_get(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return SIZE_MAX;

	if (val->type != SUBTILIS_ARM_EXP_TYPE_REG) {
		subtilis_error_set_expected(err, "register",
					    subtilis_arm_exp_type_name(val),
					    c->l->stream->name, c->l->line);
		reg = SIZE_MAX;
	} else {
		reg = val->val.reg;
	}

	subtilis_arm_exp_val_free(val);

	return reg;
}

static subtilis_arm_reg_t prv_parse_reg(subtilis_arm_ass_context_t *c,
					subtilis_error_t *err)
{
	const char *tbuf;
	subtilis_arm_reg_t reg;

	subtilis_lexer_get(c->l, c->t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return SIZE_MAX;

	tbuf = subtilis_token_get_text(c->t);
	reg = subtilis_arm_exp_parse_reg(c, tbuf, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return SIZE_MAX;

	subtilis_lexer_get(c->l, c->t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return SIZE_MAX;

	return reg;
}

static void prv_parse_branch(subtilis_arm_ass_context_t *c, const char *name,
			     subtilis_arm_instr_type_t itype,
			     subtilis_arm_ccode_type_t ccode,
			     subtilis_error_t *err)
{
	subtilis_arm_instr_t *instr;
	subtilis_arm_br_instr_t *br;
	size_t index;
	subtilis_arm_exp_val_t *val;
	bool link = (strlen(name) > 1) && (name[1] == 'l' || name[1] == 'L');

	val = subtilis_arm_exp_val_get(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	if (val->type != SUBTILIS_ARM_EXP_TYPE_ID) {
		subtilis_error_set_expected(err, "label", name,
					    c->l->stream->name, c->l->line);
		goto cleanup;
	}

	name = subtilis_buffer_get_string(&val->val.buf);
	if (!subtilis_string_pool_find(c->label_pool, name, &index)) {
		index = subtilis_string_pool_register(c->label_pool, name, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;
		subtilis_bitset_set(&c->pending_labels, index, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;
	}

	instr =
	    subtilis_arm_section_add_instr(c->arm_s, SUBTILIS_ARM_INSTR_B, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	br = &instr->operands.br;
	br->ccode = ccode;
	br->link = link;
	br->link_type = SUBTILIS_ARM_BR_LINK_VOID;
	br->target.label = index;

cleanup:

	subtilis_arm_exp_val_free(val);
}

static void prv_parse_arm_data(subtilis_arm_ass_context_t *c,
			       subtilis_arm_instr_type_t itype,
			       subtilis_arm_ccode_type_t ccode, bool status,
			       subtilis_error_t *err)
{
	subtilis_arm_reg_t dest;
	subtilis_arm_reg_t op1;
	subtilis_arm_op2_t op2;
	const char *tbuf;
	subtilis_arm_instr_t *instr;
	subtilis_arm_data_instr_t *datai;

	dest = prv_get_reg(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(c->t);
	if ((c->t->type != SUBTILIS_TOKEN_OPERATOR) && (strcmp(tbuf, ","))) {
		subtilis_error_set_expected(err, ",", tbuf, c->l->stream->name,
					    c->l->line);
		return;
	}

	op1 = prv_get_reg(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(c->t);
	if ((c->t->type != SUBTILIS_TOKEN_OPERATOR) && (strcmp(tbuf, ","))) {
		subtilis_error_set_expected(err, ",", tbuf, c->l->stream->name,
					    c->l->line);
		return;
	}

	prv_get_op2(c, &op2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	instr = subtilis_arm_section_add_instr(c->arm_s, itype, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	datai = &instr->operands.data;
	datai->ccode = ccode;
	datai->dest = dest;
	datai->op1 = op1;
	datai->op2 = op2;
	datai->status = status;
}

static void prv_parse_arm_2_arg(subtilis_arm_ass_context_t *c,
				subtilis_arm_instr_type_t itype,
				subtilis_arm_ccode_type_t ccode, bool status,
				subtilis_error_t *err)
{
	subtilis_arm_reg_t dest;
	subtilis_arm_op2_t op2;
	const char *tbuf;
	subtilis_arm_instr_t *instr;
	subtilis_arm_data_instr_t *datai;

	dest = prv_get_reg(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(c->t);
	if ((c->t->type != SUBTILIS_TOKEN_OPERATOR) && (strcmp(tbuf, ","))) {
		subtilis_error_set_expected(err, ",", tbuf, c->l->stream->name,
					    c->l->line);
		return;
	}

	prv_get_op2(c, &op2, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	instr = subtilis_arm_section_add_instr(c->arm_s, itype, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	datai = &instr->operands.data;
	datai->ccode = ccode;
	datai->dest = dest;
	datai->op1 = dest;
	datai->op2 = op2;
	datai->status = status;
}

static void prv_parse_swi(subtilis_arm_ass_context_t *c,
			  subtilis_arm_ccode_type_t ccode,
			  subtilis_error_t *err)
{
	const char *swi_name;
	subtilis_arm_exp_val_t *val;
	size_t code;

	val = subtilis_arm_exp_val_get(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	if (val->type == SUBTILIS_ARM_EXP_TYPE_INT) {
		code = (size_t)val->val.integer;
	} else if (val->type == SUBTILIS_ARM_EXP_TYPE_STRING) {
		swi_name = subtilis_buffer_get_string(&val->val.buf);
		code = c->sys_trans(swi_name);
		if (code == SIZE_MAX) {
			subtilis_error_set_sys_call_unknown(
			    err, swi_name, c->l->stream->name, c->l->line);
			goto cleanup;
		}
	} else {
		subtilis_error_set_expected(err, "string or integer expected",
					    subtilis_arm_exp_type_name(val),
					    c->l->stream->name, c->l->line);
		goto cleanup;
	}

	subtilis_arm_add_swi(c->arm_s, ccode, code, 0, 0, err);

cleanup:

	subtilis_arm_exp_val_free(val);
}

static subtilis_arm_shift_type_t prv_get_st_shift(subtilis_arm_ass_context_t *c,
						  subtilis_error_t *err)
{
	const char *tbuf;
	subtilis_arm_shift_type_t st = SUBTILIS_ARM_SHIFT_ASR;

	tbuf = subtilis_token_get_text(c->t);
	subtilis_lexer_get(c->l, c->t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return st;
	if (c->t->type != SUBTILIS_TOKEN_KEYWORD) {
		subtilis_error_set_keyword_expected(
		    err, tbuf, c->l->stream->name, c->l->line);
		return st;
	}

	switch (c->t->tok.keyword.type) {
	case SUBTILIS_ARM_KEYWORD_ASR:
		st = SUBTILIS_ARM_SHIFT_ASR;
		break;
	case SUBTILIS_ARM_KEYWORD_LSL:
		st = SUBTILIS_ARM_SHIFT_LSL;
		break;
	case SUBTILIS_ARM_KEYWORD_LSR:
		st = SUBTILIS_ARM_SHIFT_LSR;
		break;
	case SUBTILIS_ARM_KEYWORD_ROR:
		st = SUBTILIS_ARM_SHIFT_ROR;
		break;
	case SUBTILIS_ARM_KEYWORD_RRX:
		st = SUBTILIS_ARM_SHIFT_RRX;
		break;
	default:
		subtilis_error_set_expected(err, "shift keyword", tbuf,
					    c->l->stream->name, c->l->line);
	}

	return st;
}

static subtilis_arm_exp_val_t *prv_get_offset(subtilis_arm_ass_context_t *c,
					      bool *subtract,
					      subtilis_error_t *err)
{
	const char *tbuf;

	if (c->t->type == SUBTILIS_TOKEN_OPERATOR) {
		tbuf = subtilis_token_get_text(c->t);
		if (strcmp(tbuf, "-")) {
			subtilis_error_set_expected(
			    err, "-, register or integer", tbuf,
			    c->l->stream->name, c->l->line);
			return NULL;
		}
		*subtract = true;
		return subtilis_arm_exp_val_get(c, err);
	}
	return subtilis_arm_exp_pri7(c, err);
}

static void prv_parse_int_offset(subtilis_arm_ass_context_t *c,
				 subtilis_arm_exp_val_t *val,
				 subtilis_arm_op2_t *offset, bool *subtract,
				 subtilis_error_t *err)
{
	offset->type = SUBTILIS_ARM_OP2_I32;
	if (val->val.integer < 0) {
		*subtract = true;
		offset->op.integer = -val->val.integer;
	} else {
		offset->op.integer = val->val.integer;
	}
	if (offset->op.integer > 4095)
		subtilis_error_set_ass_bad_offset(
		    err, val->val.integer, c->l->stream->name, c->l->line);
}

static void prv_parse_shift_offset(subtilis_arm_ass_context_t *c,
				   subtilis_arm_exp_val_t *val,
				   subtilis_arm_op2_t *offset,
				   subtilis_error_t *err)
{
	offset->op.shift.reg = val->val.reg;
	offset->type = SUBTILIS_ARM_OP2_SHIFTED;
	offset->op.shift.type = prv_get_st_shift(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	val = subtilis_arm_exp_val_get(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;
	if (val->type == SUBTILIS_ARM_EXP_TYPE_INT) {
		offset->op.shift.shift_reg = false;
		offset->op.shift.shift.integer = val->val.integer;
	} else if (val->type == SUBTILIS_ARM_EXP_TYPE_REG) {
		offset->op.shift.shift_reg = true;
		offset->op.shift.shift.reg = val->val.reg;
	} else {
		subtilis_error_set_expected(err, "integer or register",
					    subtilis_arm_exp_type_name(val),
					    c->l->stream->name, c->l->line);
	}

	subtilis_arm_exp_val_free(val);
}

static void prv_parse_stran_pre_off(subtilis_arm_ass_context_t *c,
				    subtilis_arm_instr_type_t itype,
				    subtilis_arm_ccode_type_t ccode,
				    subtilis_arm_reg_t dest,
				    subtilis_arm_reg_t base, bool byte,
				    subtilis_arm_op2_t offset, bool subtract,
				    subtilis_error_t *err)
{
	const char *tbuf;
	subtilis_arm_instr_t *instr;
	subtilis_arm_stran_instr_t *stran;
	bool write_back = false;

	tbuf = subtilis_token_get_text(c->t);
	if ((c->t->type == SUBTILIS_TOKEN_OPERATOR) && (!strcmp(tbuf, "!"))) {
		write_back = true;

		subtilis_lexer_get(c->l, c->t, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;
	}

	instr = subtilis_arm_section_add_instr(c->arm_s, itype, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	stran = &instr->operands.stran;
	stran->ccode = ccode;
	stran->dest = dest;
	stran->base = base;
	stran->offset = offset;
	stran->pre_indexed = true;
	stran->write_back = write_back;
	stran->byte = byte;
	stran->subtract = subtract;
}

static void prv_parse_stran_pre(subtilis_arm_ass_context_t *c,
				subtilis_arm_instr_type_t itype,
				subtilis_arm_ccode_type_t ccode,
				subtilis_arm_reg_t dest,
				subtilis_arm_reg_t base, bool byte,
				subtilis_error_t *err)
{
	subtilis_arm_exp_val_t *val;
	subtilis_arm_op2_t offset;
	const char *tbuf;
	bool subtract = false;

	subtilis_lexer_get(c->l, c->t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	val = prv_get_offset(c, &subtract, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(c->t);
	if (val->type == SUBTILIS_ARM_EXP_TYPE_INT) {
		prv_parse_int_offset(c, val, &offset, &subtract, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
		if ((c->t->type != SUBTILIS_TOKEN_OPERATOR) ||
		    (strcmp(tbuf, "]"))) {
			subtilis_error_set_expected(
			    err, "]", tbuf, c->l->stream->name, c->l->line);
			goto cleanup;
		}
	} else if (val->type == SUBTILIS_ARM_EXP_TYPE_REG) {
		if (c->t->type != SUBTILIS_TOKEN_OPERATOR) {
			subtilis_error_set_expected(err, ", or ]", tbuf,
						    c->l->stream->name,
						    c->l->line);
			goto cleanup;
		}
		if (!strcmp(tbuf, "]")) {
			offset.op.reg = val->val.reg;
			offset.type = SUBTILIS_ARM_OP2_REG;
		} else if (!strcmp(tbuf, ",")) {
			prv_parse_shift_offset(c, val, &offset, err);
			if (err->type != SUBTILIS_ERROR_OK)
				goto cleanup;

			if ((c->t->type != SUBTILIS_TOKEN_OPERATOR) ||
			    (strcmp(tbuf, "]"))) {
				subtilis_error_set_expected(err, "]", tbuf,
							    c->l->stream->name,
							    c->l->line);
				goto cleanup;
			}
		} else {
			subtilis_error_set_expected(err, ", or ]", tbuf,
						    c->l->stream->name,
						    c->l->line);
			goto cleanup;
		}
	} else {
		subtilis_error_set_expected(err, "integer or register",
					    subtilis_arm_exp_type_name(val),
					    c->l->stream->name, c->l->line);
		goto cleanup;
	}

	subtilis_lexer_get(c->l, c->t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	prv_parse_stran_pre_off(c, itype, ccode, dest, base, byte, offset,
				subtract, err);

cleanup:

	subtilis_arm_exp_val_free(val);
}

static void prv_parse_stran_post(subtilis_arm_ass_context_t *c,
				 subtilis_arm_instr_type_t itype,
				 subtilis_arm_ccode_type_t ccode,
				 subtilis_arm_reg_t dest,
				 subtilis_arm_reg_t base, bool byte,
				 subtilis_error_t *err)
{
	const char *tbuf;
	subtilis_arm_exp_val_t *val;
	subtilis_arm_op2_t offset;
	subtilis_arm_instr_t *instr;
	subtilis_arm_stran_instr_t *stran;
	bool subtract = false;

	subtilis_lexer_get(c->l, c->t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(c->t);
	if ((c->t->type != SUBTILIS_TOKEN_OPERATOR) || (strcmp(tbuf, ","))) {
		offset.type = SUBTILIS_ARM_OP2_I32;
		offset.op.integer = 0;
		prv_parse_stran_pre_off(c, itype, ccode, dest, base, byte,
					offset, false, err);
		return;
	}

	subtilis_lexer_get(c->l, c->t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	val = prv_get_offset(c, &subtract, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(c->t);
	if (val->type == SUBTILIS_ARM_EXP_TYPE_INT) {
		prv_parse_int_offset(c, val, &offset, &subtract, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
	} else if (val->type == SUBTILIS_ARM_EXP_TYPE_REG) {
		if ((c->t->type == SUBTILIS_TOKEN_OPERATOR) &&
		    !strcmp(tbuf, ",")) {
			prv_parse_shift_offset(c, val, &offset, err);
			if (err->type != SUBTILIS_ERROR_OK)
				goto cleanup;
		} else {
			offset.op.reg = val->val.reg;
			offset.type = SUBTILIS_ARM_OP2_REG;
		}
	} else {
		subtilis_error_set_expected(err, "integer or register",
					    subtilis_arm_exp_type_name(val),
					    c->l->stream->name, c->l->line);
		goto cleanup;
	}

	instr = subtilis_arm_section_add_instr(c->arm_s, itype, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;
	stran = &instr->operands.stran;
	stran->ccode = ccode;
	stran->dest = dest;
	stran->base = base;
	stran->offset = offset;
	stran->pre_indexed = false;
	stran->write_back = true;
	stran->byte = byte;
	stran->subtract = subtract;

cleanup:

	subtilis_arm_exp_val_free(val);
}

static void prv_parse_stran(subtilis_arm_ass_context_t *c,
			    subtilis_arm_instr_type_t itype,
			    subtilis_arm_ccode_type_t ccode, bool byte,
			    subtilis_error_t *err)
{
	subtilis_arm_reg_t dest;
	subtilis_arm_reg_t base;
	const char *tbuf;

	dest = prv_get_reg(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(c->t);
	if ((c->t->type != SUBTILIS_TOKEN_OPERATOR) || (strcmp(tbuf, ","))) {
		subtilis_error_set_expected(err, ",", tbuf, c->l->stream->name,
					    c->l->line);
		return;
	}

	subtilis_lexer_get(c->l, c->t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(c->t);
	if ((c->t->type != SUBTILIS_TOKEN_OPERATOR) || (strcmp(tbuf, "["))) {
		subtilis_error_set_expected(err, "[", tbuf, c->l->stream->name,
					    c->l->line);
		return;
	}

	base = prv_get_reg(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(c->t);
	if (c->t->type == SUBTILIS_TOKEN_OPERATOR) {
		if (!strcmp(tbuf, "]")) {
			prv_parse_stran_post(c, itype, ccode, dest, base, byte,
					     err);
			return;
		}
		if (!strcmp(tbuf, ",")) {
			prv_parse_stran_pre(c, itype, ccode, dest, base, byte,
					    err);
			return;
		}
	}

	subtilis_error_set_expected(err, ", or ]", tbuf, c->l->stream->name,
				    c->l->line);
}

static void prv_parse_mtran_range(subtilis_arm_ass_context_t *c, size_t reg1,
				  size_t *reg_list, subtilis_error_t *err)
{
	subtilis_arm_reg_t reg2;
	size_t i;

	reg2 = prv_parse_reg(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	if (reg1 >= reg2) {
		subtilis_error_set_ass_bad_range(
		    err, reg1, reg2, c->l->stream->name, c->l->line);
		return;
	}

	for (i = reg1 + 1; i <= reg2; i++)
		*reg_list |= 1 << i;
}

static void prv_parse_mtran(subtilis_arm_ass_context_t *c,
			    subtilis_arm_instr_type_t itype,
			    subtilis_arm_ccode_type_t ccode,
			    subtilis_arm_mtran_type_t mtran_type,
			    subtilis_error_t *err)
{
	subtilis_arm_reg_t dest;
	const char *tbuf;
	subtilis_arm_reg_t reg1;
	size_t reg_list = 0;
	bool write_back = false;

	dest = prv_get_reg(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(c->t);
	if ((c->t->type == SUBTILIS_TOKEN_OPERATOR) && !strcmp(tbuf, "!")) {
		write_back = true;
		subtilis_lexer_get(c->l, c->t, err);
		if (err->type != SUBTILIS_ERROR_OK)
			return;
		tbuf = subtilis_token_get_text(c->t);
	}

	if ((c->t->type != SUBTILIS_TOKEN_OPERATOR) || strcmp(tbuf, ",")) {
		subtilis_error_set_expected(err, ",", tbuf, c->l->stream->name,
					    c->l->line);
		return;
	}

	subtilis_lexer_get(c->l, c->t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(c->t);
	if ((c->t->type != SUBTILIS_TOKEN_OPERATOR) || strcmp(tbuf, "{")) {
		subtilis_error_set_expected(err, "{", tbuf, c->l->stream->name,
					    c->l->line);
		return;
	}

	do {
		reg1 = prv_parse_reg(c, err);
		reg_list |= 1 << reg1;
		if (err->type != SUBTILIS_ERROR_OK)
			return;

		tbuf = subtilis_token_get_text(c->t);
		if (c->t->type != SUBTILIS_TOKEN_OPERATOR) {
			subtilis_error_set_expected(err, "-, } or ,", tbuf,
						    c->l->stream->name,
						    c->l->line);
			return;
		}

		if (!strcmp(tbuf, ","))
			continue;
		if (!strcmp(tbuf, "-")) {
			prv_parse_mtran_range(c, reg1, &reg_list, err);
			if (err->type != SUBTILIS_ERROR_OK)
				return;
			tbuf = subtilis_token_get_text(c->t);
			if (c->t->type != SUBTILIS_TOKEN_OPERATOR) {
				subtilis_error_set_expected(err, "} or ,", tbuf,
							    c->l->stream->name,
							    c->l->line);
				return;
			}
			if (!strcmp(tbuf, ","))
				continue;
			if (!strcmp(tbuf, "}"))
				break;

			subtilis_error_set_expected(err, "} or ,", tbuf,
						    c->l->stream->name,
						    c->l->line);
			return;
		} else if (!strcmp(tbuf, "}")) {
			break;
		}

		subtilis_error_set_expected(err, "} or ,", tbuf,
					    c->l->stream->name, c->l->line);
		return;
	} while (true);

	subtilis_lexer_get(c->l, c->t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	subtilis_arm_add_mtran(c->arm_s, itype, ccode, dest, reg_list,
			       mtran_type, write_back, err);
}

static void prv_parse_adr(subtilis_arm_ass_context_t *c,
			  subtilis_arm_ccode_type_t ccode,
			  subtilis_error_t *err)
{
	subtilis_arm_exp_val_t *val;
	const char *str;
	size_t index;
	subtilis_arm_reg_t dest;
	const char *tbuf;
	const char *bad_reg = "R15";

	val = subtilis_arm_exp_val_get(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	if (val->type != SUBTILIS_ARM_EXP_TYPE_REG) {
		subtilis_error_set_expected(err, "register",
					    subtilis_arm_exp_type_name(val),
					    c->l->stream->name, c->l->line);
		goto cleanup;
	}

	dest = val->val.reg;
	if (dest == 15) {
		subtilis_error_set_ass_bad_reg(err, bad_reg, c->l->stream->name,
					       c->l->line);
		goto cleanup;
	}

	subtilis_arm_exp_val_free(val);
	val = NULL;

	tbuf = subtilis_token_get_text(c->t);
	if ((c->t->type != SUBTILIS_TOKEN_OPERATOR) || (strcmp(tbuf, ","))) {
		subtilis_error_set_expected(err, ",", tbuf, c->l->stream->name,
					    c->l->line);
		goto cleanup;
	}

	val = subtilis_arm_exp_val_get(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	if (val->type != SUBTILIS_ARM_EXP_TYPE_ID) {
		subtilis_error_set_expected(err, "label",
					    subtilis_arm_exp_type_name(val),
					    c->l->stream->name, c->l->line);
		goto cleanup;
	}

	str = subtilis_buffer_get_string(&val->val.buf);
	if (!subtilis_string_pool_find(c->label_pool, str, &index)) {
		index = subtilis_string_pool_register(c->label_pool, str, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
		subtilis_bitset_set(&c->pending_labels, index, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
	}

	subtilis_add_adr(c->arm_s, ccode, dest, index, err);

cleanup:

	subtilis_arm_exp_val_free(val);
}

static void prv_parse_instruction(subtilis_arm_ass_context_t *c,
				  const char *name,
				  subtilis_arm_instr_type_t itype,
				  subtilis_arm_ccode_type_t ccode, bool status,
				  subtilis_arm_mtran_type_t mtran_type,
				  bool byte, subtilis_error_t *err)
{
	switch (itype) {
	case SUBTILIS_ARM_INSTR_B:
		prv_parse_branch(c, name, itype, ccode, err);
		break;
	case SUBTILIS_ARM_INSTR_MOV:
	case SUBTILIS_ARM_INSTR_MVN:
		prv_parse_arm_2_arg(c, itype, ccode, status, err);
		break;
	case SUBTILIS_ARM_INSTR_AND:
	case SUBTILIS_ARM_INSTR_EOR:
	case SUBTILIS_ARM_INSTR_SUB:
	case SUBTILIS_ARM_INSTR_RSB:
	case SUBTILIS_ARM_INSTR_ADD:
	case SUBTILIS_ARM_INSTR_ADC:
	case SUBTILIS_ARM_INSTR_SBC:
	case SUBTILIS_ARM_INSTR_RSC:
	case SUBTILIS_ARM_INSTR_ORR:
	case SUBTILIS_ARM_INSTR_BIC:
		prv_parse_arm_data(c, itype, ccode, status, err);
		break;
	case SUBTILIS_ARM_INSTR_TST:
	case SUBTILIS_ARM_INSTR_TEQ:
	case SUBTILIS_ARM_INSTR_CMP:
	case SUBTILIS_ARM_INSTR_CMN:
		prv_parse_arm_2_arg(c, itype, ccode, true, err);
		break;
	case SUBTILIS_ARM_INSTR_SWI:
		prv_parse_swi(c, ccode, err);
		break;
	case SUBTILIS_ARM_INSTR_LDR:
	case SUBTILIS_ARM_INSTR_STR:
		prv_parse_stran(c, itype, ccode, byte, err);
		break;
	case SUBTILIS_ARM_INSTR_LDM:
	case SUBTILIS_ARM_INSTR_STM:
		prv_parse_mtran(c, itype, ccode, mtran_type, err);
		break;
	case SUBTILIS_ARM_INSTR_ADR:
		prv_parse_adr(c, ccode, err);
		break;
	default:
		subtilis_error_set_not_supported(err, name, c->l->stream->name,
						 c->l->line);
		break;
	}
}

static void prv_parse_identifier(subtilis_arm_ass_context_t *c,
				 subtilis_error_t *err)
{
	const char *tbuf;
	size_t index;
	size_t i;
	size_t base_len;
	size_t token_end;
	bool status_valid;
	bool byte_valid;
	size_t ptr;
	size_t search_len;
	subtilis_arm_instr_type_t itype;
	subtilis_arm_mtran_type_t mtran_type = SUBTILIS_ARM_MTRAN_IA;
	subtilis_arm_ccode_type_t ccode = SUBTILIS_ARM_CCODE_AL;
	bool status = false;
	bool byte = false;
	size_t max_length = 0;
	size_t max_index = SIZE_MAX;

	tbuf = subtilis_token_get_text(c->t);
	if (tbuf[0] < 'A' || tbuf[0] > 'Z') {
		prv_parse_label(c, tbuf, err);
		return;
	}

	index = tbuf[0] - 'A';
	if (!keyword_map[index].mnems) {
		prv_parse_label(c, tbuf, err);
		return;
	}

	for (i = 0; i < keyword_map[index].count; i++) {
		search_len = strlen(keyword_map[index].mnems[i].name);
		if (search_len > keyword_map[index].max_length)
			search_len = keyword_map[index].max_length;
		if (!strncmp(keyword_map[index].mnems[i].name, tbuf,
			     search_len)) {
			base_len = strlen(keyword_map[index].mnems[i].name);
			if (base_len > max_length) {
				max_length = base_len;
				max_index = i;
			}
		}
	}

	if (max_index == SIZE_MAX) {
		prv_parse_label(c, tbuf, err);
		return;
	}

	/*
	 * It might still be an identifier
	 */

	itype = keyword_map[index].mnems[max_index].type;
	status_valid = keyword_map[index].mnems[max_index].status_valid;
	byte_valid =
	    itype == SUBTILIS_ARM_INSTR_LDR || itype == SUBTILIS_ARM_INSTR_STR;
	ptr = max_length;
	token_end = strlen(tbuf);
	if (ptr == token_end) {
		if ((itype == SUBTILIS_ARM_INSTR_LDM) ||
		    (itype == SUBTILIS_ARM_INSTR_STM))
			prv_parse_label(c, tbuf, err);
		else
			prv_parse_instruction(c, tbuf, itype, ccode, status,
					      mtran_type, byte, err);
		return;
	}

	if (ptr + 2 <= token_end) {
		for (i = 0; i < sizeof(condition_codes) / sizeof(const char *);
		     i++)
			if ((tbuf[ptr] == condition_codes[i][0]) &&
			    (tbuf[ptr + 1] == condition_codes[i][1]))
				break;

		if (i < sizeof(condition_codes) / sizeof(const char *)) {
			ptr += 2;
			ccode = (subtilis_arm_ccode_type_t)i;
		}
	}

	if ((ptr + 2 == token_end) && ((itype == SUBTILIS_ARM_INSTR_LDM) ||
				       (itype == SUBTILIS_ARM_INSTR_STM))) {
		for (i = 0; i < sizeof(mtran_types) / sizeof(mtran_types[0]);
		     i++)
			if (!strcmp(&tbuf[ptr], mtran_types[i]))
				break;

		if (i < sizeof(mtran_types) / sizeof(mtran_types[0])) {
			mtran_type = (subtilis_arm_mtran_type_t)i;
			ptr += 2;
		}
	}

	if (ptr + 1 == token_end) {
		status = tbuf[ptr] == 'S' && status_valid;
		byte = tbuf[ptr] == 'B' && byte_valid;
		if (!status && !byte) {
			prv_parse_label(c, tbuf, err);
			return;
		}
	} else if (ptr != token_end) {
		prv_parse_label(c, tbuf, err);
		return;
	}

	prv_parse_instruction(c, tbuf, itype, ccode, status, mtran_type, byte,
			      err);
}

typedef bool (*subtilis_arm_ass_valid_int_fn_t)(int32_t);

static int32_t *prv_get_uint32_list(subtilis_arm_ass_context_t *c,
				    size_t *count,
				    subtilis_arm_ass_valid_int_fn_t fn,
				    subtilis_error_t *err)
{
	const char *tbuf;
	int32_t *new_nums;
	int32_t num;
	int32_t *nums = NULL;
	size_t max_nums = 0;
	size_t num_count = 0;
	subtilis_arm_exp_val_t *val = NULL;

	do {
		val = subtilis_arm_exp_val_get(c, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;

		if (val->type == SUBTILIS_ARM_EXP_TYPE_INT) {
			num = val->val.integer;
		} else if (val->type == SUBTILIS_ARM_EXP_TYPE_REAL) {
			num = (int32_t)val->val.real;
		} else {
			subtilis_error_set_integer_expected(
			    err, subtilis_arm_exp_type_name(val),
			    c->l->stream->name, c->l->line);
			goto cleanup;
		}

		subtilis_arm_exp_val_free(val);
		val = NULL;

		if (fn && !fn(num)) {
			subtilis_error_set_ass_integer_too_big(
			    err, c->t->tok.integer, c->l->stream->name,
			    c->l->line);
			goto cleanup;
		}

		if (num_count == max_nums) {
			max_nums += 32;
			new_nums = realloc(nums, max_nums * sizeof(*nums));
			if (!new_nums)
				goto cleanup;
			nums = new_nums;
		}

		nums[num_count++] = num;
		tbuf = subtilis_token_get_text(c->t);
	} while ((c->t->type == SUBTILIS_TOKEN_OPERATOR) && !strcmp(tbuf, ","));

	*count = num_count;

	return nums;

cleanup:

	subtilis_arm_exp_val_free(val);
	free(nums);

	return NULL;
}

static bool prv_check_equb(int32_t i) { return !(i < -128 || i > 255); }

static void prv_parse_equb(subtilis_arm_ass_context_t *c, subtilis_error_t *err)
{
	int32_t *nums;
	size_t num_count;
	size_t i;

	nums = prv_get_uint32_list(c, &num_count, prv_check_equb, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	for (i = 0; i < num_count; i++) {
		subtilis_arm_add_byte(c->arm_s, (uint8_t)nums[i], err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
	}

cleanup:

	free(nums);
}

static bool prv_check_equw(int32_t i) { return !(i < -32768 || i > 0xffff); }

static void prv_parse_equw(subtilis_arm_ass_context_t *c, subtilis_error_t *err)
{
	int32_t *nums;
	size_t num_count;
	size_t i;

	nums = prv_get_uint32_list(c, &num_count, prv_check_equw, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	for (i = 0; i < num_count; i++) {
		subtilis_arm_add_two_bytes(c->arm_s, (uint16_t)nums[i], err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
	}

cleanup:

	free(nums);
}

static void prv_parse_equd(subtilis_arm_ass_context_t *c, subtilis_error_t *err)
{
	int32_t *nums;
	size_t num_count;
	size_t i;

	nums = prv_get_uint32_list(c, &num_count, NULL, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	for (i = 0; i < num_count; i++) {
		subtilis_arm_add_four_bytes(c->arm_s, nums[i], err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
	}

cleanup:

	free(nums);
}

static double *prv_get_double_list(subtilis_arm_ass_context_t *c, size_t *count,
				   subtilis_error_t *err)
{
	const char *tbuf;
	double *new_nums;
	double num;
	double *nums = NULL;
	size_t max_nums = 0;
	size_t num_count = 0;
	subtilis_arm_exp_val_t *val = NULL;

	do {
		val = subtilis_arm_exp_val_get(c, err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;

		if (val->type == SUBTILIS_ARM_EXP_TYPE_INT) {
			num = (double)val->val.integer;
		} else if (val->type == SUBTILIS_ARM_EXP_TYPE_REAL) {
			num = val->val.real;
		} else {
			subtilis_error_set_numeric_expected(
			    err, subtilis_arm_exp_type_name(val),
			    c->l->stream->name, c->l->line);
			goto cleanup;
		}

		subtilis_arm_exp_val_free(val);
		val = NULL;

		if (num_count == max_nums) {
			max_nums += 32;
			new_nums = realloc(nums, max_nums * sizeof(*nums));
			if (!new_nums)
				goto cleanup;
			nums = new_nums;
		}

		nums[num_count++] = num;
		tbuf = subtilis_token_get_text(c->t);
	} while ((c->t->type == SUBTILIS_TOKEN_OPERATOR) && !strcmp(tbuf, ","));

	*count = num_count;

	return nums;

cleanup:

	subtilis_arm_exp_val_free(val);
	free(nums);

	return NULL;
}

static void prv_parse_equdbl(subtilis_arm_ass_context_t *c,
			     subtilis_error_t *err)
{
	double *nums;
	size_t num_count;
	size_t i;

	nums = prv_get_double_list(c, &num_count, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	for (i = 0; i < num_count; i++) {
		subtilis_arm_add_double(c->arm_s, nums[i], err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
	}

cleanup:

	free(nums);
}

static void prv_parse_equdblr(subtilis_arm_ass_context_t *c,
			      subtilis_error_t *err)
{
	double *nums;
	size_t num_count;
	size_t i;

	nums = prv_get_double_list(c, &num_count, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	for (i = 0; i < num_count; i++) {
		subtilis_arm_add_doubler(c->arm_s, nums[i], err);
		if (err->type != SUBTILIS_ERROR_OK)
			goto cleanup;
	}

cleanup:

	free(nums);
}

static void prv_parse_equs(subtilis_arm_ass_context_t *c, subtilis_error_t *err)
{
	subtilis_arm_exp_val_t *val;
	const char *str;

	val = subtilis_arm_exp_val_get(c, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	if (val->type != SUBTILIS_ARM_EXP_TYPE_STRING) {
		subtilis_error_set_expected(err, "string",
					    subtilis_arm_exp_type_name(val),
					    c->l->stream->name, c->l->line);
		goto cleanup;
	}

	str = subtilis_buffer_get_string(&val->val.buf);
	subtilis_arm_add_string(c->arm_s, str, err);

cleanup:

	subtilis_arm_exp_val_free(val);
}

static void prv_parse_keyword(subtilis_arm_ass_context_t *c, const char *name,
			      const subtilis_token_keyword_t *keyword,
			      subtilis_error_t *err)
{
	switch (keyword->type) {
	case SUBTILIS_ARM_KEYWORD_EQUB:
		prv_parse_equb(c, err);
		break;
	case SUBTILIS_ARM_KEYWORD_EQUD:
		prv_parse_equd(c, err);
		break;
	case SUBTILIS_ARM_KEYWORD_EQUDBL:
		prv_parse_equdbl(c, err);
		break;
	case SUBTILIS_ARM_KEYWORD_EQUDBLR:
		prv_parse_equdblr(c, err);
		break;
	case SUBTILIS_ARM_KEYWORD_EQUS:
		prv_parse_equs(c, err);
		break;
	case SUBTILIS_ARM_KEYWORD_EQUW:
		prv_parse_equw(c, err);
		break;
	default:
		subtilis_error_set_ass_keyword_bad_use(
		    err, name, c->l->stream->name, c->l->line);
		break;
	}
}

static void prv_parser_main_loop(subtilis_arm_ass_context_t *c,
				 subtilis_error_t *err)
{
	const char *tbuf;

	subtilis_lexer_get(c->l, c->t, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return;

	tbuf = subtilis_token_get_text(c->t);
	while ((c->t->type != SUBTILIS_TOKEN_OPERATOR) && (strcmp(tbuf, "]"))) {
		switch (c->t->type) {
		case SUBTILIS_TOKEN_KEYWORD:
			prv_parse_keyword(c, tbuf, &c->t->tok.keyword, err);
			break;
		case SUBTILIS_TOKEN_IDENTIFIER:
			prv_parse_identifier(c, err);
			break;
		default:
			subtilis_error_set_expected(
			    err, "keyword or identifier", tbuf,
			    c->l->stream->name, c->l->line);
			return;
		}

		if (err->type != SUBTILIS_ERROR_OK)
			return;

		tbuf = subtilis_token_get_text(c->t);
	}
}

/* clang-format off */
subtilis_arm_section_t *subtilis_arm_asm_parse(
	subtilis_lexer_t *l, subtilis_token_t *t, subtilis_arm_op_pool_t *pool,
	subtilis_type_section_t *stype, const subtilis_settings_t *set,
	subtilis_backend_sys_trans sys_trans, subtilis_error_t *err)
/* clang-format on */
{
	subtilis_arm_ass_context_t context;
	size_t *pending_labels;
	size_t pending_labels_num;
	const char *label_name;
	subtilis_arm_section_t *arm_s = NULL;
	subtilis_string_pool_t *label_pool = NULL;

	label_pool = subtilis_string_pool_new(err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	arm_s = subtilis_arm_section_new(pool, stype, 0, 0, 0, 0, set, err);
	if (err->type != SUBTILIS_ERROR_OK)
		return NULL;

	context.arm_s = arm_s;
	context.l = l;
	context.t = t;
	context.stype = stype;
	context.set = set;
	context.sys_trans = sys_trans;
	context.label_pool = label_pool;
	subtilis_bitset_init(&context.pending_labels);

	prv_parser_main_loop(&context, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	pending_labels = subtilis_bitset_values(&context.pending_labels,
						&pending_labels_num, err);
	if (err->type != SUBTILIS_ERROR_OK)
		goto cleanup;

	if (pending_labels_num > 0) {
		if (pending_labels[0] > label_pool->length) {
			subtilis_error_set_assertion_failed(err);
			goto cleanup;
		}
		label_name = label_pool->strings[pending_labels[0]];
		subtilis_error_set_ass_unknown_label(err, label_name,
						     l->stream->name, l->line);
		goto cleanup;
	}

	subtilis_bitset_free(&context.pending_labels);
	subtilis_string_pool_delete(label_pool);

	return arm_s;

cleanup:
	subtilis_bitset_free(&context.pending_labels);
	subtilis_arm_section_delete(arm_s);
	subtilis_string_pool_delete(label_pool);

	return NULL;
}
