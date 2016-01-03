/*
 * shared.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#include <fstream>
#include <memory>
#include <json/json/src/json.hpp>
using json = nlohmann::json;
#include <base/base.h>
using namespace smart_utils;

#include "../include/atp.h"
#include "shared.h"
#include "quothandler.h"
#include "tradehandler.h"

namespace satp
{
	shared::shared()
	{
		// TODO Auto-generated constructor stub

	}

	shared::~shared()
	{
		// TODO Auto-generated destructor stub
	}

//int_fast32_t shared::async_send_command(trade_cmd_t& tc)
//{
//	if (ATP_ORDER_CMD == tc.cid)
//	{
//		dce_orders_[0].LocalOrderNo = dce_local_no_gen_++;
//		return dce_th_.ReqTraderInsertOrders(NULL, dce_orders_);
//	}
//	else if (ATP_CANCEL_CMD == tc.cid)
//	{
//		dce_cancel_.LocalOrderNo = dce_local_no_gen_++;
//		return dce_th_.ReqTraderCancelOrder(NULL, dce_cancel_);
//	}
//	else
//	{
//		SU_CHECK(false);
//	}
//
//	return SU_EC_SUC;
//}

	void shared::exec()
	{
//	///check trade conn
//	dth_.check_conn();
//
//	///check quot conn
//	dqh_.check_conn();
	}

	int_fast32_t shared::init(const string_t cf)
	{
		///init log
		try
		{
			//Create console, multithreaded logger
			c_logger_ = spd::stdout_logger_mt("console");
			c_logger_->info("Welcome to spdlog!");
			c_logger_->info("An info message example {}..", 1);
			c_logger_->info() << "Streams are supported too  " << 1;

			//Formatting examples
			c_logger_->info("Easy padding in numbers like {:08d}", 12);
			c_logger_->info("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
			c_logger_->info("Support for floats {:03.2f}", 1.23456);
			c_logger_->info("Positional args are {1} {0}..", "too", "supported");

			c_logger_->info("{:<30}", "left aligned");
			c_logger_->info("{:>30}", "right aligned");
			c_logger_->info("{:^30}", "centered");

			//
			// Runtime log levels
			//
			spd::set_level(spd::level::info); //Set global log level to info
			c_logger_->debug("This message shold not be displayed!");
			c_logger_->set_level(spd::level::debug); // Set specific logger's log level
			c_logger_->debug("Now it should..");

//		//
//		// Create a file rotating logger with 5mb size max and 3 rotated files
//		//
//		file_logger_ = spd::rotating_logger_mt("file_logger_",
//				"logs/mylogfile", 1048576 * 5, 3);
//		for (int i = 0; i < 10; ++i)
//			file_logger_->info("{} * {} equals {:>10}", i, i, i * i);

			//
			// Create a daily logger - a new file is created every day on 2:30am
			//
			// auto daily_logger = spd::daily_logger_mt("daily_logger", "logs/daily", 2, 30);

			//
			// Customize msg format for all messages
			//
			spd::set_pattern("*** [%H:%M:%S %z] [thread %t] %v ***");
//		file_logger_->info("This is another message with custom format");
//
//		spd::get("console")->info(
//				"loggers can be retrieved from a global registry using the spdlog::get(logger_name) function");

			//
			// Compile time debug or trace macros.
			// Enabled #ifdef SPDLOG_DEBUG_ON or #ifdef SPDLOG_TRACE_ON
			//
			SPDLOG_TRACE(c_logger_, "Enabled only #ifdef SPDLOG_TRACE_ON..{} ,{}", 1,
													3.23);SPDLOG_DEBUG(c_logger_, "Enabled only #ifdef SPDLOG_DEBUG_ON.. {} ,{}", 1,
													3.23);

			//
			// Asynchronous logging is very fast..
			// Just call spdlog::set_async_mode(q_size) and all created loggers from now on will be asynchronous..
			//
			size_t q_size = 1048576; //queue size must be power of 2
			spdlog::set_async_mode(q_size);
			af_logger_ = spd::daily_logger_st("async_file_logger", "logs/async_log.txt");
			af_logger_->info() << "This is async log.." << "Should be very fast!";
			//spdlog::drop_all(); //Close all loggers
			//
			// syslog example. linux only..
			//
//#ifdef __linux__
//		std::string ident = "spdlog-example";
//		auto syslog_logger = spd::syslog_logger("syslog", ident, LOG_PID);
//		syslog_logger->warn(
//				"This is warning that will end up in syslog. This is Linux only!");
//#endif
		}
		catch (const spd::spdlog_ex& ex)
		{
			std::cout << "Log failed: " << ex.what() << std::endl;
			return -1;
		}

		///get conf info
		json j;
		std::ifstream f(cf);
		j << f;

		c_logger_->debug("json 1: {0}", j["x"]);

		return SU_EC_SUC;
	}

	int_fast8_t algo_trade_platform::init()
	{
		return 0;
	}

//int_fast32_t algo_trade_platform::reg_algo(algo_base::pointer_t& algos,
//		std::vector<std::string>& contract_ids)
//{
//	return 0;
//}

//int_fast32_t algo_trade_platform::run_and_wait()
//{
//	return SHARED().run_and_wait();
//}

////////////////////////////////////////////////////////////////////////////////////////////////
//int_fast32_t algo_base::async_send_cmd(trade_cmd_t &tc)
//{
//	////
//	return SHARED().async_send_command(tc);
//}

	int_fast32_t shared::run_and_wait()
	{
		while (r_flag_)
		{
			ne_.check_once(1);
		}

		return SU_EC_SUC;
	}

//	void check_conn_timer::handle_timeout(uint64_t times)
//	{
//		SHARED().handle_timeout(times);
//	}

	int_fast8_t shared::add_quot_engine(quot_engine::pointer_t &qe)
	{
		smart_utils::notifier::pointer_t ptr = std::dynamic_pointer_cast<smart_utils::notifier, quot_engine>(qe);
		SU_ASSERT(nullptr != ptr);

		ne_.async_add_notifier(ptr);
		return EC_SUC;
	}

	int_fast8_t shared::rem_quot_engine(quot_engine::pointer_t&)
	{
		return 0;
	}

	int_fast8_t shared::add_trade_engine(trade_engine::pointer_t &te)
	{
		smart_utils::notifier::pointer_t ptr = std::dynamic_pointer_cast<smart_utils::notifier, trade_engine>(te);
		SU_ASSERT(nullptr != ptr);

		ne_.async_add_notifier(ptr);

//		satp::dce_trade_engine::pointer_t ptra = std::dynamic_pointer_cast<satp::dce_trade_engine, satp::trade_engine>(te);
//		ne_.async_add_notifier(ptra->get_event());

		return EC_SUC;
	}

	int_fast8_t shared::rem_trade_engine(trade_engine::pointer_t&)
	{
		return 0;
	}

//	void shared::exec()
//	{
//		ne_.check_once();
//	}

	int_fast8_t shared::destroy()
	{
		return 0;
	}

	void shared::handle_timeout(uint64_t times)
	{
	}

	quot_engine::pointer_t algo_trade_platform::create_quot_engine(const exc_info_t& ei, const std::vector<std::string> &contracts)
	{
		quot_engine::pointer_t ptr = nullptr;
		switch (ei.id_)
		{
			case satp::exchanges::EX_DCE:
			{
				ptr = std::make_shared<satp::dce_quot_engine>();
				if (EC_SUC != ptr->init(ei, contracts))
				{
					ptr = nullptr;
				}
				else
				{
					SHARED().add_quot_engine(ptr);
				}
				break;
			}
			default:
			{
				break;
			}
		}

		return ptr;
	}

	trade_engine::pointer_t algo_trade_platform::create_trade_engine(const exc_info_t& ei, const std::vector<std::string> &contracts)
	{
		trade_engine::pointer_t ptr = nullptr;
		switch (ei.id_)
		{
			case satp::exchanges::EX_DCE:
			{
				ptr = std::make_shared<satp::dce_trade_engine>();
				if (EC_SUC != ptr->init(ei, contracts))
				{
					ptr = nullptr;
				}
				else
				{
					SHARED().add_trade_engine(ptr);
				}
				break;
			}
			default:
			{
				break;
			}
		}

		return ptr;
	}

} /* namespace qtp_bl */
