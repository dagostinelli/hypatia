default: check_compile

clean:
	@rm -rf dist/
	@rm -rf build/

check_compile_headers:
	find src/ -name '*.h' ! -path "*single-file-lib*" \( -print -exec gcc -fsyntax-only {} \; -o -quit \)

check_compile_source:
	find src/ -name '*.c' ! -path "*single-file-lib*" \( -print -exec gcc -fsyntax-only {} \; -o -quit \)

check_compile: check_compile_headers check_compile_source

single:
	@mkdir -p dist/

	@cat src/config.h src/hypatia.h src/vector2.h src/vector3.h src/vector4.h src/matrix3.h src/matrix4.h src/quaternion.h src/experimental.h > dist/intermediate.h
	@sed -i '/#include/d' dist/intermediate.h
	@cat src/single-file-lib-begin.h dist/intermediate.h src/single-file-lib-end.h > dist/hypatia.h
	@gcc -fsyntax-only dist/hypatia.h
	@rm dist/intermediate.h

	@cat src/scalar.c src/vector2.c src/vector3.c src/vector4.c src/matrix3.c src/matrix4.c src/quaternion.c src/experimental.c > dist/intermediate.c
	@sed -i '/#include/d' dist/intermediate.c
	@cat src/single-file-lib-begin.c dist/intermediate.c src/single-file-lib-end.c > dist/hypatia.c
	@gcc -fsyntax-only dist/hypatia.c
	@rm dist/intermediate.c
