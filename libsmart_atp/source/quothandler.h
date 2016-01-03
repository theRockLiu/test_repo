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
	class dce_quot_engine: public CQuotAPI, public quot_engine, public smart_utils::timer_base
	{
		public:
			dce_quot_engine();
			virtual ~dce_quot_engine();

		public:
			///quot_engine
			int_fast8_t init(const exc_info_t &ei, const std::vector<std::string> &contracts)
			{
				return EC_SUC;
			}
			evt_t* get_evt()
			{
				return NULL;
			}

			///timer_base
			void handle_timeout(uint64_t times);

			///quot_api
			virtual int onRspQuotUserLogout(UINT4 nSeqNo, const _fldRspMsg & rspmsg, const _fldTraderLogoutRsp & traderlogoutrsp, BYTE bChainFlag =
			CHAIN_SINGLE);
			virtual int onRspQryQuotRight(UINT4 nSeqNo, const _fldRspMsg & rspmsg, CAPIVector<_fldQuotSubsRight> & lstQuotSubsRight, BYTE bChainFlag =
			CHAIN_SINGLE);
			virtual int onRspUpdQuotRight(UINT4 nSeqNo, const _fldRspMsg & rspmsg, const _fldQuotSubsRight & quotsubsright, BYTE bChainFlag =
			CHAIN_SINGLE);
			///the func
			virtual int onMarketDataMBLQuot(UINT4 nSeqNo, const _fldBestQuot & bestquot, CAPIVector<_fldOptPara> & lstOptPara, CAPIVector<_fldMBLQuot> & lstMBLQuot, BYTE bChainFlag = CHAIN_SINGLE);
			///the func
			virtual int onMarketDataArbiMBLQuot(UINT4 nSeqNo, const _fldArbiBestQuot & arbibestquot, CAPIVector<_fldMBLQuot> & lstMBLQuot, BYTE bChainFlag = CHAIN_SINGLE);
			virtual int onRspQuotTraderPwdUpd(UINT4 nSeqNo, const _fldRspMsg & rspmsg, const _fldTraderPwdUpdReq & traderpwdupdreq, BYTE bChainFlag =
			CHAIN_SINGLE);
			virtual int onNtyCloseMktNotice(UINT4 nSeqNo, const _fldMktDataNotice & mktdatanotice, BYTE bChainFlag =
			CHAIN_SINGLE);
			virtual int onInvalidPackage(UINT4 nTID, WORD nSeries, UINT4 nSequenceNo, WORD nFieldCount, WORD nFieldsLen, const char *pAddr);
			virtual void onChannelLost(const char *szErrMsg);

		private:
			bool init_;
			volatile bool conn_ok_;
	};

} /* namespace qtp_bl */

#endif /* QUOTHANDLER_H_ */
