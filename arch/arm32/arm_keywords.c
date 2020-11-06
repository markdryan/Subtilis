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

#include <stdbool.h>

#include "arm_keywords.h"

/* clang-format off */
const subtilis_keyword_t subtilis_arm_keywords_list[] = {
	{"ABS",       SUBTILIS_ARM_KEYWORD_ABS,         true},
	{"ACS",       SUBTILIS_ARM_KEYWORD_ACS,         true},
	{"ALIGN",     SUBTILIS_ARM_KEYWORD_ALIGN,       true},
	{"AND",       SUBTILIS_ARM_KEYWORD_AND,         true},
	{"ASC",       SUBTILIS_ARM_KEYWORD_ASC,         true},
	{"ASN",       SUBTILIS_ARM_KEYWORD_ASN,         true},
	{"ASR",       SUBTILIS_ARM_KEYWORD_ASR,         true},
	{"ATN",       SUBTILIS_ARM_KEYWORD_ATN,         true},
	{"CHR$",      SUBTILIS_ARM_KEYWORD_CHR_STR,     true},
	{"COS",       SUBTILIS_ARM_KEYWORD_COS,         true},
	{"DEF",       SUBTILIS_ARM_KEYWORD_DEF,         true},
	{"DIV",       SUBTILIS_ARM_KEYWORD_DIV,         true},
	{"EOR",       SUBTILIS_ARM_KEYWORD_EOR,         true},
	{"EQUB",      SUBTILIS_ARM_KEYWORD_EQUB,        true},
	{"EQUD",      SUBTILIS_ARM_KEYWORD_EQUD,        true},
	{"EQUDBL",    SUBTILIS_ARM_KEYWORD_EQUDBL,      true},
	{"EQUDBLR",   SUBTILIS_ARM_KEYWORD_EQUDBLR,     true},
	{"EQUS",      SUBTILIS_ARM_KEYWORD_EQUS,        true},
	{"EQUW",      SUBTILIS_ARM_KEYWORD_EQUW,        true},
	{"EXP",       SUBTILIS_ARM_KEYWORD_EXP,         true},
	{"FALSE",     SUBTILIS_ARM_KEYWORD_FALSE,       true},
	{"FN",        SUBTILIS_KEYWORD_FN,              true},
	{"INT",       SUBTILIS_ARM_KEYWORD_INT,         true},
	{"LEFT$",     SUBTILIS_ARM_KEYWORD_LEFT_STR,    true},
	{"LEN",       SUBTILIS_ARM_KEYWORD_LEN,         true},
	{"LN",        SUBTILIS_ARM_KEYWORD_LN,          true},
	{"LOG",       SUBTILIS_ARM_KEYWORD_LOG,         true},
	{"LSL",       SUBTILIS_ARM_KEYWORD_LSL,         true},
	{"LSR",       SUBTILIS_ARM_KEYWORD_LSR,         true},
	{"MID$",      SUBTILIS_ARM_KEYWORD_MID_STR,     true},
	{"MOD",       SUBTILIS_ARM_KEYWORD_MOD,         true},
	{"NOT",       SUBTILIS_ARM_KEYWORD_NOT,         true},
	{"OR",        SUBTILIS_ARM_KEYWORD_OR,          true},
	{"PI",        SUBTILIS_ARM_KEYWORD_PI,          true},
	{"PROC",      SUBTILIS_KEYWORD_PROC,            true},
	{"RAD",       SUBTILIS_ARM_KEYWORD_RAD,         true},
	{"REM",       SUBTILIS_KEYWORD_REM,             true},
	{"RIGHT$",    SUBTILIS_ARM_KEYWORD_RIGHT_STR,   true},
	{"ROR",       SUBTILIS_ARM_KEYWORD_ROR,         true},
	{"RRX",       SUBTILIS_ARM_KEYWORD_RRX,         true},
	{"SGN",       SUBTILIS_ARM_KEYWORD_SGN,         true},
	{"SIN",       SUBTILIS_ARM_KEYWORD_SIN,         true},
	{"SQR",       SUBTILIS_ARM_KEYWORD_SQR,         true},
	{"STR$",      SUBTILIS_ARM_KEYWORD_STR_STR,     true},
	{"STRING$",   SUBTILIS_ARM_KEYWORD_STRING_STR,  true},
	{"TAN",       SUBTILIS_ARM_KEYWORD_TAN,         true},
	{"TRUE",      SUBTILIS_ARM_KEYWORD_TRUE,        true},
	{"abs",       SUBTILIS_ARM_KEYWORD_ABS,         true},
	{"acs",       SUBTILIS_ARM_KEYWORD_ACS,         true},
	{"align",     SUBTILIS_ARM_KEYWORD_ALIGN,       true},
	{"and",       SUBTILIS_ARM_KEYWORD_AND,         true},
	{"asc",       SUBTILIS_ARM_KEYWORD_ASC,         true},
	{"asn",       SUBTILIS_ARM_KEYWORD_ASN,         true},
	{"asr",       SUBTILIS_ARM_KEYWORD_ASR,         true},
	{"atn",       SUBTILIS_ARM_KEYWORD_ATN,         true},
	{"chr$",      SUBTILIS_ARM_KEYWORD_CHR_STR,     true},
	{"cos",       SUBTILIS_ARM_KEYWORD_COS,         true},
	{"def",       SUBTILIS_ARM_KEYWORD_DEF,         true},
	{"div",       SUBTILIS_ARM_KEYWORD_DIV,         true},
	{"eor",       SUBTILIS_ARM_KEYWORD_EOR,         true},
	{"equb",      SUBTILIS_ARM_KEYWORD_EQUB,        true},
	{"equd",      SUBTILIS_ARM_KEYWORD_EQUD,        true},
	{"equdbl",    SUBTILIS_ARM_KEYWORD_EQUDBL,      true},
	{"equdblr",   SUBTILIS_ARM_KEYWORD_EQUDBLR,     true},
	{"equs",      SUBTILIS_ARM_KEYWORD_EQUS,        true},
	{"equw",      SUBTILIS_ARM_KEYWORD_EQUW,        true},
	{"exp",       SUBTILIS_ARM_KEYWORD_EXP,         true},
	{"false",     SUBTILIS_ARM_KEYWORD_FALSE,       true},
	{"int",       SUBTILIS_ARM_KEYWORD_INT,         true},
	{"left$",     SUBTILIS_ARM_KEYWORD_LEFT_STR,    true},
	{"len",       SUBTILIS_ARM_KEYWORD_LEN,         true},
	{"ln",        SUBTILIS_ARM_KEYWORD_LN,          true},
	{"log",       SUBTILIS_ARM_KEYWORD_LOG,         true},
	{"lsl",       SUBTILIS_ARM_KEYWORD_LSL,         true},
	{"lsr",       SUBTILIS_ARM_KEYWORD_LSR,         true},
	{"mid$",      SUBTILIS_ARM_KEYWORD_MID_STR,     true},
	{"mod",       SUBTILIS_ARM_KEYWORD_MOD,         true},
	{"not",       SUBTILIS_ARM_KEYWORD_NOT,         true},
	{"or",        SUBTILIS_ARM_KEYWORD_OR,          true},
	{"pi",        SUBTILIS_ARM_KEYWORD_PI,          true},
	{"rad",       SUBTILIS_ARM_KEYWORD_RAD,         true},
	{"rem",       SUBTILIS_KEYWORD_REM,             true},
	{"right$",    SUBTILIS_ARM_KEYWORD_RIGHT_STR,   true},
	{"ror",       SUBTILIS_ARM_KEYWORD_ROR,         true},
	{"rrx",       SUBTILIS_ARM_KEYWORD_RRX,         true},
	{"sgn",       SUBTILIS_ARM_KEYWORD_SGN,         true},
	{"sin",       SUBTILIS_ARM_KEYWORD_SIN,         true},
	{"sqr",       SUBTILIS_ARM_KEYWORD_SQR,         true},
	{"str$",      SUBTILIS_ARM_KEYWORD_STR_STR,     true},
	{"string$",   SUBTILIS_ARM_KEYWORD_STRING_STR,  true},
	{"tan",       SUBTILIS_ARM_KEYWORD_TAN,         true},
	{"true",      SUBTILIS_ARM_KEYWORD_TRUE,        true},
};

/* clang-format on */
