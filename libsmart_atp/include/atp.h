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

#define LLRB_SIZE (1024 * 1024 * 1024)
#define ELEM_SIZE (256)

namespace satp
{
	inline uint64_t hash_str(const char* data)
	{
		//return *((uint64_t*)data);

		uint64_t hash = 5381;
		int c;

		while (c = *data++)
			hash = ((hash << 5) + hash) + c;

		return hash;
	}

	enum err_code
		: int8_t
		{
			EC_SUC = 0
	};

	enum engine_type
		: uint8_t
		{
			ET_DCE_TRADE = 0, ET_DCE_L1_QUOT, ET_DCE_L2_QUOT, EX_SSE
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
			EVT_L1_BASE_QUOT = 0,
		EVT_L1_ARBI_QUOT,
		EVT_L2_BASE_QUOT,
		EVT_L2_ARBI_QUOT,
		EVT_SEND_BASE_ORDER_REQ,
		EVT_SEND_ARBI_ORDER_REQ,
		EVT_SEND_ORDER_RSP,
		EVT_WITHDRAW_BASE_ORDER_REQ,
		EVT_WITHDRAW_ARBI_ORDER_REQ,
		EVT_WITHDRAW_ORDER_RSP,
		EVT_MATCH_RSP,
		EVT_MARKET_STATUS,
		EVT_VARIETY_STATUS,
		EVT_ORDER_STATUS,
		EVT_CNT
	};

	enum error_code
		: int16_t
		{
			RET_SUC = 0
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

					struct order_status_rsp
					{
							uint64_t contract_id_;
							uint64_t client_id_;
							uint32_t sys_no_;
							uint32_t local_no_;
					} osr_;

					struct l1_base_quot
					{
							uint64_t contract_id_;
							double last_price_;
							double high_price_;
							double low_price_;
							double bid_price_;
							double ask_price_;
							uint32_t bid_qty_;
							uint32_t ask_qty_;
							byte_t tm_[8];
					} lbq_;

					struct l1_arbi_quot
					{
							uint64_t contract_id_;
							double last_price_;
							double high_price_;
							double low_price_;
							double bid_price_;
							double ask_price_;
							uint32_t bid_qty_;
							uint32_t ask_qty_;
							byte_t tm_[8];

					} laq_;

			} body_;
	} evt_t;

	typedef struct engine_info
	{
			string_t server_ip_;
			uint32_t server_port_ :16;

	} engine_info_t;

	typedef struct trade_cmd
	{
			uint32_t id_ :8;

			union body
			{
					struct send_order_req
					{
							uint64_t contract_id_;
							double price_;
					} sor_;

					struct withdraw_order_req
					{
							uint64_t cid_;
							uint32_t sys_no_;
					} wor_;
			} body_;
	} trade_cmd_t;

	/**
	 *
	 * */
	class quot_engine_interface
	{
		public:
			typedef std::shared_ptr<satp::quot_engine_interface> pointer_t;
		public:
			quot_engine_interface()
			{
			}
			virtual ~quot_engine_interface()
			{
			}

		public:
			virtual int_fast8_t init(const engine_info_t &ei) = 0;
			virtual evt_t* get_evt() = 0;
			virtual void done() = 0;
	};

	/**
	 *
	 * */
	class trade_engine_interface
	{
		public:
			typedef std::shared_ptr<satp::trade_engine_interface> pointer_t;
		public:
			trade_engine_interface()
			{
			}
			virtual ~trade_engine_interface()
			{
			}

		public:
			virtual int_fast8_t init(const engine_info_t &ei) = 0;
			virtual evt_t* get_evt() = 0;
			virtual void done() = 0;
			virtual int_fast8_t async_send_cmd(trade_cmd_t&) = 0;
	};

	/**
	 * algorithm trade platform
	 * */
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
			int_fast8_t init(const string_t &cf);
			int_fast8_t start();
			int_fast8_t stop();
			quot_engine_interface::pointer_t create_quot_engine(enum engine_type et);
			trade_engine_interface::pointer_t create_trade_engine(enum engine_type et);
	};
}

#endif /* ATP_H_ */
