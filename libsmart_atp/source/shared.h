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

#include <utils/singleton.h>
#include <utils/notifiers.h>

#include "quothandler.h"
#include "tradehandler.h"
#include "../include/atp.h"

namespace satp
{
struct order_info
{
	uint32_t excode :4;
	uint32_t cid :10;
};

class check_conn_timer : smart_utils::timer_base
{
public:
	void handle_timeout(uint64_t times);
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
	///
//	inline void handle_quot(quot_info_t& qi)
//	{
//#if (__SINGLE_ALGO_VER__)
//#else
//		for (auto iter = quot_subscribers_[qi.cid].begin();
//				iter != quot_subscribers_[qi.cid].end(); ++iter)
//		{
//			(*iter)->handle_quot(qi);
//		}
//#endif
//	}

//	inline void handle_cmd_res(trade_cmd_res_t& cr)
//	{
//		cmd_algos_[cr.local_no]->handle_cmd_res(cr);
//	}
//
//	///
//	inline void handle_match_res(match_res_t &mr)
//	{
//		cmd_algos_[mr.local_no]->handle_match_res(mr);
//	}

//	int_fast32_t async_send_command(trade_cmd_t& oi);
	int_fast32_t run_and_wait();
	void handle_timeout(uint64_t times);
	inline std::shared_ptr<spd::logger>& get_logger(){return async_file_logger_;}

private:

	///config info
	std::vector<addr_info_t> trade_gws_;
	std::vector<addr_info_t> quot_gws_;

	///
	bool running_flag_;
	///dce_conn;
	uint_fast32_t dce_local_no_gen_;
	//dce_trade_engine dce_th_;
	CAPIVector<_fldOrder> dce_orders_;
	_fldOrderAction dce_cancel_;
	//dce_quot_engine dce_qh_;

	//typedef std::unordered_map<uint32_t, satp::algo_base::pointer_t> cmd_algos_t;
	//typedef std::vector<satp::algo_base::pointer_t> algos_t;
	///
	//cmd_algos_t cmd_algos_;
	///
	//std::unordered_map<uint32_t, algos_t> quot_subscribers_;
	///logger
	std::shared_ptr<spd::logger> console_logger_, async_file_logger_;

	///
	smart_utils::notifier_engine ne_;
};

#define SHARED() (smart_utils::singleton<satp::shared>::inst())
#define LOGGER() (SHARED().get_logger())

} /* namespace qtp_bl */

#endif /* SHARED_H_ */
