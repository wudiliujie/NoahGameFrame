// -------------------------------------------------------------------------
//    @FileName         :    NFIMysqlDriver.h
//    @Author           :    chuano.guo
//    @Date             :    2014-07-28
//    @Module           :    NFIMysqlDriver
//    @Desc             :
// -------------------------------------------------------------------------
#ifndef NFI_MYSQL_DRIVER_H
#define NFI_MYSQL_DRIVER_H
//#define MS_HIREDIS
#ifdef _MSC_VER
#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdint.h>
#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
//#include <sys/timeb.h>
#include <sys/types.h>

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#endif

#include <string>
#include <vector>
#include <set>
#include "mysqlpp/mysql++.h"

class NFIMysqlDriver
{
public:
	virtual ~NFIMysqlDriver() {}
	
    virtual bool Connect(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd) = 0;
    virtual  bool Execute() = 0;
    virtual mysqlpp::Connection* GetConnection() = 0;
    virtual bool Query(const std::string& qstr, mysqlpp::StoreQueryResult& queryResult) = 0;
    virtual bool Query(const std::string& qstr) = 0;

    virtual bool Enable() = 0;
    virtual bool Reconnect() = 0;
    virtual bool CanReconnect() = 0;

	virtual bool ExecuteNonQuery(const std::string& strSql,const std::vector<std::string>& fieldVec,const std::vector<std::string>& valueVec) = 0;
	virtual bool ExecuteScalar(const std::string& strSql, const std::vector<std::string>& fieldVec,  std::vector<std::string>& valueVec) = 0;
	virtual bool ExecuteReader(const std::string& strSql, std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)=0;
};

#endif // !_NFI_REDIS_DRIVER_MODULE_H_