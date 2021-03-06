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

#ifndef __SUBTILIS_BUILTINS_H__
#define __SUBTILIS_BUILTINS_H__

#include <stdbool.h>

#include "type.h"

typedef enum {
	SUBTILIS_BUILTINS_IDIV,
	SUBTILIS_BUILTINS_MEMSETI32,
	SUBTILIS_BUILTINS_MEMCPY,
	SUBTILIS_BUILTINS_MEMCMP,
	SUBTILIS_BUILTINS_COMPARE,
	SUBTILIS_BUILTINS_MEMSETI8,
	SUBTILIS_BUILTINS_ALLOC,
	SUBTILIS_BUILTINS_DEREF,
	SUBTILIS_BUILTINS_MEMSETI64,
	SUBTILIS_BUILTINS_MAX
} subtilis_builtin_type_t;

struct _subtilis_builtin_t {
	const char *str;
	subtilis_builtin_type_t type;
	subtilis_type_t ret_type;
	size_t num_parameters;
	subtilis_type_t arg_types[4];
	bool generates_error;
};

typedef struct _subtilis_builtin_t subtilis_builtin_t;

extern const subtilis_builtin_t subtilis_builtin_list[SUBTILIS_BUILTINS_MAX];

subtilis_type_section_t *subtilis_builtin_ts(subtilis_builtin_type_t type,
					     subtilis_error_t *err);

#endif
