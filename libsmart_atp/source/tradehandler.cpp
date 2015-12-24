/*
 * tradehandler.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#include "shared.h"

#include "tradehandler.h"

namespace satp
{

	dce_trade_engine::dce_trade_engine()
											: timer_base(smart_utils::timer_base::ETT_MONOTONIC, 5, 0)
	{
		// TODO Auto-generated constructor stub

	}

	dce_trade_engine::~dce_trade_engine()
	{
		// TODO Auto-generated destructor stub
	}

	void dce_trade_engine::check_conn()
	{
//		if (conn_ok_)
//		{
//			return;
//		}
//
//		if (!init_)
//		{
//			SU_CHECK(SU_EC_SUC == InitAPI());
//			SU_CHECK(SU_EC_SUC == InitCA(0, "", "", "", "", false));
//			///
//			LOGGER()->info("quot fens ip list size is : %d\n", lb_gws.size());
//			for (auto x = lb_gws.begin(); x != lb_gws.end(); ++x)
//			{
//				LOGGER()->info("quot fens ip is : %s:%d\n", (*x).ip_.c_str(), (*x).port_);
//				RegisterFens((*x).ip_.c_str(), (*x).port_);
//			}
//
//			LOGGER()->info("quot gw ip list size is : %d\n", gws.size());
//			for (auto x = gws.begin(); x != gws.end(); ++x)
//			{
//				LOGGER()->info("quot gw ip is : %s:%d\n", (*x).ip_.c_str(), (*x).port_);
//				SetService((*x).ip_.c_str(), (*x).port_);
//			}
//
//			init_ = true;
//		}
//
//		///
//		int_fast32_t ret = ConnectByFens();
//		if (SU_EC_SUC != ret)
//		{
//			LOGGER()->error("connect to trade system with load balancer failed, return err code %d\n", ret);
//			ret = Connect();
//			if (SU_EC_SUC != ret)
//			{
//				LOGGER()->error("connect to trade system directly failed, return err code %d\n", ret);
//				return;
//			}
//		}
//		LOGGER()->info("connect to trade system successfully!\n");
//
//		///
//		_fldTraderSessionReq
//		req =
//		{	MemberID : (char*) member_id.c_str(),
//			TraderNo : (char*) trader_no.c_str(),
//			IsShortCert : '0',
//			//			ProgramID : {(char*) prog_id.c_str()},
//			//			ProgramVer : {(char*) prog_ver.c_str()}
//		};
//
//		_fldRspMsg rspmsg = { 0 };
//		_fldTraderSessionRsp rsp = { 0 };
//		ret = TraderSession(&req, (char*) passwd.c_str(), &rspmsg, &rsp);
//		if (0 != ret)
//		{
//			LOGGER()->error("create session failed, return %d\n", ret);
//			return;
//		}
//
//		_fldTraderLoginReq
//		loginreq =
//		{	MemberID : (char*)member_id.c_str(),
//			TraderNo : (char*)trader_no.c_str(),
//			Pwd : (char*)passwd.c_str(),
//			AppName : (char*)prog_id.c_str(),
//			AppVersion : (char*)prog_ver.c_str()
//		};
//		_fldTraderLoginRsp loginrsp = { 0 };
//		ret = Login(&loginreq, &rspmsg, &loginrsp);
//		if (0 != ret)
//		{
//			LOGGER()->error("login failed, return %d\n", ret);
//			return;
//		}
//
//		LOGGER()->info("login successfully, %s : %s\n", loginrsp.Date.getValue(), loginrsp.Time.getValue());
//
//		long nMaxLocalID = 0;
//		nMaxLocalID = loginrsp.LatestOrderNo + 1;
//		if (nMaxLocalID > m_nOrderLocalID)
//		{
//#if DEBUG_LOG_ENABLE
//			char tmplog[64] =
//			{	0};
//			sprintf(tmplog, "%ld", nMaxLocalID);
//			DEBUG_LOG(tmplog);
//#endif
//			m_nOrderLocalID = nMaxLocalID;
//			g_xw_info.SetMaxSBJLH(m_nOrderLocalID);
//		}
//		GetMaxLocalOrderID();
//
//		WriteBackLastSeqNo(); //V1.0.3.7//GK 20140108 ??Ȩ?ڻ??ϲ?
//		if (m_bUpdateTimeToJYS)
//		{
//			SetLocalTimeToJYS();
//		}
//		UpdateTimeDiff();          //??¼ʱ???
//		DFComm::AxSvrSetDispVal(INFO_GRP, GRPIM_TXZT, "????");
//		//sNote.Format("???뻹??%d?????",loginrsp.ExpireDays)
//		sNote.Format("?ɹ???¼?????ڻ?????ƽ̨");
//		NOTIFY_LOG_KEY_EVN(sNote, MACRO_JYLINK, MACRO_JYLINK);

	}

	int dce_trade_engine::onRspTraderInsertOrders(UINT4 nSeqNo, const _fldRspMsg& rspmsg, CAPIVector<_fldOrder>& lstOrder, BYTE bChainFlag)
	{
		SU_CHECK(CHAIN_SINGLE == bChainFlag);

		//evt_t *data = rb_.get_next_buf();

		return 0;
	}

	int dce_trade_engine::onRspTraderCancelOrder(UINT4 nSeqNo, const _fldRspMsg& rspmsg, const _fldOrderAction& orderaction, BYTE bChainFlag)
	{
		return 0;
	}

	int dce_trade_engine::onNtyTraderMatch(UINT4 nSeqNo, const _fldMatch& match, BYTE bChainFlag)
	{
		return 0;
	}

	int_fast8_t dce_trade_engine::init(exc_info_t& ei)
	{
	}

	evt_t* dce_trade_engine::get_evt()
	{
	}

	int_fast8_t dce_trade_engine::async_send_cmd(cmd_t& cmd)
	{
	}

	void dce_trade_engine::handle_timeout(uint64_t times)
	{
		static class temp
		{
			public:
				temp(bool is_logged)
				{
					SU_ASSERT(0 == InitAPI(is_logged, NULL));
				}
		} x(is_logged_);

		if (CONN_OPENED == SUAO(conn_state_))
		{
			return;
		}

		int32_t ret = InitCA(0, "", "", "", "", false);
		if (0 != ret)
		{
			LOGGER()->error("init ca failed: %d!\n", ret);
			return;
		}

		for (auto x : fens_addrs_)
		{
			RegisterFens(x.ip.c_str(), x.port);
		}
		for (auto x : gw_addrs_)
		{
			SetService(x.ip.c_str(), x.port);
		}

		ret = ConnectByFens();
		if (0 != ret)
		{
			ret = Connect();
			if (0 != ret)
			{
				LOGGER()->error("connect to trade sys failed: %d!\n", ret);
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
			LOGGER()->error("trader session failed: %d!\n", ret);
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
			LOGGER()->error("login trade sys failed: %d\n", ret);
			return;
		}

		conn_state_ = CONN_OPENED;
	}

} /* namespace qtp_bl */
