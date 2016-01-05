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
#include <utils/notifiers.h>
#include <utils/llrbex.h>
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

			///quot api
			int onInvalidPackage(UINT4 nTID, WORD nSeries, UINT4 nSequenceNo, WORD nFieldCount, WORD nFieldsLen, const char *pAddr);
			int onRspQuotUserLogout(UINT4 nSeqNo, const _fldRspMsg & rspmsg, const _fldTraderLogoutRsp & traderlogoutrsp, BYTE bChainFlag = CHAIN_SINGLE);
			int onMarketDataMBLQuot(UINT4 nSeqNo, const _fldBestQuot & bestquot, CAPIVector<_fldOptPara> & lstOptPara, CAPIVector<_fldMBLQuot> & lstMBLQuot, BYTE bChainFlag = CHAIN_SINGLE);
			int onMarketDataArbiMBLQuot(UINT4 nSeqNo, const _fldArbiBestQuot & arbibestquot, CAPIVector<_fldMBLQuot> & lstMBLQuot, BYTE bChainFlag = CHAIN_SINGLE);
			int onNtyCloseMktNotice(UINT4 nSeqNo, const _fldMktDataNotice & mktdatanotice, BYTE bChainFlag = CHAIN_SINGLE);
			void onChannelLost(const char *szErrMsg);

		private:
			bool is_logged_;
			enum
			{
				CONN_CLOSED = 0, CONN_OPENING = 1, CONN_OPENED = 2
			};
			uint8_t conn_state_ __attribute__ ((aligned (64)));

			struct addr
			{
					string_t ip;
					uint16_t port;
			};

			std::vector<addr> fens_addrs_;
			std::vector<addr> gw_addrs_;
			string_t member_id_;
			string_t trader_no_;
			string_t program_id_;
			string_t program_ver_;
			string_t passwd_;
			_fldTraderLoginRsp loginrsp_;
			smart_utils::event_base::pointer_t evt_ptr_;
			smart_utils::smart_llrb llrb_;
	};

} /* namespace qtp_bl */

#endif /* QUOTHANDLER_H_ */
