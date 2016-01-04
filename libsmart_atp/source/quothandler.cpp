/*
 * quothandler.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#include <string.h>
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

		evt_t* evt = (evt_t*) llrb_.writer_get_bytes(1);

		evt->id_ = EVT_L1_ARBI_QUOT;
		memcpy(evt->body_.laq_.tm_, bestquot.GenTime.getValue(), bestquot.GenTime.Length());
		evt->body_.laq_.contract_id_ = hash_str(bestquot.ArbiContractID.getValue());
		evt->body_.laq_.last_price_ = bestquot.LastPrice;
		evt->body_.laq_.high_price_ = bestquot.HighPrice;
		evt->body_.laq_.low_price_ = bestquot.LowPrice;
		evt->body_.laq_.bid_price_ = bestquot.BidPrice;
		evt->body_.laq_.ask_price_ = bestquot.AskPrice;
		evt->body_.laq_.bid_qty_ = bestquot.BidQty;
		evt->body_.laq_.ask_qty_ = bestquot.AskQty;

		llrb_.writer_commit_bytes(1);

		return 0;
	}

	int dce_quot_engine::onMarketDataArbiMBLQuot(UINT4 nSeqNo, const _fldArbiBestQuot& arbibestquot, CAPIVector<_fldMBLQuot>& lstMBLQuot, BYTE bChainFlag)
	{
		SU_CHECK(CHAIN_SINGLE == bChainFlag);

		evt_t* evt = (evt_t*) llrb_.writer_get_bytes(1);

		evt->id_ = EVT_L1_ARBI_QUOT;
		memcpy(evt->body_.laq_.tm_, arbibestquot.GenTime.getValue(), arbibestquot.GenTime.Length());
		evt->body_.laq_.contract_id_ = hash_str(arbibestquot.ArbiContractID.getValue());
		evt->body_.laq_.last_price_ = arbibestquot.LastPrice;
		evt->body_.laq_.high_price_ = arbibestquot.HighPrice;
		evt->body_.laq_.low_price_ = arbibestquot.LowPrice;
		evt->body_.laq_.bid_price_ = arbibestquot.BidPrice;
		evt->body_.laq_.ask_price_ = arbibestquot.AskPrice;
		evt->body_.laq_.bid_qty_ = arbibestquot.BidQty;
		evt->body_.laq_.ask_qty_ = arbibestquot.AskQty;

		llrb_.writer_commit_bytes(1);

		return 0;
	}

	int dce_quot_engine::onRspQuotTraderPwdUpd(UINT4 nSeqNo, const _fldRspMsg& rspmsg, const _fldTraderPwdUpdReq& traderpwdupdreq, BYTE bChainFlag)
	{
		return 0;
	}

	int dce_quot_engine::onNtyCloseMktNotice(UINT4 nSeqNo, const _fldMktDataNotice& mktdatanotice, BYTE bChainFlag)
	{
		LOGGER()->info("got market close msg: %s, %s, %s", mktdatanotice.Content.getValue(), mktdatanotice.Time.getValue(), mktdatanotice.Type.getValue());
		return 0;
	}

	int dce_quot_engine::onInvalidPackage(UINT4 nTID, WORD nSeries, UINT4 nSequenceNo, WORD nFieldCount, WORD nFieldsLen, const char* pAddr)
	{
		LOGGER()->error("invalid package: %u, %u, %u, %u, %u, %s\n", nTID, nSeries, nSequenceNo, nFieldCount, nFieldsLen, pAddr);
		return 0;
	}

	void dce_quot_engine::onChannelLost(const char* szErrMsg)
	{
		LOGGER()->error("quot conn closed:%s, %s, %s\n", member_id_.c_str(), trader_no_.c_str(), szErrMsg);
		evt_ptr_->notify(CONN_CLOSED);
	}

	void dce_quot_engine::handle_timeout(uint64_t times)
	{
		static class temp
		{
			public:
				temp(dce_quot_engine* x, bool is_logged)
				{
					LOGGER()->info("api version: %s\n", x->Version());
					SU_ASSERT(0 == x->InitAPI(is_logged, NULL));
				}
		} x(this, is_logged_);

		if (0 == evt_ptr_->wait_evt())
		{
			SU_AO(conn_state_) = CONN_CLOSED;
		}

		if (CONN_OPENED == SU_AO(conn_state_))
		{
			return;
		}

		int32_t ret = InitCA(0, "", "", "", "", false);
		if (0 != ret)
		{
			LOGGER()->error("init ca failed: %d!\n", ret);
			return;
		}

		for (auto &x : fens_addrs_)
		{
			RegisterFens(x.ip.c_str(), x.port);
		}
		for (auto &x : gw_addrs_)
		{
			SetService(x.ip.c_str(), x.port);
		}

		ret = ConnectByFens();
		if (0 != ret)
		{
			ret = Connect();
			if (0 != ret)
			{
				LOGGER()->error("connect to quot sys failed: %d!\n", ret);
				return;
			}
		}

		_fldTraderSessionReq session_req;
		_fldRspMsg rsp;
		_fldTraderSessionRsp session_rsp;
		strncpy(session_req.MemberID.buf, member_id_.c_str(), session_req.MemberID.Length() - 1);
		strncpy(session_req.TraderNo.buf, trader_no_.c_str(), session_req.TraderNo.Length() - 1);
		session_req.IsShortCert = '0';
		strncpy(session_req.ProgramID.buf, program_id_.c_str(), session_req.ProgramID.Length() - 1);               //V1.0.8.0
		strncpy(session_req.ProgramVer.buf, program_ver_.c_str(), session_req.ProgramVer.Length() - 1);               //V1.0.8.0
		ret = TraderSession(&session_req, (char *) passwd_.c_str(), &rsp, &session_rsp);
		if (0 != ret)
		{
			LOGGER()->error("quot session failed: %d!\n", ret);
			return;
		}

		_fldTraderLoginReq loginreq;
		strncpy(loginreq.MemberID.buf, member_id_.c_str(), loginreq.MemberID.Length() - 1);
		strncpy(loginreq.TraderNo.buf, trader_no_.c_str(), loginreq.TraderNo.Length() - 1);
		strncpy(loginreq.Pwd.buf, passwd_.c_str(), loginreq.Pwd.Length() - 1);
		strncpy(loginreq.AppName.buf, program_id_.c_str(), loginreq.AppName.Length() - 1);   //V1.0.8.0
		strncpy(loginreq.AppVersion.buf, program_ver_.c_str(), loginreq.AppVersion.Length() - 1);   //V1.0.8.0
		ret = Login(&loginreq, &rsp, &loginrsp_);
		if (0 != ret)
		{
			LOGGER()->error("login quot sys failed: %d\n", ret);
			return;
		}

		Ready(READY, READY, false);

		SU_AO(conn_state_) = CONN_OPENED;
	}

} /* namespace qtp_bl */
