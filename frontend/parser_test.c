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

#include <string.h>

#include "parser_test.h"

#include "../common/lexer.h"
#include "../test_cases/bad_test_cases.h"
#include "../test_cases/test_cases.h"
#include "parser.h"
#include "vm.h"

/*
 * We'll just implement a few RiscOS SWIs to test the SWI mechanism.
 */

static size_t prv_sys_trans(const char *call_name)
{
	bool x_flag = call_name[0] == 'X';

	if (x_flag)
		call_name++;

	if (!strcmp(call_name, "OS_Write0"))
		return 2 | ((x_flag) ? 0x20000 : 0);

	if (!strcmp(call_name, "OS_GetEnv"))
		return 0x10 | ((x_flag) ? 0x20000 : 0);

	if (!strcmp(call_name, "OS_ConvertInteger4"))
		return 0xdc | ((x_flag) ? 0x20000 : 0);

	return SIZE_MAX;
}

static bool prv_sys_check(size_t call_id, uint32_t *in_regs, uint32_t *out_regs,
			  bool *check_for_errors)
{
	size_t code = call_id & ~((size_t)0x20000);

	if ((code >= 256 + 32) && (code < 512)) {
		*in_regs = 0;
		*out_regs = 0;
		*check_for_errors = call_id & 0x20000;
		return true;
	}

	switch (call_id) {
	case 2 + 0x20000:
	case 2:
		*in_regs = 1;
		*out_regs = 1;
		*check_for_errors = call_id & 0x20000;
		return true;
	case 0x10:
	case 0x10 + 0x20000:
		*in_regs = 0;
		*out_regs = 7;
		*check_for_errors = call_id & 0x20000;
		return true;
	case 0xdc:
	case 0xdc + 0x20000:
		*in_regs = 7;
		*out_regs = 7;
		*check_for_errors = call_id & 0x20000;
		return true;
	default:
		return false;
	}
}

int parser_test_wrapper(const char *text, subtilis_backend_caps_t caps,
			int (*fn)(subtilis_lexer_t *, subtilis_parser_t *,
				  subtilis_error_type_t, const char *expected,
				  bool mem_leaks_ok),
			subtilis_error_type_t expected_err,
			const char *expected, bool mem_leaks_ok)
{
	subtilis_stream_t s;
	subtilis_error_t err;
	int retval;
	subtilis_settings_t settings;
	subtilis_backend_t backend;
	subtilis_lexer_t *l = NULL;
	subtilis_parser_t *p = NULL;

	subtilis_error_init(&err);
	subtilis_stream_from_text(&s, text, &err);
	if (err.type != SUBTILIS_ERROR_OK)
		goto fail;

	l = subtilis_lexer_new(&s, SUBTILIS_CONFIG_LEXER_BUF_SIZE,
			       subtilis_keywords_list, SUBTILIS_KEYWORD_TOKENS,
			       NULL, 0, &err);
	if (err.type != SUBTILIS_ERROR_OK) {
		s.close(s.handle, &err);
		goto fail;
	}

	settings.handle_escapes = true;
	settings.ignore_graphics_errors = true;
	settings.check_mem_leaks = !mem_leaks_ok;
	backend.caps = caps;
	backend.sys_trans = prv_sys_trans;
	backend.sys_check = prv_sys_check;

	p = subtilis_parser_new(l, &backend, &settings, &err);
	if (err.type != SUBTILIS_ERROR_OK)
		goto fail;

	retval = fn(l, p, expected_err, expected, mem_leaks_ok);

	printf(": [%s]\n", retval ? "FAIL" : "OK");

	subtilis_parser_delete(p);
	subtilis_lexer_delete(l, &err);

	return retval;

fail:
	printf(": [FAIL]\n");

	subtilis_error_fprintf(stderr, &err, true);
	subtilis_parser_delete(p);
	subtilis_lexer_delete(l, &err);

	return 1;
}

static void prv_check_for_error(subtilis_lexer_t *l,
				subtilis_error_type_t expected_err,
				subtilis_error_t *err)
{
	if (expected_err == SUBTILIS_ERROR_OK)
		return;

	if (expected_err != err->type) {
		subtilis_error_set_bad_error(err, expected_err, err->type,
					     l->stream->name, l->line);
		return;
	}
	subtilis_error_init(err);
}

