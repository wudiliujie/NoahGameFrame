// -------------------------------------------------------------------------
//    @FileName			:    NFCLoginLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginLogicModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFLoginLogicPlugin.h"
#include "NFCLoginLogicModule.h"


bool NFCLoginLogicModule::Init()
{
	m_pAccountRedisModule = pPluginManager->FindModule<NFIAccountRedisModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pAsyMysqlModule = pPluginManager->FindModule<NFIAsyMysqlModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	return true;
}

bool NFCLoginLogicModule::Shut()
{
	return true;
}

void NFCLoginLogicModule::OnLoginProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqAccountLogin xMsg;
	if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NetObject* pNetObject = m_pNetModule->GetNet()->GetNetObject(nSockIndex);
	if (pNetObject)
	{
		if (pNetObject->GetConnectKeyState() == 0)
		{
			if (!m_pAccountRedisModule->ExistAccount(xMsg.account()))
			{
				m_pAccountRedisModule->AddAccount(xMsg.account(), xMsg.password());
			}

			int nState = m_pAccountRedisModule->VerifyAccount(xMsg.account(), xMsg.password());
			if (0 != nState)
			{
				std::ostringstream strLog;
				strLog << "Check password failed, Account = " << xMsg.account() << " Password = " << xMsg.password();
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NFGUID(0, nSockIndex), strLog, __FUNCTION__, __LINE__);

				NFMsg::AckEventResult xMsg;
				xMsg.set_event_code(NFMsg::EGEC_ACCOUNTPWD_INVALID);

				m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_LOGIN, xMsg, nSockIndex);
				return;
			}

			pNetObject->SetConnectKeyState(1);
			pNetObject->SetAccount(xMsg.account());

			NFMsg::AckEventResult xData;
			xData.set_event_code(NFMsg::EGEC_ACCOUNT_SUCCESS);

			m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_LOGIN, xData, nSockIndex);

			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "Login successed :", xMsg.account().c_str());
		}
	}
}

bool NFCLoginLogicModule::ReadyExecute()
{
	m_pNetModule->RemoveReceiveCallBack(NFMsg::EGMI_REQ_LOGIN);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LOGIN, this, &NFCLoginLogicModule::OnLoginProcess);

	return true;
}

bool NFCLoginLogicModule::Execute()
{


	return true;
}


bool NFCLoginLogicModule::AfterInit()
{
	//m_pMysqlModule->AddMysqlServer(1, "127.0.0.1", "127.0.0.1", 3306, "nf_account", "root", "111111");
	//std::vector<std::string> fields;
	//std::vector<std::string> fieldvalues;
	//m_pMysqlModule->ExecuteReader("select * from user_account", NULL, fields, fieldvalues);
	//std::vector<std::string> paramVec;
	//paramVec.push_back("@userpass='222222'");
	//NFGUID playerid = m_pKernelModule->CreateGUID();
	//NFDataList agr;
	//NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(playerid, 1, 0, NFrame::Player::ThisName(), "", agr);
	//pObject->GetPropertyManager().

	//m_pMysqlModule->ExecuteNonQuery("update user_account set userpass=@userpass", &paramVec);

	return true;
}

int NFCLoginLogicModule::VerifyAccount(const std::string& strAccount, const std::string& strPwd)
{
	//m_pAsyMysqlModule->Query("select * from user_account ");
	return 0;

}
