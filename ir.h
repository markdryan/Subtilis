/*
 * Copyright (c) 2017 Mark Ryan
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

#ifndef __SUBTILIS_IR_H
#define __SUBTILIS_IR_H

#include <stdint.h>

#include "buffer.h"
#include "error.h"
#include "string_pool.h"
#include "type.h"

/* clang-format off */
enum {
	SUBTILIS_IR_REG_UNDEFINED,
	SUBTILIS_IR_REG_GLOBAL,
	SUBTILIS_IR_REG_LOCAL,
	SUBTILIS_IR_REG_STACK,
	SUBTILIS_IR_REG_TEMP_START,
};

/* clang-format on */

typedef enum {
	SUBTILIS_OP_INSTR,
	SUBTILIS_OP_LABEL,
	SUBTILIS_OP_PHI,
	SUBTILIS_OP_MAX,
} subtilis_op_type_t;

/* IMPORTANT.
 * Don't change the order of these enums.  There are static arrays
 * in ir.c and vm.c that rely on this ordering.
 */

typedef enum {
	/*
	 * addi32 r0, r1, r2
	 *
	 * Adds two 32 bit signed integers stored in registers and stores the
	 * result in another register.
	 *
	 * r0 = r1 + r2
	 */
	SUBTILIS_OP_INSTR_ADD_I32,

	/*
	 * addr fp0, fp1, fp2
	 *
	 * Adds two 64 bit doubles stored in floating point registers and stores
	 * the result in a third floating point register.
	 *
	 * fp0 = fp1 + fp2
	 */
	SUBTILIS_OP_INSTR_ADD_REAL,

	/*
	 * subi32 r0, r1, r2
	 *
	 * Subtracts two 32 bit integers stored in registers storing the result
	 * in a third register.
	 *
	 * r0 = r1 - r2
	 */

	SUBTILIS_OP_INSTR_SUB_I32,

	/*
	 * subr fp0, fp1, fp2
	 *
	 * Subtracts two 64 bit doubles stored in floating pointers registers
	 * storing the result in a third floating point register.
	 *
	 * fp0 = fp1 - fp2
	 */
	SUBTILIS_OP_INSTR_SUB_REAL,

	/*
	 * muli32 r0, r1, r2
	 *
	 * Multiples two 32 bit signed integers stored in registers storing the
	 * result in a third register.
	 *
	 * r0 = r1 * r2
	 */

	SUBTILIS_OP_INSTR_MUL_I32,

	/*
	 * mulr fp0, fp1, fp2
	 *
	 * Multiplies two 64 bit doubles stored in floating point registers
	 * storing the result in a third floating point register.
	 *
	 * fp0 = fp1 * fp2
	 */

	SUBTILIS_OP_INSTR_MUL_REAL,

	/*
	 * divi32 r0, r1, r2
	 *
	 * Divides two 32 bit signed integers stored in registers storing the
	 * result in a third register.
	 *
	 * r0 = r1 / r2
	 */

	SUBTILIS_OP_INSTR_DIV_I32,

	/*
	 * divr fp0, fp1, fp2
	 *
	 * Divides two 64 bit doubles stored in floating point registers.
	 * The result is stored in a third floating point register.
	 *
	 * fp0 = fp1 / fp2
	 */

	SUBTILIS_OP_INSTR_DIV_REAL,

	/*
	 * addii32 r0, r1, #i32
	 *
	 * Adds a 32 bit immediate constant to a 32 bit integer stored
	 * in a register.  The result is stored in a second register.
	 *
	 * r0 = r1 + #i32
	 */

	SUBTILIS_OP_INSTR_ADDI_I32,

	/*
	 * addir fp0, fp1, #r
	 *
	 * Adds a 64 bit double immediate constant to a 64 bit double
	 * stored in a floating point register.  The result is stored
	 * in a second floating point register.
	 *
	 * fp0 = fp1 + #r
	 */

	SUBTILIS_OP_INSTR_ADDI_REAL,

	/*
	 * subii32 r0, r1, #i32
	 *
	 * Subtracts a 32 bit integer immediate constant from a 32 bit
	 * integer stored in a register.  The result is stored in a second
	 * register.
	 *
	 * r0 = r1 - #i32
	 */

	SUBTILIS_OP_INSTR_SUBI_I32,

	/*
	 * subir fp0, fp1, #r
	 *
	 * Subtracts a 64 bit double immediate constant from a 64 bit
	 * double stored in a register.  The result is stored in a second
	 * register.
	 *
	 * fp0 = fp1 - #r
	 */

	SUBTILIS_OP_INSTR_SUBI_REAL,

	/*
	 * mulii32 r0, r1, #i32
	 *
	 * Multiplies a 32 bit immediate constant by a 32 bit integer
	 * stored in a register.  The resulting product is stored in
	 * a another register.
	 *
	 * r0 = r1 * #i32
	 */

	SUBTILIS_OP_INSTR_MULI_I32,

	/*
	 * mulir fp0, fp1, #r
	 *
	 * Multiplies a 64 bit double immediate constant by a 64 bit
	 * double stored in a register.  The result is placed in a
	 * second floating point register.
	 *
	 * fp0 = fp1 * r
	 */

	SUBTILIS_OP_INSTR_MULI_REAL,

	/*
	 * divii32 r0, r1, #i32
	 *
	 *- Divides a 32 bit integer stored in a register by a 32 bit
	 * integer immediate constant.  The result is stored in a second
	 * register.
	 *
	 * r0 = r1 / #i32
	 */

	SUBTILIS_OP_INSTR_DIVI_I32,

	/*
	 * divir fp0, fp1, #r
	 *
	 * Divides a 64 bit double stored in a register by 64 bit double
	 * immediate constant.  The result is stored in a second floating
	 * point register.
	 *
	 * fp0 = fp1 / #r
	 */

	SUBTILIS_OP_INSTR_DIVI_REAL,

	/*
	 * loadoi32 r0, r1, #off
	 *
	 * Loads a 32 bit integer from a memory location formed by
	 * the sum of the contents of a register and a constant into another
	 * register.
	 *
	 * r0 = [r1 + #off]
	 */

	SUBTILIS_OP_INSTR_LOADO_I32,

	/*
	 * loador fp0, r0, #off
	 *
	 * Loads a 64 bit double from a memory location formed by
	 * the sum of the contents of a register and a constant.stored in a
	 * register into a floating pointer register.
	 *
	 * fp0 = [r0 + #off]
	 */

	SUBTILIS_OP_INSTR_LOADO_REAL,

	/*
	 * loadi32 r0, r1
	 *
	 * Loads a 32 bit integer from a memory location stored in a
	 * register into another register.
	 *
	 * r0 = [r1]
	 */

	SUBTILIS_OP_INSTR_LOAD_I32,

	/*
	 * loadr fp0, [r0]
	 *
	 * Loads a 64 bit double from a memory location stored in a
	 * register into a floating point register.
	 *
	 * fp0 = [r0]
	 */

	SUBTILIS_OP_INSTR_LOAD_REAL,

	/*
	 * storeoi32 r0, r1, #off
	 *
	 * Stores a 32 bit integer stored in a register into the
	 * memory location defined by the sum of a register and a constant.
	 *
	 * [r1 + #off] = r0
	 */

	SUBTILIS_OP_INSTR_STOREO_I32,

	/*
	 * storeor fp0, r1, #off
	 *
	 * Stores a 64 bit double stored in a floating point register into
	 * the  memory location defined by the sum of a register and a constant.
	 *
	 * [r1 + off] = fp0
	 */

	SUBTILIS_OP_INSTR_STOREO_REAL,

	/*
	 * storei32 r0, r1
	 *
	 * Stores a 32 bit integer contained in a register into a
	 * memory location contained within a second register.
	 *
	 * [r1] = r0
	 */

	SUBTILIS_OP_INSTR_STORE_I32,

	/*
	 * storer fp0, r1
	 *
	 * Stores a 64 bit double  contained in a register into a memory
	 * location contained within a register.
	 *
	 * [r1] = fp0
	 */

	SUBTILIS_OP_INSTR_STORE_REAL,

	/*
	 * movii32 r0, #i32
	 *
	 * Moves a 32 bit integer constant into a register.
	 *
	 * r0 = #i32
	 */

	SUBTILIS_OP_INSTR_MOVI_I32,

	/*
	 * movir fp0, #r
	 *
	 * Moves a 64 bit floating point constant into a floating point
	 * register.
	 *
	 * fp0 = r
	 */

	SUBTILIS_OP_INSTR_MOV_REAL,

	/*
	 * mov r0, r1
	 *
	 * Moves the contents of one 32 bit integer register into another.
	 *
	 * r0 = r1
	 */

	SUBTILIS_OP_INSTR_MOV,

	/*
	 * movfp fp0, fp1
	 *
	 * Moves the contents of one floating point register into another.
	 *
	 * fp0 = fp1
	 */

	SUBTILIS_OP_INSTR_MOVFP,

	/*
	 * printi32 r0
	 *
	 * Prints the 32 bit integer stored in r0 to the output stream.
	 */

	SUBTILIS_OP_INSTR_PRINT_I32,

	/*
	 * rsubii32 r0, r1, #i32
	 *
	 * Subtracts a 32 bit integer stored in a register from a
	 * 32 bit integer immediate constant.  The result is stored in a second
	 * register.
	 *
	 * r0 = #i32 - r1
	 */

	SUBTILIS_OP_INSTR_RSUBI_I32,

	/*
	 * rsubir fp0, fp1, #r
	 *
	 * Subtracts a 64 bit double stored in a register from a 64 bit double
	 * immediate constant.  The result is stored in a second register.
	 *
	 * fp0 = #r - fp1
	 */

	SUBTILIS_OP_INSTR_RSUBI_REAL,

	/*
	 * rdivii32 r0, r1, #i32
	 *
	 * Divides a 32 bit integer immediate constant by a 32 bit integer
	 * stored in a register.  The result is stored in a second register.
	 *
	 * r0 = #i32 / r1
	 */

	SUBTILIS_OP_INSTR_RDIVI_I32,

	/*
	 * rdivir fp0, fp1, #r
	 *
	 * Divides a 64 bit double immediate constant by a 64 bit double
	 * stored in a register.  The result is stored in a second register.
	 *
	 * fp0 = #r / fp1
	 */

	SUBTILIS_OP_INSTR_RDIVI_REAL,

	/*
	 * andi32 r0, r1, r2
	 *
	 * Ands two 32 bit signed integers stored in registers and stores the
	 * result in another register.
	 *
	 * r0 = r1 & r2
	 */
	SUBTILIS_OP_INSTR_AND_I32,

	/*
	 * andii32 r0, r1, #i32
	 *
	 * Ands a 32 bit immediate constant with a 32 bit integer stored
	 * in a register.  The result is stored in a second register.
	 *
	 * r0 = r1 & #i32
	 */

	SUBTILIS_OP_INSTR_ANDI_I32,

	/*
	 * ori32 r0, r1, r2
	 *
	 * Ors two 32 bit signed integers stored in registers and stores the
	 * result in another register.
	 *
	 * r0 = r1 | r2
	 */
	SUBTILIS_OP_INSTR_OR_I32,

	/*
	 * orii32 r0, r1, #i32
	 *
	 * Ors a 32 bit immediate constant with a 32 bit integer stored
	 * in a register.  The result is stored in a second register.
	 *
	 * r0 = r1 | #i32
	 */

	SUBTILIS_OP_INSTR_ORI_I32,

	/*
	 * eori32 r0, r1, r2
	 *
	 * Eors two 32 bit signed integers stored in registers and stores the
	 * result in another register.
	 *
	 * r0 = r1 ^ r2
	 */
	SUBTILIS_OP_INSTR_EOR_I32,

	/*
	 * eorii32 r0, r1, #i32
	 *
	 * Eors a 32 bit immediate constant with a 32 bit integer stored
	 * in a register.  The result is stored in a second register.
	 *
	 * r0 = r1 ^ #i32
	 */

	SUBTILIS_OP_INSTR_EORI_I32,

	/*
	 * noti32 r0, r1
	 *
	 * Stores the complement of a 32 bit integer stored in a register
	 * in a second register.
	 *
	 * r0 = ~r1
	 */

	SUBTILIS_OP_INSTR_NOT_I32,

	/*
	 * eqi32 r0, r1, r2
	 *
	 * Compares the 32 bit integers in r1 and r2, storing -1 in R0
	 * if they are equal, and 0 otherwise.
	 *
	 * r0 = r1 == r2 ? -1 : 0
	 */

	SUBTILIS_OP_INSTR_EQ_I32,

	/*
	 * eqii32 r0, r1, i32
	 *
	 * Compares the 32 bit integers in r1 with a 32 bit immediate
	 * constant, storing -1 in R0 if they are equal, and 0 otherwise.
	 *
	 * r0 = r1 == i32 ? -1 : 0
	 */

	SUBTILIS_OP_INSTR_EQI_I32,

	/*
	 * neqi32 r0, r1, r2
	 *
	 * Compares the 32 bit integers in r1 and r2, storing -1 in R0
	 * if they are not equal, and 0 otherwise.
	 *
	 * r0 = r1 != r2 ? -1 : 0
	 */

	SUBTILIS_OP_INSTR_NEQ_I32,

	/*
	 * neqii32 r0, r1, i32
	 *
	 * Compares the 32 bit integers in r1 with a 32 bit immediate
	 * constant, storing -1 in R0 if they are not equal, and 0 otherwise.
	 *
	 * r0 = r1 != i32 ? -1 : 0
	 */

	SUBTILIS_OP_INSTR_NEQI_I32,

	/*
	 * gti32 r0, r1, r2
	 *
	 * Compares the 32 bit integers in r1 and r2, storing -1 in
	 * r0 if r1 > r2 and 0 otherwise.
	 *
	 * r0 = r1 > r2 ? -1 : 0
	 */

	SUBTILIS_OP_INSTR_GT_I32,

	/*
	 * gtii32 r0, r1, i32
	 *
	 * Compares the 32 bit integers in r1 with a 32 bit immediate
	 * constant, storing -1 in r0 if the value in r1 is greater than
	 * the immediate constant and 0 otherwise.
	 *
	 * r0 = r1 > i32 ? -1 : 0
	 */

	SUBTILIS_OP_INSTR_GTI_I32,

	/*
	 * ltei32 r0, r1, r2
	 *
	 * Compares the 32 bit integers in r1 and r2, storing -1 in
	 * r0 if r1 <= r2 and 0 otherwise.
	 *
	 * r0 = r1 <= r2 ? -1 : 0
	 */

	SUBTILIS_OP_INSTR_LTE_I32,

	/*
	 * lteii32 r0, r1, i32
	 *
	 * Compares the 32 bit integers in r1 with a 32 bit immediate
	 * constant, storing -1 in r0 if the value in r1 is less than
	 * or equal to the immediate constant and 0 otherwise.
	 *
	 * r0 = r1 <= i32 ? -1 : 0
	 */

	SUBTILIS_OP_INSTR_LTEI_I32,

	/*
	 * lti32 r0, r1, r2
	 *
	 * Compares the 32 bit integers in r1 and r2, storing -1 in
	 * r0 if r1 < r2 and 0 otherwise.
	 *
	 * r0 = r1 < r2 ? -1 : 0
	 */

	SUBTILIS_OP_INSTR_LT_I32,

	/*
	 * ltii32 r0, r1, i32
	 *
	 * Compares the 32 bit integers in r1 with a 32 bit immediate
	 * constant, storing -1 in r0 if the value in r1 is less than
	 * the immediate constant and 0 otherwise.
	 *
	 * r0 = r1 < i32 ? -1 : 0
	 */

	SUBTILIS_OP_INSTR_LTI_I32,

	/*
	 * gtei32 r0, r1, r2
	 *
	 * Compares the 32 bit integers in r1 and r2, storing -1 in
	 * r0 if r1 >= r2 and 0 otherwise.
	 *
	 * r0 = r1 >= r2 ? -1 : 0
	 */

	SUBTILIS_OP_INSTR_GTE_I32,

	/*
	 * gteii32 r0, r1, i32
	 *
	 * Compares the 32 bit integers in r1 with a 32 bit immediate
	 * constant, storing -1 in r0 if the value in r1 is greater than
	 * or equal to the immediate constant and 0 otherwise.
	 *
	 * r0 = r1 >= i32 ? -1 : 0
	 */

	SUBTILIS_OP_INSTR_GTEI_I32,

	/*
	 * JMPC r0, label1, label2
	 *
	 * Jumps to label1 if the value of r0 is non zero and jumps to label2
	 * otherwise.
	 *
	 */

	SUBTILIS_OP_INSTR_JMPC,

	/*
	 * JMPC label1
	 *
	 * Unconditionally Jumps to label1.
	 *
	 */

	SUBTILIS_OP_INSTR_JMP,
} subtilis_op_instr_type_t;

