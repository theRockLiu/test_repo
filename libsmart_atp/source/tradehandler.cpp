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
	class evt_helper: public smart_utils::event_base
	{
		public:
			evt_helper(const dce_trade_engine::pointer_t &ptr)
													: ptr_(ptr)
			{
			}
			~evt_helper()
			{
			}

		public:
			void handle_event(uint64_t val)
			{
				ptr_->handle_evt(val);
			}

		private:
			dce_trade_engine::pointer_t ptr_;
	};

	dce_trade_engine::dce_trade_engine()
											: timer_base(smart_utils::timer_base::ETT_MONOTONIC, 5, 0), conn_state_(CONN_CLOSED)
	{
		// TODO Auto-generated constructor stub

	}

	dce_trade_engine::~dce_trade_engine()
	{
		// TODO Auto-generated destructor stub
	}

//	void dce_trade_engine::check_conn()
//	{
//
//	}

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
		evt_ptr_ = std::make_shared<evt_helper>(shared_from_this());

		return 0;
	}

	evt_t* dce_trade_engine::get_evt()
	{
		return NULL;
	}

	smart_utils::notifier::pointer_t dce_trade_engine::get_event()
	{
		return std::dynamic_pointer_cast<smart_utils::notifier, smart_utils::event_base>(evt_ptr_);
	}

	int_fast8_t dce_trade_engine::async_send_cmd(cmd_t& cmd)
	{
		return 0;
	}

	void dce_trade_engine::handle_timeout(uint64_t times)
	{
		static class temp
		{
			public:
				temp(dce_trade_engine* x, bool is_logged)
				{
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

		SU_AO(conn_state_) = CONN_OPENED;
	}

	void dce_trade_engine::handle_evt(uint64_t val)
	{

	}

} /* namespace qtp_bl */
