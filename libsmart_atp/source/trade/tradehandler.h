/*
 * tradehandler.h
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#ifndef TRADEHANDLER_H_
#define TRADEHANDLER_H_

#include <unordered_map>
#include <cinttypes>
#include <utils/notifiers.h>
#include <utils/llrbex.h>
#include "../include/atp.h"
#include "../apis/dce/Linux/trade/lib/TradeAPI.h"

namespace satp
{

	enum
	{
		CONN_CLOSED = 1
	};

	class dce_trade_engine: public CTradeAPI, public trade_engine_interface, public smart_utils::timer_notifier, public std::enable_shared_from_this<dce_trade_engine>
	{
		public:
			typedef std::shared_ptr<satp::dce_trade_engine> pointer_t;

		public:
			dce_trade_engine();
			virtual ~dce_trade_engine();

		public:
			///trade engine
			int_fast8_t init(const engine_info_t &ei);
			evt_t* get_evt();
			void done();
			int_fast8_t async_send_cmd(trade_cmd_t &cmd);
			//smart_utils::notifier::pointer_t get_event();

			///timer base.
			void handle_timeout(uint64_t times);
			void handle_evt(uint64_t val);

			///callback
			int onRspTraderInsertOrders(UINT4 nSeqNo, const _fldRspMsg & rspmsg, CAPIVector<_fldOrder> & lstOrder, BYTE bChainFlag = CHAIN_SINGLE);
			int onRspTraderCancelOrder(UINT4 nSeqNo, const _fldRspMsg & rspmsg, const _fldOrderAction & orderaction, BYTE bChainFlag = CHAIN_SINGLE);
			int onNtyTraderMatch(UINT4 nSeqNo, const _fldMatch & match, BYTE bChainFlag = CHAIN_SINGLE);
			int onTraderOrdersConfirmation(UINT4 nSeqNo, const _fldOrderStatus & orderstatus, BYTE bChainFlag = CHAIN_SINGLE);
			int onNtyMktStatus(UINT4 nSeqNo, const _fldMktStatus & mktstatus, CAPIVector<_fldVarietyMktStatus> & mVarietyMktStatus, BYTE bChainFlag = CHAIN_SINGLE);
			int onInvalidPackage(UINT4 nTID, WORD nSeries, UINT4 nSequenceNo, WORD nFieldCount, WORD nFieldsLen, const char *pAddr);
			void onChannelLost(const char *szErrMsg);

		private:
			enum
			{
				CONN_CLOSED = 0, CONN_OPENING = 1, CONN_OPENED = 2
			};
			uint8_t conn_state_ __attribute__ ((aligned (64)));
			bool is_logged_;

			struct addr
			{
					string_t ip;
					uint16_t port;
			};

			std::vector<addr> fens_addrs_;
			std::vector<addr> gw_addrs_;
			string_t member_id_;
			string_t trader_id_;
			string_t program_id_;
			string_t program_ver_;
			string_t passwd_;
			_fldTraderLoginRsp loginrsp_;
			smart_utils::event_notifier conn_evt_notifier_;
			smart_utils::event_notifier init_evt_notifier_;
			smart_utils::smart_llrb llrb_;
			struct base_contract_info
			{
					CAPIVector<_fldOrder> send_req_;
					_fldOrderAction withdraw_req_;
					_fldFtrContract contract_info_;
			};
			struct arbi_contract_info
			{
					CAPIVector<_fldOrder> send_req_;
					_fldOrderAction withdraw_req_;
					_fldArbiContract arbi_contract_info_;
			};
			std::unordered_map<uint64_t, struct base_contract_info> base_contract_infos_;
			std::unordered_map<uint64_t, struct arbi_contract_info> arbi_contract_infos_;

			engine_info_t engine_info_;
	};

} /* namespace qtp_bl */

#endif /* TRADEHANDLER_H_ */
