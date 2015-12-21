/*
 * tradehandler.h
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#ifndef TRADEHANDLER_H_
#define TRADEHANDLER_H_

#include <cinttypes>
#include <utils/notifiers.h>
#include "../include/atp.h"
#include "../apis/dce/Linux/trade/lib/TradeAPI.h"

namespace satp
{

class dce_trade_engine : public CTradeAPI, public trade_engine, public smart_utils::timer_base
{
public:
	dce_trade_engine();
	virtual ~dce_trade_engine();

public:
	///trade api
	int_fast8_t init(){}
	void check_conn();
	virtual int onRspTraderInsertOrders(UINT4 nSeqNo,const _fldRspMsg & rspmsg,CAPIVector<_fldOrder> & lstOrder,BYTE bChainFlag=CHAIN_SINGLE);
	virtual int onRspTraderCancelOrder(UINT4 nSeqNo,const _fldRspMsg & rspmsg,const _fldOrderAction & orderaction,BYTE bChainFlag=CHAIN_SINGLE);
	virtual int onNtyTraderMatch(UINT4 nSeqNo,const _fldMatch & match,BYTE bChainFlag=CHAIN_SINGLE);

	///trade_engine
	virtual byte_t* get_data(){return NULL;}
	virtual int_fast8_t async_send_cmd( cmd_t &cmd){return 0;}

	///timer base
	virtual void handle_timeout(uint64_t ui){}

private:
	//trade_cmd_res_t cr_;
};

} /* namespace qtp_bl */

#endif /* TRADEHANDLER_H_ */
