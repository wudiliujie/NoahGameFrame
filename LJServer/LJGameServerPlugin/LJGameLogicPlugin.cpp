// -------------------------------------------------------------------------
//    @FileName			:    NFCProxyPlugin.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyPlugin
//
// -------------------------------------------------------------------------


#include "LJGameLogicPlugin.h"
#include "LJCGameLogicModule.h"
#include "LJCGameServerNet_ServerModule.h"

#ifdef NF_DYNAMIC_PLUGIN

NF_EXPORT void DllStartPlugin(NFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, LJGameLogicPlugin)
};

NF_EXPORT void DllStopPlugin(NFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, LJGameLogicPlugin)
};

#endif
//////////////////////////////////////////////////////////////////////////

const int LJGameLogicPlugin::GetPluginVersion()
{
    return 0;
}

const std::string LJGameLogicPlugin::GetPluginName()
{
	return GET_CLASS_NAME(LJGameLogicPlugin);
}

void LJGameLogicPlugin::Install()
{
	REGISTER_MODULE(pPluginManager, LJIGameServerNet_ServerModule, LJCGameServerNet_ServerModule)
    REGISTER_MODULE(pPluginManager, LJIGameLogicModule, LJCGameLogicModule)
	

}

void LJGameLogicPlugin::Uninstall()
{
	UNREGISTER_MODULE(pPluginManager, LJIGameServerNet_ServerModule, LJCGameServerNet_ServerModule)
    UNREGISTER_MODULE(pPluginManager, LJIGameLogicModule, LJCGameLogicModule)
}
