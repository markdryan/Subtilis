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

#ifndef __SUBTILIS_ERROR_H
#define __SUBTILIS_ERROR_H

#include <stdbool.h>
#include <stdio.h>

#include "config.h"

typedef enum {
	SUBTILIS_ERROR_OK,
	SUBTILIS_ERROR_OOM,
	SUBTILIS_ERROR_OPEN,
	SUBTILIS_ERROR_READ,
	SUBTILIS_ERROR_CLOSE,
	SUBTILIS_ERROR_UNTERMINATED_STRING,
	SUBTILIS_ERROR_STRING_TOO_LONG,
	SUBTILIS_ERROR_NUMBER_TOO_LONG,
	SUBTILIS_ERROR_IDENTIFIER_TOO_LONG,
	SUBTILIS_ERROR_UNKNOWN_TOKEN,
	SUBTILIS_ERROR_BAD_PROC_NAME,
	SUBTILIS_ERROR_BAD_FN_NAME,
	SUBTILIS_ERROR_ASSERTION_FAILED,
} subtilis_error_type_t;

struct _subtilis_error_t {
	subtilis_error_type_t type;
	char data1[SUBTILIS_CONFIG_ERROR_LEN];
	char data2[SUBTILIS_CONFIG_ERROR_LEN];
	char file[SUBTILIS_CONFIG_PATH_MAX];
	unsigned int line;
	char subtilis_file[SUBTILIS_CONFIG_PATH_MAX];
	unsigned int subtilis_line;
};

typedef struct _subtilis_error_t subtilis_error_t;

void subtilis_error_init(subtilis_error_t *e);
#define subtilis_error_set0(e, t, f, l)                                        \
	subtilis_error_set_full(e, t, "", "", f, l, __FILE__, __LINE__)
#define subtilis_error_set1(e, t, d1, f, l)                                    \
	subtilis_error_set_full(e, t, d1, "", f, l, __FILE__, __LINE__)
#define subtilis_error_set2(e, t, d1, d2, f, l)                                \
	subtilis_error_set_full(e, t, d1, d2, f, l, __FILE__, __LINE__)

#define subtilis_error_set_oom(e)                                              \
	subtilis_error_set_basic(e, SUBTILIS_ERROR_OOM, __FILE__, __LINE__)
#define subtilis_error_set_file_open(e, path)                                  \
	subtilis_error_set_errno(e, SUBTILIS_ERROR_OPEN, path, __FILE__,       \
				 __LINE__)
#define subtilis_error_set_file_read(e)                                        \
	subtilis_error_set_errno(e, SUBTILIS_ERROR_READ, "", __FILE__, __LINE__)
#define subtilis_error_set_file_close(e)                                       \
	subtilis_error_set_errno(e, SUBTILIS_ERROR_CLOSE, "", __FILE__,        \
				 __LINE__)
#define subtilis_error_set_unterminated_string(e, str, file, line)             \
	subtilis_error_set1(e, SUBTILIS_ERROR_UNTERMINATED_STRING, str, file,  \
			    line)
#define subtilis_error_set_string_too_long(e, str, file, line)                 \
	subtilis_error_set1(e, SUBTILIS_ERROR_STRING_TOO_LONG, str, file, line)
#define subtilis_error_set_number_too_long(e, str, file, line)                 \
	subtilis_error_set1(e, SUBTILIS_ERROR_NUMBER_TOO_LONG, str, file, line)
#define subtilis_error_set_identifier_too_long(e, str, file, line)             \
	subtilis_error_set1(e, SUBTILIS_ERROR_IDENTIFIER_TOO_LONG, str, file,  \
			    line)
#define subtilis_error_set_unknown_token(e, str, file, line)                   \
	subtilis_error_set1(e, SUBTILIS_ERROR_UNKNOWN_TOKEN, str, file, line)
#define subtilis_error_set_bad_proc_name(e, str, file, line)                   \
	subtilis_error_set1(e, SUBTILIS_ERROR_BAD_PROC_NAME, str, file, line)
#define subtilis_error_set_bad_fn_name(e, str, file, line)                     \
	subtilis_error_set1(e, SUBTILIS_ERROR_BAD_FN_NAME, str, file, line)
#define subtilis_error_set_asssertion_failed(e)                                \
	subtilis_error_set_basic(e, SUBTILIS_ERROR_ASSERTION_FAILED, __FILE__, \
				 __LINE__)

void subtilis_error_set_full(subtilis_error_t *e, subtilis_error_type_t type,
			     const char *data1, const char *data2,
			     const char *file, unsigned int line,
			     const char *subtilis_file,
			     unsigned int subtilis_line);
void subtilis_error_set_basic(subtilis_error_t *e, subtilis_error_type_t type,
			      const char *subtilis_file,
			      unsigned int subtilis_line);
void subtilis_error_set_errno(subtilis_error_t *e, subtilis_error_type_t type,
			      const char *data1, const char *subtilis_file,
			      unsigned int subtilis_line);
void subtilis_error_fprintf(FILE *f, subtilis_error_t *e, bool verbose);

#endif