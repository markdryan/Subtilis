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
	SUBTILIS_TEST_CASE_ID_MAX,
} subtilis_test_case_id_t;

extern const subtilis_test_case_t test_cases[];

#endif
