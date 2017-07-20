// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataBaseModule
//    Row,Col; ; ; ; ; ; ;
// -------------------------------------------------------------------------

#include <algorithm>
#include "NFCMysqlDriver.h"
#include "NFCMysqlModule.h"
#include "NFCMysqlDriverManager.h"

NFCMysqlModule::NFCMysqlModule(NFIPluginManager* p)
{
	pPluginManager = p;
	mnLastCheckTime = 0;
}

NFCMysqlModule::~NFCMysqlModule()
{

}

bool NFCMysqlModule::Init()
{
	m_pMysqlDriverManager = NF_SHARE_PTR<NFIMysqlDriverManager>(NF_NEW NFCMysqlDriverManager());
	return true;
}

bool NFCMysqlModule::Shut()
{
	return true;
}

bool NFCMysqlModule::AfterInit()
{
	return true;
}

bool NFCMysqlModule::Execute()
{
	if (mnLastCheckTime + 10 > GetPluginManager()->GetNowTime())
	{
		return false;
	}

	mnLastCheckTime = GetPluginManager()->GetNowTime();

	if (m_pMysqlDriverManager.get())
	{
		m_pMysqlDriverManager->CheckMysql();
	}

	return true;
}

bool NFCMysqlModule::AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime/* = 10*/, const int nRconneCount/* = -1*/)
{
	if (!m_pMysqlDriverManager.get())
	{
		return false;
	}

	return m_pMysqlDriverManager->AddMysqlServer(nServerID, strDns, strIP, nPort, strDBName, strDBUser, strDBPwd, nRconnectTime, nRconneCount);
}

bool NFCMysqlModule::ExecuteReader(const std::string & strSql,  std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->ExecuteReader(strSql, fieldVec, valueVec);
	}
}

bool NFCMysqlModule::ExecuteNonQuery(const std::string & strSql, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->ExecuteNonQuery(strSql, fieldVec, valueVec);
	}
}

bool NFCMysqlModule::ExecuteScalar(const std::string& strSql, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (pDriver)
	{
		return pDriver->ExecuteScalar(strSql, fieldVec, valueVec);
	}
}
