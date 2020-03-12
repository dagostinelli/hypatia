default: check_compile

clean:
	@rm -rf build/

check_compile_headers:
	find src/ -name '*.h' ! -path "*single-file-lib*" \( -print -exec gcc -fsyntax-only {} \; -o -quit \)

check_compile: check_compile_headers
