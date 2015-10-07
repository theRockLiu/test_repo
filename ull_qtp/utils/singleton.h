/*
 * singleton.h
 *
 *  Created on: Sep 22, 2015
 *      Author: rock
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

namespace utils
{

template<typename T>
class singleton
{
	class dummy
	{
	public:
		dummy() :
				singleton<T>::x_(new T)
		{
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

private:
	static T* x_;
};

}

#endif /* SINGLETON_H_ */
