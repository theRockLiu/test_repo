/*
 * tradehandler.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#include "../apis/dce/Linux/trade/PublicAPI/Constant.h"
#include "shared.h"
#include "tradehandler.h"

namespace satp
{
	dce_trade_engine::dce_trade_engine()
											: timer_notifier(smart_utils::timer_notifier::ETT_MONOTONIC, 5, 0), conn_state_(CONN_CLOSED), is_logged_(false)
	{
		// TODO Auto-generated constructor stub
	}

	dce_trade_engine::~dce_trade_engine()
	{
		// TODO Auto-generated destructor stub
		conn_evt_notifier_.close();
		init_evt_notifier_.close();
		llrb_.destroy();
	}

	int_fast8_t dce_trade_engine::init(const engine_info_t &ei)
	{
		engine_info_ = ei;
		conn_evt_notifier_.open(true);
		init_evt_notifier_.open(true);

//		for (auto &x : contracts)
//		{
//			struct base_contract_info s;
//			strncpy(s.send_req_[0].ContractID.buf, x.c_str(), s.send_req_[0].ContractID.Length());
//			strncpy(s.withdraw_req_.ContractID.buf, x.c_str(), s.withdraw_req_.ContractID.Length());
//			s.contract_id_ = x;
//			base_contract_infos_[hash_str(x.c_str())] = s;
//		}

		return llrb_.init(LLRB_SIZE, ELEM_SIZE);
	}

//	int_fast8_t dce_trade_engine::destroy()
//	{
//		SHARED()->rem_quot_engine(std::dynamic_pointer_cast<satp::quot_engine_interface&>(shared_from_this()));
//
//		return RET_SUC;
//	}

	evt_t* dce_trade_engine::get_evt()
	{
		return (evt_t*) llrb_.reader_get_data(1);
	}

	void dce_trade_engine::done()
	{
		llrb_.reader_commit_data(1);
	}

	int_fast8_t dce_trade_engine::async_send_cmd(trade_cmd_t& cmd)
	{
		switch (cmd.id_)
		{
			case EVT_SEND_BASE_ORDER_REQ:
			{
				struct base_contract_info &x = base_contract_infos_[cmd.body_.sor_.contract_id_];
				x.send_req_[0].Price = cmd.body_.sor_.price_;
				uint32_t seqno = 0;
				return ReqTraderInsertOrders(&seqno, x.send_req_);
			}
			case EVT_SEND_ARBI_ORDER_REQ:
			{
				struct arbi_contract_info &x = arbi_contract_infos_[cmd.body_.sor_.contract_id_];
				x.send_req_[0].Price = cmd.body_.sor_.price_;
				uint32_t seqno = 0;
				return ReqTraderInsertOrders(&seqno, x.send_req_);
			}
			case EVT_WITHDRAW_BASE_ORDER_REQ:
			{
				struct base_contract_info &x = base_contract_infos_[cmd.body_.wor_.cid_];
				x.withdraw_req_.SysOrderNo = cmd.body_.wor_.sys_no_;
				uint32_t seqno = 0;
				return ReqTraderCancelOrder(&seqno, x.withdraw_req_);
			}
			case EVT_WITHDRAW_ARBI_ORDER_REQ:
			{
				struct arbi_contract_info &x = arbi_contract_infos_[cmd.body_.wor_.cid_];
				x.withdraw_req_.SysOrderNo = cmd.body_.wor_.sys_no_;
				uint32_t seqno = 0;
				return ReqTraderCancelOrder(&seqno, x.withdraw_req_);
			}
			default:
			{
				SU_ASSERT(false);
				break;
			}
		}

		return 0;
	}

	void dce_trade_engine::handle_timeout(uint64_t times)
	{
		if (0 != init_evt_notifier_.wait_evt())
		{
			return;
		}

		static class temp
		{
			public:
				temp(dce_trade_engine* x, bool is_logged)
				{
					LOGGER()->info("api version: %s\n", x->Version());
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
				LOGGER()->error("connect to trade sys failed: %d!\n", ret);
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
			LOGGER()->error("trader session failed: %d!\n", ret);
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
			LOGGER()->error("login trade sys failed: %d\n", ret);
			return;
		}

#if 0
		CAPIVector<_fldFtrContract> *base_contract_info = nullptr;
		for (auto &x : base_contract_infos_)
		{
			base_contract_info = nullptr;
			_fldContractQryReq req =
			{	0};
			strncpy(req.ContractID.buf, x.second.contract_info_.ContractID.getValue(), req.ContractID.Length());
			//ZERO_BYTES(&(x.second.contract_info_), sizeof(_fldFtrContract));
			//ZERO_BYTES(&(x.second.arbi_contract_info_), sizeof(_fldArbiContract));
			ret = QryFtrContr(req, &rsp, &base_contract_info);
			if (0 != ret)
			{
				LOGGER()->error("query base contract info failed : %s, %d, %s\n", req.ContractID.getValue(), rsp.ErrCode, rsp.RspMsg.getValue());
				LOGGER()->flush();
				return;
			}

			SU_ASSERT(base_contract_info != nullptr);
			SU_ASSERT(base_contract_info->GetCount() == 1);
			x.second.contract_info_ = (*base_contract_info)[0];

		}

		CAPIVector<_fldArbiContract> *arbi_contract_info = nullptr;
		CAPIVector<_fldArbiLeg> *arbi_contract_leg = nullptr;
		for (auto &x : arbi_contract_infos_)
		{
			arbi_contract_info = nullptr;
			_fldContractQryReq req =
			{	0};
			strncpy(req.ContractID.buf, x.second.arbi_contract_info_.ArbiContractID.getValue(), req.ContractID.Length());
			ret = QryArbiContr(req, &rsp, &arbi_contract_info, &arbi_contract_leg);
			if (0 != ret)
			{
				LOGGER()->error("query arbi contract info failed : %s, %d, %s\n", req.ContractID.getValue(), rsp.ErrCode, rsp.RspMsg.getValue());
				LOGGER()->flush();
				return;
			}

			SU_ASSERT(arbi_contract_info != nullptr);
			SU_ASSERT(arbi_contract_info->GetCount() == 1);
			x.second.arbi_contract_info_ = (*arbi_contract_info)[0];
		}
#endif

		Ready(READY, READY, true);

		SU_AO(conn_state_) = CONN_OPENED;
	}

	int dce_trade_engine::onRspTraderInsertOrders(UINT4 nSeqNo, const _fldRspMsg& rspmsg, CAPIVector<
											_fldOrder>& lstOrder, BYTE bChainFlag)
	{
		SU_CHECK(CHAIN_SINGLE == bChainFlag);

		evt_t* evt = (evt_t*) llrb_.writer_get_bytes(1);

		evt->id_ = EVT_SEND_ORDER_RSP;
		evt->body_.sor_.err_code_ = rspmsg.ErrCode;
		evt->body_.sor_.cid_ = hash_str(lstOrder[0].ContractID.getValue());
		evt->body_.sor_.local_no_ = lstOrder[0].LocalOrderNo;
		evt->body_.sor_.sys_no_ = lstOrder[0].SysOrderNo;

		llrb_.writer_commit_bytes(1);

		return 0;
	}

	int dce_trade_engine::onRspTraderCancelOrder(UINT4 nSeqNo, const _fldRspMsg& rspmsg, const _fldOrderAction& orderaction, BYTE bChainFlag)
	{
		evt_t* evt = (evt_t*) llrb_.writer_get_bytes(1);

		evt->id_ = EVT_SEND_ORDER_RSP;
		evt->body_.wor_.err_code_ = rspmsg.ErrCode;
		evt->body_.wor_.cid_ = hash_str(const_cast<_fldOrderAction&>(orderaction).ContractID.getValue());
		evt->body_.wor_.local_no_ = orderaction.LocalOrderNo;
		evt->body_.wor_.sys_no_ = orderaction.SysOrderNo;

		llrb_.writer_commit_bytes(1);

		return 0;
	}

	int dce_trade_engine::onNtyTraderMatch(UINT4 nSeqNo, const _fldMatch& match, BYTE bChainFlag)
	{
		evt_t* evt = (evt_t*) llrb_.writer_get_bytes(1);

		evt->id_ = EVT_MATCH_RSP;
		evt->body_.omr_.cid_ = hash_str(const_cast<_fldMatch&>(match).ContractID.getValue());
		evt->body_.omr_.local_no_ = match.LocalID;
		evt->body_.omr_.sys_no_ = match.SysOrderNo;

		llrb_.writer_commit_bytes(1);

		return 0;
	}

	int dce_trade_engine::onTraderOrdersConfirmation(UINT4 nSeqNo, const _fldOrderStatus& orderstatus, BYTE bChainFlag)
	{
		SU_ASSERT(CHAIN_SINGLE == bChainFlag);

		evt_t* evt = (evt_t*) llrb_.writer_get_bytes(1);

		evt->id_ = EVT_ORDER_STATUS;
		evt->body_.osr_.sys_no_ = orderstatus.SysOrderNo;
		evt->body_.osr_.local_no_ = orderstatus.LocalOrderNo;
		evt->body_.osr_.client_id_ = hash_str(const_cast<_fldOrderStatus&>(orderstatus).ClientID.getValue());
		evt->body_.osr_.contract_id_ = hash_str(const_cast<_fldOrderStatus&>(orderstatus).ContractID.getValue());

		llrb_.writer_commit_bytes(1);

		return 0;
	}

	int dce_trade_engine::onNtyMktStatus(UINT4 nSeqNo, const _fldMktStatus& mktstatus, CAPIVector<
											_fldVarietyMktStatus>& mVarietyMktStatus, BYTE bChainFlag)
	{
		SU_ASSERT(CHAIN_SINGLE == bChainFlag);
		uint8_t status = 0;
		switch (atoi(const_cast<_fldMktStatus&>(mktstatus).MktStatus.getValue()))
		{
			case 11: //11 EC_INIT
			{
				status = MS_TRADE_INIT;
				break;
			}
			case 12:  //12 EC_AUCTION_ORDER
			{
				status = MS_TRADE_AUCTION_SEND_ORDERS;
				break;
			}
			case 13:  //13 EC_AUCTION_PAUSE
			{
				status = MS_TRADE_AUCTION_PAUSE;
				break;
			}
			case 14:   //14 EC_AUCTION_MATCH
			{
				status = MS_TRADE_AUCTION_MATCH;
				break;
			}
			case 15:   //15 EC_TRADE
			{
				status = MS_TRADE_CONTINUE;
				break;
			}
			case 16:   //16 EC_TRADE_PAUSE
			{
				status = MS_TRADE_PAUSE;
				break;
			}
			case 50: //50 EC_CLOSE            "50"	          //????
			{
				status = MS_TRADE_CLOSED;
				break;
			}
			default:
			{
				SU_ASSERT(false);
				break;
			}
		}

		evt_t* evt = NULL;
		for (int i = 0; i < mVarietyMktStatus.GetCount();
												i++)
		{
			if (0 == i % MAX_MARKET_STATUS_CNT)
			{
				evt = (evt_t*) llrb_.writer_get_bytes(1);
				evt->id_ = EVT_MARKET_STATUS;
				evt->body_.msr_.market_status_ = status;
				evt->body_.msr_.cnt_ = 0;
			}
			evt->body_.msr_.cnt_++;
			switch (atoi(mVarietyMktStatus.Get(i).Status.getValue()))
			{
				case 11:
				{
					evt->body_.msr_.vsr_[evt->body_.msr_.cnt_ - 1].status_ = MS_TRADE_INIT;
					break;
				}
				case 12:  //12 EC_AUCTION_ORDER ???۱???
				{
					evt->body_.msr_.vsr_[evt->body_.msr_.cnt_ - 1].status_ = MS_TRADE_AUCTION_SEND_ORDERS;
					break;
				}
				case 13:  //13 EC_AUCTION_PAUSE ??????ͣ
				{
					evt->body_.msr_.vsr_[evt->body_.msr_.cnt_ - 1].status_ = MS_TRADE_AUCTION_PAUSE;
					break;
				}
				case 14:   //14 EC_AUCTION_MATCH ???۴??
				{
					evt->body_.msr_.vsr_[evt->body_.msr_.cnt_ - 1].status_ = MS_TRADE_AUCTION_MATCH;
					break;
				}
				case 15:   //15 EC_TRADE         ????????
				{
					evt->body_.msr_.vsr_[evt->body_.msr_.cnt_ - 1].status_ = MS_TRADE_CONTINUE;
					break;
				}
				case 16:   //16 EC_TRADE_PAUSE   ??????ͣ
				{
					evt->body_.msr_.vsr_[evt->body_.msr_.cnt_ - 1].status_ = MS_TRADE_PAUSE;
					break;
				}
				case 50:   //50 EC_CLOSE         ???̱???
				{
					evt->body_.msr_.vsr_[evt->body_.msr_.cnt_ - 1].status_ = MS_TRADE_CLOSED;
					break;
				}
				default:
				{
					SU_ASSERT(false);
					break;
				}
			}
			evt->body_.msr_.vsr_[evt->body_.msr_.cnt_ - 1].trade_type_ = (
			TT_OPT == mVarietyMktStatus.Get(i).TradeType ? TT_OPTION : TT_FUTURE);
			evt->body_.msr_.vsr_[evt->body_.msr_.cnt_ - 1].var_ = hash_str(mVarietyMktStatus.Get(i).VarietyId.getValue());
			if (MAX_MARKET_STATUS_CNT == evt->body_.msr_.cnt_)
			{
				llrb_.writer_commit_bytes(1);
			}
		}
		if (mVarietyMktStatus.GetCount() % MAX_MARKET_STATUS_CNT != 0)
		{
			llrb_.writer_commit_bytes(1);
		}

		return 0;
	}

	int dce_trade_engine::onInvalidPackage(UINT4 nTID, WORD nSeries, UINT4 nSequenceNo, WORD nFieldCount, WORD nFieldsLen, const char* pAddr)
	{
		LOGGER()->error("invalid package: %u, %u, %u, %u, %u, %s\n", nTID, nSeries, nSequenceNo, nFieldCount, nFieldsLen, pAddr);
		LOGGER()->flush();
		SU_ASSERT(false);
		return 0;
	}

	void dce_trade_engine::onChannelLost(const char* szErrMsg)
	{
		LOGGER()->error("trade conn closed:%s, %s, %s\n", member_id_.c_str(), trader_id_.c_str(), szErrMsg);
		conn_evt_notifier_.notify(CONN_CLOSED);
	}

} /* namespace qtp_bl */