typedef enum {
	SUBTILIS_OP_CLASS_REG_REG_REG,
	SUBTILIS_OP_CLASS_REG_REG_I32,
	SUBTILIS_OP_CLASS_REG_REG_REAL,
	SUBTILIS_OP_CLASS_REG_LABEL_LABEL,
	SUBTILIS_OP_CLASS_REG_I32,
	SUBTILIS_OP_CLASS_REG_REAL,
	SUBTILIS_OP_CLASS_REG_REG,
	SUBTILIS_OP_CLASS_REG,
	SUBTILIS_OP_CLASS_LABEL,
} subtilis_op_instr_class_t;

// TODO: Need a type for pointer offsets.  These may not always
// be 32bit integers.

union subtilis_ir_operand_t_ {
	int32_t integer;
	double real;
	size_t reg;
	size_t label;
};

typedef union subtilis_ir_operand_t_ subtilis_ir_operand_t;

struct subtilis_ir_inst_t_ {
	subtilis_op_instr_type_t type;
	subtilis_ir_operand_t operands[3];
};

typedef struct subtilis_ir_inst_t_ subtilis_ir_inst_t;

struct subtilis_ir_op_t_ {
	subtilis_op_type_t type;
	union {
		subtilis_ir_inst_t instr;
		size_t label;
	} op;
};

