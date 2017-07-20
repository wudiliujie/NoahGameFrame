// -------------------------------------------------------------------------
//    @FileName			:    NFCProxyPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyPlugin
//
// -------------------------------------------------------------------------

#ifndef NFC_PROXYSERVERPLUGIN_H
#define NFC_PROXYSERVERPLUGIN_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/LJIGameLogicModule.h"


class LJGameLogicPlugin : public NFIPlugin
{
public:
	LJGameLogicPlugin(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};

#endif