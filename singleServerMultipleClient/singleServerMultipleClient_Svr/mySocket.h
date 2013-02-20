#ifndef mySocket_H
#define mySocket_H

#include "myHostInfo.h"

#ifdef UNIX
    #include <sys/socket.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <iostream.h>
    #include <sys/types.h>
    #include <stropts.h>
    #include <sys/filio.h>
#else
    #include <winsock2.h>
#endif

// so far we only consider the TCP socket, UDP will be added in later release
//const int MAX_RECV_LEN = 1048576;
const int MAX_RECV_LEN = 8096;
const int MAX_MSG_LEN = 1024;
const int PORTNUM = 1200;

class MySocket
{
public:

    MySocket(int);                       // given a port number, create a socket

    virtual ~MySocket()
    {
		#ifdef WINDOWS_XP
            closesocket(m_nSocketId);
        #else
            close(m_nSocketId);
        #endif
     }

protected:
	/*
	   only used when the socket is used for client communication
	   once this is done, the next necessary call is setSocketId(int)
	*/
	MySocket() {}
	void setSocketId(int socketFd) { m_nSocketId = socketFd; }

public:

	// socket options : ON/OFF

    void setDebug(int);
    void setReuseAddr(int);
    void setKeepAlive(int);
    void setLingerOnOff(bool);
	void setLingerSeconds(int);
    void setSocketBlocking(int);

    // size of the send and receive buffer

    void setSendBufSize(int);
    void setReceiveBufSize(int);

    // retrieve socket option settings 取得.....

    int  getDebug();
    int  getReuseAddr();
    int  getKeepAlive();
    int  getSendBufSize();
    int  getReceiveBufSize();
    int  getSocketBlocking() { return m_nBlocking; }
	int  getLingerSeconds();
    bool getLingerOnOff();
	
    // returns the socket file descriptor
    int getSocketId() { return m_nSocketId; }

	// returns the port number
	int getPortNumber() { return m_nPortNumber; }

	// show the socket 
	friend ostream& operator<<(ostream&,MySocket&);

private:
	// Gets the system error
	#ifdef WINDOWS_XP
		void detectErrorOpenWinSocket(int*,string&);
		void detectErrorSetSocketOption(int*,string&);
		void detectErrorGetSocketOption(int*,string&);
	#endif

    #ifdef UNIX
		char *sGetError()
		{
           return strerror(errno);
        }
	#endif

protected:
	int m_nPortNumber;        // Socket port number    
	int m_nSocketId;          // Socket file descriptor

	int m_nBlocking;          // Blocking flag
	int m_nBindFlag;          // Binding flag

	struct sockaddr_in m_stutNetAddr;    //地址 Address of the client that sent data
};

class MyTcpSocket : public MySocket
{

private:

	#ifdef WINDOWS_XP
		// Windows version of the MSG_WAITALL option
		int XPrecieveMessage(string&);
	#endif

public:

	// initialization work, the clean up is in the destructor
	static void initialize();

public:

	/* 
	   Constructor. used for creating instances dedicated to client 
	   communication:

	   when accept() is successful, a socketId is generated and returned
	   this socket id is then used to build a new socket using the following
	   constructor, therefore, the next necessary call should be setSocketId()
	   using this newly generated socket fd
	*/
	MyTcpSocket() {};
	~MyTcpSocket();

	// Constructor.  Used to create a new TCP socket given a port
	MyTcpSocket(int portId) : MySocket(portId) { };			//调用父类构造函数

	/*
	   Binds the socket to an address and port number
	   a server call
	*/
	void bindSocket();

	// Listens to connecting clients, a server call
	void listenToClient(int numPorts = 5);

	/*
	   accepts a connecting client.  The address of the connected client 
	   is stored in the parameter
	   a server call
	*/
	MyTcpSocket* acceptClient(string&);

	/*
	   Sends a message to a connected host. The number of bytes sent is returned
	   can be either server call or client call
	*/
	int sendMessage(string&);

	/*
	   receive messages and stores the message in a buffer
	*/
	int recieveMessage(string&);

	// connect to the server, a client call //客户端才需要connect server
	virtual void connectToServer(string&,hostType);

private:

	void detectErrorBind(int*,string&);
	void detectErrorSend(int*,string&);
	void detectErrorRecv(int*,string&);
	void detectErrorConnect(int*,string&);
    void detectErrorAccept(int*,string&);
	void detectErrorListen(int*,string&);

};

#endif
        
