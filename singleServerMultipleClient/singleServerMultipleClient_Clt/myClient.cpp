#include "mySocket.h"
#include "myLog.h"
#include "myException.h"
#include "myHostInfo.h"

MyLog winLog;
static void readServerConfig(string&);
static void checkFileExistence(const string&);

int main()
{
	// Initialize the winsock library
	MyTcpSocket::initialize();
        
	// get client's information (assume neither the name nor the address is given)
	winLog << endl;
	winLog << "Retrieve the localHost [CLIENT] name and address:" << endl;
	winLog.flush();

	//以下是客户端程序，所有服务器IP从配置文件中读取
	//*********************************************************************************
	//  【①】   客户端HostInfo(hostent)
    MyHostInfo clientInfo;
	string clientName = clientInfo.getHostName();
    string clientIPAddress = clientInfo.getHostIPAddress();
    cout << "Name: " << clientName << endl;
    cout << "Address: " << clientIPAddress << endl;
	winLog << "		==> Name: " << clientName << endl;
	winLog << "		==> Address: " << clientIPAddress << endl;

	//*********************************************************************************
	// get server's IP address and name 【服务器IP，从配置文件读取】
	string serverIPAddress = "";
	readServerConfig(serverIPAddress);
	winLog << endl;
	winLog << "Retrieve the remoteHost [SERVER] name and address:" << endl;
	winLog << "		==> the given address is " << serverIPAddress << endl;

	//  【②】   服务器端HostInfo(hostent)
    MyHostInfo serverInfo(serverIPAddress,ADDRESS);	
	string serverName = serverInfo.getHostName();
    cout << "Name: " << serverName << endl;
    cout << "Address: " << serverIPAddress << endl;
	winLog << "		==> Name: " << serverName << endl;
	winLog << "		==> Address: " << serverIPAddress << endl;

	//*********************************************************************************
    // create the socket for client
    MyTcpSocket myClient(PORTNUM);
	cout << myClient;
	winLog << "client configuation: " << endl;
	winLog << myClient;

	//*********************************************************************************
    // connect to the server.
	cout   << "connecting to the server [" << serverName << "] ... " << endl;
	winLog << "connecting to the server [" << serverName << "] ... " << endl;
	myClient.connectToServer(serverIPAddress,ADDRESS);

	int recvBytes = 0;
	while (1)
	{
		// send message to server
		char messageToServer[MAX_MSG_LEN+1];
		memset(messageToServer,0,sizeof(messageToServer));
		cout << "[SEND] ";
		cin.getline(messageToServer,MAX_MSG_LEN);

		winLog << "[SEND] " << messageToServer << endl;
		myClient.sendMessage(string(messageToServer));
		if ( !string(messageToServer).compare("Quit") || !string(messageToServer).compare("quit") ) break;

		// receive message from server
		string messageFromServer = "";
        recvBytes = myClient.recieveMessage(messageFromServer);
		if ( recvBytes == -99 ) break;

		cout   << "[RECV:" << serverName << "]: " << messageFromServer << endl;
		winLog << "[RECV:" << serverName << "]: " << messageFromServer << endl;

	}

    return EXIT_SUCCESS;
}

void readServerConfig(string& serverIPAddr)
{
	string serverConfigFile = "serverConfig.txt";	
	checkFileExistence(serverConfigFile);
	ifstream serverConfig(serverConfigFile.c_str());

	// read server's IP address
	getline(serverConfig,serverIPAddr);
	serverConfig.close();
}

void checkFileExistence(const string& fileName)
{
	ifstream file(fileName.c_str());
	if (!file) 
	{
		cout << "Cannot continue:" << fileName << " does NOT exist!" << endl;
		exit(1);
	}
	file.close();
}