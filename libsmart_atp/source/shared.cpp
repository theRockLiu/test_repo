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

namespace
{
	void ne_func()
	{
		SHARED().exec();
	}
}

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

	int_fast8_t shared::init(const string_t cf)
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

	int_fast8_t shared::start()
	{
		ne_thread_ = std::make_shared<std::thread>(ne_func);

		return 0;
	}

	void shared::exec()
	{
		while (SU_AO(r_flag_))
		{
			ne_.check_once(1);
		}
	}

	int_fast8_t shared::stop()
	{
		SU_AO(r_flag_) = false;
		ne_thread_->join();
		ne_thread_ = nullptr;

		return 0;
	}

	int_fast8_t shared::add_quot_engine(quot_engine_interface::pointer_t &qe)
	{
		smart_utils::notifier_interface::pointer_t ptr = std::dynamic_pointer_cast<
												smart_utils::notifier_interface,
												quot_engine_interface>(qe);
		SU_ASSERT(nullptr != ptr);

		ne_.async_add_notifier(ptr);
		return EC_SUC;
	}

	int_fast8_t shared::rem_quot_engine(quot_engine_interface::pointer_t &qe)
	{
		smart_utils::notifier_interface::pointer_t ptr = std::dynamic_pointer_cast<
												smart_utils::notifier_interface,
												quot_engine_interface>(qe);
		SU_ASSERT(nullptr != ptr);

		ne_.async_rem_notifier(ptr);
		return 0;
	}

	int_fast8_t shared::add_trade_engine(trade_engine_interface::pointer_t &te)
	{
		smart_utils::notifier_interface::pointer_t ptr = std::dynamic_pointer_cast<
												smart_utils::notifier_interface,
												trade_engine_interface>(te);
		SU_ASSERT(nullptr != ptr);

		ne_.async_add_notifier(ptr);
		return EC_SUC;
	}

	int_fast8_t shared::rem_trade_engine(trade_engine_interface::pointer_t &te)
	{
		smart_utils::notifier_interface::pointer_t ptr = std::dynamic_pointer_cast<
												smart_utils::notifier_interface,
												trade_engine_interface>(te);
		SU_ASSERT(nullptr != ptr);

		ne_.async_rem_notifier(ptr);
		return 0;
	}

	int_fast8_t shared::destroy()
	{
		return 0;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	int_fast8_t algo_trade_platform::init(const string_t &cf)
	{
		return SHARED().init(cf);
	}

	int_fast8_t algo_trade_platform::start()
	{
		return SHARED().start();
	}

	int_fast8_t algo_trade_platform::stop()
	{
		return SHARED().stop();
	}

	quot_engine_interface::pointer_t algo_trade_platform::create_quot_engine(enum engine_type et)
	{
		quot_engine_interface::pointer_t ptr = nullptr;
		switch (et)
		{
			case satp::engine_type::ET_DCE_L1_QUOT:
			{
				ptr = std::make_shared<
														satp::dce_l1_quot_engine>();
				SHARED().add_quot_engine(ptr);

				break;
			}
			case satp::engine_type::ET_DCE_L2_QUOT:
			{
				//				ptr = std::make_shared<satp::dce_l2_quot_engine>();
				//				SHARED().add_trade_engine(ptr);

				break;
			}
			default:
			{
				SU_ASSERT(false);
				break;
			}
		}

		return ptr;
	}

	trade_engine_interface::pointer_t algo_trade_platform::create_trade_engine(enum engine_type et)
	{
		trade_engine_interface::pointer_t ptr = nullptr;
		switch (et)
		{
			case satp::engine_type::ET_DCE_TRADE:
			{
				ptr = std::make_shared<
														satp::dce_trade_engine>();
				SHARED().add_trade_engine(ptr);

				break;
			}
			default:
			{
				SU_ASSERT(false);
				break;
			}
		}

		return ptr;
	}

	int_fast8_t satp::shared::check_send_base_order(trade_cmd_t &cmd)
	{
		auto x = client_infos_.find(cmd.body_.sor_.client_id_);
		if (x == client_infos_.end())
		{
			return RET_ERR;
		}

		///check cmd
		auto y = x->second.base_constracts_.find(cmd.body_.sor_.contract_id_);
		if (y == x->second.base_constracts_.end())
		{
			return RET_ERR;
		}

		if (cmd.body_.sor_.price_level_ < 0 || cmd.body_.sor_.price_level_ > y->second.max_price_level_)
		{
			return RET_ERR;
		}

		switch (cmd.body_.sor_.open_or_close_)
		{
			case OOC_OPEN:
			{
				switch (cmd.body_.sor_.bid_or_ask_)
				{
					case BOA_BID:
					{
						uint32_t posi = y->second.bid_req_posi_ - SU_AO(y->second.bid_rsp_withdraw_posi_) - SU_AO(y->second.bid_rsp_close_posi_) - SU_AO(y->second.bid_rsp_err_posi_) + cmd.body_.sor_.cnt_;
						if (posi > y->second.max_bid_posi_)
						{
							return RET_ERR;
						}

						uint64_t margin_used = x->second.margin_info_.req_sum_ - SU_AO(x->second.margin_info_.rsp_withdraw_) - SU_AO(x->second.margin_info_.rsp_err_) - SU_AO(x->second.margin_info_.rsp_close_) + (cmd.body_.sor_.cnt_ * y->second.margin_unit);
						if (margin_used > x->second.margin_info_.sum_)
						{
							return RET_ERR;
						}

						y->second.bid_req_posi_ += cmd.body_.sor_.cnt_;
						x->second.margin_info_.req_sum_ += (cmd.body_.sor_.cnt_ * y->second.margin_unit);

						break;
					}
					case BOA_ASK:
					{
						break;
					}
					default:
					{
						SU_ASSERT(false);
						break;
					}
				}

				break;
			}
			case OOC_CLOSE:
			{
				break;
			}
			default:
			{
				SU_ASSERT(false);
				break;
			}
		}

		return RET_SUC;
	}

	inline int_fast8_t satp::shared::check_send_arbi_order(trade_cmd_t &cmd)
	{
		return RET_SUC;
	}

	inline int_fast8_t satp::shared::check_withdraw_base_order(trade_cmd_t &cmd)
	{
		return RET_SUC;
	}

	inline int_fast8_t satp::shared::check_withdraw_arbi_order(trade_cmd_t &cmd)
	{
		return RET_SUC;
	}

} /* namespace qtp_bl */
