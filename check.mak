default: check

clean:
	@rm -rf build/

check:
	@$(MAKE) -f check.mak checksyntax
	@$(MAKE) -f check.mak checksparse
	@$(MAKE) -f check.mak checkpatch

checksparse:
	@echo checksparse
	@sparse hypatia.h
	@sparse -I. test/main.c -Wno-missing-braces

checksyntax:
	@echo checksyntax
	@gcc -fsyntax-only hypatia.h
	@gcc -fsyntax-only -I. test/main.c

checkpatch:
	@echo checkpatch
	@/usr/src/kernels/`uname -r`/scripts/checkpatch.pl \
	-f --no-tree --no-summary --terse --show-types --subjective \
	--ignore LONG_LINE_COMMENT,BRACES,COMPARISON_TO_NULL,LONG_LINE,\
	MACRO_WITH_FLOW_CONTROL,CAMELCASE,SPACING,PREFER_KERNEL_TYPES,\
	SPDX_LICENSE_TAG,UNNECESSARY_PARENTHESES,LONG_LINE_STRING,\
	BLOCK_COMMENT_STYLE,AVOID_EXTERNS,UNNECESSARY_ELSE,MULTISTATEMENT_MACRO_USE_DO_WHILE,\
	DEEP_INDENTATION,GLOBAL_INITIALISERS,PARENTHESIS_ALIGNMENT,LINE_SPACING \
	test/*.c hypatia.h
