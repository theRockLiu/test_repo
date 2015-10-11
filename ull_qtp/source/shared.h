/*
 * shared.h
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#ifndef SHARED_H_
#define SHARED_H_

#include <cstdint>
#include <vector>

#include <log/spdlog/include/spdlog/spdlog.h>
namespace spd = spdlog;

#include "../utils/singleton.h"
#include "quothandler.h"
#include "tradehandler.h"
#include "../include/qtp.h"

namespace atp
{

struct quot_info
{
	uint32_t excode : 4; //exchnge code
	uint32_t cid : 10; //contract id
	uint32_t l1_bid_pl : 8;
	uint32_t l1_ask_pl : 8;
	uint32_t l2_bid_pl : 8;
	uint32_t l2_ask_pl : 8;
};

struct order_info
{
	uint32_t excode : 4;
	uint32_t cid : 10;
};

///
class shared
{
public:
	shared();
	virtual ~shared();

public:
	int_fast32_t init(const string_t cf);
	void exec();
	void handle_quot(struct quot_info& qi);
	void handle_cmd_res();
	void handle_match_res();
	void handle_x();
	void handle_res();

	/**
	 *
	 * */
	int_fast32_t async_send_command(struct order_info& oi);

private:
//	qtp::dce_trade_handler dth_;
//	qtp::dce_quot_handler dqh_;
	//
	std::vector<atp::algo_base::pointer_t> algos_;
	///logger
	std::shared_ptr<spd::logger> console_logger_, async_file_logger_;
};

#define SHARED_OBJ() utils::singleton<qtp::shared>::inst()

} /* namespace qtp_bl */

#endif /* SHARED_H_ */
