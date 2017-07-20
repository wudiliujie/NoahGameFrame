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
#include "NFComm/NFPluginModule/LJIPlayerDbModule.h"
#include "NFComm/NFPluginModule/NFIAsyMysqlModule.h"


class LJCPlayerDbModule
    : public LJIPlayerDbModule
{
public:
	LJCPlayerDbModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();
public:
	virtual bool ReadUserData(NFGUID& UserId);

	void OnReadUserDataCallBack(const NFGUID& self, const int nRet, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, const std::string& strUseData);
protected:

	
protected:
    NFIClassModule* m_pClassModule;
    NFIKernelModule* m_pKernelModule;
	NFIAsyMysqlModule * m_pPlayerDbModule;
private:
};

#endif