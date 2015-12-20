/*
 * quothandler.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#include "shared.h"

#include "quothandler.h"

namespace satp
{

	dce_quot_engine::dce_quot_engine()
											: init_(false), conn_ok_(false)
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

//		struct quot_info qi; // = {.cid = utils::hash_str(bestquot.ContractID.GetBuf())};
//		SHARED().handle_quot(qi);

		return 0;
	}

	int dce_quot_engine::onMarketDataArbiMBLQuot(UINT4 nSeqNo, const _fldArbiBestQuot& arbibestquot, CAPIVector<_fldMBLQuot>& lstMBLQuot, BYTE bChainFlag)
	{
		return 0;
	}

	int dce_quot_engine::onRspQuotTraderPwdUpd(UINT4 nSeqNo, const _fldRspMsg& rspmsg, const _fldTraderPwdUpdReq& traderpwdupdreq, BYTE bChainFlag)
	{
		return 0;
	}

	int dce_quot_engine::onNtyCloseMktNotice(UINT4 nSeqNo, const _fldMktDataNotice& mktdatanotice, BYTE bChainFlag)
	{
		return 0;
	}

	int dce_quot_engine::onInvalidPackage(UINT4 nTID, WORD nSeries, UINT4 nSequenceNo, WORD nFieldCount, WORD nFieldsLen, const char* pAddr)
	{
		return 0;
	}

///lb_gws load balancer gws.
	void dce_quot_engine::check_conn(const std::vector<addr_info_t> &lb_gws, const std::vector<addr_info_t> &gws, const string_t &member_id, const string_t &trader_no, const string_t &passwd, const string_t &prog_id, const string_t &prog_ver)
	{
//		if (conn_ok_)
//		{
//			return;
//		}
//
//		if (!init_)
//		{
//			SU_CHECK(SU_EC_SUC == InitAPI(true, NULL));
//			SU_CHECK(SU_EC_SUC == SetSyncTimeOut(20));
//			SU_CHECK(SU_EC_SUC == InitCA(0, "", "", "", "", false));
//			init_ = true;
//		}
//
//		///
//		LOGGER()->info("quot fens ip list size is : %d\n", lb_gws.size());
//		for (auto x = lb_gws.begin(); x != lb_gws.end(); ++x)
//		{
//			LOGGER()->info("quot fens ip is : %s:%d\n", (*x).ip_.c_str(), (*x).port_);
//			RegisterFens((*x).ip_.c_str(), (*x).port_);
//		}
//
//		LOGGER()->info("quot gw ip list size is : %d\n", gws.size());
//		for (auto x = gws.begin(); x != gws.end(); ++x)
//		{
//			LOGGER()->info("quot gw ip is : %s:%d\n", (*x).ip_.c_str(), (*x).port_);
//			SetService((*x).ip_.c_str(), (*x).port_);
//
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
//			DEBUG_LOG("CQuoteSpi::Init 4");
//			if (!m_sIP.empty() && m_nPort > 0) //V1.0.3.0
//			{
//				DEBUG_LOG("CQuoteSpi::Init 4.1");
//				///******GK 20140107 ??Ȩ?ڻ??ϲ? End****************/
//				SetService(m_sIP.c_str(), m_nPort);  //?????ö??ǰ?û???ַ
//				for (size_t i = 0; i < m_arHostIP.size(); i++)
//				{
//					SetService(m_arHostIP[i].c_str(), m_arHostPort[i]);
//					sNote.Format("?????ǰ?õ?ַ:%s,%li", m_arHostIP[i].c_str(), m_arHostPort[i]);
//					NOTIFY_LOG(sNote.c_str());
//					///*****GK 20140107 ??Ȩ?ڻ??ϲ? Begin***************
//				}
//				///******GK 20140107 ??Ȩ?ڻ??ϲ? End****************/
//			}
//			DEBUG_LOG("CQuoteSpi::Init 5");
//			//????
//			///*****GK 20140107 ??Ȩ?ڻ??ϲ? Begin***************
//			if (m_bUseFENS) //V1.0.3.0
//			{
//				int nRet;
//				nRet = ConnectByFens();
//				if (nRet != 0)
//				{
//					sNote.Format("??FENS????????ǰ??ʧ??! ??????:%d, ????ֱ??????ǰ??...", nRet);
//					WARN_LOG(sNote.c_str());
//					nRet = Connect();
//					if (nRet != 0)
//					{
//						sNote.Format("????????????ǰ??ʧ??! ??????:%d", nRet);
//						WARN_LOG(sNote.c_str());
//						return false;
//					}
//				}
//				DEBUG_LOG("CQuoteSpi::Init 6");
//			}
//			else         //?ɵ?ֱ????ʽ
//			{
//				///******GK 20140107 ??Ȩ?ڻ??ϲ? End****************/
//				if (Connect() < 0)
//				{
//					WARN_LOG("?뽻????????????ʧ??!");
//					return false;
//				}         //GK 20140107 ??Ȩ?ڻ??ϲ?
//				DEBUG_LOG("CQuoteSpi::Init 7");
//			}
//
//			//?Ự??֤????
//			_fldTraderSessionReq pSessionReq;
//			_fldRspMsg pRspMsg;
//			_fldTraderSessionRsp pSessionRsp;
//			strcpy(pSessionReq.MemberID.buf, m_sParticipantId.c_str());
//			strcpy(pSessionReq.TraderNo.buf, m_sUserId.c_str());
//			SafeCopy(pSessionReq.ProgramID.buf, g_strTXModName.c_str(), pSessionReq.ProgramID.Length());   //V1.0.8.0
//			SafeCopy(pSessionReq.ProgramVer.buf, g_strTXModVer.c_str(), pSessionReq.ProgramVer.Length());   //V1.0.8.0
//			pSessionReq.IsShortCert = m_cIsShortCert;               //0:?????п????֤,1?????֤
//			if (m_cIsShortCert == '1')
//			{
//				DEBUG_LOG("CQuoteSpi::Init 7.1");
//				strcpy(pSessionReq.ShortCode, m_sShortCode.c_str());
//			}
//			DEBUG_LOG("CQuoteSpi::Init 8");
//#if DEBUG_LOG_ENABLE
//			{
//				char tmpLog[1024] =
//				{	0};
//				sprintf(tmpLog, "pSessionReq.MemberID:%s, pSessionReq.TraderNo:%s, pSessionReq.IsShortCert:%d, pSessionReq.ShortCode:%s", pSessionReq.MemberID.buf, pSessionReq.TraderNo.buf, pSessionReq.IsShortCert, pSessionReq.ShortCode.buf);
//				DEBUG_LOG(tmpLog);
//			}
//#endif
//			iRet = TraderSession(&pSessionReq, (char *) m_sPwd.c_str(), &pRspMsg, &pSessionRsp);
//			if (iRet == 0)
//			{
//				DEBUG_LOG("CQuoteSpi::Init 8.1");
//				if (pSessionRsp.CertOrNot == '1')   //??Ҫ??֤
//				{
//					DEBUG_LOG("CQuoteSpi::Init 8.2");
//					_fldTraderCertReq pCertReq;
//					_fldTraderCertRsp pCerRsp;
//
//					strcpy(pCertReq.SessionID.buf, pSessionRsp.SessionID.buf);
//					strcpy(pCertReq.MemberID.buf, m_sParticipantId.c_str());
//					strcpy(pCertReq.TraderNo.buf, m_sUserId.c_str());
//					//pCertReq.SRandomString=;       //????˲??????????
//					//pCertReq.SRandomSignature=;    //????????????ǩ??
//					//pCertReq.SignatureLength=;     //??????????ǩ??????
//					//pCertReq.CRandomString=;       //????Ա?˲?????????ַ???
//					//pCertReq.ClientCA=;            //?ͻ?CA֤??
//					//pCertReq.CALength=;            //CA֤?鳤??
//					iRet = TraderCert(&pCertReq, &pRspMsg, &pCerRsp);
//					if (iRet == 0)
//					{
//						NOTIFY_LOG("??֤?ɹ?");
//					}
//					else
//					{
//						sNote.Format("??֤ʧ??: Code=%d,Msg=%s", iRet, pRspMsg.RspMsg.buf);
//						WARN_LOG (sNote);
//						return false;
//					}
//				}
//				DEBUG_LOG(pRspMsg.RspMsg.buf);
//				DEBUG_LOG("CQuoteSpi::Init 9");
//			}
//			else
//			{
//
//				sNote.Format("?Ựʧ??:Code=%d,Msg=%s", iRet, pRspMsg.RspMsg.buf);
//				WARN_LOG (sNote);
//				DEBUG_LOG("CQuoteSpi::Init 10");
//				return false;
//			}
//
//			DEBUG_LOG("CQuoteSpi::Init 11");
//			//??¼
//			_fldTraderLoginReq loginreq;
//			_fldTraderLoginRsp loginrsp;
//
//			strcpy(loginreq.MemberID.buf, m_sParticipantId.c_str());
//			strcpy(loginreq.TraderNo.buf, m_sUserId.c_str());
//			strcpy(loginreq.Pwd.buf, m_sPwd.c_str());
//			SafeCopy(loginreq.AppName.buf, g_strTXModName.c_str(), loginreq.AppName.Length());   //V1.0.8.0
//			SafeCopy(loginreq.AppVersion.buf, g_strTXModVer.c_str(), loginreq.AppVersion.Length());   //V1.0.8.0
//
//#if DEBUG_LOG_ENABLE
//			char tmpLog[64] =
//			{	0};
//			sprintf(tmpLog, "memid:%s, TraderNo:%s, Pwd:%s", m_sParticipantId.c_str(), m_sUserId.c_str(), m_sPwd.c_str());
//			DEBUG_LOG(tmpLog);
//#endif
//			if (Login(&loginreq, &pRspMsg, &loginrsp) != 0)  //5011ϯλ?Ѿ???¼
//			{
//				sNote.Format("??¼ʧ??: Code=%d,Msg=%s", pRspMsg.ErrCode, pRspMsg.RspMsg.buf);
//				NOTIFY_LOG (sNote);
//				//return false;
//			}
//			DEBUG_LOG("CQuoteSpi::Init 12");
//			//????ָ??ǰͬ???????ȡ????????
//			m_bLogin = true;
//			//??????ָ??
//			if (Ready() != 0)
//			{
//				WARN_LOG("????????ʧ??");
//			}
//			else
//			{
//				NOTIFY_LOG("?ɹ???¼?????ڻ?????ƽ̨");
//			}
//			DEBUG_LOG("CQuoteSpi::Init 13");

	}

	void dce_quot_engine::onChannelLost(const char* szErrMsg)
	{
	}

} /* namespace qtp_bl */
