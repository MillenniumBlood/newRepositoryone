#ifndef myThread_H
#define myThread_H

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0400
#endif

#include <windows.h>
#include <string>
using namespace std;

class MyThread  
{

public:

    MyThread(
		LPTHREAD_START_ROUTINE pThreadFunc,    // address of thread CallBack
		LPVOID pThreadFuncParameter=NULL,      // address of callback's parameter,
		DWORD  exeFlags=0,                     // creation flag
		DWORD  sSize=0,                        // stack size
		BOOL   inheritable=FALSE               // inheritable
	);        

    ~MyThread();

public:

	void execute();
	void waitForThreadToFinish(); 		//等待线程结束   
	void suspend();								//暂停、挂起
    void resume();								//重启

	void setThreadPriority(int);			//get/set 线程优先级
    int  getThreadPriority();

    void disablePriorityBoost(BOOL);
	BOOL isPriorityBoostDisabled();

    void setProcessorMask(DWORD);
	void setIdealProcessor(DWORD);

	LPVOID getThreadArgument() { return m_threadArgument; }
    DWORD getExitCode();    
	DWORD getThreadId();
	void  getThreadStatistics(LPFILETIME,LPFILETIME,LPFILETIME,LPFILETIME);		//get统计

private:
	
    HANDLE m_hThread;
    DWORD  m_dThreadId;

	LPTHREAD_START_ROUTINE m_threadFunction;
	LPVOID m_threadArgument;
	DWORD  m_dStackSize;
	DWORD  m_dExecuteFlag;

    LPSECURITY_ATTRIBUTES m_pSA;
    DWORD m_dExitCode;

private:

    static void getErrorDetail(int&,string&);
};
#endif