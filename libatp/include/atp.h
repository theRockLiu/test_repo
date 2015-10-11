/*
 * atp.h
 *
 *  Created on: Sep 23, 2015
 *      Author: rock
 */

#ifndef ATP_H_
#define ATP_H_

#include <cinttypes>
#include <memory>
#include <vector>

namespace atp
{

typedef struct quot_info
{
	uint32_t excode : 4; //exchnge code
	uint32_t cid : 10; //contract id
	uint32_t l1_bid_pl : 8;
	uint32_t l1_ask_pl : 8;
	uint32_t l2_bid_pl : 8;
	uint32_t l2_ask_pl : 8;
} quot_info_t;

class algo_base
{
public:
	typedef std::shared_ptr<atp::algo_base> pointer_t;

public:
	algo_base(){}
	virtual ~algo_base(){}

public:
	virtual void handle_quot(quot_info_t&) = 0;
	virtual void handle_state() = 0;
	virtual void handle_timer() = 0;
};

class algo_engine
{
public:

public:
	int_fast32_t init();
	int_fast32_t reg_algo(algo_base::pointer_t& algo, std::vector<std::string>& contract_ids);
	int_fast32_t run_and_wait();
};

}

#endif /* ATP_H_ */
