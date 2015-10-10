/*
 * shared.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */


#include <fstream>
#include <json/json/src/json.hpp>
using json = nlohmann::json;

#include "shared.h"

namespace qtp
{

shared::shared()
{
	// TODO Auto-generated constructor stub

}

shared::~shared()
{
	// TODO Auto-generated destructor stub
}

void shared::handle_quot(struct quot_info& qi)
{
	for (auto iter = algos_.begin(); iter != algos_.end(); ++iter)
	{
		(*iter)->handle_quot(qi);
	}
//	///TODO: quant logic.
//
//	///send command
//	struct order_info oi =
//	{ 0 };
//	int_fast32_t ret = async_send_command(oi);

}

int_fast32_t shared::async_send_command(struct order_info& oi)
{


	return 0;
}



void shared::exec()
{
	///check trade conn
	dth_.check_conn();

	///check quot conn
	dqh_.check_conn();
}

void shared::handle_x()
{
}

void shared::handle_res()
{
}

int_fast32_t shared::init(const string_t cf)
{
	///init log
	try
	{
		//Create console, multithreaded logger
		console_logger_ = spd::stdout_logger_mt("console");
		console_logger_->info("Welcome to spdlog!");
		console_logger_->info("An info message example {}..", 1);
		console_logger_->info() << "Streams are supported too  " << 1;

		//Formatting examples
		console_logger_->info("Easy padding in numbers like {:08d}", 12);
		console_logger_->info(
				"Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}",
				42);
		console_logger_->info("Support for floats {:03.2f}", 1.23456);
		console_logger_->info("Positional args are {1} {0}..", "too", "supported");

		console_logger_->info("{:<30}", "left aligned");
		console_logger_->info("{:>30}", "right aligned");
		console_logger_->info("{:^30}", "centered");

		//
		// Runtime log levels
		//
		spd::set_level(spd::level::info); //Set global log level to info
		console_logger_->debug("This message shold not be displayed!");
		console_logger_->set_level(spd::level::debug); // Set specific logger's log level
		console_logger_->debug("Now it should..");

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
		SPDLOG_TRACE(console_logger_, "Enabled only #ifdef SPDLOG_TRACE_ON..{} ,{}", 1,
				3.23);
		SPDLOG_DEBUG(console_logger_, "Enabled only #ifdef SPDLOG_DEBUG_ON.. {} ,{}", 1,
				3.23);

		//
		// Asynchronous logging is very fast..
		// Just call spdlog::set_async_mode(q_size) and all created loggers from now on will be asynchronous..
		//
		size_t q_size = 1048576; //queue size must be power of 2
		spdlog::set_async_mode(q_size);
		async_file_logger_ = spd::daily_logger_st("async_file_logger",
				"logs/async_log.txt");
		async_file_logger_->info() << "This is async log.." << "Should be very fast!";
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
	} catch (const spd::spdlog_ex& ex)
	{
		std::cout << "Log failed: " << ex.what() << std::endl;
		return -1;
	}

	///get conf info
	json j;
	std::ifstream f(cf);
	j << f;

	console_logger_->debug("json 1: {0}", j["x"]);

	return RET_SUC;
}

//int_fast32_t qtp::shared::async_send_command(struct order_info& oi)
//{
//}
//int_fast32_t qtp::shared::async_send_command(struct order_info& oi)
//{
//}

} /* namespace qtp_bl */
