/*
 * quothandler.h
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#ifndef QUOTHANDLER_H_
#define QUOTHANDLER_H_

#include "../apis/dce/Linux/quot/lib/QuotAPI.h"

namespace atp
{

class dce_quot_handler: public CQuotAPI
{
public:
	dce_quot_handler();
	virtual ~dce_quot_handler();

public:
	void check_conn();
	virtual int onRspTraderPwdUpd(UINT4 nSeqNo, const _fldRspMsg & rspmsg,
			const _fldTraderPwdUpdReq & traderpwdupdreq, BYTE bChainFlag =
			CHAIN_SINGLE);
	virtual int onNtySnapShotQuot(UINT4 nSeqNo,
			const _fldQuotQryReq & quotqryreq, BYTE bChainFlag = CHAIN_SINGLE);
	virtual int onRspQuotUserLogout(UINT4 nSeqNo, const _fldRspMsg & rspmsg,
			const _fldTraderLogoutRsp & traderlogoutrsp, BYTE bChainFlag =
			CHAIN_SINGLE);
	virtual int onRspQryQuotRight(UINT4 nSeqNo, const _fldRspMsg & rspmsg,
			CAPIVector<_fldQuotSubsRight> & lstQuotSubsRight, BYTE bChainFlag =
			CHAIN_SINGLE);
	virtual int onRspUpdQuotRight(UINT4 nSeqNo, const _fldRspMsg & rspmsg,
			const _fldQuotSubsRight & quotsubsright, BYTE bChainFlag =
			CHAIN_SINGLE);
	virtual int onMarketDataMBLQuot(UINT4 nSeqNo, const _fldBestQuot & bestquot,
			CAPIVector<_fldOptPara> & lstOptPara,
			CAPIVector<_fldMBLQuot> & lstMBLQuot,
			BYTE bChainFlag = CHAIN_SINGLE);
	virtual int onMarketDataArbiMBLQuot(UINT4 nSeqNo,
			const _fldArbiBestQuot & arbibestquot,
			CAPIVector<_fldMBLQuot> & lstMBLQuot,
			BYTE bChainFlag = CHAIN_SINGLE);
	virtual int onRspQuotTraderPwdUpd(UINT4 nSeqNo, const _fldRspMsg & rspmsg,
			const _fldTraderPwdUpdReq & traderpwdupdreq, BYTE bChainFlag =
			CHAIN_SINGLE);
	virtual int onRspQueryHistoryQuot(UINT4 nSeqNo, const _fldRspMsg & rspmsg,
			const _fldBestQuot & bestquot, CAPIVector<_fldOptPara> & lstOptPara,
			CAPIVector<_fldMBLQuot> & lstMBLQuot,
			BYTE bChainFlag = CHAIN_SINGLE);
	virtual int onNtyCloseMktNotice(UINT4 nSeqNo,
			const _fldMktDataNotice & mktdatanotice, BYTE bChainFlag =
			CHAIN_SINGLE);

	virtual int onInvalidPackage(UINT4 nTID, WORD nSeries, UINT4 nSequenceNo,
			WORD nFieldCount, WORD nFieldsLen, const char *pAddr);
	virtual void onChannelLost(const char *szErrMsg);
};

} /* namespace qtp_bl */

#endif /* QUOTHANDLER_H_ */
