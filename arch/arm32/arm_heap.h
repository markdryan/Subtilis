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

#ifndef __SUBTILIS_ARM_HEAP_H
#define __SUBTILIS_ARM_HEAP_H

#include "arm_core.h"

const uint32_t subtilis_arm_heap_min_size(void);
void subtilis_arm_heap_init(subtilis_arm_section_t *arm_s,
			    subtilis_error_t *err);
void subtilis_arm_heap_alloc(subtilis_arm_section_t *arm_s, size_t good_label,
			     size_t bad_label, subtilis_error_t *err);
void subtilis_arm_heap_free(subtilis_arm_section_t *arm_s,
			    subtilis_arm_reg_t heap_start,
			    subtilis_arm_reg_t block, subtilis_error_t *err);
void subtilis_arm_heap_free_space(subtilis_arm_section_t *arm_s,
				  subtilis_arm_reg_t heap_start,
				  subtilis_arm_reg_t result,
				  subtilis_error_t *err);
#endif
