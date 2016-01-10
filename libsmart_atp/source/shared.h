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
	class client_info
	{

	};

	class base_contract_info
	{

	};

	class arbi_contract_info
	{

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

			inline int_fast8_t check_send_base_order();
			inline int_fast8_t check_send_arbi_order();
			inline int_fast8_t check_withdraw_base_order();
			inline int_fast8_t check_withdraw_arbi_order();

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
			std::unordered_map<uint64_t, base_contract_info> base_constracts_;
			std::unordered_map<uint64_t, arbi_contract_info> arbi_constracts_;
			std::unordered_map<uint64_t, client_info> client_infos_;
	};

#define SHARED() (smart_utils::singleton<satp::shared>::inst())
#define LOGGER() (SHARED().get_logger())

} /* namespace qtp_bl */

#endif /* SHARED_H_ */
