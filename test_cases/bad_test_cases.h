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

#ifndef __SUBTILIS_BAD_TEST_CASES_H
#define __SUBTILIS_BAD_TEST_CASES_H

#include "../common/error.h"

struct subtilis_bad_test_case_t_ {
	const char *name;
	const char *source;
	subtilis_error_type_t err;
};

typedef struct subtilis_bad_test_case_t_ subtilis_bad_test_case_t;

typedef enum {
	SUBTILIS_BAD_TEST_CASE_ID_ALREADY_DEFINED_LOCAL,
	SUBTILIS_BAD_TEST_CASE_ID_ALREADY_DEFINED_NESTED,
	SUBTILIS_BAD_TEST_CASE_ID_ALREADY_DEFINED_ASSOP,
	SUBTILIS_BAD_TEST_CASE_ID_ALREADY_DEFINED_FOR,
	SUBTILIS_BAD_TEST_CASE_ID_ALREADY_DEFINED_DIM_GLOBAL,
	SUBTILIS_BAD_TEST_CASE_ID_ALREADY_DEFINED_DIM_LOCAL,
	SUBTILIS_BAD_TEST_CASE_ID_UNKNOWN_PROCEDURE,
	SUBTILIS_BAD_TEST_CASE_ID_UNKNOWN_FN,
	SUBTILIS_BAD_TEST_CASE_ID_NOT_KEYWORD,
	SUBTILIS_BAD_TEST_CASE_ID_NOT_TERMINATED_IF,
	SUBTILIS_BAD_TEST_CASE_ID_NOT_TERMINATED_WHILE,
	SUBTILIS_BAD_TEST_CASE_ID_NOT_TERMINATED_REPEAT,
	SUBTILIS_BAD_TEST_CASE_ID_NOT_TERMINATED_PROC,
	SUBTILIS_BAD_TEST_CASE_ID_NOT_TERMINATED_FN,
	SUBTILIS_BAD_TEST_CASE_ID_NOT_TERMINATED_ON_ERROR,
	SUBTILIS_BAD_TEST_CASE_ID_PROC_TOO_MANY_ARGS1,
	SUBTILIS_BAD_TEST_CASE_ID_PROC_TOO_MANY_ARGS2,
	SUBTILIS_BAD_TEST_CASE_ID_FN_TOO_MANY_ARGS1,
	SUBTILIS_BAD_TEST_CASE_ID_FN_TOO_MANY_ARGS2,
	SUBTILIS_BAD_TEST_CASE_ID_IF_NO_THEN,
	SUBTILIS_BAD_TEST_CASE_ID_ENDPROC_IN_MAIN,
	SUBTILIS_BAD_TEST_CASE_ID_RETURN_IN_MAIN,
	SUBTILIS_BAD_TEST_CASE_ID_MISSING_BRACKET,
	SUBTILIS_BAD_TEST_CASE_ID_USELESS_STATEMENT,
	SUBTILIS_BAD_TEST_CASE_ID_USELESS_STATEMENT_IN_PROC,
	SUBTILIS_BAD_TEST_CASE_ID_NESTED_PROCEDURE,
	SUBTILIS_BAD_TEST_CASE_ID_NESTED_FUNCTION,
	SUBTILIS_BAD_TEST_CASE_ID_NESTED_HANDLER,
	SUBTILIS_BAD_TEST_CASE_ID_DIM_IN_PROC,
	SUBTILIS_BAD_TEST_CASE_ID_BAD_DIM,
	SUBTILIS_BAD_TEST_CASE_ID_NOT_ARRAY,
	SUBTILIS_BAD_TEST_CASE_ID_BAD_INDEX,
	SUBTILIS_BAD_TEST_CASE_ID_BAD_COUNT,
	SUBTILIS_BAD_TEST_CASE_ID_ZERO_STEP,
	SUBTILIS_BAD_TEST_CASE_ID_EXP_EXPECTED,
	SUBTILIS_BAD_TEST_CASE_ID_ID_EXPECTED,
	SUBTILIS_BAD_TEST_CASE_ID_BAD_PROC_NAME,
	SUBTILIS_BAD_TEST_CASE_ID_TOO_MANY_BLOCKS,
	SUBTILIS_BAD_TEST_CASE_ID_DIV_BY_ZERO,
	SUBTILIS_BAD_TEST_CASE_ID_DIVIDE_BY_ZERO,
	SUBTILIS_BAD_TEST_CASE_ID_MISSING_SQUARE_BRACKET,
	SUBTILIS_BAD_TEST_CASE_ID_ARRAY_ASSIGN_MISMATCH,
	SUBTILIS_BAD_TEST_CASE_ID_ARRAY_ASSIGN_MISMATCH2,
	SUBTILIS_BAD_TEST_CASE_ID_ARRAY_PLUS_ASSIGN,
	SUBTILIS_BAD_TEST_CASE_ID_ARRAY_BAD_ARG_INT,
	SUBTILIS_BAD_TEST_CASE_ID_ARRAY_BAD_ARG_REAL,
	SUBTILIS_BAD_TEST_CASE_ID_ARRAY_FN_BAD_ASSIGN,
	SUBTILIS_BAD_TEST_CASE_ID_ARRAY_DIM_TOO_MANY_ARGS,
	SUBTILIS_BAD_TEST_CASE_ID_ARRAY_DIM_NO_ARGS,
	SUBTILIS_BAD_TEST_CASE_ID_ARRAY_DIM_ZERO_ARGS,
	SUBTILIS_BAD_TEST_CASE_ID_ARRAY_DIM_ARG_TOO_BIG,
	SUBTILIS_BAD_TEST_CASE_ID_FOR_NOT_NUMERIC,
	SUBTILIS_BAD_TEST_CASE_ID_MAX,
} subtilis_bad_test_case_id_t;

extern const subtilis_bad_test_case_t bad_test_cases[];

#endif
