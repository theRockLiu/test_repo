/*
 * shared.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#include <fstream>
#include <memory>

//#include <json/json/src/json.hpp>
//using json = nlohmann::json;
#include <base/base.h>
using namespace smart_utils;

#include "../../include/atp.h"
#include "shared.h"
#include "../quot/quothandler.h"
#include "../trade/tradehandler.h"

namespace
{
	void ne_func()
	{
		SHARED().exec();
	}
}

namespace satp
{

	rest_service::rest_service(utility::string_t url)
											: http_listener_(url)
	{
		http_listener_.support(methods::GET, std::bind(&rest_service::handle_get, this, std::placeholders::_1));
		http_listener_.support(methods::POST, std::bind(&rest_service::handle_post, this, std::placeholders::_1));

		m_htmlcontentmap[U("/")] = std::make_tuple(U("AppCode.html"), U("text/html"));
		m_htmlcontentmap[U("/js/default.js")] = std::make_tuple(U("js/default.js"), U("application/javascript"));
		m_htmlcontentmap[U("/css/default.css")] = std::make_tuple(U("css/default.css"), U("text/css"));
		m_htmlcontentmap[U("/image/logo.png")] = std::make_tuple(U("image/logo.png"), U("application/octet-stream"));
		m_htmlcontentmap[U("/image/bing-logo.jpg")] = std::make_tuple(U("image/bing-logo.jpg"), U("application/octet-stream"));
		m_htmlcontentmap[U("/image/wall.jpg")] = std::make_tuple(U("image/wall.jpg"), U("application/octet-stream"));
	}

	void rest_service::handle_error(pplx::task<void>& t)
	{
		try
		{
			t.get();
		}
		catch (...)
		{
			// Ignore the error, Log it if a logger is available
		}
	}

	pplx::task<void> rest_service::open()
	{
		return http_listener_.open().then([](pplx::task<void> t)
		{	handle_error(t);});
	}

	pplx::task<void> rest_service::close()
	{
		return http_listener_.close().then([](pplx::task<void> t)
		{	handle_error(t);});
	}

	// Handler to process HTTP::GET requests.
	// Replies to the request with data.
	void rest_service::handle_get(http_request message)
	{
		auto path = message.relative_uri().path();
		auto content_data = m_htmlcontentmap.find(path);
		if (content_data == m_htmlcontentmap.end())
		{
			message.reply(status_codes::NotFound, U("Path not found")).then([](pplx::task<void> t)
			{	handle_error(t);});
			return;
		}

		auto file_name = std::get<0>(content_data->second);
		auto content_type = std::get<1>(content_data->second);
		concurrency::streams::fstream::open_istream(file_name, std::ios::in).then([=](concurrency::streams::istream is)
		{
			message.reply(status_codes::OK, is, content_type).then([](pplx::task<void> t)
													{	handle_error(t);});
		}).then([=](pplx::task<void> t)
		{
			try
			{
				t.get();
			}
			catch(...)
			{
				// opening the file (open_istream) failed.
				// Reply with an error.
				message.reply(status_codes::InternalError);
			}
		});
	}

	// Respond to HTTP::POST messages
	// Post data will contain the postal code or location string.
	// Aggregate location data from different services and reply to the POST request.
	void rest_service::handle_post(http_request message)
	{
		auto path = message.relative_uri().path();
		if (0 == path.compare(U("/")))
		{
			message.extract_string().then([=](const utility::string_t& location)
			{
				//get_data(message, location);
			}).then([](pplx::task<void> t)
			{	handle_error(t);});
		}
		else
		{
			message.reply(status_codes::NotFound, U("Path not found")).then([](pplx::task<void> t)
			{	handle_error(t);});
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	shared::shared()
											: r_flag_(true)
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
			spd::set_level(spd::level::debug); //Set global log level to info
			c_logger_->debug("This message shold not be displayed!");
			c_logger_->set_level(spd::level::debug); // Set specific logger's log level
			c_logger_->debug("Now it should..");

			spd::set_pattern("*** [%H:%M:%S %z] [thread %t] %v ***");
			SPDLOG_TRACE(c_logger_, "Enabled only #ifdef SPDLOG_TRACE_ON..{} ,{}", 1,
													3.23);SPDLOG_DEBUG(c_logger_, "Enabled only #ifdef SPDLOG_DEBUG_ON.. {} ,{}", 1,
													3.23);

			af_logger_ = spd::daily_logger_st("async_file_logger", "async_log.txt");
			af_logger_->set_level(spd::level::debug);
			af_logger_->info() << "This is async log.." << "Should be very fast!";
		}
		catch (const spd::spdlog_ex& ex)
		{
			std::cout << "Log failed: " << ex.what() << std::endl;
			return -1;
		}

		/**
		 * get conf info .
		 * */
		//json json_obj;
		std::ifstream file(cf);
		//file >> json_obj;

		//LOGGER()->debug("read conf, the client count is {:d}!\n", json_obj.count("client_info"));
		//af_logger_->flush();
		web::json::value v = web::json::value::parse(file);
		LOGGER()->debug("json size: %d\n", v.size());
		//rs_addr_ = ;

