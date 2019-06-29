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

#ifndef __SUBTILIS_TEST_CASES_H
#define __SUBTILIS_TEST_CASES_H

struct subtilis_test_case_t_ {
	const char *name;
	const char *source;
	const char *result;
};

typedef struct subtilis_test_case_t_ subtilis_test_case_t;

typedef enum {
	SUBTILIS_TEST_CASE_ID_SUBTRACTION,
	SUBTILIS_TEST_CASE_ID_DIVISION,
	SUBTILIS_TEST_CASE_ID_MODULO,
	SUBTILIS_TEST_CASE_ID_MULTIPLICATION,
	SUBTILIS_TEST_CASE_ID_ADDITION,
	SUBTILIS_TEST_CASE_ID_NESTED_EXP,
	SUBTILIS_TEST_CASE_ID_UNARY_MINUS,
	SUBTILIS_TEST_CASE_ID_AND,
	SUBTILIS_TEST_CASE_ID_OR,
	SUBTILIS_TEST_CASE_ID_EOR,
	SUBTILIS_TEST_CASE_ID_NOT,
	SUBTILIS_TEST_CASE_ID_EQ,
	SUBTILIS_TEST_CASE_ID_NEQ,
	SUBTILIS_TEST_CASE_ID_GT,
	SUBTILIS_TEST_CASE_ID_LTE,
	SUBTILIS_TEST_CASE_ID_LT,
	SUBTILIS_TEST_CASE_ID_GTE,
	SUBTILIS_TEST_CASE_ID_IF,
	SUBTILIS_TEST_CASE_ID_WHILE,
	SUBTILIS_TEST_CASE_ID_IF_AND_WHILE,
	SUBTILIS_TEST_CASE_ID_EQ_AND_NEQ_BR_IMM,
	SUBTILIS_TEST_CASE_ID_EQ_AND_NEQ_BR,
	SUBTILIS_TEST_CASE_ID_BASIC_PROC,
	SUBTILIS_TEST_CASE_ID_LOCAL_PROC,
	SUBTILIS_TEST_CASE_ID_PROC_ARGS,
	SUBTILIS_TEST_CASE_ID_FN_FACT,
	SUBTILIS_TEST_CASE_ID_FN_FACT_NO_LET,
	SUBTILIS_TEST_CASE_ID_ABS,
	SUBTILIS_TEST_CASE_ID_FPA_SMALL,
	SUBTILIS_TEST_CASE_ID_FPA_LOGICAL,
	SUBTILIS_TEST_CASE_ID_FPA_IF,
	SUBTILIS_TEST_CASE_ID_FPA_REPEAT,
	SUBTILIS_TEST_CASE_ID_INT_REG_ALLOC_BASIC,
	SUBTILIS_TEST_CASE_ID_FPA_REG_ALLOC_BASIC,
	SUBTILIS_TEST_CASE_ID_FPA_SAVE,
	SUBTILIS_TEST_CASE_ID_INT_SAVE,
	SUBTILIS_TEST_CASE_ID_BRANCH_SAVE,
	SUBTILIS_TEST_CASE_ID_TIME,
	SUBTILIS_TEST_CASE_ID_SIN_COS,
	SUBTILIS_TEST_CASE_ID_PI,
	SUBTILIS_TEST_CASE_ID_SQR,
	SUBTILIS_TEST_CASE_ID_MIXED_ARGS,
	SUBTILIS_TEST_CASE_ID_VDU,
	SUBTILIS_TEST_CASE_ID_VOID_FN,
	SUBTILIS_TEST_CASE_ID_ASSIGN_FN,
	SUBTILIS_TEST_CASE_ID_FOR_BASIC_INT,
	SUBTILIS_TEST_CASE_ID_FOR_BASIC_REAL,
	SUBTILIS_TEST_CASE_ID_FOR_STEP_INT_CONST,
	SUBTILIS_TEST_CASE_ID_FOR_STEP_REAL_CONST,
	SUBTILIS_TEST_CASE_ID_FOR_STEP_INT_VAR,
	SUBTILIS_TEST_CASE_ID_FOR_STEP_REAL_VAR,
	SUBTILIS_TEST_CASE_ID_FOR_MOD_STEP,
	SUBTILIS_TEST_CASE_ID_POINT_TINT,
	SUBTILIS_TEST_CASE_ID_REG_ALLOC_BUSTER,
	SUBTILIS_TEST_CASE_ID_SAVE_THIRD,
	SUBTILIS_TEST_CASE_ID_SAVE_USELESS_MOVE,
	SUBTILIS_TEST_CASE_ID_SAVE_FIXED_REG_ARG,
	SUBTILIS_TEST_CASE_ID_CMP_IMM,
	SUBTILIS_TEST_CASE_ID_ASSIGNMENT_OPS,
	SUBTILIS_TEST_CASE_ID_INT,
	SUBTILIS_TEST_CASE_ID_TRIG,
	SUBTILIS_TEST_CASE_ID_LOG,
	SUBTILIS_TEST_CASE_ID_RND_INT,
	SUBTILIS_TEST_CASE_ID_RND_REAL,
	SUBTILIS_TEST_CASE_ID_PRINTFP,
	SUBTILIS_TEST_CASE_ID_END,
	SUBTILIS_TEST_CASE_ID_ERROR_ENDPROC_EARLY,
	SUBTILIS_TEST_CASE_ID_ERROR_HANDLED,
	SUBTILIS_TEST_CASE_ID_ERROR_HANDLED_FN,
	SUBTILIS_TEST_CASE_ID_ERROR_MAIN,
	SUBTILIS_TEST_CASE_ID_ERROR_MAIN_END,
	SUBTILIS_TEST_CASE_ID_ERROR_NESTED,
	SUBTILIS_TEST_CASE_ID_ERROR_NESTED_ENDFN_EARLY,
	SUBTILIS_TEST_CASE_ID_ERROR_NESTED_ENDPROC_EARLY,
	SUBTILIS_TEST_CASE_ID_ERROR_NESTED_FN,
	SUBTILIS_TEST_CASE_ID_ERROR_THROW_FN,
	SUBTILIS_TEST_CASE_ID_ERROR_ERROR_IN_MAIN,
	SUBTILIS_TEST_CASE_ID_ERROR_DIV_BY_ZERO,
	SUBTILIS_TEST_CASE_ID_ERROR_LOGRANGE,
	SUBTILIS_TEST_CASE_ID_END_IN_PROC,
	SUBTILIS_TEST_CASE_ID_MAX,
} subtilis_test_case_id_t;

extern const subtilis_test_case_t test_cases[];

#endif