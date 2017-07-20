// -------------------------------------------------------------------------
//    @FileName			:    NFIProxyLogicModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIProxyLogicModule
//
// -------------------------------------------------------------------------

#ifndef LJI_PLAYERDB_MODULE_H
#define LJI_PLAYERDB_MODULE_H

#include "NFIModule.h"

/*
    A user's attributes and the length of the table is fixed in this module,
    and all of the data of the user only attribute data and table data.
*/
class LJIPlayerDbModule
    : public NFIModule
{
public:
	virtual bool ReadUserData(NFGUID& UserId) = 0;
protected:
private:
};

#endif