//		for (auto x : json_obj)
//		{
//			LOGGER()->debug(x.dump());
//			auto cid = x["client_id"];
//			LOGGER()->debug(cid);
//			auto base_contracts = x["base_contracts"];
//			LOGGER()->debug(base_contracts.dump());
//			for (auto y : base_contracts)
//			{
//				LOGGER()->debug(y.dump());
//				string_t tmp = y["id"];
//				client_infos_[cid].base_contracts_[hash_str(tmp.c_str())].max_bid_posi_ = y["max_bid_posi"];
//			}
//
//			auto arbi_contracts = x["arbi_contracts"];
//			for (auto y : arbi_contracts)
//			{
//				LOGGER()->debug(y.dump());
//				string_t tmp = y[0];
//				client_infos_[cid].arbi_contracts_[hash_str(tmp.c_str())].max_bid_posi_ = 1;
//			}
//
//		}

		///start rest service
		// Build our listener's URI from the configured address
		///and the hard-coded path "blackjack/dealer"
		rs_ptr_ = std::unique_ptr<rest_service>(new rest_service(rs_addr_));
		rs_ptr_->open().wait();

		return ne_.open();
	}

	int_fast8_t shared::start()
	{
		SU_AO(r_flag_) = true;
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
		smart_utils::notifier_interface::pointer_t ptr = std::dynamic_pointer_cast<smart_utils::notifier_interface, quot_engine_interface>(qe);
		SU_ASSERT(nullptr != ptr);

		ne_.async_add_notifier(ptr);
		return EC_SUC;
	}

	int_fast8_t shared::rem_quot_engine(quot_engine_interface::pointer_t &qe)
	{
		smart_utils::notifier_interface::pointer_t ptr = std::dynamic_pointer_cast<smart_utils::notifier_interface, quot_engine_interface>(qe);
		SU_ASSERT(nullptr != ptr);

		ne_.async_rem_notifier(ptr);
		return 0;
	}

	int_fast8_t shared::add_trade_engine(trade_engine_interface::pointer_t &te)
	{
		smart_utils::notifier_interface::pointer_t ptr = std::dynamic_pointer_cast<smart_utils::notifier_interface, trade_engine_interface>(te);
		SU_ASSERT(nullptr != ptr);

		ne_.async_add_notifier(ptr);
		return EC_SUC;
	}

	int_fast8_t shared::rem_trade_engine(trade_engine_interface::pointer_t &te)
	{
		smart_utils::notifier_interface::pointer_t ptr = std::dynamic_pointer_cast<smart_utils::notifier_interface, trade_engine_interface>(te);
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
				ptr = std::make_shared<satp::dce_l1_quot_engine>();
				//SHARED().add_quot_engine(ptr);

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
				ptr = std::make_shared<satp::dce_trade_engine>();
				//SHARED().add_trade_engine(ptr);

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
		auto y = x->second.base_contracts_.find(cmd.body_.sor_.contract_id_);
		if (y == x->second.base_contracts_.end())
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
