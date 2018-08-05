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

#include "test_cases.h"

/* clang-format off */
const subtilis_test_case_t test_cases[] = {
	{ "subtraction",
	  "LET b% = 100 - 5\n"
	  "LET c% = 10 - b% -10\n"
	  "LET d% = c% - b% - 1\n"
	  "PRINT d%\n",
	  "-191\n"},
	{ "division",
	  "LOCAL f%\n"
	  "LET b% = 100 DIV 5\n"
	  "LET c% = 1000 DIV b% DIV 10\n"
	  "LET d% = b% DIV c% DIV 2\n"
	  "LET e% = c% DIV -2\n"
	  "LET f% = -b% DIV -c%\n"
	  "LET g% = -1\n"
	  "PRINT d%\n"
	  "PRINT e%\n"
	  "PRINT f%\n"
	  "PRINT g% DIV 2\n",
	  "2\n-2\n4\n0\n"},
	{ "mod",
	  "LOCAL b%\n"
	  "LET b% = 100 MOD 6\n"
	  "LET c% = 6 MOD b%\n"
	  "LET d% = b% MOD c%\n"
	  "LET e% = -(c% * 10) MOD 3\n"
	  "LET f% = 34\n"
	  "LET g% = f% MOD 4\n"
	  "LET h% = -f% MOD 4\n"
	  "PRINT b%\n"
	  "PRINT c%\n"
	  "PRINT d%\n"
	  "PRINT e%\n"
	  "PRINT g%\n"
	  "PRINT h%\n",
	  "4\n2\n0\n-2\n2\n-2\n"},
	{ "multiplication",
	  "LET b% = 1 * 10\n"
	  "LET c% = 10 * b% * 5\n"
	  "LET d% = c% * 2 * b%\n"
	  "PRINT d%\n",
	  "10000\n"},
	{ "addition",
	  "LET b% = &ff + 10\n"
	  "LET c% = &10 + b% + 5\n"
	  "LET d% = c% + 2 + b%\n"
	  "PRINT d%\n",
	  "553\n"},
	{ "expression",
	  "LET a% = (1 + 1)\n"
	  "LET b% = ((((10 - a%) -5) * 10))\n"
	  "PRINT b%",
	  "30\n"},
	{ "unary_minus",
	  "LET b% = -110\n"
	  "LET c% = -b%\n"
	  "LET d% = 10 - -(c% - 0)\n"
	  "PRINT d%\n"
	  "PRINT -&ffffffff\n"
	  "PRINT -b% + c%\n",
	  "120\n1\n220\n"},
	{ "and",
	  "LET b% = -1\n"
	  "PRINT b% AND -1 AND TRUE AND b%"
	  "LET c% = b% AND FALSE\n"
	  "PRINT c%\n"
	  "PRINT 255 AND 128\n",
	  "-1\n0\n128\n"},
	{ "or",
	  "LET b% = -1\n"
	  "PRINT b% OR -1 OR TRUE OR b%\n"
	  "LET c% = b% OR FALSE\n"
	  "PRINT c%\n"
	  "PRINT FALSE OR FALSE\n"
	  "LET d% = FALSE\n"
	  "PRINT d% OR FALSE\n"
	  "PRINT 64 OR 128\n",
	  "-1\n-1\n0\n0\n192\n"},
	{ "eor",
	  "LET b% = -1\n"
	  "PRINT b% EOR -1 EOR TRUE EOR b%\n"
	  "LET c% = b% EOR FALSE\n"
	  "PRINT c%\n"
	  "PRINT FALSE EOR FALSE\n"
	  "PRINT TRUE EOR TRUE\n"
	  "LET d% = FALSE\n"
	  "PRINT d% EOR FALSE\n"
	  "PRINT 192 EOR 128\n",
	  "0\n-1\n0\n0\n0\n64\n"},
	{ "not",
	  "LET b% = -1\n"
	  "PRINT NOT b%\n"
	  "PRINT NOT NOT b%\n"
	  "PRINT NOT TRUE\n"
	  "PRINT NOT &fffffff0\n"
	  "LET c% = &fffffff0\n"
	  "PRINT NOT c% AND &f\n",
	  "0\n-1\n0\n15\n15\n"},
	{ "eq",
	  "LET b% = &ff\n"
	  "PRINT 10 = 5 + 5\n"
	  "PRINT b% = 255\n"
	  "PRINT b% = 254\n"
	  "LET c% = 255\n"
	  "PRINT c% = b%\n"
	  "LET c% = b% = 254\n"
	  "PRINT c%\n",
	  "-1\n-1\n0\n-1\n0\n"},
	{ "neq",
	  "LET b% = &ff\n"
	  "PRINT 10 <> 5 + 5\n"
	  "PRINT b% <> 255\n"
	  "PRINT b% <> 254\n"
	  "LET c% = 255\n"
	  "PRINT c% <> b%\n"
	  "LET c% = b% <> 254\n"
	  "PRINT c%\n",
	  "0\n0\n-1\n0\n-1\n"},
	{ "gt",
	  "LET b% = &ff\n"
	  "PRINT 10 > 5 + 5\n"
	  "PRINT b% > 255\n"
	  "PRINT 256 > b%\n"
	  "LET c% = 255\n"
	  "PRINT c% > b%\n"
	  "LET c% = b% > 254\n"
	  "PRINT c%\n",
	  "0\n0\n-1\n0\n-1\n"},
	{ "lte",
	  "LET b% = &ff\n"
	  "PRINT 10 <= 5 + 5\n"
	  "PRINT b% <= 255\n"
	  "PRINT 256 <= b%\n"
	  "LET c% = 255\n"
	  "PRINT c% <= b% +1\n"
	  "LET c% = b% <= 254\n"
	  "PRINT c%\n",
	  "-1\n-1\n0\n-1\n0\n"},
	{ "lt",
	  "LET b% = &ff\n"
	  "PRINT 10 < 5 + 5\n"
	  "PRINT b% < 255\n"
	  "PRINT 256 < b%\n"
	  "LET c% = 255\n"
	  "PRINT c% < b%\n"
	  "LET c% = 254 < b%\n"
	  "PRINT c%\n",
	  "0\n0\n0\n0\n-1\n"},
	{ "gte",
	  "LET b% = &ff\n"
	  "PRINT 10 >= 5 + 5\n"
	  "PRINT b% >= 255\n"
	  "PRINT 256 >= b%\n"
	  "LET c% = 255\n"
	  "PRINT c% >= b%\n"
	  "LET c% = 254 >= b%\n"
	  "PRINT c%\n",
	  "-1\n-1\n-1\n-1\n0\n"},
	{ "if",
	  "IF 10 >= 5 + 5 THEN\n"
	  "  PRINT TRUE\n"
	  "ELSE\n"
	  "  PRINT FALSE\n"
	  "ENDIF\n"
	  "PRINT 32\n"
	  "LET b% = &ff\n"
	  "IF b% > 255 THEN PRINT 100 ENDIF\n"
	  "IF b% <= 255 THEN\n"
	  "  PRINT 200\n"
	  "  PRINT 300\n"
	  "ENDIF\n",
	  "-1\n32\n200\n300\n"},
	{ "while",
	  "LET i% = 0\n"
	  "WHILE i% < 5\n"
	  "  PRINT i%\n"
	  "  LET i%=i%+1\n"
	  "ENDWHILE\n",
	  "0\n1\n2\n3\n4\n"},
	{ "if_and_while",
	  "LET i% = 0\n"
	  "WHILE i% < 6\n"
	  "  IF i% AND 1 THEN\n"
	  "    PRINT FALSE\n"
	  "  ELSE\n"
	  "    PRINT TRUE\n"
	  "  ENDIF\n"
	  "  LET i%=i%+1\n"
	  "ENDWHILE\n",
	  "-1\n0\n-1\n0\n-1\n0\n"},
	{ "eq_and_neq_br_imm",
	  "LET i% = 0\n"
	  "WHILE i% <> 6\n"
	  "  LET i%=i%+1\n"
	  "ENDWHILE\n"
	  "PRINT i%\n"
	  "IF i% = 6 THEN\n"
	  "  PRINT TRUE\n"
	  "ELSE\n"
	  "  PRINT FALSE\n"
	  "ENDIF\n",
	  "6\n-1\n"},
	{ "eq_and_neq_br",
	  "LET i% = 0\n"
	  "LET j% = 6\n"
	  "WHILE i% <> j%\n"
	  "  LET i%=i%+1\n"
	  "ENDWHILE\n"
	  "PRINT i%\n"
	  "IF i% = j% THEN\n"
	  "  PRINT TRUE\n"
	  "ELSE\n"
	  "  PRINT FALSE\n"
	  "ENDIF\n",
	  "6\n-1\n"},
	{ "basic_proc",
	  "LET i% = 0\n"
	  "PROCInci\n"
	  "PROCInci\n"
	  "PRINT i%\n"
	  "DEF PROCInci"
	  "  LET i%=i%+1\n"
	  "ENDPROC\n",
	  "2\n"},
	{ "local",
	  "LOCAL i%\n"
	  "LET i% = 5\n"
	  "LET i%=i%+1\n"
	  "PROCInci\n"
	  "PRINT i%\n"
	  "DEF PROCInci"
	  "  LOCAL i%\n"
	  "  LET i%=i%+1\n"
	  "  PRINT i%\n"
	  "ENDPROC\n",
	  "1\n6\n"},
	{ "proc_args",
	  "LOCAL i%\n"
	  "LET i% = 1\n"
	  "LET j% = 2\n"
	  "PROCAdd(i%, j%, 10, 11, 12)\n"
	  "DEF PROCAdd(a%, b%, c%, d%, e%)\n"
	  "PRINT a% + b% + c% +d% + e%\n"
	  "ENDPROC\n",
	  "36\n"},
	{ "fn_fact",
	  "LOCAL x%\n"
	  "LET x% = FNFac%(4)\n"
	  "PRINT x%\n"
	  "DEF FNFac%(a%)\n"
	  "  LOCAL res%\n"
	  "  IF a% <= 1 THEN\n"
	  "    LET res% = 1\n"
	  "  ELSE\n"
	  "    LET res% = a% * FNFac%(a%-1)\n"
	  "  ENDIF\n"
	  "=res%\n",
	  "24\n"},
	{ "abs",
	  "LET x%=-10\n"
	  "PRINT ABS(x%)\n"
	  "PRINT ABS(10)\n",
	  "10\n10\n"},
};

/* clang-format on */
