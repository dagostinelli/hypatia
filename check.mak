default: check

clean:
	@rm -rf build/

check:
	@echo try:
	@echo  make -f check.mak checksyntax
	@echo  make -f check.mak checksparse
	@echo  make -f check.mak checkpatch

checksparse:
	@echo checksparse
	@sparse hypatia.h
	@sparse -I. test/implementation.c
	@sparse -I. test/main.c

checksyntax:
	@echo checksyntax
	@gcc -fsyntax-only -std=c90 -Wno-missing-braces -Wextra -Wmissing-prototypes -Wall -Wold-style-definition -Wdeclaration-after-statement -Wundef -Wpointer-arith -Werror -Wcast-qual -Wcast-align -Wfloat-equal -Wconversion hypatia.h
	@gcc -fsyntax-only -std=c90 -Wno-missing-braces -Wextra -Wmissing-prototypes -Wall -Wold-style-definition -Wdeclaration-after-statement -Wundef -Wpointer-arith -Werror -Wcast-qual -Wcast-align -Wfloat-equal -Wconversion -I. test/implementation.c
	@gcc -fsyntax-only -std=c90 -Wno-missing-braces -Wextra -Wmissing-prototypes -Wall -Wold-style-definition -Wdeclaration-after-statement -Wundef -Wpointer-arith -Werror -Wcast-qual -Wcast-align -Wfloat-equal -Wconversion -I. test/main.c

checkpatch:
	@echo checkpatch
	@/usr/src/kernels/`uname -r`/scripts/checkpatch.pl \
	-f --no-tree --no-summary --terse --show-types --subjective \
	--ignore INLINE,LONG_LINE_COMMENT,BRACES,LONG_LINE,\
	CAMELCASE,SPACING,PREFER_KERNEL_TYPES,\
	SPDX_LICENSE_TAG,UNNECESSARY_PARENTHESES,LONG_LINE_STRING,\
	BLOCK_COMMENT_STYLE,AVOID_EXTERNS,UNNECESSARY_ELSE,MULTISTATEMENT_MACRO_USE_DO_WHILE,\
	DEEP_INDENTATION,GLOBAL_INITIALISERS,PARENTHESIS_ALIGNMENT,LINE_SPACING \
	test/*.c hypatia.h test/implementation.c
