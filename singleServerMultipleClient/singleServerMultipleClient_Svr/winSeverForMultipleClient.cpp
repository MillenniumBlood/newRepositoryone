#include "mySocket.h"
#include "myLog.h"
#include "myException.h"
#include "myHostInfo.h"
#include "myThreadArgument.h"
#include "myEvent.h"
#include "myThread.h"
#include "mySemaphore.h"

MyLog winLog;

//与客户端通信线程
DWORD WINAPI clientHandleThread(LPVOID threadInfo)
{
	// this structure will contain all the data this callback will work on
    MyThreadArgument* clientArgument = (MyThreadArgument*)threadInfo;
		
	// the semamphore to protect the access to the std output
    MySemaphore* coutSemaphore = clientArgument->getCoutSemaphore();

	// get the client connection: receiving messages from client and
	// sending messages to the client will all be done by using
	// this client connection
	MyTcpSocket* clientConnection = clientArgument->getClientConnect();
	string clientName = clientArgument->getHostName();

	// the server is communicating with this client here
	while(1)
	{
		string messageFromClient = "";
		
		// receive from the client

        int numBytes = clientConnection->recieveMessage(messageFromClient);
		if ( numBytes == -99 ) break;

		// write to the console and the log file, so lock the semaphore
		coutSemaphore->lock();

		cout   << "[RECV fr " << clientName << "]: " << messageFromClient << endl;
		winLog << "[RECV fr " << clientName << "]: " << messageFromClient << endl;

		// if the client wants to discount
		if ( messageFromClient.compare("quit") == 0 || messageFromClient.compare("Quit") == 0 )
		{
			coutSemaphore->unlock();
			break;
		}
		else // send to the client
		{
			char messageToClient[MAX_MSG_LEN+1];
			memset(messageToClient,0,sizeof(messageToClient));
			cout << "[SEND to " << clientName << "]: ";
			cin.getline(messageToClient,MAX_MSG_LEN);
			winLog << "[SEND to " << clientName << "]: " << messageToClient << endl;
			clientConnection->sendMessage(string(messageToClient));
			coutSemaphore->unlock();
		}
    }

	// if we reach here, this session with the client is done,
	// so we set the event on this thread to inform the main
	// control that this session is finished	//回调通知main

	/******设置clientArgument中的 m_pExitEvent为有信号*****/
	clientArgument->getExitEvent()->setEvent();
	return 1;
}

//服务器线程
DWORD WINAPI serverHandleThread(LPVOID threadInfo)
{
	// this structure will contain all the data this callback will work on
    MyThreadArgument* serverArgument = (MyThreadArgument*)threadInfo; //线程参数
		
	// the semamphore to protect the access to the std output
    MySemaphore* coutSemaphore = serverArgument->getCoutSemaphore();

	// get the server
	MyTcpSocket* myServer = serverArgument->getClientConnect();
	string serverName = serverArgument->getHostName();

	// bind the server to the socket

	//****************将m_nSocketId(套接字)绑定到一个已知的地址上********************

    myServer->bindSocket();
	cout   << endl << "server finishes binding process... " << endl;
	winLog << endl << "server finishes binding process... " << endl;
	
	// server starts to wait for client calls
	myServer->listenToClient();
	cout   << "server is waiting for client calls ... " << endl;
	winLog << "server is waiting for client calls ... " << endl;
	
	// server starts to listen, and generates a thread to 
	// handle each client

	MyThreadArgument* arrClientArgument[MAX_NUM_CLIENTS];
	MyThread* arrClientHandle[MAX_NUM_CLIENTS];
	for ( int i = 0; i < MAX_NUM_CLIENTS; i++ )
	{
		arrClientArgument[i] = NULL;
		arrClientHandle[i] = NULL;
	}
	int currNumOfClients = 0;

	while ( 1 )
	{
		// wait to accept a client connection.  
		// processing is suspended until the client connects
    	MyTcpSocket* client;    // connection dedicated for client communication
		string clientName;      // client name 
		client = myServer->acceptClient(clientName);	
		clientName = clientName + "-" + char(65+currNumOfClients);

		// lock the std out so we can write to the console
		coutSemaphore->lock();
        cout   << endl << "==> A client from [" << clientName << "] is connected!" << endl << endl;
		winLog << endl << "==> A client from [" << clientName << "] is connected!" << endl << endl;
		coutSemaphore->unlock();

		// for this client, generate a thread to handle it
		if ( currNumOfClients < MAX_NUM_CLIENTS-1 )
		{
			//     new.........
			arrClientArgument[currNumOfClients] = new MyThreadArgument(client,coutSemaphore,clientName);
			//========服务线程保留与客户端通信线程的参数 MyThreadArgument
			serverArgument->addClientArgument(arrClientArgument[currNumOfClients]);

			//与客户端通信启动
			arrClientHandle[currNumOfClients] = new MyThread(clientHandleThread,(void*)arrClientArgument[currNumOfClients]);
			arrClientHandle[currNumOfClients]->execute();
			currNumOfClients++;
		}
	}

    return 1;
}

