//============================================================================
// Name        : Platform.h
// Author      : TheRockLiuHY
// Date        : Apr 29, 2015
// Copyright   : liuhongyang's copyright
// Description : SmartUtils
//============================================================================

#ifndef COMMON_COMMON_H_
#define COMMON_COMMON_H_

#if !defined(__linux__) || !defined(__GNUC__) || (__GNUC__ < 4)
#error only support linux and using gnu compiler 4.x...
#endif

#if (__cplusplus <= 199711L)
#define __SUPPORT_CPP_11__ (0)
#else
#define __SUPPORT_CPP_11__ (1)
#endif

///common header files...
#include <cstdint>

#include "defines.h"
#include "types.h"
#include "config.h"

namespace smart_utils
{
enum
	: int8_t
	{
		SU_EC_SUC = 0, SU_EC_ERR = -1, SU_EC_REDO_ERR = -2
};
}
#endif /* COMMON_COMMON_H_ */