static int prv_check_eval_res(subtilis_lexer_t *l, subtilis_parser_t *p,
			      subtilis_error_type_t expected_err,
			      const char *expected, bool mem_leaks_ok)
{
	subtilis_buffer_t b;
	subtilis_error_t err;
	const char *tbuf;
	subitlis_vm_t *vm = NULL;
	int retval = 1;

	subtilis_error_init(&err);
	subtilis_buffer_init(&b, 1024);

	subtilis_parse(p, &err);
	prv_check_for_error(l, expected_err, &err);
	if (err.type != SUBTILIS_ERROR_OK) {
		subtilis_error_fprintf(stderr, &err, true);
		goto cleanup;
	} else if (expected_err != SUBTILIS_ERROR_OK) {
		retval = 0;
		goto cleanup;
	}

	vm = subitlis_vm_new(p->prog, p->st, &err);
	prv_check_for_error(l, expected_err, &err);
	if (err.type != SUBTILIS_ERROR_OK) {
		subtilis_error_fprintf(stderr, &err, true);
		goto cleanup;
	} else if (expected_err != SUBTILIS_ERROR_OK) {
		retval = 0;
		goto cleanup;
	}

	subitlis_vm_run(vm, &b, &err);
	prv_check_for_error(l, expected_err, &err);
	if (err.type != SUBTILIS_ERROR_OK) {
		subtilis_error_fprintf(stderr, &err, true);
		goto cleanup;
	} else if (expected_err != SUBTILIS_ERROR_OK) {
		retval = 0;
		goto cleanup;
	}

	subtilis_buffer_zero_terminate(&b, &err);
	if (err.type != SUBTILIS_ERROR_OK) {
		subtilis_error_fprintf(stderr, &err, true);
		goto cleanup;
	}

	tbuf = subtilis_buffer_get_string(&b);

	if (strcmp(tbuf, expected)) {
		fprintf(stderr, "Expected result %s got %s\n", expected, tbuf);
		goto cleanup;
	}

	retval = 0;

cleanup:

	subitlis_vm_delete(vm);
	subtilis_buffer_free(&b);

	return retval;
}

static int prv_test_let(void)
{
	const char *let_test = "LET b% = 99\n"
			       "PRINT 10 + 10 + b%\n";

	printf("parser_let");
	return parser_test_wrapper(let_test, SUBTILIS_BACKEND_INTER_CAPS,
				   prv_check_eval_res, SUBTILIS_ERROR_OK,
				   "119\n", false);
}

static int prv_test_expressions(void)
{
	size_t i;
	int retval = 0;

	for (i = 0; i < SUBTILIS_TEST_CASE_ID_MAX; i++) {
		printf("parser_%s", test_cases[i].name);
		retval |= parser_test_wrapper(
		    test_cases[i].source, SUBTILIS_BACKEND_INTER_CAPS,
		    prv_check_eval_res, SUBTILIS_ERROR_OK, test_cases[i].result,
		    false);
	}

	return retval;
}

static int prv_test_bad_cases(void)
{
	size_t i;
	int retval = 0;

	for (i = 0; i < SUBTILIS_BAD_TEST_CASE_ID_MAX; i++) {
		printf("parser_bad_%s", bad_test_cases[i].name);
		retval |= parser_test_wrapper(
		    bad_test_cases[i].source, SUBTILIS_BACKEND_INTER_CAPS,
		    prv_check_eval_res, bad_test_cases[i].err, "", false);
	}

	return retval;
}

static int prv_test_print(void)
{
	printf("parser_print");
	return parser_test_wrapper(
	    "PRINT (10 * 3 * 3 + 1) DIV 2", SUBTILIS_BACKEND_INTER_CAPS,
	    prv_check_eval_res, SUBTILIS_ERROR_OK, "45\n", false);
}

int parser_test(void)
{
	int failure = 0;

	failure |= prv_test_let();
	failure |= prv_test_print();
	failure |= prv_test_expressions();
	failure |= prv_test_bad_cases();

	return failure;
}
