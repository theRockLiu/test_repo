/*
 * tradehandler.h
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#ifndef TRADEHANDLER_H_
#define TRADEHANDLER_H_

#include "../include/atp.h"
#include "../apis/dce/Linux/trade/lib/TradeAPI.h"

namespace atp
{

class dce_trade_handler : public CTradeAPI
{
public:
	dce_trade_handler();
	virtual ~dce_trade_handler();

public:
	void check_conn();
	virtual int onRspTraderInsertOrders(UINT4 nSeqNo,const _fldRspMsg & rspmsg,CAPIVector<_fldOrder> & lstOrder,BYTE bChainFlag=CHAIN_SINGLE);
	virtual int onRspTraderCancelOrder(UINT4 nSeqNo,const _fldRspMsg & rspmsg,const _fldOrderAction & orderaction,BYTE bChainFlag=CHAIN_SINGLE);
	virtual int onNtyTraderMatch(UINT4 nSeqNo,const _fldMatch & match,BYTE bChainFlag=CHAIN_SINGLE);

private:
	trade_cmd_res_t cr_;
};

} /* namespace qtp_bl */

#endif /* TRADEHANDLER_H_ */
