include(CheckCCompilerFlag)

if(MSVC)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /WX /W3")
elseif(CMAKE_C_COMPILER_ID MATCHES "^(GNU|Clang|AppleClang)$")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c90 -Wextra -Wmissing-prototypes -Wall -Wold-style-definition -Wdeclaration-after-statement")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wundef -Wpointer-arith -Werror -Wcast-qual -Wcast-align -Wfloat-equal -Wconversion -Wwrite-strings")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-missing-braces")

	# flags that only some compilers or versions support
	foreach(flag -Wcomma -Wreserved-identifier -Wtrampolines -Wunsafe-loop-optimizations -Wfloat-conversion)
		string(MAKE_C_IDENTIFIER "HYP_HAS${flag}" supported)
		CHECK_C_COMPILER_FLAG(${flag} ${supported})
		if(${supported})
			SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}")
		endif()
	endforeach()

	SET(HYP_DOUBLE_PRECISION_FLAGS -Wdouble-promotion)

	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		SET(HYP_SINGLE_PRECISION_FLAGS -Wdouble-promotion)
	endif()
endif()
