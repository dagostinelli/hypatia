/* SPDX-License-Identifier: MIT */

/* The whole test suite in one file, with HYP_STATIC: every library function is
 * static, as in a program that includes the implementation in each file that
 * uses it.
 */
#define HYP_STATIC

#include "implementation.c"
#include "main.c"
