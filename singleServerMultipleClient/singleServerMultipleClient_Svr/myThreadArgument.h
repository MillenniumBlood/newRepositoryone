#ifndef myThreadArgument_H
#define myThreadArgument_H

class MySemaphore;
class MyTcpSocket;
class MyEvent;

const int MAX_NUM_CLIENTS = 10;

class MyThreadArgument
{
public:

	// you have to change here to fit your needs
	MyThreadArgument(MyTcpSocket* client,MySemaphore* coutSem,string& cName);
	~MyThreadArgument() ;

	// use the accessor functions that fit your needs

	MySemaphore* getCoutSemaphore()     { return m_pCoutSemaphore; }
	MyTcpSocket* getClientConnect()     { return m_pClientConnection; }
	string&      getHostName()          { return m_sHostName; }
	bool getSignalToEnd()               { return m_bSignalToEnd; }
	void setSignalToEnd(bool s)         { m_bSignalToEnd = s; }

	void addClientArgument(MyThreadArgument*);
	MyThreadArgument* getClientArgument(int);

	// do not change here
    MyEvent* getExitEvent() { return m_pExitEvent;}


private:
	// you have to change here to fit your needs
	MySemaphore* m_pCoutSemaphore;				// semaphore for accessing the log file
	MyTcpSocket* m_pClientConnection;			// the connecting socket to the client
	string m_sHostName;							// the name of the client

	MyThreadArgument* m_arrThreadArgument[MAX_NUM_CLIENTS];
	int m_numOfConnectedClients;

	// but the exitEvent is always needed
	MyEvent* m_pExitEvent;
	bool m_bSignalToEnd;
};

#endif