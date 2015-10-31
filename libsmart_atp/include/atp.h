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

namespace atp
{

enum cmd_type
	: uint8_t
	{
		ATP_ORDER_CMD = 0, ATP_CANCEL_CMD

};

typedef struct quot_info
{
	uint32_t excode :4; //exchnge code
	uint32_t cid :10; //contract id
	uint32_t l1_bid_pl :8;
	uint32_t l1_ask_pl :8;
	uint32_t l2_bid_pl :8;
	uint32_t l2_ask_pl :8;
} quot_info_t;

typedef struct order_cmd
{
	uint32_t excode :4; //exchnge code
	uint32_t cid :10; //contract id
	uint32_t l1_bid_pl :8;
	uint32_t l1_ask_pl :8;
	uint32_t l2_bid_pl :8;
	uint32_t l2_ask_pl :8;
} trade_cmd_t;

//typedef struct _cmd
//{
//	uint32_t excode :4; //exchnge code
//	uint32_t cid :10; //contract id
//	uint32_t l1_bid_pl :8;
//	uint32_t l1_ask_pl :8;
//	uint32_t l2_bid_pl :8;
//	uint32_t l2_ask_pl :8;
//} cmd_t;

typedef struct cmd_res
{
	uint32_t cid : 2;
	uint32_t err_code : 8;
	uint32_t local_no;
	uint32_t pl;
	uint32_t type;
} trade_cmd_res_t;

typedef struct match_res
{
	uint32_t local_no;

} match_res_t;

//typedef struct trade_cmd
//{
////	uint32_t op_code :4;
////	uint32_t sys_no;
////	uint32_t local_no;
////	int8_t client_id[16];
////	int8_t contract_id[80];
////	uint32_t ba_flg :1; //bid or ask
////	uint32_t oc_flg :1; //open or close
////	uint32_t sh_flg :1; //speculate or hedge.
////	REAL8 StopPrice;
////	REAL8 Price;
////	UINT4 Qty;
////	BYTE OrderType;
////	BYTE OrderAttr;
////	INT1 MatchSession;
////	BYTEARRAY<8> ValidThrough;
////	UINT4 MinQty;
////	INT1 AutoSuspend;
////	BYTEARRAY<12> OrderTime;
////	BYTEARRAY<6> MsgRef;
////	BYTE OrderSort;
////	BYTE OrderSrc;
////	BYTE ForceOffsetReason;
////	BYTEARRAY<80> ArbiContractID;
////	UINT4 OrderBatchNo;
////	INT1 IsMktMk;
////	BYTEARRAY<14> ClearAccountNo;
////	BYTE TradeType;
//} trade_cmd_t;

typedef struct cancel_cmd
{
	int32_t x;
} cancel_cmd_t;

class algo_base
{
public:
	typedef std::shared_ptr<atp::algo_base> pointer_t;

public:
	algo_base()
	{
	}
	virtual ~algo_base()
	{
	}

public:
	int_fast32_t async_send_cmd(trade_cmd_t&);
	void set_timer();

public:
	virtual void handle_quot(quot_info_t&) = 0;
	virtual void handle_cmd_res(trade_cmd_res_t&) = 0;
	virtual void handle_match_res(match_res_t&) = 0;
	virtual void handle_state(){};
	virtual void handle_timer(){};
};

class algo_engine
{
public:

public:
	int_fast32_t init();
	int_fast32_t reg_algo(algo_base::pointer_t& algo,
			std::vector<std::string>& contract_ids);
	int_fast32_t run_and_wait();
};

}

#endif /* ATP_H_ */
