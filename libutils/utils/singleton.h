/*
 * singleton.h
 *
 *  Created on: Sep 22, 2015
 *      Author: rock
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

#include "../common/common.h"

namespace utils
{

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
			ASSERT(nullptr != singleton<T>::x_);
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

}

#endif /* SINGLETON_H_ */
