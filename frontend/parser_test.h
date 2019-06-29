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

#ifndef __SUBTILIS_PARSER_TEST_H
#define __SUBTILIS_PARSER_TEST_H

#include "parser.h"

int parser_test(void);
int parser_test_wrapper(const char *text, subtilis_backend_caps_t caps,
			int (*fn)(subtilis_lexer_t *, subtilis_parser_t *,
				  const char *expected),
			const char *expected);

#endif