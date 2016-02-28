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
#include <functional>
#include <unordered_map>

#include <log/spdlog/include/spdlog/spdlog.h>
namespace spd = spdlog;

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <cpprest/filestream.h>
#include <cpprest/containerstream.h>
#include <cpprest/producerconsumerstream.h>
#include <cpprest/http_listener.h>
#include <cpprest/http_client.h>

#include <utils/singleton.h>
#include <utils/notifiers.h>

#include "../quot/quothandler.h"
#include "../trade/tradehandler.h"
#include "../../include/atp.h"

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
		public:
			uint32_t max_bid_posi_;
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
			std::unordered_map<uint64_t, base_contract_info> base_contracts_;
			std::unordered_map<uint64_t, arbi_contract_info> arbi_contracts_;
			std::unordered_map<uint64_t, opt_contract_info> opt_contracts_;
			margin_info margin_info_;

	};

	using namespace web::http;
	/**
	 *
	 *
	 * */
	class rest_service
	{
		public:
			rest_service(){}
			rest_service(utility::string_t url);

			pplx::task<void> open();
			pplx::task<void> close();

			void handle_get(web::http::http_request message);
			void handle_post(web::http::http_request message);

		private:

			// Error handlers
			static void handle_error(pplx::task<void>& t);
			pplx::task<web::json::value> handle_exception(pplx::task<web::json::value>& t, const utility::string_t& field_name);

			// methods to fetch data from the services
			pplx::task<web::json::value> get_events(const utility::string_t& post_code);
			pplx::task<web::json::value> get_weather(const utility::string_t& post_code);
			pplx::task<web::json::value> get_pictures(const utility::string_t& location, const utility::string_t& count);
			pplx::task<web::json::value> get_movies(const utility::string_t& post_code);

			// Utility functions
			bool is_number(const std::string& s);
			std::wstring get_date();

			void fetch_data(web::http::http_request message, const std::wstring& postal_code, const std::wstring& location);
			void get_data(web::http::http_request message, const std::wstring& location);

			static const int num_events = 5;
			static const int num_images = 5;
			static const int num_movies = 5;

			static const utility::string_t events_json_key;
			static const utility::string_t movies_json_key;
			static const utility::string_t weather_json_key;
			static const utility::string_t images_json_key;
			static const utility::string_t error_json_key;

			// Lock to the in memory cache (m_data)
			//concurrency::reader_writer_lock m_rwlock;

			// key: City name or postal code
			// Value: JSON response data to be sent to clients
			// We are caching the data for multiple requests.
			std::map<utility::string_t, web::json::value> m_data;

			// m_htmlcontentmap contains data about the html contents of the website, their mime types
			// key: relative URI path of the HTML content being requested
			// value: Tuple where:
			// Element1: relative path on the disk of the file being requested
			// Element2: Mime type/content type of the file
			std::map<utility::string_t, std::tuple<utility::string_t, utility::string_t>> m_htmlcontentmap;

			// HTTP listener
			web::http::experimental::listener::http_listener http_listener_;
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
				return c_logger_;
			}

			int_fast8_t check_send_base_order(trade_cmd_t &cmd);
			inline int_fast8_t check_send_arbi_order(trade_cmd_t &cmd);
			inline int_fast8_t check_withdraw_base_order(trade_cmd_t &cmd);
			inline int_fast8_t check_withdraw_arbi_order(trade_cmd_t &cmd);

		private:
			///running flag...
			bool r_flag_;
			///loggers...
			std::shared_ptr<spd::logger> c_logger_, af_logger_;
			///notifiers...
			smart_utils::notifiers_engine ne_;
			///thread
			std::shared_ptr<std::thread> ne_thread_;
			///
			std::unordered_map<uint64_t, client_info> client_infos_;
			///
			std::string rest_service_addr_; ///e.g. "http://localhost:8888"
			rest_service rs_;

	};

#define SHARED() (smart_utils::singleton<satp::shared>::inst())
#define LOGGER() (SHARED().get_logger())

} /* namespace qtp_bl */

#endif /* SHARED_H_ */
