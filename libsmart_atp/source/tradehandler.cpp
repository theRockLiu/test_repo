/*
 * tradehandler.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#include "shared.h"

#include "tradehandler.h"

namespace atp
{

dce_trade_handler::dce_trade_handler()
{
	// TODO Auto-generated constructor stub

}

dce_trade_handler::~dce_trade_handler()
{
	// TODO Auto-generated destructor stub
}


void dce_trade_handler::check_conn()
{
}

int dce_trade_handler::onRspTraderInsertOrders(UINT4 nSeqNo,
		const _fldRspMsg& rspmsg, CAPIVector<_fldOrder>& lstOrder,
		BYTE bChainFlag)
{
	SU_ASSERT(CHAIN_SINGLE == bChainFlag);
	cr_.err_code = rspmsg.ErrCode;
	cr_.local_no = lstOrder[0].LocalOrderNo;

	SHARED().handle_cmd_res(cr_);

	return 0;
}

int dce_trade_handler::onRspTraderCancelOrder(UINT4 nSeqNo,
		const _fldRspMsg& rspmsg, const _fldOrderAction& orderaction,
		BYTE bChainFlag)
{
}

int dce_trade_handler::onNtyTraderMatch(UINT4 nSeqNo,
		const _fldMatch& match, BYTE bChainFlag)
{
}


} /* namespace qtp_bl */
