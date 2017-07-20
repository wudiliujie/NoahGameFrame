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
class Player
{
public:
	Player();
private:	
	NF_SHARE_PTR<NFIObject> m_Data;	//玩家的数据
};

#endif