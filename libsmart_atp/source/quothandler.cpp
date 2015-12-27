/*
 * quothandler.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#include "shared.h"
#include "quothandler.h"

namespace satp
{
	dce_quot_engine::dce_quot_engine()
											: timer_base(smart_utils::timer_base::ETT_MONOTONIC, 5, 0), init_(false), conn_ok_(false)
	{
		// TODO Auto-generated constructor stub
	}

	dce_quot_engine::~dce_quot_engine()
	{
		// TODO Auto-generated destructor stub
	}

	int dce_quot_engine::onRspQuotUserLogout(UINT4 nSeqNo, const _fldRspMsg& rspmsg, const _fldTraderLogoutRsp& traderlogoutrsp, BYTE bChainFlag)
	{
		return 0;
	}

	int dce_quot_engine::onRspQryQuotRight(UINT4 nSeqNo, const _fldRspMsg& rspmsg, CAPIVector<_fldQuotSubsRight>& lstQuotSubsRight, BYTE bChainFlag)
	{
		return 0;
	}

	int dce_quot_engine::onRspUpdQuotRight(UINT4 nSeqNo, const _fldRspMsg& rspmsg, const _fldQuotSubsRight& quotsubsright, BYTE bChainFlag)
	{
		return 0;
	}

	int dce_quot_engine::onMarketDataMBLQuot(UINT4 nSeqNo, const _fldBestQuot& bestquot, CAPIVector<_fldOptPara>& lstOptPara, CAPIVector<_fldMBLQuot>& lstMBLQuot, BYTE bChainFlag)
	{
		SU_CHECK(CHAIN_SINGLE == bChainFlag);

//		struct quot_info qi; // = {.cid = utils::hash_str(bestquot.ContractID.GetBuf())};
//		SHARED().handle_quot(qi);

		return 0;
	}

	int dce_quot_engine::onMarketDataArbiMBLQuot(UINT4 nSeqNo, const _fldArbiBestQuot& arbibestquot, CAPIVector<_fldMBLQuot>& lstMBLQuot, BYTE bChainFlag)
	{
		return 0;
	}

	int dce_quot_engine::onRspQuotTraderPwdUpd(UINT4 nSeqNo, const _fldRspMsg& rspmsg, const _fldTraderPwdUpdReq& traderpwdupdreq, BYTE bChainFlag)
	{
		return 0;
	}

	int dce_quot_engine::onNtyCloseMktNotice(UINT4 nSeqNo, const _fldMktDataNotice& mktdatanotice, BYTE bChainFlag)
	{
		return 0;
	}

	int dce_quot_engine::onInvalidPackage(UINT4 nTID, WORD nSeries, UINT4 nSequenceNo, WORD nFieldCount, WORD nFieldsLen, const char* pAddr)
	{
		return 0;
	}

	void dce_quot_engine::onChannelLost(const char* szErrMsg)
	{
	}

} /* namespace qtp_bl */
