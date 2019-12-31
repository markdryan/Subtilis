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
	  "LET e% = d% - 1030\n"
	  "PRINT d%\n"
	  "PRINT e%\n",
	  "-191\n-1221\n"},
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
	  "LOCAL b% = 100 MOD 6\n"
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
	  "LOCAL i%= 5\n"
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
	  "    IF a% <= 1 THEN\n"
	  "       <- 1\n"
	  "    ENDIF\n"
	  "<- a% * FNFac%(a%-1)\n",
	  "24\n"},
	{ "fn_fact_no_let",
	  "LOCAL x%\n"
	  "x% = FNFac%(4)\n"
	  "PRINT x%\n"
	  "DEF FNFac%(a%)\n"
	  "  LOCAL res%\n"
	  "  IF a% <= 1 THEN\n"
	  "    res% = 1\n"
	  "  ELSE\n"
	  "    res% = a% * FNFac%(a%-1)\n"
	  "  ENDIF\n"
	  "<-res%\n",
	  "24\n"},
	{ "abs",
	  "LOCAL A\n"
	  "LOCAL x%\n"
	  "\n"
	  "LET x%=-10\n"
	  "PRINT ABS(x%)\n"
	  "PRINT ABS(10)\n"
	  "PRINT INT(ABS(10.0))\n"
	  "PRINT INT(ABS(-10.0))\n"
	  "A = 10\n"
	  "PRINT INT(ABS(A))\n"
	  "A = -10\n"
	  "PRINT INT(ABS(A))\n"
	  "\n"
	  "PRINT ABS(-1)\n"
	  "X% = &ffffffff\n"
	  "PRINT ABS(X%)\n"
	  "\n"
	  "PRINT ABS(&80000000)\n"
	  "X% = &80000000\n"
	  "PRINT ABS(X%)\n",
	  "10\n10\n10\n10\n10\n10\n1\n1\n-2147483648\n-2147483648\n"},
	{ "fpa_small",
	  "LOCAL x\n"
	  "LOCAL y\n"
	  "LOCAL a\n"
	  "LOCAL b\n"
	  "LOCAL c\n"
	  "LOCAL d\n"
	  "\n"
	  "LET x = 2.0\n"
	  "LET y = 3.14\n"
	  "LET z = (x+x) / 2\n"
	  "LET a = z - x -10.0\n"
	  "LET b = a * 10\n"
	  "\n"
	  "PROCa(a,b,x,y,z)\n"
	  "\n"
	  "LET c = b * a\n"
	  "LET d = 2 / c\n"
	  "LET e% = d\n"
	  "LET f = e%\n"
	  "LET h = -y\n"
	  "LET e% = z\n"
	  "PRINT e%\n"
	  "\n"
	  "DEF PROCa(a,b,c,d,e)\n"
	  "LOCAL a%\n"
	  "LET a% = a + b + c + d + e\n"
	  "PRINT a%\n"
	  "ENDPROC\n",
	  "-102\n2\n"},
	{ "fpa_logical",
	  "LOCAL a = 3.14\n"
	  "LET b = 17.6666\n"
	  "LET c = 3.14\n"
	  "PRINT a < b\n"
	  "PRINT a > b\n"
	  "PRINT a = b\n"
	  "PRINT a = c\n"
	  "PRINT a <> b\n"
	  "PRINT a <> c\n"
	  "PRINT a < 17.6666\n"
	  "PRINT a > 17.6666\n"
	  "PRINT a = 17.6666\n"
	  "PRINT a = 3.14\n"
	  "PRINT a <> 17.6666\n"
	  "PRINT a <> 3.14\n"
	  "PRINT 3.14 < b\n"
	  "PRINT 3.14 > b\n"
	  "PRINT 3.14 = b\n"
	  "PRINT 3.14 = c\n"
	  "PRINT 3.14 <> b\n"
	  "PRINT 3.14 <> c\n"
	  "PRINT a <> 2.0\n"
	  "PRINT a OR b\n"
	  "PRINT a AND b\n"
	  "PRINT a EOR b\n"
	  "PRINT NOT a\n"
	  "PRINT a OR 17.6666\n"
	  "PRINT a AND 17.6666\n"
	  "PRINT a EOR 17.6666\n"
	  "PRINT NOT 3.14\n",
	  "-1\n0\n0\n-1\n-1\n0\n-1\n0\n0\n-1\n-1\n0\n-1\n0\n0\n"
	  "-1\n-1\n0\n-1\n19\n1\n18\n-4\n19\n1\n18\n-4\n"},
	{ "fpa_if",
	  "LOCAL a\n"
	  "LET a = 3.14\n"
	  "LET B% = 11\n"
	  "IF B% > a THEN\n"
	  "PRINT TRUE\n"
	  "ELSE\n"
	  "PRINT FALSE\n"
	  "ENDIF\n"
	  "IF 10 < a THEN\n"
	  "PRINT TRUE\n"
	  "ELSE\n"
	  "PRINT FALSE\n"
	  "ENDIF\n"
	  "IF 10 <= a THEN\n"
	  "PRINT TRUE\n"
	  "ELSE\n"
	  "PRINT FALSE\n"
	  "ENDIF\n"
	  "PROCIf2\n"
	  "DEF PROCIf2\n"
	  "LOCAL a\n"
	  "a = 3.14\n"
	  "IF 11 = a THEN\n"
	  "PRINT TRUE\n"
	  "ELSE\n"
	  "PRINT FALSE\n"
	  "ENDIF\n"
	  "IF 3.14 = a THEN\n"
	  "PRINT TRUE\n"
	  "ELSE\n"
	  "PRINT FALSE\n"
	  "ENDIF\n"
	  "IF 3.14 <> a THEN\n"
	  "PRINT TRUE\n"
	  "ELSE\n"
	  "PRINT FALSE\n"
	  "ENDIF\n"
	  "IF 3.14 >= a THEN\n"
	  "PRINT TRUE\n"
	  "ELSE\n"
	  "PRINT FALSE\n"
	  "ENDIF\n"
	  "PROCTestVars(a, 11.0)\n"
	  "ENDPROC\n"
	  "DEF PROCTestVars(a,b)\n"
	  "IF a < b THEN\n"
	  "PRINT TRUE\n"
	  "ELSE\n"
	  "PRINT FALSE\n"
	  "ENDIF\n"
	  "IF a <= a THEN\n"
	  "PRINT TRUE\n"
	  "ELSE\n"
	  "PRINT FALSE\n"
	  "ENDIF\n"
	  "IF a > b THEN\n"
	  "PRINT TRUE\n"
	  "ELSE\n"
	  "PRINT FALSE\n"
	  "ENDIF\n"
	  "IF b >= b THEN\n"
	  "PRINT TRUE\n"
	  "ELSE\n"
	  "PRINT FALSE\n"
	  "ENDIF\n"
	  "IF a = b THEN\n"
	  "PRINT TRUE\n"
	  "ELSE\n"
	  "PRINT FALSE\n"
	  "ENDIF\n"
	  "IF a <> b THEN\n"
	  "PRINT TRUE\n"
	  "ELSE\n"
	  "PRINT FALSE\n"
	  "ENDIF\n"
	  "ENDPROC\n",
	  "-1\n0\n0\n0\n-1\n0\n-1\n-1\n-1\n0\n-1\n0\n-1\n"},
	{ "repeat",
	  "LET i% = 0\n"
	  "REPEAT\n"
	  "  PRINT i%\n"
	  "  LET i%=i%+1\n"
	  "UNTIL i%=5\n",
	  "0\n1\n2\n3\n4\n"},
	{ "int_reg_alloc_basic",
	  "LOCAL a%\n"
	  "LOCAL b%\n"
	  "LOCAL c%\n"
	  "LOCAL d%\n"
	  "LOCAL e%\n"
	  "LOCAL f%\n"
	  "LOCAL i%\n"
	  "LOCAL g%\n"
	  "LOCAL h%\n"
	  "LOCAL k%\n"
	  "LOCAL l%\n"
	  "LOCAL m%\n"
	  "LOCAL n%\n"
	  "LOCAL o%\n"
	  "\n"
	  "LET a% = 0\n"
	  "LET b% = 1\n"
	  "LET c% = 2\n"
	  "LET d% = 3\n"
	  "LET e% = 4\n"
	  "LET f% = 5\n"
	  "LET k% = 6\n"
	  "LET l% = 7\n"
	  "LET n% = 8\n"
	  "\n"
	  "LET g% = a% + b% + c% + d% + e% + f% + i% + k% + l% + n%\n"
	  "LET o% = g%\n"
	  "LET h% = a% + b% + c% + d% + e% + f% + i% + k% + l% + n%\n"
	  "LET m% = a% + b% + c% + d% + e% + f% + i% + k% + l% + n%\n"
	  "PRINT g%\n"
	  "PRINT h%\n"
	  "PRINT m%\n"
	  "PRINT o%\n"
	  "LET i% = i% + 1\n",
	  "36\n36\n36\n36\n"},
	{"fpa_reg_alloc_basic",
	 "LOCAL a\n"
	 "LOCAL b\n"
	 "LOCAL c\n"
	 "LOCAL d\n"
	 "LOCAL e\n"
	 "LOCAL f\n"
	 "LOCAL g\n"
	 "LOCAL h\n"
	 "LOCAL i\n"
	 "LOCAL m\n"
	 "\n"
	 "LET a = 0\n"
	 "LET b = 1\n"
	 "LET c = 2\n"
	 "LET d = 3\n"
	 "LET e = 4\n"
	 "LET i = 5\n"
	 "\n"
	 "LET a = a + 1\n"
	 "LET b = b + 1\n"
	 "LET c = c + 1\n"
	 "LET d = d + 1\n"
	 "LET e = e + 1\n"
	 "LET i = i + 1\n"
	 "\n"
	 "LET g = a + b + c + d + e + i\n"
	 "LET h = a + b + c + d + e + i\n"
	 "LET m = a + b + c + d + e + i\n"
	 "LET a% = g\n"
	 "PRINT a%\n"
	 "LET a% = h\n"
	 "PRINT a%\n"
	 "LET a% = m\n"
	 "PRINT a%\n"
	 "LET i = i + 1\n",
	 "21\n21\n21\n"},
	{"fp_save",
	"LOCAL a\n"
	"LOCAL b\n"
	"LOCAL c\n"
	"LOCAL d\n"
	"LOCAL e\n"
	"LOCAL f\n"
	"\n"
	"LET a = 1\n"
	"LET b = 2\n"
	"LET c = 3\n"
	"LET d = 4\n"
	"LET e = 4\n"
	"LET f = 5\n"
	"\n"
	"PROCOverwrite(f)\n"
	"\n"
	"LET a% = a + b + c + d\n"
	"PRINT a%\n"
	"\n"
	"DEF PROCOverwrite(a)\n"
	"LOCAL b\n"
	"LOCAL c\n"
	"LOCAL a%\n"
	"\n"
	"LET a = 10\n"
	"LET b = 20\n"
	"LET c = 30\n"
	"LET a% = a + b + c\n"
	"PRINT a%\n"
	"ENDPROC\n",
	 "60\n10\n"
	},
	{"int_save",
	 "LOCAL a%\n"
	 "LOCAL b%\n"
	 "LOCAL c%\n"
	 "LOCAL d%\n"
	 "LOCAL e%\n"
	 "LOCAL f%\n"
	 "\n"
	 "LET a% = 1\n"
	 "LET b% = 2\n"
	 "LET c% = 3\n"
	 "LET d% = 4\n"
	 "LET e% = 4\n"
	 "LET f% = 5\n"
	 "\n"
	 "PROCOverwrite(f%)\n"
	 "\n"
	 "LET a% = a% + b% + c% + d%\n"
	 "PRINT a%\n"
	 "\n"
	 "DEF PROCOverwrite(a%)\n"
	 "LOCAL b%\n"
	 "LOCAL c%\n"
	 "\n"
	 "LET a% = 10\n"
	 "LET b% = 20\n"
	 "LET c% = 30\n"
	 "LET a% = a% + b% + c%\n"
	 "PRINT a%\n"
	 "ENDPROC\n",
	 "60\n10\n"},
	{"branch_save",
	 "LOCAL a%\n"
	 "LOCAL c%\n"
	 "WHILE c% < 2\n"
	 "PRINT a%\n"
	 "PROCa\n"
	 "LET c% = c% + 1\n"
	 "ENDWHILE\n"
	 "DEF PROCa\n"
	 "LOCAL b%\n"
	 "LET b% = 2\n"
	 "ENDPROC\n",
	 "0\n0\n"},
	{"time",
	 "LOCAL a%\n"
	 "LET a% = TIME\n"
	 "PRINT TRUE\n",
	 "-1\n"},
	{"cos_and_sin",
	 "LOCAL a\n"
	 "PROCCheck(SIN(0), 0)\n"
	 "PROCCheck(COS(0), 1)\n"
	 "LET a = 0\n"
	 "LET b = 30\n"
	 "LET c = 60\n"
	 "PROCCheck(SIN(a), 0)\n"
	 "PROCCheck(COS(a), 1)\n"
	 "PROCCheck(SIN(RAD(30)), 0.5)\n"
	 "PROCCheck(COS(RAD(60)), 0.5)\n"
	 "LET a = RAD(b)\n"
	 "PROCCheck(SIN(a), 0.5)\n"
	 "LET a = RAD(c)\n"
	 "PROCCheck(COS(a), 0.5)\n"
	 "DEF PROCCheck(a, e)\n"
	 "LET a = e - a\n"
	 "IF a < 0.0 THEN LET a = -a ENDIF\n"
	 "PRINT a < 0.001\n"
	 "ENDPROC\n",
	 "-1\n-1\n-1\n-1\n-1\n-1\n-1\n-1\n"},
	{"pi",
	 "PROCCheck(2 * PI , RAD(360))\n"
	 "DEF PROCCheck(a, e)\n"
	 "LET a = e - a\n"
	 "IF a < 0.0 THEN LET a = -a ENDIF\n"
	 "PRINT a < 0.001\n"
	 "ENDPROC\n",
	 "-1\n"},
	{"sqr",
	 "PROCCheck(SQR(2), 1.414)\n"
	 "LET A=2\n"
	 "PROCCheck(SQR(A), 1.414)\n"
	 "DEF PROCCheck(a, e)\n"
	 "LET a = e - a\n"
	 "IF a < 0.0 THEN LET a = -a ENDIF\n"
	 "PRINT a < 0.001\n"
	 "ENDPROC\n",
	 "-1\n-1\n"},
	{"mixed_args",
	 "PROCAdd(10, 10.0)\n"
	 "DEF PROCAdd(A%, A)\n"
	 "LOCAL R%\n"
	 "LET R% = A% + A\n"
	 "PRINT R%\n"
	 "ENDPROC",
	 "20\n"},
	{"vdu",
	 "LOCAL C%\n"
	 "LOCAL D%\n"
	 "VDU 115, 117, 98, 116, 105, 108, 105, 115\n"
	 "VDU [32]\n"
	 "VDU [&7573;&7462;&6C69;&7369;]\n"
	 "LET C% = 98\n"
	 "LET D% = &7573\n"
	 "VDU 32\n"
	 "VDU D%; C%, 116, 105, 108, 105, 115\n",
	 "subtilis subtilis subtilis"},
	{"void_fn",
	 "LOCAL C%\n"
	 "LET C%=FNA\n"
	 "PRINT C%\n"
	 "PROCB\n"
	 "DEF PROCB\n"
	 "PRINT FNA%\n"
	 "ENDPROC\n"
	 "DEF FNA%\n"
	 "<--1\n"
	 "DEF FNA\n"
	 "<-1.0\n",
	 "1\n-1\n"},
	{"assign_fn",
	 "LOCAL C%\n"
	 "PRINT FNA%\n"
	 "DEF FNA%\n"
	 "LOCAL A%\n"
	 "A%= 10\n"
	 "<-A%\n",
	 "10\n"},
	{"for_basic_int",
	 "LOCAL I%\n"
	 "FOR I% = 1 TO 4\n"
	 "PRINT I%\n"
	 "NEXT\n"
	 "FOR I% = 1 TO 0\n"
	 "PRINT I%\n"
	 "NEXT\n"
	 "FOR A% = 1 TO 4\n"
	 "PRINT A%\n"
	 "NEXT\n"
	 "FOR A% = 1 TO 0\n"
	 "PRINT A%\n"
	 "NEXT\n",
	 "1\n2\n3\n4\n1\n1\n2\n3\n4\n1\n"},
	{"for_basic_real",
	 "LOCAL I\n"
	 "LOCAL A%\n"
	 "FOR I = 1 TO 4\n"
	 "A%=I\n"
	 "PRINT A%\n"
	 "NEXT\n"
	 "FOR I = 1 TO 0\n"
	 "A%=I\n"
	 "PRINT A%\n"
	 "NEXT\n"
	 "FOR A = 1 TO 4\n"
	 "A%=A\n"
	 "PRINT A%\n"
	 "NEXT\n"
	 "FOR A = 1 TO 0\n"
	 "A%=A\n"
	 "PRINT A%\n"
	 "NEXT\n",
	 "1\n2\n3\n4\n1\n1\n2\n3\n4\n1\n"},
	{"for_step_int_const",
	 "FOR A% = 1 TO 10 STEP 2\n"
	 "PRINT A%\n"
	 "NEXT\n"
	 "FOR A% = 10 TO 0 STEP -2\n"
	 "PRINT A%\n"
	 "NEXT\n",
	 "1\n3\n5\n7\n9\n10\n8\n6\n4\n2\n0\n"},
	{"for_step_real_const",
	 "LOCAL A\n"
	 "LOCAL A%\n"
	 "FOR A = 1 TO 10 STEP 2\n"
	 "A%=A\n"
	 "PRINT A%\n"
	 "NEXT\n"
	 "FOR I = 10 TO 0 STEP -2\n"
	 "A%=I\n"
	 "PRINT A%\n"
	 "NEXT\n",
	 "1\n3\n5\n7\n9\n10\n8\n6\n4\n2\n0\n"},
	{"for_step_int_var",
	 "LOCAL S%\n"
	 "LOCAL A%\n"
	 "S% = 2\n"
	 "FOR A% = 0 TO 10 STEP S%\n"
	 "PRINT A%\n"
	 "NEXT\n"
	 "S% = -2\n"
	 "FOR A% = -1 TO -10 STEP S%\n"
	 "PRINT A%\n"
	 "NEXT\n"
	 "FOR A% = 0 TO -10 STEP S%\n"
	 "PRINT A%\n"
	 "NEXT\n",
	 "0\n2\n4\n6\n8\n10\n-1\n-3\n-5\n-7\n-9\n0\n-2\n-4\n-6\n-8\n-10\n"},
	{"for_step_real_var",
	 "LOCAL S\n"
	 "LOCAL A\n"
	 "LOCAL A%\n"
	 "S = 1.5\n"
	 "FOR A = 0 TO 10 STEP S\n"
	 "A% = A\n"
	 "PRINT A%\n"
	 "NEXT\n"
	 "S = 2.0\n"
	 "FOR A = -1 TO -10 STEP -S\n"
	 "A% = A\n"
	 "PRINT A%\n"
	 "NEXT\n"
	 "S = -2.0\n"
	 "FOR A = 0 TO -10 STEP S\n"
	 "A% = A\n"
	 "PRINT A%\n"
	 "NEXT\n",
	 "0\n1\n3\n4\n6\n7\n9\n-1\n-3\n-5\n-7\n-9\n0\n-2\n-4\n-6\n-8\n-10\n"},
	{"for_mod_step",
	 "LOCAL A%\n"
	 "LOCAL S%\n"
	 "LOCAL A\n"
	 "LOCAL S\n"
	 "S% = 2\n"
	 "FOR A% = 0 TO 10 STEP S%\n"
	 "PRINT A%\n"
	 "S%=S%+1\n"
	 "NEXT\n"
	 "S = 2\n"
	 "FOR A = 0 TO 10 STEP S\n"
	 "A% = A\n"
	 "PRINT A%\n"
	 "S=S+1\n"
	 "NEXT\n",
	 "0\n2\n4\n6\n8\n10\n0\n2\n4\n6\n8\n10\n"},
	{"point_tint",
	 "CLG\n"
	 "PRINT POINT(0,0)\n"
	 "PRINT TINT(0,0)\n",
	 "0\n0\n"},
	{"reg_alloc_buster",
	 "PROCTWELVE(1,2,3,4,5,6,7.0,8.0,9.0,10.0,11.0,12.0)\n"
	 "DEF PROCTWELVE(A%, B%, C%, D%, E%, F%, A, B, C, D, E, F)\n"
	 "  LOCAL Z%\n"
	 "  LOCAL G\n"
	 "  LOCAL H\n"
	 "  LOCAL I\n"
	 "\n"
	 "  PRINT A%\n"
	 "  PRINT B%\n"
	 "  PRINT C%\n"
	 "  PRINT D%\n"
	 "  PRINT E%\n"
	 "  PRINT F%\n"
	 "  Z% = A\n"
	 "  PRINT Z%\n"
	 "  Z% = B\n"
	 "  PRINT Z%\n"
	 "  Z% = C\n"
	 "  PRINT Z%\n"
	 "  Z% = D\n"
	 "  PRINT Z%\n"
	 "  Z% = E\n"
	 "  PRINT Z%\n"
	 "  Z% = F\n"
	 "  PRINT Z%\n"
	 "  G = F + 1\n"
	 "  H = G + 1\n"
	 "  I = H + 1\n"
	 "  Z% = G\n"
	 "  PRINT Z%\n"
	 "  Z% = H\n"
	 "  PRINT Z%\n"
	 "  Z% = I\n"
	 "  PRINT Z%\n"
	 "  Z% = A\n"
	 "  PRINT Z%\n"
	 "  Z% = B\n"
	 "  PRINT Z%\n"
	 "  Z% = C\n"
	 "  PRINT Z%\n"
	 "  Z% = D\n"
	 "  PRINT Z%\n"
	 "  Z% = E\n"
	 "  PRINT Z%\n"
	 "  Z% = F\n"
	 "ENDPROC\n",
	 "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n7\n8\n9\n10\n11\n"
	},
	{"save_third",
	 "PROCA(1,2,3)\n"
	 "PROCF(1.0,2.0,3.0)\n"
	 "DEF PROCA(A%, B%, C%)\n"
	 "  PROCB(A%, B%)\n"
	 "  PRINT C%\n"
	 "ENDPROC\n"
	 "DEF PROCB(A%, B%)\n"
	 "    PRINT A%\n"
	 "    PRINT B%\n"
	 "ENDPROC\n"
	 "DEF PROCF(A, B, C)\n"
	 "    LOCAL A%\n"
	 "    PROCG\n"
	 "    A% = C\n"
	 "    PRINT A%\n"
	 "ENDPROC\n"
	 "DEF PROCG\n"
	 "    PROCH(4.0, 5.0, 6.0)\n"
	 "ENDPROC\n"
	 "DEF PROCH(A, B, C)\n"
	 "ENDPROC\n",
	 "1\n2\n3\n3\n"
	},
	{"save_useless_move",
	 "PROCA(10)\n"
	 "\n"
	 "DEF PROCA(A%)\n"
	 "    PROCB\n"
	 "    PRINT A%\n"
	 "ENDPROC\n"
	 "\n"
	 "DEF PROCB\n"
	 "    GCOL 0, 1\n"
	 "ENDPROC\n",
	 "10\n"
	},
	{"save_fixed_reg_arg",
	 "PROCFOR(2)\n"
	 "\n"
	 "DEF PROCFOR(S%)\n"
	 "  LOCAL I%\n"
	 "\n"
	 "  GCOL 0, 2\n"
	 "  FOR I% = -10 TO 10 STEP S%\n"
	 "    PRINT I%\n"
	 "  NEXT\n"
	 "ENDPROC\n",
	 "-10\n-8\n-6\n-4\n-2\n0\n2\n4\n6\n8\n10\n"
	},
	{ "cmp_imm",
	"FOR X% = 0 TO 1300 STEP 1300\n"
	"    PRINT X%\n"
	"NEXT",
	"0\n1300\n"
	},
	{ "assignment_ops",
	  "LOCAL A%\n"
	  "LOCAL B%\n"
	  "LOCAL B\n"
	  "\n"
	  "A% = 10\n"
	  "A% += 1\n"
	  "PRINT A%\n"
	  "A% -= 10\n"
	  "PRINT A%\n"
	  "\n"
	  "B% = 10\n"
	  "A% += B%\n"
	  "PRINT A%\n"
	  "A% -= B%\n"
	  "PRINT A%\n"
	  "\n"
	  "A% += 10.0\n"
	  "PRINT A%\n"
	  "A% -= 10.0\n"
	  "PRINT A%\n"
	  "\n"
	  "B = 10.0\n"
	  "A% += B\n"
	  "PRINT A%\n"
	  "A% -= B\n"
	  "PRINT A%\n"
	  "\n"
	  "C = 1\n"
	  "C += 10.0\n"
	  "A% = C\n"
	  "PRINT A%\n"
	  "\n"
	  "C -= 10\n"
	  "A% = C\n"
	  "PRINT A%\n"
	  "\n"
	  "C += B\n"
	  "A% = C\n"
	  "PRINT A%\n"
	  "\n"
	  "C -= B\n"
	  "A% = C\n"
	  "PRINT A%\n"
	  "\n"
	  "C += B%\n"
	  "A% = C\n"
	  "PRINT A%\n"
	  "C -= B%\n"
	  "A% = C\n"
	  "PRINT A%\n",
	  "11\n1\n11\n1\n11\n1\n11\n1\n11\n1\n11\n1\n11\n1\n"
	},
	{ "int",
	  "PRINT INT(1.5)\n"
	  "PRINT INT(-1.5)\n"
	  "A = 1.5\n"
	  "\n"
	  "PRINT INT(A)\n"
	  "PRINT INT(-A)\n",
	  "1\n-2\n1\n-2\n"
	},
	{ "trig",
	  "PRINT INT(TAN(RAD(45)))\n"
	  "A = RAD(45)\n"
	  "PRINT INT(TAN(A))\n"
	  "\n"
	  "A = ASN(0.5) - 0.5236\n"
	  "PRINT NOT (A > 0.001 OR A < - 0.001)\n"
	  "\n"
	  "A = 0.5\n"
	  "A = ASN(A) - 0.5236\n"
	  "PRINT NOT (A > 0.001 OR A < - 0.001)\n"
	  "\n"
	  "A = ACS(0.3) - 1.2661\n"
	  "PRINT NOT (A > 0.001 OR A < - 0.001)\n"
	  "\n"
	  "A = 0.3\n"
	  "A = ACS(A) - 1.2661\n"
	  "PRINT NOT (A > 0.001 OR A < - 0.001)\n"
	  "\n"
	  "A = ATN(0.8) - 0.6747\n"
	  "PRINT NOT (A > 0.001 OR A < - 0.001)\n"
	  "\n"
	  "A = 0.8\n"
	  "A = ATN(A) - 0.6747\n"
	  "PRINT NOT (A > 0.001 OR A < - 0.001)\n",
	  "1\n1\n-1\n-1\n-1\n-1\n-1\n-1\n"
	},
	{"log",
	 "PRINT INT(LOG(1000))\n"
	 "A = 1000\n"
	 "PRINT INT(LOG(A))\n"
	 "\n"
	 "A = LN(10)\n"
	 "A -= 2.30258509\n"
	 "PRINT NOT (A > 0.001 OR A < - 0.001)\n"
	 "\n"
	 "A = 10\n"
	 "A = LN(A) - 2.30258509\n"
	 "PRINT NOT (A > 0.001 OR A < - 0.001)\n",
	 "3\n3\n-1\n-1\n"},
	{"rnd_int",
	 "X%=100\n"
	 "PRINT INT(RND(-X%))\n"
	 "FOR I% = 0 TO 5\n"
	 "PRINT INT(RND(X%))\n"
	 "NEXT\n"
	 "PRINT RND(-100)\n"
	 "FOR I% = 0 TO 5\n"
	 "PRINT RND(100)\n"
	 "NEXT\n",
	 "100\n24\n99\n74\n1\n16\n23\n100\n24\n99\n74\n1\n16\n23\n"},
	{"rnd_real",
	 "X = RND(1)\n"
	 "PRINT X >= 0 AND X <= 1\n"
	 "Y = RND(0)\n"
	 "PRINT X = Y\n"
	 "I = 1\n"
	 "X = RND(I)"
	 "PRINT X >= 0 AND X <= 1\n"
	 "I = 0\n"
	 "Y = RND(I)\n"
	 "PRINT X = Y\n",
	 "-1\n-1\n-1\n-1\n"},
	{"printfp",
	 "PRINT PI\n"
	 "PRINT -PI\n"
	 "PRINT 3.5\n",
	 "3.1428571428\n-3.1428571428\n3.5\n"},
	{"end",
	 "FOR I% = 0 TO 10\n"
	 "  IF I% = 5 THEN\n"
	 "    END\n"
	 "  ENDIF\n"
	 "  PRINT I%\n"
	 "NEXT\n",
	 "0\n1\n2\n3\n4\n"},
	{"error_endproc_early",
	 "PRINT 0\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT 1\n"
	 "ENDERROR\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT 2\n"
	 "END\n"
	 "ENDERROR\n"
	 "\n"
	 "ERROR 3\n",
	 "0\n2\n"},
	{"error_handled",
	 "PRINT 0\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT ERR\n"
	 "ENDERROR\n"
	 "\n"
	 "PROCFail\n"
	 "\n"
	 "PRINT 5\n"
	 "\n"
	 "DEF PROCFail\n"
	 "PRINT 1\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT 3\n"
	 "ENDPROC\n"
	 "ENDERROR\n"
	 "\n"
	 "PROCFail2\n"
	 "ENDPROC\n"
	 "\n"
	 "DEF PROCFail2\n"
	 "PRINT 2\n"
	 "ERROR 4\n"
	 "ENDPROC\n",
	 "0\n1\n2\n3\n5\n"},
	{"error_handled_fn",
	 "PRINT 0\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT ERR\n"
	 "ENDERROR\n"
	 "\n"
	 "PRINT FNFail%\n"
	 "\n"
	 "PRINT 6\n"
	 "\n"
	 "DEF FNFail%\n"
	 "PRINT 1\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT 3\n"
	 "<-4\n"
	 "ENDERROR\n"
	 "\n"
	 "PRINT FNFail2%\n"
	 "<-0\n"
	 "\n"
	 "DEF FNFail2%\n"
	 "PRINT 2\n"
	 "ERROR 5\n"
	 "<-0\n",
	 "0\n1\n2\n3\n4\n6\n",
	},
	{"error_main",
	"PRINT 0\n"
	"\n"
	"ONERROR\n"
	"PRINT ERR\n"
	"ENDERROR\n"
	"\n"
	"ONERROR\n"
	"PRINT 1\n"
	"ENDERROR\n"
	"\n"
	"ERROR 2\n",
	"0\n1\n2\n"},
	{"main_end",
	 "PRINT 0\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT ERR\n"
	 "ENDERROR\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT 1\n"
	 "END\n"
	 "ENDERROR\n"
	 "\n"
	 "ERROR 2\n",
	 "0\n1\n"},
	{"error_nested",
	"PRINT 0\n"
	"\n"
	"ONERROR\n"
	"PRINT ERR\n"
	"ENDERROR\n"
	"\n"
	"PROCFail\n"
	"\n"
	"DEF PROCFail\n"
	"PRINT 1\n"
	"\n"
	"ONERROR\n"
	"PRINT 3\n"
	"ENDERROR\n"
	"\n"
	"PROCFail2\n"
	"ENDPROC\n"
	"\n"
	"DEF PROCFail2\n"
	"PRINT 2\n"
	"ERROR 4\n"
	"ENDPROC\n",
	"0\n1\n2\n3\n4\n"},
	{"error_nested_endfn_early",
	 "PRINT 0\n"
	 "\n"
	 "PRINT FNNested%\n"
	 "\n"
	 "PRINT 1\n"
	 "\n"
	 "DEF FNNested%\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT 1\n"
	 "ENDERROR\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT 2\n"
	 "<-0\n"
	 "ENDERROR\n"
	 "\n"
	 "ERROR 3\n"
	 "<-0\n",
	 "0\n2\n0\n1\n"},
	{"error_nested_endproc_early",
	 "PRINT 0\n"
	 "\n"
	 "PROCNested\n"
	 "\n"
	 "PRINT 1\n"
	 "\n"
	 "DEF PROCNested\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT 1\n"
	 "ENDERROR\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT 2\n"
	 "ENDPROC\n"
	 "ENDERROR\n"
	 "\n"
	 "ERROR 3\n"
	 "ENDPROC\n",
	 "0\n2\n1\n"},
	{"error_nested_fn",
	 "PRINT 0\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT ERR\n"
	 "ENDERROR\n"
	 "\n"
	 "PRINT FNFail%\n"
	 "\n"
	 "DEF FNFail%\n"
	 "PRINT 1\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT 3\n"
	 "ENDERROR\n"
	 "\n"
	 "PRINT FNFail2%\n"
	 "<-1\n"
	 "\n"
	 "DEF FNFail2%\n"
	 "PRINT 2\n"
	 "ERROR 4\n"
	 "<-1\n",
	 "0\n1\n2\n3\n4\n"},
	{"error_throw_fn",
	 "PRINT 0\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT ERR\n"
	 "ENDERROR\n"
	 "\n"
	 "PRINT FNFail%\n"
	 "\n"
	 "PRINT 5\n"
	 "\n"
	 "DEF FNFail%\n"
	 "PRINT 1\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT 3\n"
	 "ERROR 6\n"
	 "ENDERROR\n"
	 "\n"
	 "PRINT FNFail2%\n"
	 "<-0\n"
	 "\n"
	 "DEF FNFail2%\n"
	 "PRINT 2\n"
	 "ERROR 4\n"
	 "<-0\n",
	 "0\n1\n2\n3\n6\n"},
	{"error_error_in_main",
	 "ONERROR\n"
	 "PRINT 0\n"
	 "ENDERROR\n"
	 "\n"
	 "FOR I% = 0 TO 10\n"
	 "ONERROR\n"
	 "PRINT 1\n"
	 "ERROR 1\n"
	 "ENDERROR\n"
	 "ERROR 29\n"
	 "NEXT\n",
	 "1\n"},
	{"dividebyzero",
	 "PROCDIVByZero\n"
	 "PROCDivideByZero\n"
	 "PROCRDivideByZero\n"
	 "\n"
	 "DEF PROCDIVByZero\n"
	 "LOCAL A%\n"
	 "LOCAL B%\n"
	 "\n"
	 "A% = 100\n"
	 "B% = 0\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT ERR\n"
	 "ENDPROC\n"
	 "ENDERROR\n"
	 "\n"
	 "PRINT A% DIV B%\n"
	 "PRINT &ff\n"
	 "ENDPROC\n"
	 "\n"
	 "DEF PROCDivideByZero\n"
	 "LOCAL A\n"
	 "LOCAL B\n"
	 "\n"
	 "A = 100\n"
	 "B = 0\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT ERR\n"
	 "ENDPROC\n"
	 "ENDERROR\n"
	 "\n"
	 "PRINT A / B\n"
	 "PRINT &ff\n"
	 "ENDPROC\n"
	 "\n"
	 "DEF PROCRDivideByZero\n"
	 "LOCAL A\n"
	 "LOCAL B\n"
	 "\n"
	 "A = 100\n"
	 "B = 0\n"
	 "\n"
	 "ONERROR\n"
	 "PRINT ERR\n"
	 "ENDPROC\n"
	 "ENDERROR\n"
	 "\n"
	 "PRINT 10 / B\n"
	 "PRINT &ff\n"
	 "ENDPROC\n",
	 "18\n18\n18\n"},
	{ "logrange",
	"PROCLogZero\n"
	"PROCLnNeg\n"
	"A = 10\n"
	"PRINT 105 / A\n"
	"\n"
	"DEF PROCLogZero\n"
	"LOCAL A\n"
	"A = 0\n"
	"ONERROR\n"
	"        PRINT ERR\n"
	"ENDPROC\n"
	"ENDERROR\n"
	"\n"
	"PRINT LOG(A)\n"
	"ENDPROC\n"
	"\n"
	"DEF PROCLnNeg\n"
	"LOCAL A\n"
	"A = -1\n"
	"ONERROR\n"
	"        PRINT ERR\n"
	"ENDPROC\n"
	"ENDERROR\n"
	"\n"
	"PRINT LN(A)\n"
	"ENDPROC\n",
	"22\n22\n10.5\n"},
	{"end_in_proc",
	 "PRINT TRUE\n"
	 "PROCEnd\n"
	 "PRINT TRUE\n"
	 "DEF PROCEnd\n"
	 "END\n"
	 "ENDPROC\n",
	 "-1\n"},
	{"array_int_simple",
	 "LOCAL i%\n"
	 "DIM a%(10)\n"
	 "FOR i% = 0 TO 10\n"
	 "    a%(i%) = i%\n"
	 "NEXT\n"
	 "FOR i% = 0 TO 10\n"
	 "    PRINT a%(i%)\n"
	 "NEXT\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n"},
	{"array_double_simple",
	 "LOCAL i\n"
	 "DIM a(10)\n"
	 "FOR i% = 0 TO 10\n"
	 "    a(i%) = i% + 0.5\n"
	 "NEXT\n"
	 "FOR i% = 0 TO 10\n"
	 "    PRINT a(i%)\n"
	 "NEXT\n",
	 "0.5\n1.5\n2.5\n3.5\n4.5\n5.5\n6.5\n7.5\n8.5\n9.5\n10.5\n"},
	{"array2D_int_const",
	 "LOCAL i%\n"
	 "LOCAL j%\n"
	 "LOCAL c%\n"
	 "DIM a%(2,2)\n"
	 "FOR i% = 0 TO 2\n"
	 "    FOR j% = 0 TO 2\n"
	 "        a%(i%,j%) = c%\n"
	 "        c% += 1\n"
	 "    NEXT\n"
	 "NEXT\n"
	 "FOR i% = 0 TO 2\n"
	 "    FOR j% = 0 TO 2\n"
	 "        PRINT a%(i%,j%)\n"
	 "    NEXT\n"
	 "NEXT\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n"},
	{"array2D_int_var",
	 "LOCAL i% = 2\n"
	 "LOCAL j% = 2\n"
	 "LOCAL c%\n"
	 "DIM a%(i%, j%)\n"
	 "FOR i% = 0 TO 2\n"
	 "    FOR j% = 0 TO 2\n"
	 "        a%(i%,j%) = c%\n"
	 "        c% += 1\n"
	 "    NEXT\n"
	 "NEXT\n"
	 "FOR i% = 0 TO 2\n"
	 "    FOR j% = 0 TO 2\n"
	 "        PRINT a%(i%,j%)\n"
	 "    NEXT\n"
	 "NEXT\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n"},
	{"array_bad_index_var",
	 "ONERROR\n"
	 "    PRINT ERR\n"
	 "ENDERROR\n"
	 "DIM a%(2)\n"
	 "i%=3\n"
	 "PRINT a%(i%)\n",
	 "10\n"},
	{"array2d_bad_index_var",
	 "ONERROR\n"
	 "    PRINT ERR\n"
	 "ENDERROR\n"
	 "DIM a%(2,2)\n"
	 "i%=3\n"
	 "PRINT a%(i%,0)\n",
	 "10\n"},
	{"array2d_int_zero",
	 "DIM a%(1024)\n"
	 "FOR i% = 0 TO 1024\n"
	 "  IF a%(i%) <> 0 THEN\n"
	 "      PRINT 0\n"
	 "      END\n"
	 "  ENDIF\n"
	 "NEXT\n"
	 "PRINT 1\n",
	 "1\n"},
	{"array2d_int_real",
	 "DIM a(1024)\n"
	 "FOR i% = 0 TO 1024\n"
	 "  IF a(i%) <> 0.0 THEN\n"
	 "      PRINT 0\n"
	 "      END\n"
	 "  ENDIF\n"
	 "NEXT\n"
	 "PRINT 1\n",
	 "1\n"},
	{"array1d_int_plus_equals",
	 "DIM a%(8)\n"
	 "FOR i% = 0 TO 8\n"
	 "  a%(i%) = 1\n"
	 "NEXT\n"
	 "FOR i% = 0 TO 8\n"
	 "  a%(i%) += i%\n"
	 "NEXT\n"
	 "FOR i% = 0 TO 8\n"
	 "  PRINT a%(i%)\n"
	 "NEXT\n",
	 "1\n2\n3\n4\n5\n6\n7\n8\n9\n"},
	{"array1d_int_minus_equals",
	 "DIM a%(8)\n"
	 "FOR i% = 0 TO 8\n"
	 "  a%(i%) = 10\n"
	 "NEXT\n"
	 "FOR i% = 0 TO 8\n"
	 "  a%(i%) -= i%\n"
	 "NEXT\n"
	 "FOR i% = 0 TO 8\n"
	 "  PRINT a%(i%)\n"
	 "NEXT\n",
	 "10\n9\n8\n7\n6\n5\n4\n3\n2\n"},
	{"array1d_real_plus_equals",
	 "DIM a(8)\n"
	 "FOR i% = 0 TO 8\n"
	 "  a(i%) = 1.5\n"
	 "NEXT\n"
	 "FOR i% = 0 TO 8\n"
	 "  a(i%) += i%\n"
	 "NEXT\n"
	 "FOR i% = 0 TO 8\n"
	 "  PRINT a(i%)\n"
	 "NEXT\n",
	 "1.5\n2.5\n3.5\n4.5\n5.5\n6.5\n7.5\n8.5\n9.5\n"},
	{"array1d_real_minus_equals",
	 "DIM a(8)\n"
	 "FOR i% = 0 TO 8\n"
	 "  a(i%) = 10.5\n"
	 "NEXT\n"
	 "FOR i% = 0 TO 8\n"
	 "  a(i%) -= i%\n"
	 "NEXT\n"
	 "FOR i% = 0 TO 8\n"
	 "  PRINT a(i%)\n"
	 "NEXT\n",
	 "10.5\n9.5\n8.5\n7.5\n6.5\n5.5\n4.5\n3.5\n2.5\n"},
	{"array2d_int_plus_equals",
	 "DIM a%(2,2)\n"
	 "LOCAL i%\n"
	 "LOCAL j%\n"
	 "FOR i% = 0 TO 2\n"
	 "  FOR j% = 0 TO 2\n"
	 "    a%(i%, j%) = 1\n"
	 "  NEXT\n"
	 "NEXT\n"
	 "FOR i% = 0 TO 2\n"
	 "  FOR j% = 0 TO 2\n"
	 "    a%(i%, j%) += (i%*3) + j%\n"
	 "  NEXT\n"
	 "NEXT\n"
	 "FOR i% = 0 TO 2\n"
	 "  FOR j% = 0 TO 2\n"
	 "    PRINT a%(i%, j%)\n"
	 "  NEXT\n"
	 "NEXT\n",
	 "1\n2\n3\n4\n5\n6\n7\n8\n9\n"},
	{"for_array_int_var",
	 "DIM a%(2)\n"
	 "FOR a%(0) = 0 TO 10\n"
	 "  PRINT a%(0)\n"
	 "NEXT\n"
	 "PRINT a%(0)\n"
	 "PRINT a%(1)\n"
	 "PRINT a%(2)\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n0\n0\n"},
	{"for_array_real_var",
	 "DIM a(2)\n"
	 "FOR a(0) = 0.5 TO 10.5\n"
	 "  PRINT a(0)\n"
	 "NEXT\n"
	 "PRINT a(0)\n"
	 "PRINT a(1)\n"
	 "PRINT a(2)\n",
	 "0.5\n1.5\n2.5\n3.5\n4.5\n5.5\n6.5\n7.5\n8.5\n9.5\n10.5\n11.5\n0\n0\n"
	},
	{"for_array_int_var_step",
	 "DIM a%(2)\n"
	 "FOR a%(0) = 0 TO 10 STEP 2\n"
	 "  PRINT a%(0)\n"
	 "NEXT\n"
	 "PRINT a%(0)\n"
	 "PRINT a%(1)\n"
	 "PRINT a%(2)\n",
	 "0\n2\n4\n6\n8\n10\n12\n0\n0\n"},
	{"for_array_real_var",
	 "DIM a(2)\n"
	 "FOR a(0) = 0.5 TO 10.5 STEP 2\n"
	 "  PRINT a(0)\n"
	 "NEXT\n"
	 "PRINT a(0)\n"
	 "PRINT a(1)\n"
	 "PRINT a(2)\n",
	 "0.5\n2.5\n4.5\n6.5\n8.5\n10.5\n12.5\n0\n0\n"
	},
	{"for_array_int_var_step_var",
	 "LOCAL st% = 2\n"
	 "DIM a%(2)\n"
	 "FOR a%(0) = 0 TO 10 STEP st%\n"
	 "  PRINT a%(0)\n"
	 "NEXT\n"
	 "PRINT a%(0)\n"
	 "PRINT a%(1)\n"
	 "PRINT a%(2)\n",
	 "0\n2\n4\n6\n8\n10\n12\n0\n0\n"},
	{"array_dim_oom",
	 "ONERROR\n"
	 "PRINT ERR\n"
	 "ENDERROR\n"
	 "DIM a%(10000000)\n"
	 "PRINT 1\n",
	 "11\n"},
	{"array_local_dim_oom",
	 "ONERROR\n"
	 "PRINT ERR\n"
	 "ENDERROR\n"
	 "PROCAlloc\n"
	 "PRINT 2\n"
	 "DEF PROCAlloc\n"
	 "LOCAL DIM a%(10000000)\n"
	 "PRINT 1\n"
	 "ENDPROC\n",
	 "11\n"},
	{"global_deref",
	 "DIM c(100)\n",
	 ""
	},
	{"local_deref",
	 "LOCAL DIM c(100)\n",
	 ""
	},
	{"local_if_deref",
	 "IF TRUE THEN\n"
	 "    LOCAL DIM c(100)\n"
	 "ENDIF",
	 ""
	},
	{"local_loop_deref",
	 "FOR I% = 0 TO 3\n"
	 "    LOCAL DIM c(100)\n"
	 "NEXT",
	 ""
	},
	{"local_proc_deref",
	 "PROCP\n"
	 "DEF PROCP\n"
	 "  LOCAL DIM c(100)\n"
	 "ENDPROC\n",
	 ""
	},
	{"local_if_fn_deref",
	 "PRINT FNF\n"
	 "DEF FNF\n"
	 "  IF TRUE THEN\n"
	 "    LOCAL DIM c(100)\n"
	 "  ENDIF"
	 "<-0\n",
	 "0\n"
	},
	{"local_proc_nested_deref",
	 "PROCP\n"
	 "DEF PROCP\n"
	 "  LOCAL DIM a(10)\n"
	 "    REPEAT\n"
	 "        LOCAL DIM b(10)\n"
	 "        IF TRUE THEN\n"
	 "            LOCAL DIM c(10)\n"
	 "        ENDIF\n"
	 "    UNTIL TRUE\n"
	 "ENDPROC\n",
	 ""
	},
	{"local_on_error_deref",
	 "ONERROR\n"
	 "    LOCAL DIM c(100)\n"
	 "ENDERROR\n"
	 "ERROR 1\n",
	 ""
	},
	{"local_proc_on_error_deref",
	 "PROCP\n"
	 "DEF PROCP\n"
	 "    ONERROR\n"
	 "         LOCAL DIM c(100)\n"
	 "    ENDERROR\n"
	 "    ERROR 1\n"
	 "ENDPROC\n",
	 ""
	},
	{"local_proc_return_deref",
	 "PROCP\n"
	 "DEF PROCP\n"
	 "    LOCAL DIM c(100)\n"
	 "    IF TRUE THEN\n"
	 "         LOCAL DIM d(10)\n"
	 "         ENDPROC\n"
	 "    ENDIF\n"
	 "ENDPROC\n",
	 ""
	},
	{"local_proc_return_deref",
	 "PRINT FNF\n"
	 "DEF FNF\n"
	 "    LOCAL DIM c(100)\n"
	 "    IF TRUE THEN\n"
	 "         LOCAL DIM d(10)\n"
	 "         <-0\n"
	 "    ENDIF\n"
	 "<-0\n",
	 "0\n"
	},
	{"error_deref",
	 "ONERROR\n"
	 "  PRINT ERR\n"
	 "ENDERROR\n"
	 "PROCP\n"
	 "DEF PROCP\n"
	 "  LOCAL DIM a%(10)\n"
	 "  IF TRUE THEN\n"
	 "    LOCAL DIM b%(10)\n"
	 "    REPEAT\n"
	 "      LOCAL DIM c%(10)\n"
	 "      ERROR 1\n"
	 "    UNTIL TRUE\n"
	 "  ENDIF\n"
	 "ENDPROC\n",
	 "1\n"},
	{"global_shadow",
	 "x% = 1\n"
	 "if true then\n"
	 "  x% := 2\n"
	 "  print x%\n"
	 "endif\n"
	 "print x%\n",
	 "2\n1\n"
	},
	{"for_global_shadow",
	 "i% = 0\n"
	 "if true then\n"
	 "  for i% := 0 to 10\n"
	 "    print i%\n"
	 "  next\n"
	 "endif\n"
	 "print i%\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n0\n"
	},
	{"rem",
	 "rem Copyright Mark Ryan (c) 2019\n"
	 "rem\n"
	 "for i% := 0 to 10 REM Loop 10 times\n"
	 "  print i%\n"
	 "next\n"
	 "rem the end\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n"
	},
	{"array_global_assign",
	 "dim a%(10)\n"
	 "dim b%(10)\n"
	 "for i% := 0 to 10\n"
	 "  a%(i%) = i%\n"
	 "next\n"
	 "b%() = a%()\n"
	 "for i% = 0 to 10\n"
	 "  print b%(i%)\n"
	 "next\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n"
	},
	{"array_local_assign",
	 "local dim a%(10)\n"
	 "local dim b%(10)\n"
	 "for i% := 0 to 10\n"
	 "  a%(i%) = i%\n"
	 "next\n"
	 "b%() = a%()\n"
	 "for i% = 0 to 10\n"
	 "  print b%(i%)\n"
	 "next\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n"
	},
	{"array_global_local_assign",
	 "dim b%(10)\n"
	 "PROCAssign\n"
	 "DEF PROCAssign\n"
	 "  local dim a%(10)\n"
	 "  for i% := 0 to 10\n"
	 "    a%(i%) = i%\n"
	 "  next\n"
	 "  b%() = a%()\n"
	 "ENDPROC\n"
	 "for i% = 0 to 10\n"
	 "  print b%(i%)\n"
	 "next\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n"
	},
	{"array_int_arg",
	 "local dim a%(10)\n"
	 "for i% := 0 to 10\n"
	 "  a%(i%) = i%\n"
	 "next\n"
	 "PROCArr(a%())\n"
	 "def PROCArr(b%(1))\n"
	 "for i% := 0 to 10\n"
	 "    print b%(i%)\n"
	 "next\n"
	 "endproc\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n"
	},
	{"array_two_int_arg",
	 "local dim a%(10)\n"
	 "local dim b%(10)\n"
	 "for i% := 0 to 10\n"
	 "  a%(i%) = i%\n"
	 "  b%(i%) = i% * 2\n"
	 "next\n"
	 "PROCSum(a%(), b%())\n"
	 "def PROCSum(c%(1), d%(1))\n"
	 "for i% := 0 to 10\n"
	 "print c%(i%) + d%(i%)\n"
	 "next\n"
	 "endproc\n",
	 "0\n3\n6\n9\n12\n15\n18\n21\n24\n27\n30\n"
	},
	{"array_int_arg_fifth",
	 "local dim a%(10)\n"
	 "for i% := 0 to 10\n"
	 "  a%(i%) = i%\n"
	 "next\n"
	 "PROCArr(1,2,3,4,a%())\n"
	 "def PROCArr(a%, b%, c%, d%, e%(1))\n"
	 "num% := a% + b% + c% + d%\n"
	 "for i% := 0 to 10\n"
	 "    num% += e%(i%)\n"
	 "next\n"
	 "print num%\n"
	 "endproc\n",
	 "65\n",
	},
	{"array_real_arg",
	 "local dim a(10)\n"
	 "for i% := 0 to 10\n"
	 "  a(i%) = i%\n"
	 "next\n"
	 "PROCArr(a())\n"
	 "def PROCArr(b(1))\n"
	 "for i% := 0 to 10\n"
	 "    print b(i%)\n"
	 "next\n"
	 "endproc\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n"
	},
	{"array_two_real_arg",
	 "local dim a(10)\n"
	 "local dim b(10)\n"
	 "for i% := 0 to 10\n"
	 "  a(i%) = i%\n"
	 "  b(i%) = i% * 2\n"
	 "next\n"
	 "PROCSum(a(), b())\n"
	 "def PROCSum(c(1), d(1))\n"
	 "for i% := 0 to 10\n"
	 "print c(i%) + d(i%)\n"
	 "next\n"
	 "endproc\n",
	 "0\n3\n6\n9\n12\n15\n18\n21\n24\n27\n30\n"
	},
	{"array_real_arg_fifth",
	 "local dim a(10)\n"
	 "for i% := 0 to 10\n"
	 "  a(i%) = i%\n"
	 "next\n"
	 "PROCArr(1,2,3,4,a())\n"
	 "def PROCArr(a%, b%, c%, d%, e(1))\n"
	 "num% := a% + b% + c% + d%\n"
	 "for i% := 0 to 10\n"
	 "    num% += e(i%)\n"
	 "next\n"
	 "print num%\n"
	 "endproc\n",
	 "65\n",
	},
	{"array_global_arg",
	 "dim a%(10)\n"
	 "for i% := 0 to 10\n"
	 "  a%(i%) = i%\n"
	 "next\n"
	 "PROCArr(a%())\n"
	 "def PROCArr(b%(1))\n"
	 "for i% := 0 to 10\n"
	 "    print b%(i%)\n"
	 "next\n"
	 "endproc\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n"
	},
	{"array_constant_dim",
	 "local dim a%(2,4,6)\n"
	 "print dim(a%())\n"
	 "print dim(a%(), 3)\n",
	 "3\n6\n",
	},
	{"array_dynamic_dim",
	 "local d% = 3\n"
	 "local dim a%(2,4,6)\n"
	 "print dim(a%(), d%)\n",
	 "6\n",
	},
	{"array_dynamic_dim_zero",
	 "onerror\n"
	 "  print err\n"
	 "enderror\n"
	 "local d%\n"
	 "local dim a%(2,4,6)\n"
	 "print dim(a%(), d%)\n",
	 "10\n",
	},
	{"array_bad_dynamic_dim",
	 "onerror\n"
	 "  print err\n"
	 "enderror\n"
	 "d% := 4\n"
	 "local dim a%(2,4,6)\n"
	 "print dim(a%(), d%)\n",
	 "10\n",
	},
	{"array_return_ref_fn",
	 "a() = FNArr(1)()\n"
	 "for i% = 0 to dim(a(),1)\n"
	 "  print a(i%)\n"
	 "next\n"
	 "\n"
	 "def FNArr(1)\n"
	 "  local dim a(10)\n"
	 "  for i% := 0 to 10\n"
	 "    a(i%) = i%\n"
	 "  next\n"
	 "<- a()\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n",
	},
	{"array_assign_ref",
	 "dim a%(10,10)\n"
	 "b%() := a%()\n"
	 "print dim(b%())\n"
	 "print dim(b%(), 1)\n",
	 "2\n10\n",
	},
	{"array_return_fn",
	 "c% := 10\n"
	 "dim a(c%)\n"
	 "a() = FNArr(1)()\n"
	 "for i% = 0 to dim(a(),1)\n"
	 "  print a(i%)\n"
	 "next\n"
	 "\n"
	 "def FNArr(1)\n"
	 "  local dim a(10)\n"
	 "  for i% := 0 to 10\n"
	 "    a(i%) = i%\n"
	 "  next\n"
	 "<- a()\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n",
	},
	{"array_temporaries",
	 "a%() := FNAddArray%(1)(FNFillA%(1)(), FNFillB%(1)())\n"
	 "for i% := 0 to 10\n"
	 "print a%(i%)\n"
	 "next\n"
	 "def FNFillA%(1)\n"
	 "local dim a%(10)\n"
	 "for i% := 0 to 10\n"
	 "a%(i%) = i%\n"
	 "next\n"
	 "<-a%()\n"
	 "def FNFillB%(1)\n"
	 "local dim a%(10)\n"
	 "for i% := 0 to 10\n"
	 "a%(i%) = i% * 2\n"
	 "next\n"
	 "<-a%()\n"
	 "def FNAddArray%(1)(a%(1), b%(1))\n"
	 "local dim c%(10)\n"
	 "for i% := 0 to dim(a%(),1)\n"
	 "c%(i%) = a%(i%) + b%(i%)\n"
	 "next\n"
	 "<-c%()\n",
	 "0\n3\n6\n9\n12\n15\n18\n21\n24\n27\n30\n"
	},
	{"array_assign_local_ref",
	 "dim a%(10,10)\n"
	 "local b%() = a%()\n"
	 "print dim(b%())\n"
	 "print dim(b%(), 1)\n",
	 "2\n10\n",
	},
	{"array_local_return_fn",
	 "c% := 10\n"
	 "dim a(c%)\n"
	 "local a() = FNArr(1)()\n"
	 "for i% = 0 to dim(a(),1)\n"
	 "  print a(i%)\n"
	 "next\n"
	 "\n"
	 "def FNArr(1)\n"
	 "  local dim a(10)\n"
	 "  for i% := 0 to 10\n"
	 "    a(i%) = i%\n"
	 "  next\n"
	 "<- a()\n",
	 "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n",
	},
	{"array_double_ref",
	  "local dim a%(10)\n"
	  "for i% = 0 to 10\n"
	  "  a%(i%) = i%\n"
	  "next\n"
	  "b%() = a%()\n"
	  "c%() = b%()\n"
	  "for i% = 0 to 10\n"
	  "  print c%(i%)\n"
	  "next\n",
	  "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n",
	},
	{"sgn",
	 "print SGN(11)\n"
	 "print SGN(0)\n"
	 "print SGN(-11)\n"
	 "a% := 11\n"
	 "b% := 0\n"
	 "c% := -111\n"
	 "print(sgn(a%))\n"
	 "print(sgn(b%))\n"
	 "print(sgn(c%))\n"
	 "\n"
	 "print SGN(11.0)\n"
	 "print SGN(0.0)\n"
	 "print SGN(-11.0)\n"
	 "a := 11.0\n"
	 "b := 0.0\n"
	 "c := -111.0\n"
	 "print(sgn(a))\n"
	 "print(sgn(b))\n"
	 "print(sgn(c))\n",
	 "1\n0\n-1\n1\n0\n-1\n1\n0\n-1\n1\n0\n-1\n",
	},
	{"array_dim_mixed",
	 "a% := 3\n"
	 "local dim b%(a%,3)\n"
	 "print b%(1,3)\n",
	 "0\n",
	},
	{"nested_blocks",
	 "if true then\n"
	 "  if true then\n"
	 "    a% := 1\n"
	 "    print a%\n"
	 "  endif\n"
	 "endif\n"
	 "if true then\n"
	 "  if true then\n"
	 "    a% := 1\n"
	 "    print a%\n"
	 "  endif\n"
	 "endif\n",
	 "1\n1\n",
	},
};

/* clang-format on */
