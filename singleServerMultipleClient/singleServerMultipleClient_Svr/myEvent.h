#ifndef myEvent_H 
#define myEvent_H

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0400
#endif

#include <windows.h>
#include <string>
using namespace std;

class MyEvent  
{

public:
	
	MyEvent(string eventName="",PAPCFUNC _pfnAPC=NULL,BOOL queueUserAPC=FALSE);
    virtual ~MyEvent();

	// return the event handle
    HANDLE getEventHandle() { return m_event; }

	// return the thread handle
	HANDLE getThread() { return m_thread; }

	// return the address of the callback function 
    PAPCFUNC getAPCFunc() { return m_pfnAPC; }

	// return whether the user wants to queue the callback 
	// when event happens
	BOOL getQueueUserAPC() { return m_queueUserAPC; }

    void setEvent();      // wrapper to SetEvent()  
	void pulseEvent();    // wrapper to PulseEvent()
	void resetEvent();    // wrapper to ResetEvent()

	// wait for the event
    BOOL waitForEvent(DWORD waitTime=INFINITE);
    
private:

	// called by the OS when the thread is instantiated, this
	// must have the WINAPI function declaration
    static DWORD WINAPI ThreadProc(LPVOID lpData);

private:

    HANDLE m_event;                  // event handle
    PAPCFUNC m_pfnAPC;               // address of user-provided callback
	LPSECURITY_ATTRIBUTES m_sa;      // SECURITY_ATTRBUTES structure
    DWORD  m_threadID;               // thread id
    HANDLE m_thread;                 // handle to thread

	// if the following is set to be TRUE, when the events occurs, 
	// will call QueueUserAPC() to queue the user-provided callback
	// as a user defined API. if the following is set to be FALSE;
	// then no user-provided callback will be queued. 
	// if pfnAPC is NULL (no user-provided callback is defined),
	// then the following is ignored 
	BOOL m_queueUserAPC;        
	
private:

	static void getErrorDetail(int&,string&);

};

#endif
