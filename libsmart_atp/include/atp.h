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

	enum events
		: uint8_t
		{
			EVT_QUOT = 0, EVT_CMD_RES = 1
	};

	typedef struct event
	{
			uint32_t id_ :8;
			uint32_t len_ :16;
			byte_t body_[0];
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
			virtual int_fast8_t init(exc_info_t&) = 0;
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
			}
			int_fast8_t stop()
			{
			}
			quot_engine::pointer_t create_quot_engine(exc_info_t &ei);
			trade_engine::pointer_t create_trade_engine(exc_info_t &ei);
	};
}

#endif /* ATP_H_ */
