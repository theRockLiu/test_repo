/*
 * atp.h
 *
 *  Created on: Sep 23, 2015
 *      Author: rock
 */

#ifndef ATP_H_
#define ATP_H_

#include <cinttypes>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

#include <base/types.h>
#include "../include/data.h"

namespace satp
{
	enum err_code
		: int8_t
		{
			EC_SUC = 0
	};

	enum exchanges
		: uint8_t
		{
			EX_DCE = 0, EX_SSE = 1
	};

	enum trade_type
		: uint8_t
		{
			TT_FUTURE = 0, TT_OPTION
	};

	enum market_statuss
		:uint8_t
		{
			MS_TRADE_INIT = 0, MS_TRADE_AUCTION_SEND_ORDERS, MS_TRADE_AUCTION_PAUSE, MS_TRADE_AUCTION_MATCH, MS_TRADE_CONTINUE, MS_TRADE_PAUSE, MS_TRADE_CLOSED
	};

	enum events
		: uint8_t
		{
			EVT_QUOT = 0, EVT_SEND_ORDER_REQ, EVT_SEND_ORDER_RSP, EVT_WITHDRAW_ORDER_REQ, EVT_WITHDRAW_ORDER_RSP, EVT_MATCH_RSP, EVT_MARKET_STATUS, EVT_VARIETY_STATUS
	};

	struct variety_status_rsp
	{
			uint64_t var_;
			uint8_t trade_type_;
			uint8_t status_;
	};
#define MAX_MARKET_STATUS_CNT (ELEM_SIZE / sizeof(struct variety_status_rsp) - 1)

	typedef struct event
	{
			uint8_t id_ :8;
			//uint32_t len_ :8;
			union body
			{
					struct send_order_rsp
					{
							uint64_t cid_;
							int32_t err_code_;
							uint32_t local_no_;
							uint32_t sys_no_;

					} sor_;

					struct withdraw_order_rsp
					{
							uint64_t cid_;
							int32_t err_code_;
							uint32_t local_no_;
							uint32_t sys_no_;
					} wor_;

					struct order_match_rsp
					{
							uint64_t cid_;
							uint32_t local_no_;
							uint32_t sys_no_;
							double price_;
					} omr_;

					struct market_status_rsp
					{
							uint8_t market_status_;
							uint8_t cnt_;
							struct variety_status_rsp vsr_[MAX_MARKET_STATUS_CNT];
					} msr_;

			} body_;
	} evt_t;

	typedef struct exchange_info
	{
			uint32_t id_ :8;
			string_t server_ip_;
			uint32_t server_port_ :16;

	} exc_info_t;

	typedef struct trade_cmd
	{
			uint32_t id_ :8;

			union body
			{
					struct send_order_req
					{
							uint64_t cid_;
							double price_;
					} sor_;

					struct withdraw_order_req
					{
							uint64_t cid_;
							uint32_t sys_no_;
					} wor_;
			} body_;
	} cmd_t;

	////
	class quot_engine
	{
		public:
			typedef std::shared_ptr<quot_engine> pointer_t;
		public:
			quot_engine()
			{
			}
			virtual ~quot_engine()
			{
			}

		public:
			virtual int_fast8_t init(exc_info_t&) = 0;
			virtual evt_t* get_evt() = 0;
	};

	class trade_engine
	{
		public:
			typedef std::shared_ptr<trade_engine> pointer_t;
		public:
			trade_engine()
			{
			}
			virtual ~trade_engine()
			{
			}

		public:
			virtual int_fast8_t init(exc_info_t &ei, std::unordered_map<std::string, uint64_t> &contracts) = 0;
			virtual evt_t* get_evt() = 0;
			virtual int_fast8_t async_send_cmd(cmd_t&) = 0;
	};

	class algo_trade_platform
	{
		public:
			typedef std::shared_ptr<algo_trade_platform> pointer_t;

		public:
			algo_trade_platform()
			{
			}
			~algo_trade_platform()
			{
			}

		public:
			int_fast8_t init();
			int_fast8_t start()
			{
				return 0;
			}
			int_fast8_t stop()
			{
				return 0;
			}
			quot_engine::pointer_t create_quot_engine(exc_info_t &ei);
			trade_engine::pointer_t create_trade_engine(exc_info_t &ei, std::unordered_map<std::string, uint64_t> &contracts);
	};
}

#endif /* ATP_H_ */
