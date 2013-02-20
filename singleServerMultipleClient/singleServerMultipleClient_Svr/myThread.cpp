#include "myThread.h"
#include "myException.h"

MyThread::MyThread(LPTHREAD_START_ROUTINE pThreadFunc,
				   LPVOID pThreadFuncParameter,DWORD exeFlags,
				   DWORD sSize,BOOL inheritable)
{
	m_pSA = (LPSECURITY_ATTRIBUTES)HeapAlloc(GetProcessHeap(),0,sizeof(SECURITY_ATTRIBUTES));
	m_pSA->nLength = sizeof(SECURITY_ATTRIBUTES);
    m_pSA->lpSecurityDescriptor = NULL;
	m_pSA->bInheritHandle = inheritable;

	m_threadFunction = pThreadFunc;
	m_threadArgument = pThreadFuncParameter;

	m_dExecuteFlag = exeFlags;
	m_dStackSize = sSize;
	m_dExitCode = 0xFFFFFFFF;

}

void MyThread::execute()
{
	if ( (m_hThread = CreateThread(m_pSA,m_dStackSize,m_threadFunction,m_threadArgument,m_dExecuteFlag,&m_dThreadId)) == NULL)
    {
		int errCode = 0;
		string header = "CreateThread() failed";
		getErrorDetail(errCode,header);
        MyException excp(errCode,header);
        throw excp;
        return;
    }
}

MyThread::~MyThread()
{
	if (m_pSA) 
	{
		HeapFree(GetProcessHeap(),0,m_pSA); 
		m_pSA = NULL;
	}
}

void MyThread::waitForThreadToFinish()
{
	if ( WaitForSingleObject(m_hThread,INFINITE) == WAIT_FAILED )
	{
		int errCode = 0;
		string header = "WaitForSingleObject() failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
		throw excp;
		return;
    }
}

DWORD MyThread::getExitCode()
{
	if ( GetExitCodeThread(m_hThread,&m_dExitCode) )
    {
		return m_dExitCode;
    }
    else
	{
		int errCode = 0;
		string header = "GetExitCodeThread() failed";
		getErrorDetail(errCode,header);
        MyException excp(errCode,header);
        throw excp;
    }
}

DWORD MyThread::getThreadId()
{
	return m_dThreadId;
}

void MyThread::resume()
{
	if ( ResumeThread(m_hThread) == 0xFFFFFFFF )
	{
		int errCode = 0;
		string header = "ResumeThread() failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
        throw excp;
        return;
    }
}

void MyThread::suspend()
{
	if ( SuspendThread(m_hThread) == 0xFFFFFFFF )
	{
		int errCode = 0;
		string header = "SuspendThread() failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
		throw excp;
        return;
    }
}

int MyThread::getThreadPriority()
{
    int retVal;
	if ( (retVal = GetThreadPriority(m_hThread)) == THREAD_PRIORITY_ERROR_RETURN )
	{
		int errCode;
		string header = "GetThreadPriority() failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
        throw excp;
        return 0;
    }
    return retVal;
}

void MyThread::setThreadPriority(int thePriority)
{
	if ( !SetThreadPriority(m_hThread,thePriority) )
    {
		int errCode = 0;
		string header = "SetThreadPriority() failed";
		getErrorDetail(errCode,header);
        MyException excp(errCode,header);
        throw excp;
    }
}

void MyThread::disablePriorityBoost(BOOL disable)
{
    if ( !SetThreadPriorityBoost(m_hThread,disable) )
	{
		int errCode = 0;
		string header = "SetThreadPriorityBoost() failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
        throw excp;
    }
}

BOOL MyThread::isPriorityBoostDisabled()
{
	BOOL boosted;

	if ( !GetThreadPriorityBoost(m_hThread,&boosted) )
	{
		int errCode = 0;
		string header = "GetThreadPriorityBoost() failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
		throw excp;
    }

	return boosted;
}

void MyThread::setProcessorMask(DWORD mask)
{
	if ( !SetThreadAffinityMask(m_hThread,mask) )
	{
		int errCode = 0;
		string header = "SetThreadAffinityMask() failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
		throw excp;
    }
}

void MyThread::setIdealProcessor(DWORD proc)
{
	if ( SetThreadIdealProcessor(m_hThread,proc) == -1 )
	{
		int errCode = 0;
		string header = "SetThreadIdealProcessor failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
		throw excp;
    }
}

void MyThread::getThreadStatistics(LPFILETIME  creationTime,LPFILETIME exitTime,
								   LPFILETIME kernelTime,LPFILETIME userTime)
{
	if ( !GetThreadTimes(m_hThread,creationTime,exitTime,kernelTime,userTime) )
    {
		int errCode = 0;
		string header = "GetThreadTimes() failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
        throw excp;
    }
}

void MyThread::getErrorDetail(int& errCode,string& header)
{
	char buf[256];
	LPVOID lpDetailMsgBuf;
	UINT errorCode;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
				  NULL,errorCode=GetLastError(),MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
				  (LPTSTR)&lpDetailMsgBuf,0,NULL);

	memset(buf,0,255);
	wsprintf(buf,": %s",(LPTSTR)lpDetailMsgBuf);
    LocalFree(lpDetailMsgBuf);	
	
	header.append(string(buf));
	errCode = errorCode;
}
