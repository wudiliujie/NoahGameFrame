// -------------------------------------------------------------------------
//    @FileName			:    NFCLoginLogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCLoginLogicModule
//
// -------------------------------------------------------------------------

#ifndef NFC_LOGINLOGIC_MODULE_H
#define NFC_LOGINLOGIC_MODULE_H

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILoginLogicModule.h"
#include "NFComm/NFPluginModule/NFILoginNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIAccountRedisModule.h"
#include "NFComm/NFPluginModule/NFIMysqlModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

class NFCLoginLogicModule
	: public NFILoginLogicModule
{
public:
	NFCLoginLogicModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Init();
	virtual bool Shut();
	virtual bool ReadyExecute();
	virtual bool Execute();

	virtual bool AfterInit();

	virtual void OnLoginProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:
	NFINetModule* m_pNetModule;
	NFILogModule* m_pLogModule;
	NFIAccountRedisModule* m_pAccountRedisModule;
	NFIMysqlModule* m_pMysqlModule;
	NFIElementModule* m_pElementModule;
	NFIKernelModule* m_pKernelModule;
private:
};

#endif