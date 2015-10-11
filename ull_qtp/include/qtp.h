/*
 * qtp.h
 *
 *  Created on: Sep 23, 2015
 *      Author: rock
 */

#ifndef QTP_H_
#define QTP_H_

#include <cinttypes>
#include <memory>

#include "../utils/singleton.h"
#include "../busilogic/tradehandler.h"
#include "../busilogic/quothandler.h"

namespace atp
{

class algo_base
{
public:
	typedef std::shared_ptr<atp::algo_base> pointer_t;

public:
	algo_base(){}
	virtual ~algo_base(){}

public:
	virtual void handle_quot_evt();
	virtual void handle_state_evt();
	virtual void handle_timer_evt();

};

class qtp_engine
{
public:

public:
	int_fast32_t init();
	int_fast32_t reg_algo(algo_base::pointer_t&);
	int_fast32_t rem_algo(algo_base::pointer_t&);
	void run();
};

}

inline int_fast32_t atp::qtp_engine::init()
{
	return 0;
}

inline int_fast32_t atp::qtp_engine::add_algo()
{
	return 0;
}

//inline int_fast32_t qtp::qtp_engine::run()
//{
//	return 0;
//}

#endif /* QTP_H_ */
