// -------------------------------------------------------------------------
//    @FileName         :    NFCAsyMysqlModule.h
//    @Author           :    ChuanBo.Guo
//    @Date             :    2016-01-31
//    @Module           :    NFCAsyMysqlModule
//
// -------------------------------------------------------------------------

#ifndef NFC_ASY_MYSQL_MODULE_H
#define NFC_ASY_MYSQL_MODULE_H

#include "NFCMysqlDriver.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"
#include "NFComm/NFPluginModule/NFIMysqlModule.h"
#include "NFComm/NFPluginModule/NFIMysqlDriverManager.h"
#include "NFComm/NFPluginModule/NFIActorModule.h"

struct SMysqlBaseParam
{
public:
	SMysqlBaseParam()
	{
		nRet = 0;
		nReqID = 0;
		bExit = 0;
	}

public:
	bool PackParam(std::string& strData);
	bool UnPackParam(const std::string& strData);

	std::string strSql;
	std::vector<std::string> fieldVec;
	std::vector<std::string> valueVec;
	int bExit;
	int  nRet;
	int  nReqID;
	std::string mstrUseData;
	NFGUID self;
	NFIAsyMysqlModule::ACOTERMYSQLEVENT eType;

	virtual int GetType() { return eType; };
};

struct SMysqlExecuteNonQueryParam : public SMysqlBaseParam
{
	SMysqlExecuteNonQueryParam()
	{
		eType = NFIAsyMysqlModule::ACOTERMYSQLEVENT_EXECUTENONQUERY;
	}
public:
	MYSQL_RETURN_FUNCTOR mFunReturnRsp;
};

struct SMysqlExecuteScalarParam : public SMysqlBaseParam
{
	SMysqlExecuteScalarParam()
	{
		eType = NFIAsyMysqlModule::ACOTERMYSQLEVENT_EXECUTESCALAR;
	}

public:
	MYSQL_RETURN_VECVALUE_FUNCTOR mFunReturnVecValueRsp;
};

struct SMysqlExecuteReaderParam : public SMysqlBaseParam
{
	SMysqlExecuteReaderParam()
	{
		eType = NFIAsyMysqlModule::ACOTERMYSQLEVENT_EXECUTEREADER;
	}

public:
	MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR mFunReturnVeckKeyValueRsp;
};


class NFCMysqlComponent : public NFIComponent
{
public:
	NFCMysqlComponent(NFGUID self, const std::string& strName) : NFIComponent(self, strName)
	{
	}

	NFCMysqlComponent(NFIPluginManager* pPluginManager) : NFIComponent(NFGUID(), "")
	{
	}

	virtual ~NFCMysqlComponent()
	{

	}

	virtual bool Init();
	virtual bool AfterInit();
	virtual int OnASyncEvent(const NFGUID& self, const int event, std::string& arg);

	int OnASyAddMysqlServerEvent(const NFGUID& self, const int event, std::string& arg);
	int OnASyKeepServerAliveEvent(const NFGUID& self, const int event, std::string& arg);
	int OnASyExecuteNonQueryMysqlEvent(const NFGUID& self, const int event, std::string& arg);
	int OnASyExecuteScalarMysqlEvent(const NFGUID& self, const int event, std::string& arg);
	int OnASyExecuteReaderMysqlEvent(const NFGUID& self, const int event, std::string& arg);

protected:
	virtual NF_SHARE_PTR<NFIComponent> CreateNewInstance();

	NF_SHARE_PTR<NFIMysqlDriverManager> m_pMysqlDriverManager;
};


class NFCAsyMysqlModule
	: public NFIAsyMysqlModule
{
public:

	NFCAsyMysqlModule(NFIPluginManager* p);
	virtual ~NFCAsyMysqlModule();

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();
	virtual bool AfterInit();
	virtual bool StartActorPool(const int nCount);
	virtual bool CloseActorPool();

	virtual bool AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1);

	virtual bool ExecuteNonQuery(const NFGUID& self, const std::string& strSQL, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, const MYSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData);
	virtual bool ExecuteScalar(const NFGUID& self, const std::string& strSQL, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, const MYSQL_RETURN_VECVALUE_FUNCTOR& mFunReturnRsp, const std::string& strUseData);
	virtual bool ExecuteReader(const NFGUID& self, const std::string& strSQL, std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR& mFunReturnRsp, const std::string& strUseData);


protected:
	int ApplyRequest(NF_SHARE_PTR<SMysqlBaseParam> pParam, const int nEvetID);
	int RequestAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);

	
	int OnAddMysqlServerAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);
	int OnKeepServerAliveAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData);

	int GetActor();
	bool KeepAliveMysqlServer();

	template<typename ClassParam>
	ClassParam* UnpackResult(const std::string& strMsgData);

private:
	static std::string strDefaultKey;
	static std::string strDefaultTable;

private:
	NFMapEx<int, int> mActorList; //actorid <-->Used
	NFMapEx<int, SMysqlBaseParam> mReqList;// reqID <-->Param
	int nCurReqID;
	int mnSuitIndex;
	NFINT64 mnLastCheckTime;

	NFIActorModule* m_pActorModule;
};

#endif