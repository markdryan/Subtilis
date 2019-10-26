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

#ifndef __SUBTILIS_PARSER_COMPOUND_H
#define __SUBTILIS_PARSER_COMPOUND_H

#include "expression.h"
#include "parser.h"

void subtilis_parser_locals(subtilis_parser_t *p, subtilis_token_t *t,
			    subtilis_error_t *err);
void subtilis_parser_compound(subtilis_parser_t *p, subtilis_token_t *t,
			      subtilis_keyword_type_t end_key,
			      subtilis_error_t *err);
void subtilis_parser_statement(subtilis_parser_t *p, subtilis_token_t *t,
			       subtilis_error_t *err);

#endif
