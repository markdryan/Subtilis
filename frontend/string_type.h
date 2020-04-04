/*
 * Copyright (c) 2020 Mark Ryan
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

#ifndef __SUBTILIS_STRING_TYPE_H
#define __SUBTILIS_STRING_TYPE_H

#include "expression.h"
#include "parser.h"

size_t subtilis_string_type_size(const subtilis_type_t *type);
void subtilis_string_type_zero_ref(subtilis_parser_t *p,
				   const subtilis_type_t *type, size_t mem_reg,
				   size_t loc, subtilis_error_t *err);
void subtilis_string_type_new_ref_from_char(subtilis_parser_t *p,
					    size_t mem_reg, size_t loc,
					    subtilis_exp_t *e,
					    subtilis_error_t *err);
void subtilis_string_type_new_owned_ref_from_const(subtilis_parser_t *p,
						   size_t mem_reg, size_t loc,
						   subtilis_exp_t *e,
						   subtilis_error_t *err);
void subtilis_string_type_new_ref(subtilis_parser_t *p,
				  const subtilis_type_t *type, size_t mem_reg,
				  size_t loc, subtilis_exp_t *e,
				  subtilis_error_t *err);
void subtilis_string_type_assign_ref(subtilis_parser_t *p,
				     const subtilis_type_t *type,
				     size_t mem_reg, size_t loc,
				     subtilis_exp_t *e, subtilis_error_t *err);
void subtilis_string_type_assign_to_reg(subtilis_parser_t *p, size_t reg,
					subtilis_exp_t *e,
					subtilis_error_t *err);
subtilis_exp_t *subtilis_string_type_len(subtilis_parser_t *p,
					 subtilis_exp_t *e,
					 subtilis_error_t *err);
subtilis_exp_t *subtilis_string_type_asc(subtilis_parser_t *p,
					 subtilis_exp_t *e,
					 subtilis_error_t *err);
void subtilis_string_type_print(subtilis_parser_t *p, subtilis_exp_t *e,
				subtilis_error_t *err);
void subtilis_string_type_print_const(subtilis_parser_t *p, subtilis_exp_t *e,
				      subtilis_error_t *err);

#endif