typedef struct subtilis_ir_op_t_ subtilis_ir_op_t;

struct subtilis_ir_section_t_ {
	subtilis_type_section_t *type;
	size_t reg_counter;
	size_t label_counter;
	size_t len;
	size_t max_len;
	subtilis_ir_op_t **ops;
};

typedef struct subtilis_ir_section_t_ subtilis_ir_section_t;

struct subtilis_ir_prog_t_ {
	subtilis_ir_section_t **sections;
	size_t num_sections;
	size_t max_sections;
	subtilis_string_pool_t *string_pool;
};

typedef struct subtilis_ir_prog_t_ subtilis_ir_prog_t;

/* TODO: Matcher needs to move to its own file */

typedef enum {
	SUBTILIS_OP_MATCH_ANY,
	SUBTILIS_OP_MATCH_FIXED,
	SUBTILIS_OP_MATCH_FLOATING,
} subtilis_op_match_type_t;

struct subtilis_ir_inst_match_t_ {
	subtilis_op_instr_type_t type;
	subtilis_ir_operand_t operands[3];
	subtilis_op_match_type_t op_match[3];
};

typedef struct subtilis_ir_inst_match_t_ subtilis_ir_inst_match_t;

struct subtilis_ir_label_match_t_ {
	size_t label;
	subtilis_op_match_type_t op_match;
};

