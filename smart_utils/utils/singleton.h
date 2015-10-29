//============================================================================
// Name        : Singleton.h
// Author      : TheRockLiuHY
// Date        : Apr 29, 2015
// Copyright   : liuhongyang's copyright
// Description : SmartUtils
//============================================================================

#ifndef UTILS_SINGLETON_H_
#define UTILS_SINGLETON_H_

#include "../base/base.h"

#if (!__SUPPORT_CPP_11__)
#error TheRockLHY says this singleton implementation needs C++11!
#endif

namespace smart_utils
{

	/**
	 *
	 * thread-safe singleton implementation.
	 * */
	template<typename T>
	class singleton
	{

		DISABLE_CONSTRUCT_AND_DESTRUCT(singleton<T>)
		DISABLE_COPY(singleton<T>)
		DISABLE_MOVE(singleton<T>)

	public:
		/**
		 * C++11's standard : If control enters the declaration concurrently while the variable is being initialized, the concurrent execution shall wait for completion of the initialization.
		 * TheRockLhy: this is a thread-safe implementation
		 * */
		static T& get_inst()
		{
			static T st;
			return st;
		}
	};

} /* namespace NSSmartUtils */

#endif /* UTILS_SINGLETON_H_ */
