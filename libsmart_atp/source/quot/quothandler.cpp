/*
 * quothandler.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#include <string.h>
#include "../shared/shared.h"
#include "quothandler.h"

namespace satp
{
	dce_l1_quot_engine::dce_l1_quot_engine()
											: timer_notifier(smart_utils::timer_notifier::ETT_MONOTONIC, 5, 0), conn_state_(CONN_CLOSED), is_logged_(false)
	{
		// TODO Auto-generated constructor stub
	}

	dce_l1_quot_engine::~dce_l1_quot_engine()
	{
		// TODO Auto-generated destructor stub
		conn_evt_notifier_.close();
		init_evt_notifier_.close();
		llrb_.destroy();
	}

	int_fast8_t dce_l1_quot_engine::init(const engine_info_t &ei)
	{
		engine_info_ = ei;
		conn_evt_notifier_.open(true);
		init_evt_notifier_.open(true);
		this->open(true);

		quot_engine_interface::pointer_t x = std::dynamic_pointer_cast<satp::quot_engine_interface, satp::dce_l1_quot_engine>(shared_from_this());
		SU_ASSERT(RET_SUC == SHARED().add_quot_engine(x));

		SU_ASSERT(RET_SUC == llrb_.init(LLRB_SIZE, ELEM_SIZE));

		fens_addrs_.push_back({ei.server_ip_, ei.server_port_});
		init_evt_notifier_.notify(1);

		return RET_SUC;
	}

//	int_fast8_t dce_l1_quot_engine::destroy()
//	{
//		SHARED()->rem_quot_engine(std::dynamic_pointer_cast<satp::quot_engine_interface&>(shared_from_this()));
////		conn_evt_notifier_.close();
////		init_evt_notifier_.close();
//
//		return RET_SUC;
//	}

	evt_t* dce_l1_quot_engine::get_evt()
	{
		return (evt_t*) llrb_.reader_get_data(1);
	}

	void dce_l1_quot_engine::done()
	{
		llrb_.reader_commit_data(1);
	}

	int dce_l1_quot_engine::onRspQuotUserLogout(UINT4 nSeqNo, const _fldRspMsg& rspmsg, const _fldTraderLogoutRsp& traderlogoutrsp, BYTE bChainFlag)
	{
		LOGGER()->info("Got quot user logout rsp: %d, %s, %s\n", const_cast<_fldRspMsg&>(rspmsg).ErrCode, const_cast<_fldRspMsg&>(rspmsg).RspMsg.getValue(), const_cast<_fldRspMsg&>(rspmsg).TimeStamp.getValue());
		return 0;
	}

	int dce_l1_quot_engine::onMarketDataMBLQuot(UINT4 nSeqNo, const _fldBestQuot& bestquot, CAPIVector<_fldOptPara>& lstOptPara, CAPIVector<_fldMBLQuot>& lstMBLQuot, BYTE bChainFlag)
	{
		SU_CHECK(CHAIN_SINGLE == bChainFlag);

		evt_t* evt = (evt_t*) llrb_.writer_get_bytes(1);

		evt->id_ = EVT_L1_ARBI_QUOT;
		memcpy(evt->body_.laq_.tm_, const_cast<_fldBestQuot&>(bestquot).GenTime.getValue(), const_cast<_fldBestQuot&>(bestquot).GenTime.Length());
		evt->body_.laq_.contract_id_ = hash_str(const_cast<_fldBestQuot&>(bestquot).ContractID.getValue());
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

	int dce_l1_quot_engine::onMarketDataArbiMBLQuot(UINT4 nSeqNo, const _fldArbiBestQuot& arbibestquot, CAPIVector<_fldMBLQuot>& lstMBLQuot, BYTE bChainFlag)
	{
		SU_CHECK(CHAIN_SINGLE == bChainFlag);

		evt_t* evt = (evt_t*) llrb_.writer_get_bytes(1);

		evt->id_ = EVT_L1_ARBI_QUOT;
		memcpy(evt->body_.laq_.tm_, const_cast<_fldArbiBestQuot&>(arbibestquot).GenTime.getValue(), const_cast<_fldArbiBestQuot&>(arbibestquot).GenTime.Length());
		evt->body_.laq_.contract_id_ = hash_str(const_cast<_fldArbiBestQuot&>(arbibestquot).ArbiContractID.getValue());
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

	int dce_l1_quot_engine::onNtyCloseMktNotice(UINT4 nSeqNo, const _fldMktDataNotice& mktdatanotice, BYTE bChainFlag)
	{
		LOGGER()->info("got market close msg: %s, %s, %s", const_cast<_fldMktDataNotice&>(mktdatanotice).Content.getValue(), const_cast<_fldMktDataNotice&>(mktdatanotice).Time.getValue(), const_cast<_fldMktDataNotice&>(mktdatanotice).Type.getValue());
		return 0;
	}

	int dce_l1_quot_engine::onInvalidPackage(UINT4 nTID, WORD nSeries, UINT4 nSequenceNo, WORD nFieldCount, WORD nFieldsLen, const char* pAddr)
	{
		LOGGER()->error("invalid package: %u, %u, %u, %u, %u, %s\n", nTID, nSeries, nSequenceNo, nFieldCount, nFieldsLen, pAddr);
		LOGGER()->flush();
		SU_ASSERT(false);
		return 0;
	}

	void dce_l1_quot_engine::onChannelLost(const char* szErrMsg)
	{
		LOGGER()->error("quot conn closed:%s, %s, %s\n", member_id_.c_str(), trader_id_.c_str(), szErrMsg);
		conn_evt_notifier_.notify(CONN_CLOSED);
	}

	void dce_l1_quot_engine::handle_timeout(uint64_t times)
	{
//		if (0 != init_evt_notifier_.wait_evt())
//		{
//			return;
//		}

		static class temp
		{
			public:
				temp(dce_l1_quot_engine* x, bool is_logged)
				{
					LOGGER()->info("api version: {:s}\n", x->Version());
					SU_ASSERT(0 == x->InitAPI(is_logged, NULL));
				}
		} x(this, is_logged_);

		if (0 == conn_evt_notifier_.wait_evt())
		{
			SU_AO(conn_state_) = CONN_CLOSED;
		}

		if (CONN_OPENED == SU_AO(conn_state_))
		{
			return;
		}

		LOGGER()->info("try to connect to quot system\n");

		int32_t ret = InitCA(0, "", "", "", "", false);
		if (0 != ret)
		{
			LOGGER()->error("init ca failed: {:d}!\n", ret);
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
				LOGGER()->error("connect to quot sys failed: {:d}!\n", ret);
				return;
			}
		}

		_fldTraderSessionReq session_req;
		_fldRspMsg rsp;
		_fldTraderSessionRsp session_rsp;
		strncpy(session_req.MemberID.buf, member_id_.c_str(), session_req.MemberID.Length() - 1);
		strncpy(session_req.TraderNo.buf, trader_id_.c_str(), session_req.TraderNo.Length() - 1);
		session_req.IsShortCert = '0';
		strncpy(session_req.ProgramID.buf, program_id_.c_str(), session_req.ProgramID.Length() - 1); //V1.0.8.0
		strncpy(session_req.ProgramVer.buf, program_ver_.c_str(), session_req.ProgramVer.Length() - 1); //V1.0.8.0
		ret = TraderSession(&session_req, (char *) passwd_.c_str(), &rsp, &session_rsp);
		if (0 != ret)
		{
			LOGGER()->error("quot session failed: %d!\n", ret);
			return;
		}

		_fldTraderLoginReq loginreq;
		strncpy(loginreq.MemberID.buf, member_id_.c_str(), loginreq.MemberID.Length() - 1);
		strncpy(loginreq.TraderNo.buf, trader_id_.c_str(), loginreq.TraderNo.Length() - 1);
		strncpy(loginreq.Pwd.buf, passwd_.c_str(), loginreq.Pwd.Length() - 1);
		strncpy(loginreq.AppName.buf, program_id_.c_str(), loginreq.AppName.Length() - 1); //V1.0.8.0
		strncpy(loginreq.AppVersion.buf, program_ver_.c_str(), loginreq.AppVersion.Length() - 1); //V1.0.8.0
		ret = Login(&loginreq, &rsp, &loginrsp_);
		if (0 != ret)
		{
			LOGGER()->error("login quot sys failed: %d\n", ret);
			return;
		}

		Ready(READY, READY, false);

		SU_AO(conn_state_) = CONN_OPENED;
	}

//	void dce_l1_quot_engine::on_removed(bool suc)
//	{
//		SU_ASSERT(suc);
//		conn_evt_notifier_.close();
//		init_evt_notifier_.close();
//		uint32_t sqno = 0;
//		_fldTraderLogoutReq req = { trader_id_.c_str(), member_id_.c_str() };
//		ReqQuotUserLogout(&sqno, req);
//	}

} /* namespace qtp_bl */
