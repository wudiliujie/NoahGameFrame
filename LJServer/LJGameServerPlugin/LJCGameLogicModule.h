// -------------------------------------------------------------------------
//    @FileName			:    NFCProxyLogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyLogicModule
//
// -------------------------------------------------------------------------

#ifndef NFC_PROXYLOGIC_MODULE_H
#define NFC_PROXYLOGIC_MODULE_H

#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/LJIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"
#include "NFComm/NFPluginModule/LJIPlayerDbModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
class LJCGameLogicModule
	: public LJIGameLogicModule
{
public:
	LJCGameLogicModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Init();
	virtual bool Shut();
	virtual bool Execute();

	virtual bool AfterInit();

	virtual void OnPlayerLoginSuccess(const NFGUID& accountid);
protected:


protected:
	NFIClassModule* m_pClassModule;
	NFIKernelModule* m_pKernelModule;
	LJIPlayerDbModule * m_pPlayerDbModule;
private:
};

#endif