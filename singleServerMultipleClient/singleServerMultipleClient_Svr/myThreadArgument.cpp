#include "myEvent.h"
#include "myThreadArgument.h"

MyThreadArgument::MyThreadArgument(MyTcpSocket* client,MySemaphore* coutSem,string& cName)
{
	m_pCoutSemaphore = coutSem;
	m_pClientConnection = client;
	m_sHostName = cName;

	m_pExitEvent = new MyEvent();	//����Ĭ�ϵ�MyEvent���󣬲���ʾ��ʼ��(����û���ṩ��ʾ��ʼ��ʽ)
	m_bSignalToEnd = false;

	for ( int i = 0; i < MAX_NUM_CLIENTS; i ++ ) m_arrThreadArgument[i] = NULL;
		m_numOfConnectedClients = 0;
}

MyThreadArgument::~MyThreadArgument() 
{
	delete m_pExitEvent;
}

void MyThreadArgument::addClientArgument(MyThreadArgument* argument)
{
	if ( argument )
		m_arrThreadArgument[m_numOfConnectedClients++] = argument;
}

MyThreadArgument* MyThreadArgument::getClientArgument(int index)
{
	if ( index < MAX_NUM_CLIENTS ) return m_arrThreadArgument[index];
	return NULL;
}


