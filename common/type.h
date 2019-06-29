/*
 * Copyright (c) 2018 Mark Ryan
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

#ifndef __SUBTILIS_TYPE_H
#define __SUBTILIS_TYPE_H

#include "error.h"

typedef enum {
	SUBTILIS_TYPE_CONST_REAL,
	SUBTILIS_TYPE_CONST_INTEGER,
	SUBTILIS_TYPE_CONST_STRING,
	SUBTILIS_TYPE_REAL,
	SUBTILIS_TYPE_INTEGER,
	SUBTILIS_TYPE_STRING,
	SUBTILIS_TYPE_VOID,
	SUBTILIS_TYPE_MAX,
} subtilis_type_t;

struct subtilis_type_section_t_ {
	subtilis_type_t return_type;
	size_t num_parameters;
	subtilis_type_t *parameters;
	size_t ref_count;
	size_t int_regs;
	size_t fp_regs;
};

typedef struct subtilis_type_section_t_ subtilis_type_section_t;

/* Takes ownership of parameters */
subtilis_type_section_t *subtilis_type_section_new(subtilis_type_t rtype,
						   size_t num_parameters,
						   subtilis_type_t *parameters,
						   subtilis_error_t *err);
void subtilis_type_section_delete(subtilis_type_section_t *stype);
const char *subtilis_type_name(subtilis_type_t typ);
subtilis_type_section_t *
subtilis_type_section_dup(subtilis_type_section_t *stype);

#endif