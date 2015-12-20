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
class singleton_ex
{

	DISABLE_CONSTRUCT_AND_DESTRUCT(singleton_ex<T>)
	DISABLE_COPY(singleton_ex<T>)
	DISABLE_MOVE(singleton_ex<T>)

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

template<typename T>
class singleton
{
private:
	static T* x_;

	class dummy
	{
	public:
		dummy()
		{
			singleton<T>::x_ = new T;
			SU_CHECK(nullptr != singleton<T>::x_);
		}
	};

public:
	static T& inst()
	{
		static dummy x;

		return *x_;
	}

	void destroy()
	{
		if (nullptr != x_)
		{
			delete x_;
			x_ = nullptr;
		}
	}
};

template<typename T>
T* singleton<T>::x_ = nullptr;

} /* namespace NSSmartUtils */

#endif /* UTILS_SINGLETON_H_ */
