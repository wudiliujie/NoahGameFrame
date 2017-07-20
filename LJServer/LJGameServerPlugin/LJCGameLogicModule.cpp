// -------------------------------------------------------------------------
//    @FileName			:    NFCProxyLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyLogicModule
//
// -------------------------------------------------------------------------

#include "LJGameLogicPlugin.h"
#include "LJCGameLogicModule.h"

bool LJCGameLogicModule::Init()
{



	return true;
}

bool LJCGameLogicModule::Shut()
{
	return true;
}

bool LJCGameLogicModule::Execute()
{


	return true;
}


bool LJCGameLogicModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pPlayerDbModule = pPluginManager->FindModule<LJIPlayerDbModule>();
	//m_pPlayerDbModule->AddMysqlServer()
	/*NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::NoSqlServer::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nServerID = m_pElementModule->GetPropertyInt(strId, NFrame::NoSqlServer::ServerID());
			const int nPort = m_pElementModule->GetPropertyInt(strId, NFrame::NoSqlServer::Port());
			const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::NoSqlServer::IP());
			const std::string& strAuth = m_pElementModule->GetPropertyString(strId, NFrame::NoSqlServer::Auth());

			if (this->AddConnectSql(strId, strIP, nPort, strAuth))
			{
				std::ostringstream strLog;
				strLog << "Connected NoSqlServer[" << strIP << "], Port = [" << nPort << "], Passsword = [" << strAuth << "]";
				m_pLogModule->LogNormal(NFILogModule::NF_LOG_LEVEL::NLL_INFO_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);

			}
			else
			{
				std::ostringstream strLog;
				strLog << "Cannot connect NoSqlServer[" << strIP << "], Port = " << nPort << "], Passsword = [" << strAuth << "]";
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
			}
		}*/
		//}
	return true;
}

void LJCGameLogicModule::OnPlayerLoginSuccess(const NFGUID& accountid)
{	
	//NFDataList var;
	//var.AddString(NFrame::Player::Name());
	//var.AddString(xMsg.name());

	//var.AddString(NFrame::Player::GateID());
	//var.AddInt(nGateID);

	//var.AddString(NFrame::Player::GameID());
	//var.AddInt(pPluginManager->GetAppID());

	//var.AddString(NFrame::Player::HomeSceneID());
	//var.AddInt(nSceneID);

	//var.AddString(NFrame::Player::ViewOppnent());
	//var.AddObject(nRoleID);
	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(accountid, 1,0, NFrame::Player::ThisName(), "", var);
	//if (NULL == pObject)
	//{
	//	//mRoleBaseData
	//	//mRoleFDData
	//	return;
	//}
	//m_pPlayerDbModule->ReadUserData(accountid);
}
