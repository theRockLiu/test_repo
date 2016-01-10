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
#include <thread>
#include <memory>
#include <unordered_map>

#include <log/spdlog/include/spdlog/spdlog.h>
namespace spd = spdlog;

#include <utils/singleton.h>
#include <utils/notifiers.h>

#include "quothandler.h"
#include "tradehandler.h"
#include "../include/atp.h"

namespace satp
{
	enum open_or_close
	{
		OOC_OPEN = 0, OOC_CLOSE = 1

	};

	enum bid_or_ask
	{
		BOA_BID = 0, BOA_ASK = 1
	};

	class base_contract_info
	{
		public:
			uint32_t max_bid_posi_;
			uint32_t bid_req_posi_; /**< sender thread. */
			uint32_t bid_rsp_done_posi_; /**< recver thread */
			uint32_t bid_rsp_withdraw_posi_;/**< recver thread */
			uint32_t bid_rsp_err_posi_;/**< recver thread */
			uint32_t bid_rsp_close_posi_;

			uint32_t ask_pending_posi_; /**< sender thread. */
			uint32_t ask_done_posi_; /**< recver thread */
			uint32_t ask_suc_posi_;/**< recver thread */
			uint32_t ask_err_posi_;/**< recver thread */

			uint32_t max_price_level_ :16;
			uint32_t margin_unit :16;
	};

	class opt_contract_info
	{

	};

	class arbi_contract_info
	{

	};

	class margin_info
	{
		public:
			uint64_t sum_;
			uint64_t req_sum_;
			uint64_t rsp_close_;
			uint64_t rsp_withdraw_;
			uint64_t rsp_err_;
	};

	class client_info
	{

		public:
			std::unordered_map<uint64_t, base_contract_info> base_constracts_;
			std::unordered_map<uint64_t, arbi_contract_info> arbi_constracts_;
			std::unordered_map<uint64_t, opt_contract_info> opt_constracts_;
			margin_info margin_info_;

	};

	/**
	 *
	 * */
	class shared
	{
		public:
			shared();
			~shared();

		public:
			int_fast8_t init(const string_t cf);
			int_fast8_t start();
			void exec();
			int_fast8_t stop();
			int_fast8_t destroy();
			///
			int_fast8_t add_quot_engine(quot_engine_interface::pointer_t&);
			int_fast8_t rem_quot_engine(quot_engine_interface::pointer_t&);
			int_fast8_t add_trade_engine(trade_engine_interface::pointer_t&);
			int_fast8_t rem_trade_engine(trade_engine_interface::pointer_t&);
			///
			inline std::shared_ptr<spd::logger>& get_logger()
			{
				return af_logger_;
			}

			int_fast8_t check_send_base_order(trade_cmd_t &cmd);
			inline int_fast8_t check_send_arbi_order(trade_cmd_t &cmd);
			inline int_fast8_t check_withdraw_base_order(trade_cmd_t &cmd);
			inline int_fast8_t check_withdraw_arbi_order(trade_cmd_t &cmd);

		private:
			///running flag...
			bool r_flag_;
			///loggers...
			std::shared_ptr<spd::logger> c_logger_,
													af_logger_;
			///notifiers...
			smart_utils::notifiers_engine ne_;
			///thread
			std::shared_ptr<std::thread> ne_thread_;
			///
			std::unordered_map<uint64_t, client_info> client_infos_;
	};

#define SHARED() (smart_utils::singleton<satp::shared>::inst())
#define LOGGER() (SHARED().get_logger())

} /* namespace qtp_bl */

#endif /* SHARED_H_ */
