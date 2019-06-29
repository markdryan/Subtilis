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

#ifndef __SUBTILIS_PARSER_H
#define __SUBTILIS_PARSER_H

#include "../common/backend_caps.h"
#include "../common/ir.h"
#include "call.h"
#include "lexer.h"
#include "symbol_table.h"

struct subtilis_parser_t_ {
	subtilis_lexer_t *l;
	subtilis_backend_caps_t caps;
	subtilis_ir_section_t *current;
	subtilis_ir_section_t *main;
	subtilis_ir_prog_t *prog;
	subtilis_symbol_table_t *st;
	subtilis_symbol_table_t *local_st;
	subtilis_symbol_table_t *main_st;
	size_t level;
	size_t num_calls;
	size_t max_calls;
	subtilis_parser_call_t **calls;
	int32_t error_offset;
	bool handle_escapes;
};

typedef struct subtilis_parser_t_ subtilis_parser_t;

subtilis_parser_t *subtilis_parser_new(subtilis_lexer_t *l,
				       subtilis_backend_caps_t caps,
				       subtilis_error_t *err);
void subtilis_parse(subtilis_parser_t *p, subtilis_error_t *err);
void subtilis_parser_delete(subtilis_parser_t *p);

#endif