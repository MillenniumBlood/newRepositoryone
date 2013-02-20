#include "myEvent.h"
#include "myException.h"

//���캯�����ĸ�����Ĭ��ʵ��
MyEvent::MyEvent(string eventName,PAPCFUNC _pfnAPC,BOOL _queueUserAPC)
{
	// set up the security features
    m_sa= (LPSECURITY_ATTRIBUTES) HeapAlloc(GetProcessHeap(),0,sizeof(SECURITY_ATTRIBUTES));
    m_sa->nLength= sizeof(SECURITY_ATTRIBUTES);
    m_sa->lpSecurityDescriptor= NULL;
    m_sa->bInheritHandle= TRUE;

	// create the event, it is manual reset, initial state is nonsignaled
	//��ҪReleaseEvent�ֶ���λ(�����ź�ResetEvent)
	//��ʼ���ź�״̬
	//Ĭ������
	if ((m_event= CreateEvent(m_sa,TRUE,FALSE,eventName.c_str())) == NULL)
	{
		int errCode = 0;
		string header = "CreateEvent() in MyEvent::MyEvent() failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
        throw excp;
	}

	// if a callback is provided,     ����>��create a thread to monitor
	// the event. When the event occurs��, queue the callback ��callback��������APC�����Ŷӽ�....
	// as an APC (if the user wants to do so)

    if ((m_pfnAPC=_pfnAPC) != NULL)	//Callback need _pfnAPC��NULL
    {
		m_queueUserAPC = _queueUserAPC;
		//�ص�����߳�
		if ((m_thread = CreateThread(m_sa,0,ThreadProc,this,0, &m_threadID)) == NULL)
		{
			int errCode = 0;
			string header = "CreateThread() in MyEvent::MyEvent() failed";
			getErrorDetail(errCode,header);
			MyException excp(errCode,header);
			throw excp;
		}
	}
	else m_thread= NULL;
}

MyEvent::~MyEvent()
{
	cout << "I am in MyEvent destructor..." << endl;

    HeapFree(GetProcessHeap(),0,m_sa); 
    if (m_thread) CloseHandle(m_thread);
    CloseHandle(m_event);
}

//�ȴ�
BOOL MyEvent::waitForEvent(DWORD waitTime)
{
    DWORD retVal;
	
	if ((retVal = WaitForSingleObjectEx(m_event,waitTime,TRUE)) == WAIT_FAILED)	//0xFFFFFFFF �ȴ�ʧ��
	{
		int errCode = 0;
		string header = "WaitForSingleObjectEx() in MyEvent::waitForEvent() failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
		throw excp;
	}
	if (retVal == WAIT_TIMEOUT) return FALSE;		//��ʱ
    else return TRUE;								//�ɹ�
}

//�¼���λ(���ź�)
void MyEvent::pulseEvent()
{
	if (!PulseEvent(m_event))
	{
		int errCode = 0;
		string header = "PulseEvent() in MyEvent::pulseEvent() failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
		throw excp;
	}
}

//�¼���λ(���ź�)
void MyEvent::setEvent()
{
	if (!SetEvent(m_event))
	{
		int errCode = 0;
		string header = "SetEvent() in MyEvent::setEvent() failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
		throw excp;
	}
}

//�¼���λ(���ź�)
void MyEvent::resetEvent()
{
	if (!ResetEvent(m_event))
	{
		int errCode = 0;
		string header = "ResetEvent() in MyEvent::resetEvent() failed";
		getErrorDetail(errCode,header);
		MyException excp(errCode,header);
		throw excp;
	}
}

//�¼�����
DWORD WINAPI MyEvent::ThreadProc(LPVOID lpData)
{ 
    // winLog << "[THREAD] the thread is started by OS (ThreadProc) ... " << endl;

    MyEvent* pEvent = (MyEvent*)lpData;	//���ε�MyEvent����
	DWORD waitRetVal = 0;

    while (1)
    {
		// winLog << "[THREAD] calling WaitForSingleObjectEx() ... " << endl;

		// wait on this event,no time_out and APC can finish the waiting
		waitRetVal = WaitForSingleObjectEx(pEvent->getEventHandle(),INFINITE,TRUE);
		// winLog << "[THREAD] WaitForSingleOjectEx() returned ... return value = " << waitRetVal << endl;

		if ( waitRetVal == WAIT_FAILED ) //0xFFFFFFFFF����WAIT_FAILED
		{
			int errCode = 0;
			string header = "WaitForSingleObjectEx() in MyEvent::ThreadProc() failed";
			getErrorDetail(errCode,header);
			MyException excp(errCode,header);
			throw excp;
		}
		else if ( waitRetVal == WAIT_OBJECT_0 )	//m_Event����λ�����ź�
		{
			cout << "the state of the specified event object is signaled." << endl;
			// winLog << "[THREAD] the state of the specified event object is signaled." << endl;
		}
		else if ( waitRetVal == WAIT_ABANDONED ) //����
		{
			cout << "The specified object is a mutex object that was not released by the thread" << endl;
			cout << "that owned the mutex object before the owning thread terminated." << endl;
			cout << "Ownership of the mutex object is granted to the calling thread, " << endl;
			cout << "and the mutex is set to nonsignaled." << endl;
		}
		else if ( waitRetVal == WAIT_IO_COMPLETION )
		{
			cout   << "One or more I/O completion routines are queued for execution." << endl;
			// winLog << "[THREAD] One or more I/O completion routines are queued for execution." << endl;
		}
		else if ( waitRetVal == WAIT_TIMEOUT )
			cout << "The time-out interval elapsed, and the object's state is nonsignaled." << endl; 

		if ( pEvent->getQueueUserAPC() == TRUE ) //��Ҫm_queueUserAPC����TRUE
		{
			cout << "schedule the callback..." << endl;
			// winLog << "[THREAD] calling QueueUserAPC() to schedule the callback ... " << endl;

			if ( !QueueUserAPC(pEvent->getAPCFunc(),GetCurrentThread(),(DWORD)lpData))
			{
				int errCode = 0;
				string header = "QueueUserAPC() in MyEvent::ThreadProc() failed";
				getErrorDetail(errCode,header);
				MyException excp(errCode,header);
				throw excp;
			}
			// winLog << "[THREAD] finished schedule the callback... " << endl;
			SleepEx(INFINITE,TRUE);  // INFINITE to make sure the callback is called!!
		}
		break;
    }
	
	// winLog << "[THREAD] this thread is done... " << endl; 
    return 0;
}

void MyEvent::getErrorDetail(int& errCode,string& header)
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