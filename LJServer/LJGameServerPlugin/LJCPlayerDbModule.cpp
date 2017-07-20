// -------------------------------------------------------------------------
//    @FileName			:    NFCProxyLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyLogicModule
//
// -------------------------------------------------------------------------

#include "LJGameLogicPlugin.h"
#include "LJCPlayerDbModule.h"

bool LJCPlayerDbModule::Init()
{
	return true;
}

bool LJCPlayerDbModule::Shut()
{
	return true;
}

bool LJCPlayerDbModule::Execute()
{
	return true;
}


bool LJCPlayerDbModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pPlayerDbModule = pPluginManager->FindModule<NFIAsyMysqlModule>();

	std::string strIP;
	std::string strUser;
	std::string strPass;
	std::string stdDbName;
	int nPort;
	pPluginManager->GetConfigValue("GameDbIP", strIP, "127.0.0.1");
	pPluginManager->GetConfigValue("GameDbUser", strUser, "root");
	pPluginManager->GetConfigValue("GameDbPass", strPass, "111111");
	pPluginManager->GetConfigValue("GameDbName", stdDbName, "game");
	pPluginManager->GetConfigValue("GameDbPort", nPort, 3306);
	m_pPlayerDbModule->AddMysqlServer(1, "", strIP, nPort, stdDbName, strUser, strPass);
	return true;
}

bool LJCPlayerDbModule::ReadUserData(NFGUID & UserId)
{
	std::vector < std::string>  names;
	std::vector<std::string> values;
	m_pPlayerDbModule->ExecuteReader(UserId, "select * from user_account where reg_id=1", names, values, this, &LJCPlayerDbModule::OnReadUserDataCallBack, "");
	return false;
}

void LJCPlayerDbModule::OnReadUserDataCallBack(const NFGUID& self, const int nRet, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, const std::string& strUseData)
{

}
