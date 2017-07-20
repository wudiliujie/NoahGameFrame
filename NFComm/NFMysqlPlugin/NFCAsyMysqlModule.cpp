#include "NFCAsyMysqlModule.h"
#include "NFCMysqlDriverManager.h"

bool SMysqlBaseParam::PackParam(std::string& strData)
{
	try
	{
		NFMsg::PackMysqlParam xMsg;

		xMsg.set_strsql(strSql);
		xMsg.set_bexit(bExit);
		xMsg.set_nreqid(nReqID);
		xMsg.set_nret(nRet);
		xMsg.set_etype(eType);

		for (int i = 0; i < fieldVec.size(); i++)
		{
			const std::string& strFiled = fieldVec[i];
			xMsg.add_fieldveclist(strFiled);
		}

		for (int i = 0; i < valueVec.size(); i++)
		{
			const std::string& strValue = valueVec[i];
			xMsg.add_valueveclist(strValue);
		}

		return xMsg.SerializeToString(&strData);
	}
	catch (...)
	{
		return false;
	}

	return true;
}


bool SMysqlBaseParam::UnPackParam(const std::string& strData)
{
	try
	{
		NFMsg::PackMysqlParam xMsg;
		if (!xMsg.ParseFromString(strData))
		{
			return false;
		}

		strSql = xMsg.strsql();
		bExit = xMsg.bexit();
		nReqID = xMsg.nreqid();
		nRet = xMsg.nret();
		eType = (NFIAsyMysqlModule::ACOTERMYSQLEVENT)xMsg.etype();

		for (int i = 0; i < xMsg.fieldveclist_size(); i++)
		{
			const std::string& strField = xMsg.fieldveclist(i);
			fieldVec.push_back(strField);
		}

		for (int i = 0; i < xMsg.valueveclist_size(); i++)
		{
			const std::string& strValue = xMsg.valueveclist(i);
			valueVec.push_back(strValue);
		}

	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool NFCMysqlComponent::Init()
{
	return true;
}

bool NFCMysqlComponent::AfterInit()
{
	m_pMysqlDriverManager = NF_SHARE_PTR<NFIMysqlDriverManager>(NF_NEW NFCMysqlDriverManager());

	return true;
}

int NFCMysqlComponent::OnASyncEvent(const NFGUID& self, const int event, std::string& arg)
{
	int nRet = 0;
	switch (event)
	{
	case NFCAsyMysqlModule::ACOTERMYSQLEVENT_INISERVER:
	{
		nRet = OnASyAddMysqlServerEvent(self, event, arg);
	}
	break;
	case NFCAsyMysqlModule::ACOTERMYSQLEVENT_KEEPALIVESERVER:
	{
		nRet = OnASyKeepServerAliveEvent(self, event, arg);
	}
	break;
	case NFCAsyMysqlModule::ACOTERMYSQLEVENT_EXECUTENONQUERY:
	{
		nRet = OnASyExecuteNonQueryMysqlEvent(self, event, arg);
	}
	break;
	case NFCAsyMysqlModule::ACOTERMYSQLEVENT_EXECUTESCALAR:
	{
		nRet = OnASyExecuteScalarMysqlEvent(self, event, arg);
	}
	break;
	case NFCAsyMysqlModule::ACOTERMYSQLEVENT_EXECUTEREADER:
	{
		nRet = OnASyExecuteReaderMysqlEvent(self, event, arg);
	}
	break;
	default:
		break;
	}

	return nRet;
}
int NFCMysqlComponent::OnASyAddMysqlServerEvent(const NFGUID& self, const int event, std::string& arg)
{
	NFMsg::PackMysqlServerInfo xMsg;
	if (!xMsg.ParseFromString(arg))
	{
		return -1;
	}

	if (!m_pMysqlDriverManager->AddMysqlServer(xMsg.nserverid(), "", xMsg.strdnsip(), xMsg.nport(), xMsg.strdbname(), xMsg.strdbuser(), xMsg.strdbpwd(), xMsg.nrconnecttime(), xMsg.nrconnecount()))
	{
		return -2;
	}

	return 0;
}

int NFCMysqlComponent::OnASyKeepServerAliveEvent(const NFGUID& self, const int event, std::string& arg)
{
	m_pMysqlDriverManager->CheckMysql();

	return 0;
}

int NFCMysqlComponent::OnASyExecuteNonQueryMysqlEvent(const NFGUID & self, const int event, std::string & arg)
{
	SMysqlBaseParam xparam;
	if (!xparam.UnPackParam(arg))
	{
		return -1;
	}

	if (!m_pMysqlDriverManager.get())
	{
		return -2;
	}

	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (NULL == pDriver)
	{
		return -3;
	}

	if (xparam.eType != NFCAsyMysqlModule::ACOTERMYSQLEVENT_EXECUTENONQUERY)
	{
		return -4;
	}

	bool bExit = false;
	if (!pDriver->ExecuteNonQuery(xparam.strSql,xparam.fieldVec,xparam.valueVec))
	{
		xparam.bExit = bExit;
		xparam.nRet = -1;
	}
	xparam.PackParam(arg);
	return 0;
}

int NFCMysqlComponent::OnASyExecuteScalarMysqlEvent(const NFGUID & self, const int event, std::string & arg)
{
	SMysqlBaseParam xparam;
	if (!xparam.UnPackParam(arg))
	{
		return -1;
	}

	if (!m_pMysqlDriverManager.get())
	{
		return -2;
	}

	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (NULL == pDriver)
	{
		return -3;
	}

	if (xparam.eType != NFCAsyMysqlModule::ACOTERMYSQLEVENT_EXECUTENONQUERY)
	{
		return -4;
	}

	bool bExit = false;
	if (!pDriver->ExecuteScalar(xparam.strSql, xparam.fieldVec, xparam.valueVec))
	{
		xparam.bExit = bExit;
		xparam.nRet = -1;
	}
	xparam.PackParam(arg);
	return 0;
}

int NFCMysqlComponent::OnASyExecuteReaderMysqlEvent(const NFGUID & self, const int event, std::string & arg)
{
	SMysqlBaseParam xparam;
	if (!xparam.UnPackParam(arg))
	{
		return -1;
	}

	if (!m_pMysqlDriverManager.get())
	{
		return -2;
	}

	NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
	if (NULL == pDriver)
	{
		return -3;
	}

	if (xparam.eType != NFCAsyMysqlModule::ACOTERMYSQLEVENT_EXECUTENONQUERY)
	{
		return -4;
	}

	bool bExit = false;
	if (!pDriver->ExecuteReader(xparam.strSql, xparam.fieldVec, xparam.valueVec))
	{
		xparam.bExit = bExit;
		xparam.nRet = -1;
	}
	xparam.PackParam(arg);
	return 0;
}

NF_SHARE_PTR<NFIComponent> NFCMysqlComponent::CreateNewInstance()
{
	return  NF_SHARE_PTR<NFIComponent>(NF_NEW  NFCMysqlComponent(NFGUID(1, 2), ""));
}

NFCAsyMysqlModule::NFCAsyMysqlModule(NFIPluginManager* p)
{
	pPluginManager = p;
	mnLastCheckTime = 0;
}

NFCAsyMysqlModule::~NFCAsyMysqlModule()
{

}

bool NFCAsyMysqlModule::Init()
{
	return true;
}

bool NFCAsyMysqlModule::Shut()
{

	return true;
}

bool NFCAsyMysqlModule::Execute()
{

	return true;
}

bool NFCAsyMysqlModule::AfterInit()
{
	m_pActorModule = pPluginManager->FindModule<NFIActorModule>();

	StartActorPool(10);

	return true;
}
bool NFCAsyMysqlModule::ExecuteNonQuery(const NFGUID& self, const std::string& strSQL, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, const MYSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
	NF_SHARE_PTR<SMysqlExecuteNonQueryParam> pParam(NF_NEW SMysqlExecuteNonQueryParam());
	if (NULL == pParam)
	{
		return false;
	}

	pParam->strSql = strSQL;
	pParam->fieldVec = fieldVec;
	pParam->valueVec = valueVec;
	pParam->nReqID = nCurReqID++;
	pParam->mFunReturnRsp = mFunReturnRsp;
	pParam->mstrUseData = strUseData;
	pParam->self = self;

	if (ApplyRequest(pParam, pParam->eType) < 0)
	{
		return false;
	}

	return true;
}

bool NFCAsyMysqlModule::ExecuteScalar(const NFGUID& self, const std::string& strSQL, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, const MYSQL_RETURN_VECVALUE_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
	NF_SHARE_PTR<SMysqlExecuteScalarParam> pParam(NF_NEW SMysqlExecuteScalarParam());
	if (NULL == pParam)
	{
		return false;
	}

	pParam->strSql = strSQL;
	pParam->fieldVec = fieldVec;
	pParam->valueVec = valueVec;
	pParam->nReqID = nCurReqID++;
	pParam->mFunReturnVecValueRsp = mFunReturnRsp;
	pParam->mstrUseData = strUseData;
	pParam->self = self;

	if (ApplyRequest(pParam, pParam->eType) < 0)
	{
		return false;
	}

	return true;
}

bool NFCAsyMysqlModule::ExecuteReader(const NFGUID& self, const std::string& strSQL, std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec, const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
	NF_SHARE_PTR<SMysqlExecuteReaderParam> pParam(NF_NEW SMysqlExecuteReaderParam());
	if (NULL == pParam)
	{
		return false;
	}

	pParam->strSql = strSQL;
	pParam->fieldVec = fieldVec;
	pParam->valueVec = valueVec;
	pParam->nReqID = nCurReqID++;
	pParam->mFunReturnVeckKeyValueRsp = mFunReturnRsp;
	pParam->mstrUseData = strUseData;
	pParam->self = self;

	if (ApplyRequest(pParam, pParam->eType) < 0)
	{
		return false;
	}

	return true;
}





bool NFCAsyMysqlModule::StartActorPool(const int nCount)
{
	for (int i = 0; i < nCount; i++)
	{
		int nActorID = m_pActorModule->RequireActor<NFCMysqlComponent>(this, &NFCAsyMysqlModule::RequestAsyEnd);
		if (nActorID > 0)
		{
			mActorList.AddElement(i, NF_SHARE_PTR<int>(NF_NEW int(nActorID)));
		}
	}

	return true;
}

bool NFCAsyMysqlModule::CloseActorPool()
{
	int nActor = 0;
	for (NF_SHARE_PTR<int> pData = mActorList.First(nActor); pData != NULL; pData = mActorList.Next(nActor))
	{
		m_pActorModule->ReleaseActor(nActor);
	}

	mActorList.ClearAll();
	return true;
}

int NFCAsyMysqlModule::ApplyRequest(NF_SHARE_PTR<SMysqlBaseParam> pParam, const int nEvetID/* = ACOTERMYSQLEVENT_USEDB*/)
{
	int nAcotrID = GetActor();
	if (nAcotrID <= 0)
	{
		return -2;
	}

	std::string arg;
	if (!pParam->PackParam(arg))
	{
		return -3;
	}

	if (!mReqList.AddElement(pParam->nReqID, pParam))
	{
		return -4;
	}

	if (!m_pActorModule->SendMsgToActor(nAcotrID, pParam->self, nEvetID, arg))
	{
		mReqList.RemoveElement(pParam->nReqID);
		return -5;
	}

	return 0;
}

int NFCAsyMysqlModule::RequestAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
	int nRet = 0;
	switch (nEventID)
	{
	case NFCAsyMysqlModule::ACOTERMYSQLEVENT_INISERVER:
	{
		nRet = OnAddMysqlServerAsyEnd(self, nFormActor, nEventID, strData);
	}
	break;
	case NFCAsyMysqlModule::ACOTERMYSQLEVENT_KEEPALIVESERVER:
	{
		nRet = OnKeepServerAliveAsyEnd(self, nFormActor, nEventID, strData);
	}
	break;
	default:
		break;
	}

	return nRet;
}

int NFCAsyMysqlModule::GetActor()
{
	if (mActorList.Count() <= 0)
	{
		return -1;
	}

	mnSuitIndex++;
	mnSuitIndex = mnSuitIndex % mActorList.Count();

	int i = 0;
	for (int* pData = mActorList.FirstNude(); pData != NULL; pData = mActorList.NextNude())
	{
		if (i < mActorList.Count() && i == mnSuitIndex)
		{
			return *pData;
		}

		++i;
	}

	return -1;
}

bool NFCAsyMysqlModule::AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime /*= 10*/, const int nRconneCount /*= -1*/)
{
	NFMsg::PackMysqlServerInfo xMsg;

	xMsg.set_nrconnecttime(nRconnectTime);
	xMsg.set_nrconnecount(nRconneCount);
	xMsg.set_nport(nPort);
	xMsg.set_strdbname(strDBName);
	xMsg.set_strdnsip(strIP);
	xMsg.set_strdbuser(strDBUser);
	xMsg.set_strdbpwd(strDBPwd);
	xMsg.set_nserverid(nServerID);
	std::string arg;

	if (!xMsg.SerializeToString(&arg))
	{
		return false;
	}

	const int nEvetID = ACOTERMYSQLEVENT_INISERVER;
	for (int* pData = mActorList.FirstNude(); pData != NULL; pData = mActorList.NextNude())
	{
		int nAcotrID = *pData;
		m_pActorModule->SendMsgToActor(nAcotrID, NFGUID(), nEvetID, arg);
	}

	return true;
}