int main()
{
	// build a semaphore so we can synchronize the access to std cout
	// also includes the log file
	//标准输出同步访问信号量对象coutSemaphore initialCount=1 maxCount=1 
	MySemaphore coutSemaphore(string(""),1);

	// Initialize the winsock library
	MyTcpSocket::initialize();
	// open socket on the local host(server) and show its configuration
	MyTcpSocket myServer(PORTNUM);
	cout   << myServer;
	winLog << myServer;

	// create the server: local host will be used as the server, let us 
	// first use MyHostInfo class to show the name and IP address 
	// of the local host
	winLog << endl;
	winLog << "Retrieve the local host name and address:" << endl;
	
	//Host Info..
    MyHostInfo serverInfo;
	string serverName = serverInfo.getHostName();
    string serverIPAddress = serverInfo.getHostIPAddress();
	cout << "my localhost (server) information:" << endl;
	cout << "	Name:    " << serverName << endl;
    cout << "	Address: " << serverIPAddress << endl;
	winLog << "		==> Name: " << serverName << endl;
	winLog << "		==> Address: " << serverIPAddress << endl;

	//// open socket on the local host(server) and show its configuration
	//MyTcpSocket myServer(PORTNUM);
	//cout   << myServer;
	//winLog << myServer;

	// create a thread to implement server process: listening to the socket,
	// accepting client calls and communicating with clients. This will free the 
	// main control (see below) to do other process.
	MyThreadArgument* serverArgument = new MyThreadArgument(&myServer,&coutSemaphore,serverName);
	MyThread* serverThread = new MyThread(serverHandleThread,(void*)serverArgument);
	serverThread->execute();

	// main control: since the serverThread is handling the server functions,
	// this main control is free to do other things.
	while ( 1 )
	{
		// do whatever you need to do here, I am using Sleep(x) 
		// to make a little delay, pretending to be the other 
		// possible processings.
		Sleep(50000);

		// report the server status
		coutSemaphore.lock();

		cout   << endl << "-----------------------------------------------------------------" << endl;
		winLog << endl << "-----------------------------------------------------------------" << endl;
		cout   << "server (name:" << serverName << ") status report:" << endl;
		winLog << "server (name:" << serverName << ") status report:" << endl;

		cout   << "   the following clients have successfully connected with server: " << endl;
		winLog << "   the following clients have successfully connected with server: " << endl;
		for ( int i = 0; i < MAX_NUM_CLIENTS; i ++ )
		{
			MyThreadArgument* clientInfo = serverArgument->getClientArgument(i);
			if ( clientInfo ) 
			{
				cout   << "         " << clientInfo->getHostName() << endl;
				winLog << "         " << clientInfo->getHostName() << endl;
			}
		}

		cout   << "   the following clients have shutdown the connection: " << endl;
		winLog << "   the following clients have shutdown the connection: " << endl;
		for (int i = 0; i < MAX_NUM_CLIENTS; i ++ )
		{
			MyThreadArgument* clientArgument = serverArgument->getClientArgument(i);
			//测试clientArgument中的m_pExitEvent有信号，则通信连接已经结束
			if ( clientArgument && clientArgument->getExitEvent()->waitForEvent(0) )
			{
				clientArgument->setSignalToEnd(true);
				cout   << "         " << clientArgument->getHostName() << endl;
				winLog << "         " << clientArgument->getHostName() << endl;
			}
		}


        cout   << "-----------------------------------------------------------------" << endl << endl;
		winLog << "-----------------------------------------------------------------" << endl << endl;

		coutSemaphore.unlock();
	}

    return 1;
}

