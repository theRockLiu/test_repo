/*
 * qtp.h
 *
 *  Created on: Sep 23, 2015
 *      Author: rock
 */

#ifndef QTP_H_
#define QTP_H_

#include <cinttypes>

#include "../utils/singleton.h"
#include "../busilogic/tradehandler.h"
#include "../busilogic/quothandler.h"

namespace qtp
{

class algo_base
{
public:


public:

};

class qtp_engine
{
public:

public:
	int_fast32_t init();
	int_fast32_t add_algo();
	void run();
};

}

inline int_fast32_t qtp::qtp_engine::init()
{
	return 0;
}

inline int_fast32_t qtp::qtp_engine::add_algo()
{
	return 0;
}

//inline int_fast32_t qtp::qtp_engine::run()
//{
//	return 0;
//}

#endif /* QTP_H_ */
