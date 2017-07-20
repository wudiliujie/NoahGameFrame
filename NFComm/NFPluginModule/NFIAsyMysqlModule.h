// -------------------------------------------------------------------------
//    @FileName      	:    NFIAsyMysqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIAsyMysqlModule
//
//
// -------------------------------------------------------------------------

#ifndef NFI_ASY_MYSQL_MODULE_H
#define NFI_ASY_MYSQL_MODULE_H

#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

typedef std::function<void(const NFGUID& self, const int nRet, const std::string& strUseData)> MYSQL_RETURN_FUNCTOR;
//typedef std::function<void(const NFGUID& self, const int nRet, const int nValue, const std::string& strUseData)> MYSQL_RETURN_INT_FUNCTOR;
//typedef std::function<void(const NFGUID& self, const int nRet, const std::string& nValue, const std::string& strUseData)> MYSQL_RETURN_STRING_FUNCTOR;
typedef std::function<void(const NFGUID& self, const int nRet, const std::vector<std::string>& valueVec, const std::string& strUseData)> MYSQL_RETURN_VECVALUE_FUNCTOR;
typedef std::function<void(const NFGUID& self, const int nRet, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, const std::string& strUseData)> MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR;

struct SMysqlParam;

class NFIAsyMysqlModule
	: public NFIModule
{
public:
	enum ACOTERMYSQLEVENT
	{
		ACOTERMYSQLEVENT_INISERVER=1,
		ACOTERMYSQLEVENT_KEEPALIVESERVER=2,
		ACOTERMYSQLEVENT_EXECUTENONQUERY = 3,
		ACOTERMYSQLEVENT_EXECUTESCALAR = 4,
		ACOTERMYSQLEVENT_EXECUTEREADER = 5,
		//ACOTERMYSQLEVENT_KEEPALIVESERVER = 3,
		//ACOTERMYSQLEVENT_UPDATA = 4,
		//ACOTERMYSQLEVENT_QUERY = 5,
		//ACOTERMYSQLEVENT_SELECT = 6,
		//ACOTERMYSQLEVENT_DELETE = 7,
		//ACOTERMYSQLEVENT_EXISTS = 8,
		//ACOTERMYSQLEVENT_KEYS = 9,
	};

public:
	template<typename BaseType>
	int ExecuteNonQuery(const NFGUID& self, const std::string& strSQL, std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec,
		BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const NFGUID&, const int, const std::string&), const std::string& strUseData)
	{
		const MYSQL_RETURN_FUNCTOR FunReturnRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		return ExecuteNonQuery(self, strSQL, fieldVec, valueVec, FunReturnRsp, strUseData);
	}

	template<typename BaseType>
	int ExecuteScalar(const NFGUID& self, const std::string& strSQL, std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec,
		BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const NFGUID&, const int, const std::string&,const std::string&), const std::string& strUseData)
	{
		const MYSQL_RETURN_VECVALUE_FUNCTOR FunReturnRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_3);

		return ExecuteScalar(self, strSQL, fieldVec, valueVec, FunReturnRsp, strUseData);
	}
	template<typename BaseType>
	int ExecuteReader(const NFGUID& self, const std::string& strSQL, std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec,
		BaseType* pBaseType, void (BaseType::*handleReturnRsp)(const NFGUID&, const int, const std::vector<std::string>&, const std::vector<std::string>&, const std::string&), const std::string& strUseData)
	{
		const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR FunReturnRsp = std::bind(handleReturnRsp, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);

		return ExecuteReader(self, strSQL, fieldVec, valueVec, FunReturnRsp, strUseData);
	}

	virtual bool CloseActorPool() = 0;
	virtual bool AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1) = 0;

protected:
	virtual bool StartActorPool(const int nCount) = 0;
	virtual bool ExecuteNonQuery(const NFGUID& self, const std::string& strSQL, const std::vector<std::string>& fieldVec,const std::vector<std::string>& valueVec, const MYSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData) = 0;
	virtual bool ExecuteScalar(const NFGUID& self, const std::string& strSQL, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, const MYSQL_RETURN_VECVALUE_FUNCTOR& mFunReturnRsp, const std::string& strUseData) = 0;
	virtual bool ExecuteReader(const NFGUID& self, const std::string& strSQL,  std::vector<std::string>& fieldVec,  std::vector<std::string>& valueVec, const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR& mFunReturnRsp, const std::string& strUseData) = 0;
};

#endif
