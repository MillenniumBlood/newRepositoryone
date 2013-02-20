#include "mySemaphore.h"
#include "myException.h"

MySemaphore::MySemaphore(string& sName,long initialCount,long maximumCount,int accessMode,BOOL canInherit)
{
	// initialization
	m_sa = NULL;
	m_semaphore = NULL;
	m_semaphoreName = "";
	m_initCount = 0;
	m_maxCount  = 0;
	m_inherit   = TRUE;

	// create it
	if ( accessMode == CREATE )
    {
		// security handling 在指定的堆上分配内存，并且分配后的内存不可移动。
		m_sa = (LPSECURITY_ATTRIBUTES)HeapAlloc(GetProcessHeap(),0,sizeof(SECURITY_ATTRIBUTES));
        m_sa->nLength = sizeof(SECURITY_ATTRIBUTES);
        m_sa->lpSecurityDescriptor = NULL; //no restrictions
        m_sa->bInheritHandle = canInherit; //是否可继承

		// make sure the count is legal
		// normally, you want to create the semaphore will an initialcount = 0;
		// therefore, the original state of the semaphore is locked.
		// once the system finishes all the initialization work, it can
		// call unlock() to increase it to be 1, so other threads can
		// start to use it
		if ( maximumCount < initialCount ) 
		{
			maximumCount = 1;
			initialCount = 0;
		}

		// create the semaphore
        if ( (m_semaphore = CreateSemaphore(m_sa,initialCount,maximumCount,sName.c_str())) == NULL)
        {
			int errCode = 0;
			string header = "CreateSemaphore failed";
			getErrorDetail(errCode,header);
			MyException excp(errCode,header);
            throw excp;
            return;
        }

		// save the key values
		m_initCount = initialCount;
		m_maxCount  = maximumCount;
		m_inherit   = canInherit;

		// save the name, if specified
		if ( sName.compare("") ) m_semaphoreName.append(sName);

	}

	// open it
    else
    {
        if ((m_semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS,canInherit,sName.c_str())) == NULL)
        {
			int errCode;
			string header = "OpenSemaphore failed";
			getErrorDetail(errCode,header);
            MyException excp(errCode,header);
			throw excp;
            return;
        }
    }
}

MySemaphore::~MySemaphore()
{
	if (m_sa) HeapFree(GetProcessHeap(),0,m_sa); //释放sa堆空间
	if ( m_semaphore ) closeSemaphore(); //关闭信号量句柄，它的所有句柄都关闭以后，对象自己也会删除。
}

void MySemaphore::getErrorDetail(int& errCode,string& header)
{
	char buf[256];
	LPVOID lpDetailMsgBuf;
	UINT errorCode;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
				  NULL,errorCode=GetLastError(),MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
				  (LPTSTR)&lpDetailMsgBuf,0,NULL);

	memset(buf,0,sizeof(buf));
	wsprintf(buf,": %s",(LPTSTR)lpDetailMsgBuf);
    LocalFree(lpDetailMsgBuf);	
	
	header.append(string(buf));
	errCode = errorCode;
}

void MySemaphore::closeSemaphore()
{
    if ( !CloseHandle(m_semaphore) )
    {
		int errCode = 0;
		string header = "CloseHandle failed";
		getErrorDetail(errCode,header);
        MyException excp(errCode,header);
		throw excp;
    }
	m_semaphore = NULL;
}

BOOL MySemaphore::lock(BOOL wait)
{
	if ( wait == TRUE )
	{
		if ( WaitForSingleObject(m_semaphore,INFINITE) == WAIT_FAILED )	//信号量的值自动减1
		{
			int errCode = 0;
			string header = "WaitForSingleObject failed";
			getErrorDetail(errCode,header);
			MyException excp(errCode,header);
			throw excp;
		}
		return TRUE;
	}
	else //仅检测对象状态即返回
	{
		DWORD retVal = WaitForSingleObject(m_semaphore,0);

		if ( retVal == WAIT_TIMEOUT ) return FALSE;	//超时，对象状态无信号
		if ( retVal == WAIT_OBJECT_0) return TRUE;		//对象状态有信号

		if ( retVal == WAIT_FAILED )
		{
			int errCode = 0;
			string header = "WaitForSingleObject failed";
			getErrorDetail(errCode,header);
			MyException excp(errCode,header);
			throw excp;
		}
	}

	return FALSE;
}

void MySemaphore::unlock()
{
	if ( !ReleaseSemaphore(m_semaphore,1,NULL) )
	{
		int errCode = 0;
		string header = "ReleaseSemaphore failed: ";
		header.append(m_semaphoreName);
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
		throw excp;
	}
}

/*
long MySemaphore::getSemaphoreCount()
{
	long currCount = 0;

	if ( !ReleaseSemaphore(m_semaphore,0,&currCount) )
	{
		int errCode = 0;
		string header = "ReleaseSemaphore failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
		throw excp;
	}
    return currCount;
}
*/