/*
 * quothandler.h
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#ifndef QUOTHANDLER_H_
#define QUOTHANDLER_H_

#include <vector>

#include <base/base.h>
using namespace smart_utils;

#include "../apis/dce/Linux/quot/lib/QuotAPI.h"
#include "../include/atp.h"
#include "../include/data.h"

namespace satp
{

class dce_quot_engine: public CQuotAPI, public quot_engine
{
public:
	dce_quot_engine();
	virtual ~dce_quot_engine();

public:
	int_fast8_t init(exc_info_t&){return ec_suc;}
	byte_t* get_data(){return NULL;}
	void check_conn(const std::vector<addr_info_t> &lb_gws,
			const std::vector<addr_info_t> &gws, const string_t &member_id,
			const string_t &trader_no, const string_t &passwd,
			const string_t &prog_id, const string_t &prog_ver);
//	virtual int onRspTraderPwdUpd(UINT4 nSeqNo, const _fldRspMsg & rspmsg,
//			const _fldTraderPwdUpdReq & traderpwdupdreq, BYTE bChainFlag =
//			CHAIN_SINGLE);
//	virtual int onNtySnapShotQuot(UINT4 nSeqNo,
//			const _fldQuotQryReq & quotqryreq, BYTE bChainFlag = CHAIN_SINGLE);
	virtual int onRspQuotUserLogout(UINT4 nSeqNo, const _fldRspMsg & rspmsg,
			const _fldTraderLogoutRsp & traderlogoutrsp, BYTE bChainFlag =
			CHAIN_SINGLE);
	virtual int onRspQryQuotRight(UINT4 nSeqNo, const _fldRspMsg & rspmsg,
			CAPIVector<_fldQuotSubsRight> & lstQuotSubsRight, BYTE bChainFlag =
			CHAIN_SINGLE);
	virtual int onRspUpdQuotRight(UINT4 nSeqNo, const _fldRspMsg & rspmsg,
			const _fldQuotSubsRight & quotsubsright, BYTE bChainFlag =
			CHAIN_SINGLE);

	///the func
	virtual int onMarketDataMBLQuot(UINT4 nSeqNo, const _fldBestQuot & bestquot,
			CAPIVector<_fldOptPara> & lstOptPara,
			CAPIVector<_fldMBLQuot> & lstMBLQuot,
			BYTE bChainFlag = CHAIN_SINGLE);

	///the func
	virtual int onMarketDataArbiMBLQuot(UINT4 nSeqNo,
			const _fldArbiBestQuot & arbibestquot,
			CAPIVector<_fldMBLQuot> & lstMBLQuot,
			BYTE bChainFlag = CHAIN_SINGLE);

	virtual int onRspQuotTraderPwdUpd(UINT4 nSeqNo, const _fldRspMsg & rspmsg,
			const _fldTraderPwdUpdReq & traderpwdupdreq, BYTE bChainFlag =
			CHAIN_SINGLE);
//	virtual int onRspQueryHistoryQuot(UINT4 nSeqNo, const _fldRspMsg & rspmsg,
//			const _fldBestQuot & bestquot, CAPIVector<_fldOptPara> & lstOptPara,
//			CAPIVector<_fldMBLQuot> & lstMBLQuot,
//			BYTE bChainFlag = CHAIN_SINGLE);
	virtual int onNtyCloseMktNotice(UINT4 nSeqNo,
			const _fldMktDataNotice & mktdatanotice, BYTE bChainFlag =
			CHAIN_SINGLE);

	virtual int onInvalidPackage(UINT4 nTID, WORD nSeries, UINT4 nSequenceNo,
			WORD nFieldCount, WORD nFieldsLen, const char *pAddr);
	virtual void onChannelLost(const char *szErrMsg);

private:
	bool init_;
	volatile bool conn_ok_;
};

} /* namespace qtp_bl */

#endif /* QUOTHANDLER_H_ */
