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

#ifndef __SUBTILIS_BUILTINS_IR_H__
#define __SUBTILIS_BUILTINS_IR_H__

#include "expression.h"
#include "ir.h"
#include "parser.h"

extern const char *subtilis_rnd_hidden_var;

void subtilis_builtins_ir_inkey(subtilis_ir_section_t *current,
				subtilis_error_t *err);

subtilis_exp_t *subtilis_builtins_ir_basic_rnd(subtilis_parser_t *p,
					       subtilis_error_t *err);
void subtilis_builtins_ir_rnd(subtilis_parser_t *p,
			      subtilis_ir_section_t *current,
			      subtilis_error_t *err);

subtilis_ir_section_t *
subtilis_builtins_ir_add_1_arg_int(subtilis_parser_t *p, const char *name,
				   subtilis_error_t *err);

#endif