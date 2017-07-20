#include "NFCMysqlDriver.h"

const std::string NFCMysqlDriver::strDefaultKey = "ID";
const std::string NFCMysqlDriver::strDefaultTable = "RoleInfo";

NFCMysqlDriver::NFCMysqlDriver(const int nReconnectTime/* = 60*/, const int nReconnectCount /*= -1*/)
{
	mfCheckReconnect = 0.0f;
	mnDBPort = 0;
	m_pMysqlConnect = NULL;
	mnReconnectTime = nReconnectTime;
	mnReconnectCount = nReconnectCount;
}

NFCMysqlDriver::NFCMysqlDriver(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd)
{
	mfCheckReconnect = 0.0f;
	mnDBPort = 0;
	m_pMysqlConnect = NULL;

	mnReconnectTime = 60;
	mnReconnectCount = -1;

	Connect(strDBName, strDBHost, nDBPort, strDBUser, strDBPwd);
}

NFCMysqlDriver::~NFCMysqlDriver()
{
	CloseConnection();
}

bool NFCMysqlDriver::Connect(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd)
{
	mstrDBName = strDBName;
	mstrDBHost = strDBHost;
	mnDBPort = nDBPort;
	mstrDBUser = strDBUser;
	mstrDBPwd = strDBPwd;

	return Connect();
}

bool NFCMysqlDriver::Execute()
{
	if (IsNeedReconnect() && CanReconnect())
	{
		Connect(mstrDBName, mstrDBHost, mnDBPort, mstrDBUser, mstrDBPwd);
	}

	return true;
}

bool NFCMysqlDriver::Query(const std::string& qstr, mysqlpp::StoreQueryResult& queryResult)
{
	mysqlpp::Connection* pConection = GetConnection();
	if (pConection)
	{
		NFMYSQLTRYBEGIN
			mysqlpp::Query query = pConection->query(qstr);
		//query.execute();

		queryResult = query.store();
		query.reset();

		NFMYSQLTRYEND(qstr)
			return true;
	}

	return false;
}

bool NFCMysqlDriver::Query(const std::string& qstr)
{
	mysqlpp::Connection* pConection = GetConnection();
	if (pConection)
	{
		NFMYSQLTRYBEGIN
			mysqlpp::Query query = pConection->query(qstr);
		query.execute();

		query.reset();

		NFMYSQLTRYEND(qstr)
			return true;
	}

	return false;
}

mysqlpp::Connection* NFCMysqlDriver::GetConnection()
{
	return m_pMysqlConnect;
}

void NFCMysqlDriver::CloseConnection()
{
	delete m_pMysqlConnect;
	m_pMysqlConnect = NULL;
}

bool NFCMysqlDriver::Enable()
{
	return !IsNeedReconnect();
}

bool NFCMysqlDriver::CanReconnect()
{

	mfCheckReconnect += 0.1f;

	//30分钟检查断线重连
	if (mfCheckReconnect < mnReconnectTime)
	{
		return false;
	}

	if (mnReconnectCount == 0)
	{
		return false;
	}

	mfCheckReconnect = 0.0f;

	return true;
}

bool NFCMysqlDriver::Reconnect()
{
	CloseConnection();
	Connect(mstrDBName, mstrDBHost, mnDBPort, mstrDBUser, mstrDBPwd);

	if (mnReconnectCount > 0)
	{
		mnReconnectCount--;
	}

	return true;
}

bool NFCMysqlDriver::IsNeedReconnect()
{
	//没有配置表
	if (mstrDBHost.length() < 1 || mstrDBUser.length() < 1)
	{
		return false;
	}

	if (NULL == m_pMysqlConnect)
	{
		return true;
	}

	if (!m_pMysqlConnect->connected())
	{
		CloseConnection();
		return true;
	}

	if (!m_pMysqlConnect->ping())
	{
		CloseConnection();
		return true;
	}

	return false;
}

bool NFCMysqlDriver::Connect()
{
	m_pMysqlConnect = new mysqlpp::Connection();
	if (NULL == m_pMysqlConnect)
	{
		return false;
	}
	NFMYSQLTRYBEGIN
		m_pMysqlConnect->set_option(new mysqlpp::MultiStatementsOption(true));
	m_pMysqlConnect->set_option(new mysqlpp::SetCharsetNameOption("utf8mb4"));
	m_pMysqlConnect->set_option(new mysqlpp::ReconnectOption(true));
	m_pMysqlConnect->set_option(new mysqlpp::ConnectTimeoutOption(60));
	if (!m_pMysqlConnect->connect(mstrDBName.c_str(), mstrDBHost.c_str(), mstrDBUser.c_str(), mstrDBPwd.c_str(), mnDBPort))
	{
		CloseConnection();
		// 连接失败
		return false;
	}

	// 设置超时时间为24小时
	mysqlpp::Query query = m_pMysqlConnect->query("set interactive_timeout = 24*3600");
	query.execute();
	query.reset();
	NFMYSQLTRYEND("Connect faild")
		return true;
}

bool NFCMysqlDriver::ExecuteNonQuery(const std::string& strSql, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (NULL == pConnection)
	{
		return false;
	}
	NFMYSQLTRYBEGIN
	mysqlpp::Query query = pConnection->query();
	for (int i = 0; i < fieldVec.size(); ++i)
	{
		query << "set " << fieldVec[i] << " = " << mysqlpp::quote << valueVec[i] << ";";
	}
	query << strSql << ";";
	bool xResult = query.exec();
	query.reset();
	return xResult;
	NFMYSQLTRYEND("exist error")
}

bool NFCMysqlDriver::ExecuteScalar(const std::string& strSql, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
	return false;
}

bool NFCMysqlDriver::ExecuteReader(const std::string& strSql, std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
	mysqlpp::Connection* pConnection = GetConnection();
	if (NULL == pConnection)
	{
		return false;
	}
	NFMYSQLTRYBEGIN
		mysqlpp::Query query = pConnection->query();
	for (int i = 0; i < fieldVec.size(); ++i)
	{
		query << "set " << fieldVec[i] << " = " << mysqlpp::quote << valueVec[i] << ";";
	}
	query << strSql << ";";
	mysqlpp::StoreQueryResult xResult = query.store();
	query.reset();

	if (xResult.empty() || !xResult)
	{
		return false;
	}

	fieldVec.clear();
	valueVec.clear();
	// xResult应该只有一行的，为了以后可能出现的多条，所以用了循环
	for (int i = 0; i < xResult.num_fields(); i++) //写入列名
	{
		std::string strValue(xResult.field_name(i));
	}
	for (int i = 0; i < xResult.size(); ++i)
	{
		for (int j = 0; j < xResult.num_fields(); j++)
		{
			std::string strValue(xResult[i][j].data(), xResult[i][j].length());
			valueVec.push_back(strValue);
		}
	}
	NFMYSQLTRYEND("exist error")

		return true;
}
