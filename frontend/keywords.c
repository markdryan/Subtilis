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

#include "keywords.h"

/* clang-format off */
const subtilis_keyword_t subtilis_keywords_list[] = {
	{"ABS",       SUBTILIS_KEYWORD_ABS,          true},
	{"ACS",       SUBTILIS_KEYWORD_ACS,          true},
	{"ADVAL",     SUBTILIS_KEYWORD_ADVAL,        true},
	{"AND",       SUBTILIS_KEYWORD_AND,          true},
	{"ASC",       SUBTILIS_KEYWORD_ASC,          true},
	{"ASN",       SUBTILIS_KEYWORD_ASN,          true},
	{"ATN",       SUBTILIS_KEYWORD_ATN,          true},
	{"BEAT",      SUBTILIS_KEYWORD_BEAT,         true},
	{"BEATS",     SUBTILIS_KEYWORD_BEATS,        true},
	{"BGET#",     SUBTILIS_KEYWORD_BGET_HASH,    true},
	{"BPUT#",     SUBTILIS_KEYWORD_BPUT_HASH,    true},
	{"BY",        SUBTILIS_KEYWORD_BY,           true},
	{"CALL",      SUBTILIS_KEYWORD_CALL,         true},
	{"CASE",      SUBTILIS_KEYWORD_CASE,         true},
	{"CHR$",      SUBTILIS_KEYWORD_CHR_STR,      true},
	{"CIRCLE",    SUBTILIS_KEYWORD_CIRCLE,       true},
	{"CLG",       SUBTILIS_KEYWORD_CLG,          true},
	{"CLOSE#",    SUBTILIS_KEYWORD_CLOSE_HASH,   true},
	{"CLS",       SUBTILIS_KEYWORD_CLS,          true},
	{"COLOR",     SUBTILIS_KEYWORD_COLOR,        true},
	{"COLOUR",    SUBTILIS_KEYWORD_COLOUR,       true},
	{"COS",       SUBTILIS_KEYWORD_COS,          true},
	{"COUNT",     SUBTILIS_KEYWORD_COUNT,        true},
	{"DEF",       SUBTILIS_KEYWORD_DEF,          true},
	{"DEG",       SUBTILIS_KEYWORD_DEG,          true},
	{"DIM",       SUBTILIS_KEYWORD_DIM,          true},
	{"DIV",       SUBTILIS_KEYWORD_DIV,          true},
	{"DRAW",      SUBTILIS_KEYWORD_DRAW,         true},
	{"ELLIPSE",   SUBTILIS_KEYWORD_ELLIPSE,      true},
	{"ELSE",      SUBTILIS_KEYWORD_ELSE,         true},
	{"END",       SUBTILIS_KEYWORD_END,          true},
	{"ENDCASE",   SUBTILIS_KEYWORD_ENDCASE,      true},
	{"ENDERROR",  SUBTILIS_KEYWORD_ENDERROR,     true},
	{"ENDIF",     SUBTILIS_KEYWORD_ENDIF,        true},
	{"ENDPROC",   SUBTILIS_KEYWORD_ENDPROC,      true},
	{"ENDWHILE",  SUBTILIS_KEYWORD_ENDWHILE,     true},
	{"EOF#",      SUBTILIS_KEYWORD_EOF_HASH,     true},
	{"EOR",       SUBTILIS_KEYWORD_EOR,          true},
	{"ERL",       SUBTILIS_KEYWORD_ERL,          false},
	{"ERR",       SUBTILIS_KEYWORD_ERR,          true},
	{"ERROR",     SUBTILIS_KEYWORD_ERROR,        true},
	{"EVAL",      SUBTILIS_KEYWORD_EVAL,         false},
	{"EXP",       SUBTILIS_KEYWORD_EXP,          true},
	{"EXT#",      SUBTILIS_KEYWORD_EXT_HASH,     true},
	{"FALSE",     SUBTILIS_KEYWORD_FALSE,        true},
	{"FILL",      SUBTILIS_KEYWORD_FILL,         true},
	{"FN",        SUBTILIS_KEYWORD_FN,           true},
	{"FOR",       SUBTILIS_KEYWORD_FOR,          true},
	{"GCOL",      SUBTILIS_KEYWORD_GCOL,         true},
	{"GET",       SUBTILIS_KEYWORD_GET,          true},
	{"GET$",      SUBTILIS_KEYWORD_GET_STR,      true},
	{"GET$#",     SUBTILIS_KEYWORD_GET_STR_HASH, true},
	{"IF",        SUBTILIS_KEYWORD_IF,           true},
	{"INKEY",     SUBTILIS_KEYWORD_INKEY,        true},
	{"INKEY$",    SUBTILIS_KEYWORD_INKEY_STR,    true},
	{"INPUT",     SUBTILIS_KEYWORD_INPUT,        true},
	{"INPUT#",    SUBTILIS_KEYWORD_INPUT_HASH,   true},
	{"INSTR",     SUBTILIS_KEYWORD_INSTR,        true},
	{"INT",       SUBTILIS_KEYWORD_INT,          true},
	{"LEFT$",     SUBTILIS_KEYWORD_LEFT_STR,     true},
	{"LEN",       SUBTILIS_KEYWORD_LEN,          true},
	{"LET",       SUBTILIS_KEYWORD_LET,          true},
	{"LINE",      SUBTILIS_KEYWORD_LINE,         true},
	{"LN",        SUBTILIS_KEYWORD_LN,           true},
	{"LOCAL",     SUBTILIS_KEYWORD_LOCAL,        true},
	{"LOG",       SUBTILIS_KEYWORD_LOG,          true},
	{"MID$",      SUBTILIS_KEYWORD_MID_STR,      true},
	{"MOD",       SUBTILIS_KEYWORD_MOD,          true},
	{"MODE",      SUBTILIS_KEYWORD_MODE,         true},
	{"MOUSE",     SUBTILIS_KEYWORD_MOUSE,        true},
	{"MOVE",      SUBTILIS_KEYWORD_MOVE,         true},
	{"NEXT",      SUBTILIS_KEYWORD_NEXT,         true},
	{"NOT",       SUBTILIS_KEYWORD_NOT,          true},
	{"OF",        SUBTILIS_KEYWORD_OF,           true},
	{"OFF",       SUBTILIS_KEYWORD_OFF,          true},
	{"ON",        SUBTILIS_KEYWORD_ON,           true},
	{"ONERROR",   SUBTILIS_KEYWORD_ONERROR,      true},
	{"OPENIN",    SUBTILIS_KEYWORD_OPENIN,       true},
	{"OPENOUT",   SUBTILIS_KEYWORD_OPENOUT,      true},
	{"OPENUP",    SUBTILIS_KEYWORD_OPENUP,       true},
	{"OR",        SUBTILIS_KEYWORD_OR,           true},
	{"ORIGIN",    SUBTILIS_KEYWORD_ORIGIN,       true},
	{"OSCLI",     SUBTILIS_KEYWORD_OSCLI,        true},
	{"OTHERWISE", SUBTILIS_KEYWORD_OTHERWISE,    true},
	{"PI",        SUBTILIS_KEYWORD_PI,           true},
	{"PLOT",      SUBTILIS_KEYWORD_PLOT,         true},
	{"POINT",     SUBTILIS_KEYWORD_POINT,        true},
	{"POS",       SUBTILIS_KEYWORD_POS,          true},
	{"PRINT",     SUBTILIS_KEYWORD_PRINT,        true},
	{"PRINT#",    SUBTILIS_KEYWORD_PRINT_HASH,   true},
	{"PROC",      SUBTILIS_KEYWORD_PROC,         true},
	{"PTR#",      SUBTILIS_KEYWORD_PTR_HASH,     true},
	{"QUIT",      SUBTILIS_KEYWORD_QUIT,         true},
	{"RAD",       SUBTILIS_KEYWORD_RAD,          true},
	{"RECTANGLE", SUBTILIS_KEYWORD_RECTANGLE,    true},
	{"REM",       SUBTILIS_KEYWORD_REM,          true},
	{"REPEAT",    SUBTILIS_KEYWORD_REPEAT,       true},
	{"REPORT",    SUBTILIS_KEYWORD_REPORT,       true},
	{"REPORT$",   SUBTILIS_KEYWORD_REPORT_STR,   true},
	{"RETURN",    SUBTILIS_KEYWORD_RETURN,       true},
	{"RIGHTS$",   SUBTILIS_KEYWORD_RIGHT_STR,    true},
	{"RND",       SUBTILIS_KEYWORD_RND,          true},
	{"SGN",       SUBTILIS_KEYWORD_SGN,          true},
	{"SIN",       SUBTILIS_KEYWORD_SIN,          true},
	{"SOUND",     SUBTILIS_KEYWORD_SOUND,        true},
	{"SPC",       SUBTILIS_KEYWORD_SPC,          true},
	{"SQR",       SUBTILIS_KEYWORD_SQR,          true},
	{"STEP",      SUBTILIS_KEYWORD_STEP,         true},
	{"STEREO",    SUBTILIS_KEYWORD_STEREO,       true},
	{"STOP",      SUBTILIS_KEYWORD_STOP,         true},
	{"STR$",      SUBTILIS_KEYWORD_STRS,         true},
	{"STRING$",   SUBTILIS_KEYWORD_STRING_STR,   true},
	{"SUM",       SUBTILIS_KEYWORD_SUM,          true},
	{"SUMLEN",    SUBTILIS_KEYWORD_SUMLEN,       true},
	{"SWAP",      SUBTILIS_KEYWORD_SWAP,         true},
	{"SYS",       SUBTILIS_KEYWORD_SYS,          true},
	{"TAB",       SUBTILIS_KEYWORD_TAB,          true},
	{"TAN",       SUBTILIS_KEYWORD_TAN,          true},
	{"TEMPO",     SUBTILIS_KEYWORD_TEMPO,        true},
	{"THEN",      SUBTILIS_KEYWORD_THEN,         true},
	{"TIME",      SUBTILIS_KEYWORD_TIME,         true},
	{"TIME$",     SUBTILIS_KEYWORD_TIME_STR,     true},
	{"TINT",      SUBTILIS_KEYWORD_TINT,         true},
	{"TO",        SUBTILIS_KEYWORD_TO,           true},
	{"TRUE",      SUBTILIS_KEYWORD_TRUE,         true},
	{"UNTIL",     SUBTILIS_KEYWORD_UNTIL,        true},
	{"USR",       SUBTILIS_KEYWORD_UNTIL,        true},
	{"VAL",       SUBTILIS_KEYWORD_VAL,          true},
	{"VDU",       SUBTILIS_KEYWORD_VDU,          true},
	{"VOICES",    SUBTILIS_KEYWORD_VOICES,       true},
	{"VPOS",      SUBTILIS_KEYWORD_VPOS,         true},
	{"WAIT",      SUBTILIS_KEYWORD_WAIT,         true},
	{"WHEN",      SUBTILIS_KEYWORD_WHEN,         true},
	{"WHILE",     SUBTILIS_KEYWORD_WHILE,        true},
	{"WIDTH",     SUBTILIS_KEYWORD_WIDTH,        true},
	{"abs",       SUBTILIS_KEYWORD_ABS,          true},
	{"acs",       SUBTILIS_KEYWORD_ACS,          true},
	{"adval",     SUBTILIS_KEYWORD_ADVAL,        true},
	{"and",       SUBTILIS_KEYWORD_AND,          true},
	{"asc",       SUBTILIS_KEYWORD_ASC,          true},
	{"asn",       SUBTILIS_KEYWORD_ASN,          true},
	{"atn",       SUBTILIS_KEYWORD_ATN,          true},
	{"beat",      SUBTILIS_KEYWORD_BEAT,         true},
	{"beats",     SUBTILIS_KEYWORD_BEATS,        true},
	{"bget#",     SUBTILIS_KEYWORD_BGET_HASH,    true},
	{"bput#",     SUBTILIS_KEYWORD_BPUT_HASH,    true},
	{"by",        SUBTILIS_KEYWORD_BY,           true},
	{"call",      SUBTILIS_KEYWORD_CALL,         true},
	{"case",      SUBTILIS_KEYWORD_CASE,         true},
	{"chr$",      SUBTILIS_KEYWORD_CHR_STR,      true},
	{"circle",    SUBTILIS_KEYWORD_CIRCLE,       true},
	{"clg",       SUBTILIS_KEYWORD_CLG,          true},
	{"close#",    SUBTILIS_KEYWORD_CLOSE_HASH,   true},
	{"cls",       SUBTILIS_KEYWORD_CLS,          true},
	{"color",     SUBTILIS_KEYWORD_COLOR,        true},
	{"colour",    SUBTILIS_KEYWORD_COLOUR,       true},
	{"cos",       SUBTILIS_KEYWORD_COS,          true},
	{"count",     SUBTILIS_KEYWORD_COUNT,        true},
	{"def",       SUBTILIS_KEYWORD_DEF,          true},
	{"deg",       SUBTILIS_KEYWORD_DEG,          true},
	{"dim",       SUBTILIS_KEYWORD_DIM,          true},
	{"div",       SUBTILIS_KEYWORD_DIV,          true},
	{"draw",      SUBTILIS_KEYWORD_DRAW,         true},
	{"ellipse",   SUBTILIS_KEYWORD_ELLIPSE,      true},
	{"else",      SUBTILIS_KEYWORD_ELSE,         true},
	{"end",       SUBTILIS_KEYWORD_END,          true},
	{"endcase",   SUBTILIS_KEYWORD_ENDCASE,      true},
	{"enderror",  SUBTILIS_KEYWORD_ENDERROR,     true},
	{"endif",     SUBTILIS_KEYWORD_ENDIF,        true},
	{"endproc",   SUBTILIS_KEYWORD_ENDPROC,      true},
	{"endwhile",  SUBTILIS_KEYWORD_ENDWHILE,     true},
	{"eof#",      SUBTILIS_KEYWORD_EOF_HASH,     true},
	{"eor",       SUBTILIS_KEYWORD_EOR,          true},
	{"erl",       SUBTILIS_KEYWORD_ERL,          false},
	{"err",       SUBTILIS_KEYWORD_ERR,          true},
	{"error",     SUBTILIS_KEYWORD_ERROR,        true},
	{"eval",      SUBTILIS_KEYWORD_EVAL,         false},
	{"exp",       SUBTILIS_KEYWORD_EXP,          true},
	{"ext#",      SUBTILIS_KEYWORD_EXT_HASH,     true},
	{"false",     SUBTILIS_KEYWORD_FALSE,        true},
	{"fill",      SUBTILIS_KEYWORD_FILL,         true},
	{"for",       SUBTILIS_KEYWORD_FOR,          true},
	{"gcol",      SUBTILIS_KEYWORD_GCOL,         true},
	{"get",       SUBTILIS_KEYWORD_GET,          true},
	{"get$",      SUBTILIS_KEYWORD_GET_STR,      true},
	{"get$#",     SUBTILIS_KEYWORD_GET_STR_HASH, true},
	{"if",        SUBTILIS_KEYWORD_IF,           true},
	{"inkey",     SUBTILIS_KEYWORD_INKEY,        true},
	{"inkey$",    SUBTILIS_KEYWORD_INKEY_STR,    true},
	{"input",     SUBTILIS_KEYWORD_INPUT,        true},
	{"input#",    SUBTILIS_KEYWORD_INPUT_HASH,   true},
	{"instr",     SUBTILIS_KEYWORD_INSTR,        true},
	{"int",       SUBTILIS_KEYWORD_INT,          true},
	{"left$",     SUBTILIS_KEYWORD_LEFT_STR,     true},
	{"len",       SUBTILIS_KEYWORD_LEN,          true},
	{"let",       SUBTILIS_KEYWORD_LET,          true},
	{"line",      SUBTILIS_KEYWORD_LINE,         true},
	{"ln",        SUBTILIS_KEYWORD_LN,           true},
	{"local",     SUBTILIS_KEYWORD_LOCAL,        true},
	{"log",       SUBTILIS_KEYWORD_LOG,          true},
	{"mid$",      SUBTILIS_KEYWORD_MID_STR,      true},
	{"mod",       SUBTILIS_KEYWORD_MOD,          true},
	{"mode",      SUBTILIS_KEYWORD_MODE,         true},
	{"mouse",     SUBTILIS_KEYWORD_MOUSE,        true},
	{"move",      SUBTILIS_KEYWORD_MOVE,         true},
	{"next",      SUBTILIS_KEYWORD_NEXT,         true},
	{"not",       SUBTILIS_KEYWORD_NOT,          true},
	{"of",        SUBTILIS_KEYWORD_OF,           true},
	{"off",       SUBTILIS_KEYWORD_OFF,          true},
	{"on",        SUBTILIS_KEYWORD_ON,           true},
	{"onerror",   SUBTILIS_KEYWORD_ONERROR,      true},
	{"openin",    SUBTILIS_KEYWORD_OPENIN,       true},
	{"openout",   SUBTILIS_KEYWORD_OPENOUT,      true},
	{"openup",    SUBTILIS_KEYWORD_OPENUP,       true},
	{"or",        SUBTILIS_KEYWORD_OR,           true},
	{"origin",    SUBTILIS_KEYWORD_ORIGIN,       true},
	{"oscli",     SUBTILIS_KEYWORD_OSCLI,        true},
	{"otherwise", SUBTILIS_KEYWORD_OTHERWISE,    true},
	{"pi",        SUBTILIS_KEYWORD_PI,           true},
	{"plot",      SUBTILIS_KEYWORD_PLOT,         true},
	{"point",     SUBTILIS_KEYWORD_POINT,        true},
	{"pos",       SUBTILIS_KEYWORD_POS,          true},
	{"print",     SUBTILIS_KEYWORD_PRINT,        true},
	{"print#",    SUBTILIS_KEYWORD_PRINT_HASH,   true},
	{"ptr#",      SUBTILIS_KEYWORD_PTR_HASH,     true},
	{"quit",      SUBTILIS_KEYWORD_QUIT,         true},
	{"rad",       SUBTILIS_KEYWORD_RAD,          true},
	{"rectangle", SUBTILIS_KEYWORD_RECTANGLE,    true},
	{"rem",       SUBTILIS_KEYWORD_REM,          true},
	{"repeat",    SUBTILIS_KEYWORD_REPEAT,       true},
	{"report",    SUBTILIS_KEYWORD_REPORT,       true},
	{"report$",   SUBTILIS_KEYWORD_REPORT_STR,   true},
	{"return",    SUBTILIS_KEYWORD_RETURN,       true},
	{"rights$",   SUBTILIS_KEYWORD_RIGHT_STR,    true},
	{"rnd",       SUBTILIS_KEYWORD_RND,          true},
	{"sgn",       SUBTILIS_KEYWORD_SGN,          true},
	{"sin",       SUBTILIS_KEYWORD_SIN,          true},
	{"sound",     SUBTILIS_KEYWORD_SOUND,        true},
	{"spc",       SUBTILIS_KEYWORD_SPC,          true},
	{"sqr",       SUBTILIS_KEYWORD_SQR,          true},
	{"step",      SUBTILIS_KEYWORD_STEP,         true},
	{"stereo",    SUBTILIS_KEYWORD_STEREO,       true},
	{"stop",      SUBTILIS_KEYWORD_STOP,         true},
	{"str$",      SUBTILIS_KEYWORD_STRS,         true},
	{"string$",   SUBTILIS_KEYWORD_STRING_STR,   true},
	{"sum",       SUBTILIS_KEYWORD_SUM,          true},
	{"sumlen",    SUBTILIS_KEYWORD_SUMLEN,       true},
	{"swap",      SUBTILIS_KEYWORD_SWAP,         true},
	{"sys",       SUBTILIS_KEYWORD_SYS,          true},
	{"tab",       SUBTILIS_KEYWORD_TAB,          true},
	{"tan",       SUBTILIS_KEYWORD_TAN,          true},
	{"tempo",     SUBTILIS_KEYWORD_TEMPO,        true},
	{"then",      SUBTILIS_KEYWORD_THEN,         true},
	{"time",      SUBTILIS_KEYWORD_TIME,         true},
	{"time$",     SUBTILIS_KEYWORD_TIME_STR,     true},
	{"tint",      SUBTILIS_KEYWORD_TINT,         true},
	{"to",        SUBTILIS_KEYWORD_TO,           true},
	{"true",      SUBTILIS_KEYWORD_TRUE,         true},
	{"until",     SUBTILIS_KEYWORD_UNTIL,        true},
	{"usr",       SUBTILIS_KEYWORD_UNTIL,        true},
	{"val",       SUBTILIS_KEYWORD_VAL,          true},
	{"vdu",       SUBTILIS_KEYWORD_VDU,          true},
	{"voices",    SUBTILIS_KEYWORD_VOICES,       true},
	{"vpos",      SUBTILIS_KEYWORD_VPOS,         true},
	{"wait",      SUBTILIS_KEYWORD_WAIT,         true},
	{"when",      SUBTILIS_KEYWORD_WHEN,         true},
	{"while",     SUBTILIS_KEYWORD_WHILE,        true},
	{"width",     SUBTILIS_KEYWORD_WIDTH,        true},
};

/* clang-format on */
