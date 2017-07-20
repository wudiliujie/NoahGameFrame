// -------------------------------------------------------------------------
//    @FileName			:    NFCMysqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCMysqlModule
//
// -------------------------------------------------------------------------

#ifndef NFC_MYSQL_MODULE_H
#define NFC_MYSQL_MODULE_H

#include "NFCMysqlDriver.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIMysqlModule.h"
#include "NFComm/NFPluginModule/NFIMysqlDriverManager.h"

class NFCMysqlModule
	: public NFIMysqlModule
{
public:

	NFCMysqlModule(NFIPluginManager* p);
	virtual ~NFCMysqlModule();

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();

	//////////////////////////////////////////////////////////////////////////	
	virtual bool AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1);	
	virtual bool ExecuteReader(const std::string& strSql, std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);
	virtual bool ExecuteNonQuery(const std::string& strSql, const std::vector<std::string>& fieldVec,const std::vector<std::string>& valueVec);
	virtual bool ExecuteScalar(const std::string& strSql, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);
private:
	NF_SHARE_PTR<NFIMysqlDriverManager> m_pMysqlDriverManager;

	NFINT64 mnLastCheckTime;
};

#endif