typedef struct subtilis_ir_label_match_t_ subtilis_ir_label_match_t;

struct subtilis_ir_op_match_t_ {
	subtilis_op_type_t type;
	union {
		subtilis_ir_inst_match_t instr;
		subtilis_ir_label_match_t label;
	} op;
};

typedef struct subtilis_ir_op_match_t_ subtilis_ir_op_match_t;

typedef void (*subtilis_ir_action_t)(subtilis_ir_section_t *s, size_t start,
				     void *user_data, subtilis_error_t *err);

#define SUBTILIS_IR_MAX_MATCHES 5

struct subtilis_ir_rule_t_ {
	subtilis_ir_op_match_t matches[SUBTILIS_IR_MAX_MATCHES];
	size_t matches_count;
	subtilis_ir_action_t action;
};

typedef struct subtilis_ir_rule_t_ subtilis_ir_rule_t;

struct subtilis_ir_rule_raw_t_ {
	const char *text;
	subtilis_ir_action_t action;
};

typedef struct subtilis_ir_rule_raw_t_ subtilis_ir_rule_raw_t;

subtilis_ir_prog_t *subtilis_ir_prog_new(subtilis_error_t *err);
subtilis_ir_section_t *subtilis_ir_prog_section_new(subtilis_ir_prog_t *p,
						    const char *name,
						    subtilis_type_section_t *tp,
						    subtilis_error_t *err);
