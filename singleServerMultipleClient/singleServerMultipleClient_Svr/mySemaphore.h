#ifndef mySemaphore_H
#define mySemaphore_H

#define OPEN 0
#define CREATE 1

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0400
#endif

#include <windows.h>
#include <winuser.h>
#include <winbase.h>
#include <string>
using namespace std;

class MySemaphore
{

public:

    MySemaphore(string& sName=string(""),
		        long initialCount=0,long maxCount=1,
				int accessMode=CREATE,BOOL canInherit=TRUE);

    virtual ~MySemaphore();

public:

    void closeSemaphore();    
	BOOL lock(BOOL wait=TRUE);
    void unlock();

public:

	string& getSemaphoreName() { return m_semaphoreName; }
	long    getInitialCount()  { return m_initCount; }
    long    getMaxCount()      { return m_maxCount; }
	BOOL    getInerit()        { return m_inherit; }
	
private:

	void getErrorDetail(int&,string&);

private:

	long m_initCount;   // initialCount 
	long m_maxCount;    // max count
    BOOL m_inherit;     // can be inherited by child

    string m_semaphoreName;
    HANDLE m_semaphore;
    LPSECURITY_ATTRIBUTES m_sa;
};
#endif