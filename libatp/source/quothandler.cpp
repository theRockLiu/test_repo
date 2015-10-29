/*
 * quothandler.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#include "shared.h"

#include "quothandler.h"

namespace atp
{

dce_quot_handler::dce_quot_handler()
{
	// TODO Auto-generated constructor stub

}

dce_quot_handler::~dce_quot_handler()
{
	// TODO Auto-generated destructor stub
}

int dce_quot_handler::onRspQuotUserLogout(UINT4 nSeqNo,
		const _fldRspMsg& rspmsg, const _fldTraderLogoutRsp& traderlogoutrsp,
		BYTE bChainFlag)
{
	return 0;
}

int dce_quot_handler::onRspQryQuotRight(UINT4 nSeqNo, const _fldRspMsg& rspmsg,
		CAPIVector<_fldQuotSubsRight>& lstQuotSubsRight, BYTE bChainFlag)
{
	return 0;
}

int dce_quot_handler::onRspUpdQuotRight(UINT4 nSeqNo, const _fldRspMsg& rspmsg,
		const _fldQuotSubsRight& quotsubsright, BYTE bChainFlag)
{
	return 0;
}

int dce_quot_handler::onMarketDataMBLQuot(UINT4 nSeqNo,
		const _fldBestQuot& bestquot, CAPIVector<_fldOptPara>& lstOptPara,
		CAPIVector<_fldMBLQuot>& lstMBLQuot, BYTE bChainFlag)
{
	SU_ASSERT(CHAIN_SINGLE == bChainFlag);

	struct quot_info qi;// = {.cid = utils::hash_str(bestquot.ContractID.GetBuf())};
	SHARED().handle_quot(qi);

	return 0;
}

int dce_quot_handler::onMarketDataArbiMBLQuot(UINT4 nSeqNo,
		const _fldArbiBestQuot& arbibestquot,
		CAPIVector<_fldMBLQuot>& lstMBLQuot, BYTE bChainFlag)
{
	return 0;
}

int dce_quot_handler::onRspQuotTraderPwdUpd(UINT4 nSeqNo,
		const _fldRspMsg& rspmsg, const _fldTraderPwdUpdReq& traderpwdupdreq,
		BYTE bChainFlag)
{
	return 0;
}

int dce_quot_handler::onNtyCloseMktNotice(UINT4 nSeqNo,
		const _fldMktDataNotice& mktdatanotice, BYTE bChainFlag)
{
	return 0;
}

int dce_quot_handler::onInvalidPackage(UINT4 nTID, WORD nSeries,
		UINT4 nSequenceNo, WORD nFieldCount, WORD nFieldsLen, const char* pAddr)
{
	return 0;
}

void dce_quot_handler::check_conn()
{
}

void dce_quot_handler::onChannelLost(const char* szErrMsg)
{
}

} /* namespace qtp_bl */