subtilis_ir_section_t *subtilis_ir_prog_find_section(subtilis_ir_prog_t *p,
						     const char *name);
void subtilis_ir_prog_dump(subtilis_ir_prog_t *p);
void subtilis_ir_prog_delete(subtilis_ir_prog_t *p);
size_t subtilis_ir_section_add_instr(subtilis_ir_section_t *s,
				     subtilis_op_instr_type_t type,
				     subtilis_ir_operand_t op1,
				     subtilis_ir_operand_t op2,
				     subtilis_error_t *err);
size_t subtilis_ir_section_add_instr2(subtilis_ir_section_t *s,
				      subtilis_op_instr_type_t type,
				      subtilis_ir_operand_t op1,
				      subtilis_error_t *err);
void subtilis_ir_section_add_instr_no_reg(subtilis_ir_section_t *s,
					  subtilis_op_instr_type_t type,
					  subtilis_ir_operand_t op1,
					  subtilis_error_t *err);
void subtilis_ir_section_add_instr_reg(subtilis_ir_section_t *s,
				       subtilis_op_instr_type_t type,
				       subtilis_ir_operand_t op0,
				       subtilis_ir_operand_t op1,
				       subtilis_ir_operand_t op2,
				       subtilis_error_t *err);
void subtilis_ir_section_dump(subtilis_ir_section_t *s);
size_t subtilis_ir_section_new_label(subtilis_ir_section_t *s);
void subtilis_ir_section_add_label(subtilis_ir_section_t *s, size_t l,
				   subtilis_error_t *err);
void subtilis_ir_parse_rules(const subtilis_ir_rule_raw_t *raw,
			     subtilis_ir_rule_t *parsed, size_t count,
			     subtilis_error_t *err);
void subtilis_ir_match(subtilis_ir_section_t *s, subtilis_ir_rule_t *rules,
		       size_t rule_count, void *user_data,
		       subtilis_error_t *err);

#endif
