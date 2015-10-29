//============================================================================
// Name        : commonutils.cpp
// Author      : TheRockLiuHY
// Date        : May 21, 2015
// Copyright   : liuhongyang's copyright
// Description : smart_utils
//============================================================================

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "../base/base.h"
#include "utils.h"

namespace smart_utils
{

	int32_t set_nonblock(const int32_t sock)
	{
		int32_t flags = fcntl(sock, F_GETFL);
		flags |= O_NONBLOCK;

		return fcntl(sock, F_SETFL, flags) == 0 ? SU_EC_SUC : errno;
	}

	bool get_nonblock(const int32_t sock)
	{
		return (fcntl(sock, F_GETFL) & O_NONBLOCK) == O_NONBLOCK; ///TheRock: i don't like the "0 meaning false" style.
	}

} /* namespace smart_utils */
