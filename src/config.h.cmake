/* SPDX-License-Identifier: MIT */

#ifndef _HYPATIA_CONFIG_H_
#define _HYPATIA_CONFIG_H_

/* start with HYPAPI being off */
#ifndef HYPAPI
#	define HYPAPI
#endif

#cmakedefine HYPATIA_SINGLE_PRECISION_FLOATS

#define HYPATIA_VERSION "@PROJECT_VERSION@"

#endif /* _HYPATIA_CONFIG_H_ */
