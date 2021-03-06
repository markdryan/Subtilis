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

#ifndef __SUBTILIS_ARM_ASSEMBLER_H
#define __SUBTILIS_ARM_ASSEMBLER_H

#include "../../common/backend_caps.h"
#include "../../common/bitset.h"
#include "../../common/string_pool.h"
#include "arm_core.h"
#include "arm_expression.h"
#include "arm_swi.h"

struct subtilis_arm_ass_def_t_ {
	char *name;
	subtilis_arm_exp_val_t *val;
};

typedef struct subtilis_arm_ass_def_t_ subtilis_arm_ass_def_t;

struct subtilis_ass_block_t_ {
	size_t def_start;
	subtilis_arm_exp_val_t *upto;
};

typedef struct subtilis_ass_block_t_ subtilis_ass_block_t;

struct subtilis_arm_ass_context_t_ {
	subtilis_arm_section_t *arm_s;
	subtilis_lexer_t *l;
	subtilis_token_t *t;
	subtilis_type_section_t *stype;
	const subtilis_settings_t *set;
	subtilis_backend_sys_trans sys_trans;
	subtilis_string_pool_t *label_pool;
	subtilis_bitset_t pending_labels;
	subtilis_arm_ass_def_t *defs;
	size_t def_count;
	size_t max_defs;
	subtilis_ass_block_t *blocks;
	size_t num_blocks;
	size_t max_blocks;
};

/* clang-format off */
subtilis_arm_section_t *subtilis_arm_asm_parse(
	subtilis_lexer_t *l, subtilis_token_t *t, subtilis_arm_op_pool_t *pool,
	subtilis_type_section_t *stype, const subtilis_settings_t *set,
	subtilis_backend_sys_trans sys_trans, int32_t start_address,
	subtilis_error_t *err);
/* clang-format on */

subtilis_arm_exp_val_t *subtilis_arm_asm_find_def(subtilis_arm_ass_context_t *c,
						  const char *name);

#endif
