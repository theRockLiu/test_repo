//============================================================================
// Name        : commonutils.h
// Author      : TheRockLiuHY
// Date        : May 21, 2015
// Copyright   : liuhongyang's copyright
// Description : smart_utils
//============================================================================

#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

namespace smart_utils
{

	///get or set socket non block flag.
	int32_t set_nonblock(const int32_t sock);
	bool get_nonblock(const int32_t sock);

} /* namespace smart_utils */

#endif /* UTILS_UTILS_H_ */
