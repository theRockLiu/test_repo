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

	class check_conn_timer: smart_utils::timer_base
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
			int_fast8_t destroy();
			int_fast8_t add_quot_engine(quot_engine::pointer_t&);
			int_fast8_t rem_quot_engine(quot_engine::pointer_t&);
			int_fast8_t add_trade_engine(trade_engine::pointer_t&);
			int_fast8_t rem_trade_engine(trade_engine::pointer_t&);
			int_fast32_t run_and_wait();
			void handle_timeout(uint64_t times);
			inline std::shared_ptr<spd::logger>& get_logger()
			{
				return af_logger_;
			}

		private:
			///running flag...
			bool r_flag_;
			///loggers...
			std::shared_ptr<spd::logger> c_logger_, af_logger_;
			///notifiers...
			smart_utils::notifier_engine ne_;
	};

#define SHARED() (smart_utils::singleton<satp::shared>::inst())
#define LOGGER() (SHARED().get_logger())

} /* namespace qtp_bl */

#endif /* SHARED_H_ */