bool NFCAsyMysqlModule::KeepAliveMysqlServer()
{
	std::string arg;
	const int nEvetID = ACOTERMYSQLEVENT_KEEPALIVESERVER;
	for (int* pData = mActorList.FirstNude(); pData != NULL; pData = mActorList.NextNude())
	{
		int nAcotrID = *pData;
		m_pActorModule->SendMsgToActor(nAcotrID, NFGUID(), nEvetID, arg);
	}

	return true;
}


int NFCAsyMysqlModule::OnAddMysqlServerAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
	return 0;
}

int NFCAsyMysqlModule::OnKeepServerAliveAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
	return 0;
}

template<typename ClassParam>
ClassParam* NFCAsyMysqlModule::UnpackResult(const std::string& strMsgData)
{
	SMysqlBaseParam xResultparam;
	if (!xResultparam.UnPackParam(strMsgData))
	{
		return NULL;
	}

	NF_SHARE_PTR<SMysqlBaseParam> pReqBaseData = mReqList.GetElement(xResultparam.nReqID);
	if (NULL == pReqBaseData)
	{
		return NULL;
	}

	ClassParam* pReqData = dynamic_cast<ClassParam*>(pReqBaseData.get());
	if (NULL == pReqData)
	{
		return NULL;
	}

	pReqData->valueVec = xResultparam.valueVec;
	pReqData->bExit = xResultparam.bExit;
	pReqData->nRet = xResultparam.nRet;

	return pReqData;
}
