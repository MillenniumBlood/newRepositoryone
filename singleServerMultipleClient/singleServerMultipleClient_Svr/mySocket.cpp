#include "mySocket.h"
#include "myException.h"
#include "myLog.h"

const int MSG_HEADER_LEN = 6;

MySocket::MySocket(int pNumber)
{
    m_nPortNumber = pNumber;
    m_nBlocking = 1;

    try
    {
		if ( (m_nSocketId=socket(AF_INET,SOCK_STREAM,0)) == -1)
        {
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "";
				detectErrorOpenWinSocket(&errorCode,errorMsg);
				MyException openWinSocketException(errorCode,errorMsg); //openWinSocketException对象携带异常信息并向上抛出
				throw openWinSocketException;
			#endif

			#ifdef UNIX
				MyException openUnixSocketException(0,"unix: error getting host by name");
				throw openUnixSocketException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}

    /* 
	   set the initial address of client that shall be communicated with to 
	   any address as long as they are using the same port number. 
	   The clientAddr structure is used in the future for storing the actual
	   address of client applications with which communication is going
	   to start
	*/
    m_stutNetAddr.sin_family = AF_INET;
    m_stutNetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_stutNetAddr.sin_port = htons(m_nPortNumber);
}
    
void MySocket::setDebug(int debugToggle)
{
	try 
	{
		if ( setsockopt(m_nSocketId,SOL_SOCKET,SO_DEBUG,(char *)&debugToggle,sizeof(debugToggle)) == -1 )
		{
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "DEBUG option:";
				detectErrorSetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#endif

			#ifdef UNIX
				MyException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
}

void MySocket::setReuseAddr(int reuseToggle)
{
	try 
	{
		if ( setsockopt(m_nSocketId,SOL_SOCKET,SO_REUSEADDR,(char *)&reuseToggle,sizeof(reuseToggle)) == -1 )
		{
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "REUSEADDR option:";
				detectErrorSetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#endif

			#ifdef UNIX
				MyException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
} 

void MySocket::setKeepAlive(int aliveToggle)
{
	try 
	{
		if ( setsockopt(m_nSocketId,SOL_SOCKET,SO_KEEPALIVE,(char *)&aliveToggle,sizeof(aliveToggle)) == -1 )
		{
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "ALIVE option:";
				detectErrorSetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#endif

			#ifdef UNIX
				MyException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
} 

void MySocket::setLingerSeconds(int seconds)
{
	struct linger lingerOption;
	
	if ( seconds > 0 )
	{
		lingerOption.l_linger = seconds;
		lingerOption.l_onoff = 1;
	}
	else lingerOption.l_onoff = 0;
	 
	try 
	{
		if ( setsockopt(m_nSocketId,SOL_SOCKET,SO_LINGER,(char *)&lingerOption,sizeof(struct linger)) == -1 )
		{
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "LINGER option:";
				detectErrorSetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#endif

			#ifdef UNIX
				MyException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
}

void MySocket::setLingerOnOff(bool lingerOn)
{
	struct linger lingerOption;

	if ( lingerOn ) lingerOption.l_onoff = 1;
	else lingerOption.l_onoff = 0;

	try 
	{
		if ( setsockopt(m_nSocketId,SOL_SOCKET,SO_LINGER,(char *)&lingerOption,sizeof(struct linger)) == -1 )
		{
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "LINGER option:";
				detectErrorSetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#endif

			#ifdef UNIX
				MyException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
}

void MySocket::setSendBufSize(int sendBufSize)
{
	try 
	{
		if ( setsockopt(m_nSocketId,SOL_SOCKET,SO_SNDBUF,(char *)&sendBufSize,sizeof(sendBufSize)) == -1 )
		{
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "SENDBUFSIZE option:";
				detectErrorSetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#endif

			#ifdef UNIX
				MyException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
} 

void MySocket::setReceiveBufSize(int receiveBufSize)
{
	try 
	{
		if ( setsockopt(m_nSocketId,SOL_SOCKET,SO_RCVBUF,(char *)&receiveBufSize,sizeof(receiveBufSize)) == -1 )
		{
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "RCVBUF option:";
				detectErrorSetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#endif

			#ifdef UNIX
				MyException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
}

void MySocket::setSocketBlocking(int blockingToggle)
{
    if (blockingToggle)
    {
        if (getSocketBlocking()) return;
        else m_nBlocking = 1;
	}
	else
	{
		if (!getSocketBlocking()) return;
		else m_nBlocking = 0;
	}

	try 
	{
		#ifdef WINDOWS_XP
			if (ioctlsocket(m_nSocketId,FIONBIO,(unsigned long *)&m_nBlocking) == -1)
			{
				int errorCode;
				string errorMsg = "Blocking option: ";
				detectErrorSetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			}
		#endif

		#ifdef UNIX
			if (ioctl(m_nSocketId,FIONBIO,(char *)&m_nBlocking) == -1)
			{
				MyException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			}
		#endif
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
}

int MySocket::getDebug()
{
    int myOption;
    int myOptionLen = sizeof(myOption);

	try 
	{
		if ( getsockopt(m_nSocketId,SOL_SOCKET,SO_DEBUG,(char *)&myOption,&myOptionLen) == -1 )
		{
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "get DEBUG option: ";
				detectErrorGetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#endif

			#ifdef UNIX
				MyException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
    
    return myOption;
}

int MySocket::getReuseAddr()
{
    int myOption;        
    int myOptionLen = sizeof(myOption);

	try 
	{
		if ( getsockopt(m_nSocketId,SOL_SOCKET,SO_REUSEADDR,(char *)&myOption,&myOptionLen) == -1 )
		{
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "get REUSEADDR option: ";
				detectErrorGetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#endif

			#ifdef UNIX
				MyException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}

	return myOption;
}

int MySocket::getKeepAlive()
{
    int myOption;        
    int myOptionLen = sizeof(myOption);

	try 
	{
		if ( getsockopt(m_nSocketId,SOL_SOCKET,SO_KEEPALIVE,(char *)&myOption,&myOptionLen) == -1 )
		{
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "get KEEPALIVE option: ";
				detectErrorGetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#endif

			#ifdef UNIX
				MyException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
    return myOption;    
}

int MySocket::getLingerSeconds()
{
	struct linger lingerOption;
	int myOptionLen = sizeof(struct linger);

	try 
	{
		if ( getsockopt(m_nSocketId,SOL_SOCKET,SO_LINGER,(char *)&lingerOption,&myOptionLen) == -1 )
		{
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "get LINER option: ";
				detectErrorGetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#endif

			#ifdef UNIX
				MyException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}

	return lingerOption.l_linger;
}

bool MySocket::getLingerOnOff()
{
	struct linger lingerOption;
	int myOptionLen = sizeof(struct linger);

	try 
	{
		if ( getsockopt(m_nSocketId,SOL_SOCKET,SO_LINGER,(char *)&lingerOption,&myOptionLen) == -1 )
		{
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "get LINER option: ";
				detectErrorGetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#endif

			#ifdef UNIX
				MyException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}

	if ( lingerOption.l_onoff == 1 ) return true;
	else return false;
}

int MySocket::getSendBufSize()
{
    int sendBuf;
    int myOptionLen = sizeof(sendBuf);

	try 
	{
		if ( getsockopt(m_nSocketId,SOL_SOCKET,SO_SNDBUF,(char *)&sendBuf,&myOptionLen) == -1 )
		{
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "get SNDBUF option: ";
				detectErrorGetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#endif

			#ifdef UNIX
				MyException unixSocketOptionException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
    return sendBuf;
}    

int MySocket::getReceiveBufSize()
{
    int rcvBuf;
    int myOptionLen = sizeof(rcvBuf);

	try 
	{
		if ( getsockopt(m_nSocketId,SOL_SOCKET,SO_RCVBUF,(char *)&rcvBuf,&myOptionLen) == -1 )
		{
			#ifdef WINDOWS_XP
				int errorCode;
				string errorMsg = "get RCVBUF option: ";
				detectErrorGetSocketOption(&errorCode,errorMsg);
				MyException socketOptionException(errorCode,errorMsg);
				throw socketOptionException;
			#endif

			#ifdef UNIX
				MyException unixSocketOptionException = new MyException(0,"unix: error getting host by name");
				throw unixSocketOptionException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
    return rcvBuf;
}

#ifdef WINDOWS_XP
void MySocket::detectErrorOpenWinSocket(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("Successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem or the associated service provider has failed.");
	else if ( *errCode == WSAEAFNOSUPPORT )
		errMsg.append("The specified address family is not supported.");
	else if ( *errCode == WSAEINPROGRESS )
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
	else if ( *errCode == WSAEMFILE )
		errMsg.append("No more socket descriptors are available.");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available. The socket cannot be created.");
	else if ( *errCode == WSAEPROTONOSUPPORT )
		errMsg.append("The specified protocol is not supported.");
	else if ( *errCode == WSAEPROTOTYPE )
		errMsg.append("The specified protocol is the wrong type for this socket.");
	else if ( *errCode == WSAESOCKTNOSUPPORT )
		errMsg.append("The specified socket type is not supported in this address family.");
	else errMsg.append("unknown problems!");
}

void MySocket::detectErrorSetSocketOption(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEFAULT )
		errMsg.append("optval is not in a valid part of the process address space or optlen parameter is too small.");
	else if ( *errCode == WSAEINPROGRESS )
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
	else if ( *errCode == WSAEINVAL )
		errMsg.append("level is not valid, or the information in optval is not valid.");
	else if ( *errCode == WSAENETRESET )
		errMsg.append("Connection has timed out when SO_KEEPALIVE is set.");
	else if ( *errCode == WSAENOPROTOOPT )
		errMsg.append("The option is unknown or unsupported for the specified provider or socket (see SO_GROUP_PRIORITY limitations).");
	else if ( *errCode == WSAENOTCONN )
		errMsg.append("Connection has been reset when SO_KEEPALIVE is set.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else errMsg.append("unknown problem!");
}

void MySocket::detectErrorGetSocketOption(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEFAULT )
		errMsg.append("One of the optval or the optlen parameters is not a valid part of the user address space, or the optlen parameter is too small.");
	else if ( *errCode == WSAEINPROGRESS )
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
	else if ( *errCode == WSAEINVAL )
		errMsg.append("The level parameter is unknown or invalid.");
	else if ( *errCode == WSAENOPROTOOPT )
		errMsg.append("The option is unknown or unsupported by the indicated protocol family.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");

	else errMsg.append("unknown problems!");
}

#endif

ostream& operator<<(ostream& lhIOobj,MySocket& rhs)	//"<<"操作符,作操作数lhIOobj、右操作数rhs是MyTcpSocket的父类引用
{
	string flagStr = "";

	lhIOobj << endl;
	lhIOobj << "Summary of socket settings:" << endl;
	lhIOobj << "   Socket Id:     " << rhs.getSocketId() << endl;
	lhIOobj << "   port #:        " << rhs.getPortNumber() << endl;
	lhIOobj << "   debug:         " << (flagStr = rhs.getDebug()? "true":"false" ) << endl;
	lhIOobj << "   reuse addr:    " << (flagStr = rhs.getReuseAddr()? "true":"false" ) << endl;
	lhIOobj << "   keep alive:    " << (flagStr = rhs.getKeepAlive()? "true":"false" ) << endl;
	lhIOobj << "   send buf size: " << rhs.getSendBufSize() << endl;
	lhIOobj << "   recv bug size: " << rhs.getReceiveBufSize() << endl;
	lhIOobj << "   blocking:      " << (flagStr = rhs.getSocketBlocking()? "true":"false" ) << endl;
	lhIOobj << "   linger on:     " << (flagStr = rhs.getLingerOnOff()? "true":"false" ) << endl;
	lhIOobj << "   linger seconds: " << rhs.getLingerSeconds() << endl;
	lhIOobj << endl;
	return lhIOobj;
}

MyTcpSocket::~MyTcpSocket()
{
#ifdef WINDOWS_XP
    // Close the winsock library
	winLog << endl << "system shut down ...";		
	try 
	{
		if (WSACleanup())
		{
			MyException cleanupException(0,"Error: calling WSACleanup()");
			throw cleanupException;
        }
	}
	catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
	winLog << "successful" << endl;
#endif
}

void MyTcpSocket::initialize()
{
#ifdef WINDOWS_XP
	WSADATA wsaData;
    winLog << "system started ..." << endl;
	winLog << endl << "initialize the winsock library ... ";
	try 
	{
		if (WSAStartup(0x101, &wsaData))
		{
			MyException initializationException(0,"Error: calling WSAStartup()");
			throw initializationException;
        }
	}
	catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
	winLog << "successful" << endl;
#endif
}

void MyTcpSocket::bindSocket()
{
	try
	{
		if (bind(m_nSocketId,(struct sockaddr *)&m_stutNetAddr,sizeof(struct sockaddr_in))==-1)
		{
			#ifdef WINDOWS_XP
				int errorCode = 0;
				string errorMsg = "error calling bind(): \n";
				detectErrorBind(&errorCode,errorMsg);
				MyException socketBindException(errorCode,errorMsg);
				throw socketBindException;
			#endif
			#ifdef UNIX
				MyException unixSocketBindException(0,"unix: error calling bind()");
				throw unixSocketBindException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
}

#ifdef WINDOWS_XP

void MyTcpSocket::detectErrorBind(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEADDRINUSE )
	{
		errMsg.append("A process on the machine is already bound to the same\n");
		errMsg.append("fully-qualified address and the socket has not been marked\n"); 
		errMsg.append("to allow address re-use with SO_REUSEADDR. For example,\n");
		errMsg.append("IP address and port are bound in the af_inet case");
	}
	else if ( *errCode == WSAEADDRNOTAVAIL )
		errMsg.append("The specified address is not a valid address for this machine.");
	else if ( *errCode == WSAEFAULT )
	{
		errMsg.append("The name or the namelen parameter is not a valid part of\n");
		errMsg.append("the user address space, the namelen parameter is too small,\n");
		errMsg.append("the name parameter contains incorrect address format for the\n");
		errMsg.append("associated address family, or the first two bytes of the memory\n");
		errMsg.append("block specified by name does not match the address family\n");
		errMsg.append("associated with the socket descriptor s.");
	}
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the\n");
		errMsg.append("service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEINVAL )
		errMsg.append("The socket is already bound to an address. ");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("Not enough buffers available, too many connections.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else errMsg.append("unknown problems!");
}

void MyTcpSocket::detectErrorRecv(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEFAULT )
		errMsg.append("The buf parameter is not completely contained in a valid part of the user address space.");
	else if ( *errCode == WSAENOTCONN )
		errMsg.append("The socket is not connected.");
	else if ( *errCode == WSAEINTR )
		errMsg.append("The (blocking) call was canceled through WSACancelBlockingCall.");
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the\n");
		errMsg.append("service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAENETRESET )
	{
		errMsg.append("The connection has been broken due to the keep-alive activity\n");
		errMsg.append("detecting a failure while the operation was in progress.");
	}
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAEOPNOTSUPP )
	{
		errMsg.append("MSG_OOB was specified, but the socket is not stream-style\n");
		errMsg.append("such as type SOCK_STREAM, out-of-band data is not supported\n");
		errMsg.append("in the communication domain associated with this socket, or\n");
		errMsg.append("the socket is unidirectional and supports only send operations.");
	}
	else if ( *errCode == WSAESHUTDOWN )
	{
		errMsg.append("The socket has been shut down; it is not possible to recv on a\n");
		errMsg.append("socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.");
	}
	else if ( *errCode == WSAEWOULDBLOCK )
		errMsg.append("The socket is marked as nonblocking and the receive operation would block.");
	else if ( *errCode == WSAEMSGSIZE )
		errMsg.append("The message was too large to fit into the specified buffer and was truncated.");
	else if ( *errCode == WSAEINVAL )
	{
		errMsg.append("The socket has not been bound with bind, or an unknown flag\n");
		errMsg.append("was specified, or MSG_OOB was specified for a socket with\n");
		errMsg.append("SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative.");
	}
	else if ( *errCode == WSAECONNABORTED )
	{
		errMsg.append("The virtual circuit was terminated due to a time-out or\n");
		errMsg.append("other failure. The application should close the socket as it is no longer usable.");
	}
	else if ( *errCode == WSAETIMEDOUT )
	{
		errMsg.append("The connection has been dropped because of a network\n");
		errMsg.append("failure or because the peer system failed to respond.");
	}
	else if ( *errCode == WSAECONNRESET )
	{
		errMsg.append("The virtual circuit was reset by the remote side executing a\n");
		errMsg.append("\"hard\" or \"abortive\" close. The application should close\n");
		errMsg.append("the socket as it is no longer usable. On a UDP datagram socket\n");
		errMsg.append("this error would indicate that a previous send operation\n");
		errMsg.append("resulted in an ICMP \"Port Unreachable\" message.");
	}
	else errMsg.append("unknown problems!");
}

void MyTcpSocket::detectErrorConnect(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEADDRINUSE )
	{
		errMsg.append("The socket's local address is already in use and the socket\n");
		errMsg.append("was not marked to allow address reuse with SO_REUSEADDR. This\n");
		errMsg.append("error usually occurs when executing bind, but could be delayed\n");
		errMsg.append("until this function if the bind was to a partially wild-card\n");
		errMsg.append("address (involving ADDR_ANY) and if a specific address needs\n");
		errMsg.append("to be committed at the time of this function.");
	}
	else if ( *errCode == WSAEINTR )
		errMsg.append("The (blocking) Windows Socket 1.1 call was canceled through WSACancelBlockingCall.");
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or\n");
		errMsg.append("the service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEALREADY )
	{
		errMsg.append("A nonblocking connect call is in progress on the specified socket.\n");
		errMsg.append("Note In order to preserve backward compatibility, this error is\n");
		errMsg.append("reported as WSAEINVAL to Windows Sockets 1.1 applications that\n");
		errMsg.append("link to either WINSOCK.DLL or WSOCK32.DLL.");
	}
	else if ( *errCode == WSAEADDRNOTAVAIL )
		errMsg.append("The remote address is not a valid address (such as ADDR_ANY).");
	else if ( *errCode == WSAEAFNOSUPPORT )
		errMsg.append("Addresses in the specified family cannot be used with this socket.");
	else if ( *errCode == WSAECONNREFUSED )
		errMsg.append("The attempt to connect was forcefully rejected.");
	else if ( *errCode == WSAEFAULT )
	{
		errMsg.append("The name or the namelen parameter is not a valid part of\n");
		errMsg.append("the user address space, the namelen parameter is too small,\n");
		errMsg.append("or the name parameter contains incorrect address format for\n");
		errMsg.append("the associated address family.");
	}
	else if ( *errCode == WSAEINVAL )
	{
		errMsg.append("The parameter s is a listening socket, or the destination\n");
		errMsg.append("address specified is not consistent with that of the constrained\n");
		errMsg.append("group the socket belongs to.");
	}
	else if ( *errCode == WSAEISCONN )
		errMsg.append("The socket is already connected (connection-oriented sockets only).");
	else if ( *errCode == WSAENETUNREACH )
		errMsg.append("The network cannot be reached from this host at this time.");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available. The socket cannot be connected.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAETIMEDOUT )
		errMsg.append("Attempt to connect timed out without establishing a connection.");
	else if ( *errCode == WSAEWOULDBLOCK )
	{
		errMsg.append("The socket is marked as nonblocking and the connection\n");
		errMsg.append("cannot be completed immediately.");
	}
	else if ( *errCode == WSAEACCES )
	{
		errMsg.append("Attempt to connect datagram socket to broadcast address failed\n");
		errMsg.append("because setsockopt option SO_BROADCAST is not enabled.");
	}
	else errMsg.append("unknown problems!");
}

void MyTcpSocket::detectErrorAccept(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this FUNCTION.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEFAULT )
		errMsg.append("The addrlen parameter is too small or addr is not a valid part of the user address space.");
	else if ( *errCode == WSAEINTR )
		errMsg.append("A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall.");
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the\n");
		errMsg.append("service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEINVAL )
		errMsg.append("The listen function was not invoked prior to accept.");
	else if ( *errCode == WSAEMFILE )
		errMsg.append("The queue is nonempty upon entry to accept and there are no descriptors available.");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAEOPNOTSUPP )
		errMsg.append("The referenced socket is not a type that supports connection-oriented service.");
	else if ( *errCode == WSAEWOULDBLOCK )
		errMsg.append("The socket is marked as nonblocking and no connections are present to be accepted.");
	else errMsg.append("unknown problems!");
}

void MyTcpSocket::detectErrorListen(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEADDRINUSE )
	{
		errMsg.append("The socket's local address is already in use and the socket was\n");
		errMsg.append("not marked to allow address reuse with SO_REUSEADDR. This error\n");
		errMsg.append("usually occurs during execution of the bind function, but could\n");
		errMsg.append("be delayed until this function if the bind was to a partially\n");
		errMsg.append("wild-card address (involving ADDR_ANY) and if a specific address\n");
		errMsg.append("needs to be \"committed\" at the time of this function.");
	}
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress, or the service\n");
		errMsg.append("provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEINVAL )
		errMsg.append("The socket has not been bound with bind.");
	else if ( *errCode == WSAEISCONN )
		errMsg.append("The socket is already connected.");
	else if ( *errCode == WSAEMFILE )
		errMsg.append("No more socket descriptors are available.");
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAEOPNOTSUPP )
		errMsg.append("The referenced socket is not of a type that supports the listen operation.");
	else errMsg.append("unknown problems!");
}

void MyTcpSocket::detectErrorSend(int* errCode,string& errMsg)
{
	*errCode = WSAGetLastError();

	if ( *errCode == WSANOTINITIALISED )
		errMsg.append("A successful WSAStartup must occur before using this function.");
	else if ( *errCode == WSAENETDOWN )
		errMsg.append("The network subsystem has failed.");
	else if ( *errCode == WSAEACCES )
	{
		errMsg.append("The requested address is a broadcast address,\n");
		errMsg.append("but the appropriate flag was not set. Call setsockopt\n");
		errMsg.append("with the SO_BROADCAST parameter to allow the use of the broadcast address.");
	}
	else if ( *errCode == WSAEINTR )
	{
		errMsg.append("A blocking Windows Sockets 1.1 call was canceled\n");
		errMsg.append("through WSACancelBlockingCall.");
	}
	else if ( *errCode == WSAEINPROGRESS )
	{
		errMsg.append("A blocking Windows Sockets 1.1 call is in progress,\n");
		errMsg.append("or the service provider is still processing a callback function.");
	}
	else if ( *errCode == WSAEFAULT )
	{
		errMsg.append("The buf parameter is not completely contained in a\n");
		errMsg.append("valid part of the user address space.");
	}
	else if ( *errCode == WSAENETRESET )
	{
		errMsg.append("The connection has been broken due to the keep-alive\n");
		errMsg.append("activity detecting a failure while the operation was in progress.");
	}
	else if ( *errCode == WSAENOBUFS )
		errMsg.append("No buffer space is available.");
	else if ( *errCode == WSAENOTCONN )
		errMsg.append("The socket is not connected.");
	else if ( *errCode == WSAENOTSOCK )
		errMsg.append("The descriptor is not a socket.");
	else if ( *errCode == WSAEOPNOTSUPP )
	{
		errMsg.append("MSG_OOB was specified, but the socket is not stream-style\n");
		errMsg.append("such as type SOCK_STREAM, out-of-band data is not supported\n");
		errMsg.append("in the communication domain associated with this socket,\n");
		errMsg.append("or the socket is unidirectional and supports only receive operations.");
	}
	else if ( *errCode == WSAESHUTDOWN )
	{
		errMsg.append("The socket has been shut down; it is not possible to send\n");
		errMsg.append("on a socket after shutdown has been invoked with how set\n");
		errMsg.append("to SD_SEND or SD_BOTH.");
	}
	else if ( *errCode == WSAEWOULDBLOCK )
		errMsg.append("The socket is marked as nonblocking and the requested operation would block.\n");
	else if ( *errCode == WSAEMSGSIZE )
	{
		errMsg.append("The socket is message oriented, and the message is larger\n");
		errMsg.append("than the maximum supported by the underlying transport.");
	}
	else if ( *errCode == WSAEHOSTUNREACH )
		errMsg.append("The remote host cannot be reached from this host at this time.");
	else if ( *errCode == WSAEINVAL )
	{
		errMsg.append("The socket has not been bound with bind, or an unknown flag\n");
		errMsg.append("was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled.");
	}
	else if ( *errCode == WSAECONNABORTED )
	{
		errMsg.append("The virtual circuit was terminated due to a time-out or \n");
		errMsg.append("other failure. The application should close the socket as it is no longer usable.");
	}
	else if ( *errCode == WSAECONNRESET )
	{
		errMsg.append("The virtual circuit was reset by the remote side executing a \"hard\" \n");
		errMsg.append("or \"abortive\" close. For UPD sockets, the remote host was unable to\n");
		errMsg.append("deliver a previously sent UDP datagram and responded with a\n");
		errMsg.append("\"Port Unreachable\" ICMP packet. The application should close\n");
		errMsg.append("the socket as it is no longer usable.");
	}
	else if ( *errCode == WSAETIMEDOUT )
	{
		errMsg.append("The connection has been dropped, because of a network failure\n");
		errMsg.append("or because the system on the other end went down without notice.");
	}
	else errMsg.append("unknown problems!");
}

#endif

void MyTcpSocket::connectToServer(string& serverNameOrAddr,hostType hType)
{ 
	/* 
	   when this method is called, a client socket has been built already,
	   so we have the socketId and portNumber ready.

       a MyHostInfo instance is created, no matter how the server's name is 
	   given (such as www.yuchen.net) or the server's address is given (such
	   as 169.56.32.35), we can use this MyHostInfo instance to get the 
	   IP address of the server
	*/

	MyHostInfo serverInfo(serverNameOrAddr,hType);
	
    // Store the IP address and socket port number	
	struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverInfo.getHostIPAddress());
    serverAddress.sin_port = htons(m_nPortNumber);

    // Connect to the given address
	try 
	{
		if (connect(m_nSocketId,(struct sockaddr *)&serverAddress,sizeof(serverAddress)) == -1)
		{
			#ifdef WINDOWS_XP
				int errorCode = 0;
				string errorMsg = "error calling connect():\n";
				detectErrorConnect(&errorCode,errorMsg);
				MyException socketConnectException(errorCode,errorMsg);
				throw socketConnectException;
			#endif

			#ifdef UNIX
				MyException unixSocketConnectException(0,"unix: error calling connect()");
				throw unixSocketConnectException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
}

MyTcpSocket* MyTcpSocket::acceptClient(string& clientHostName)
{
	int newSocket;   // the new socket file descriptor returned by the accept systme call

    // the length of the client's address
    struct sockaddr_in clientAddress;    // Address of the client that sent data
    int clientAddressLen = sizeof(struct sockaddr_in);

    // Accepts a new client connection and stores its socket file descriptor
	try 
	{
		if ((newSocket = accept(m_nSocketId, (struct sockaddr *)&clientAddress,&clientAddressLen)) == -1)
		{
			#ifdef WINDOWS_XP
				int errorCode = 0;
				string errorMsg = "error calling accept(): \n";
				detectErrorAccept(&errorCode,errorMsg);
				MyException socketAcceptException(errorCode,errorMsg);
				throw socketAcceptException;
			#endif

			#ifdef UNIX
				MyException unixSocketAcceptException(0,"unix: error calling accept()");
				throw unixSocketAcceptException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		return NULL;
	}
    
	// Get the host name given the address
    char *sAddress = inet_ntoa((struct in_addr)clientAddress.sin_addr);

	/*******MyHostInfo from ADDRESS**********/
	MyHostInfo clientInfo(string(sAddress),ADDRESS);
	char* hostName = clientInfo.getHostName();
    clientHostName+= string(hostName);
	
    // Create and return the new MyTcpSocket object
    MyTcpSocket* retSocket = new MyTcpSocket();
	//保存与客户端通信的句柄
	retSocket->setSocketId(newSocket);
    return retSocket;
}

void MyTcpSocket::listenToClient(int totalNumPorts)
{
	try 
	{
		if (listen(m_nSocketId,totalNumPorts) == -1)
		{
			#ifdef WINDOWS_XP
				int errorCode = 0;
				string errorMsg = "error calling listen():\n";
				detectErrorListen(&errorCode,errorMsg);
				MyException socketListenException(errorCode,errorMsg);
				throw socketListenException;
			#endif

			#ifdef UNIX
				MyException unixSocketListenException(0,"unix: error calling listen()");
				throw unixSocketListenException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}
}       

/*
int MyTcpSocket::sendMessage(string& message)
{
	int numBytes;  // the number of bytes sent


	char msgLength[MSG_HEADER_LEN+1];
    sprintf(msgLength,"%6d",message.size());
	string sendMsg = string(msgLength);
    sendMsg += message;

	// Sends the message to the connected host
	try 
	{
		if (numBytes = send(socketId,sendMsg.c_str(),sendMsg.size(),0) == -1)
		{
			#ifdef WINDOWS_XP
				int errorCode = 0;
				string errorMsg = "error calling send():\n";
				detectErrorSend(&errorCode,errorMsg);
				MyException socketSendException = new MyException(errorCode,errorMsg);
				throw socketSendException;
			#endif

			#ifdef UNIX
				MyException unixSocketSendException = new MyException(0,"unix: error calling send()");
				throw unixSocketSendException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}

	return numBytes;
}
*/
int MyTcpSocket::sendMessage(string& message)
{
	int numBytes;  // the number of bytes sent

	/* 
	   for each message to be sent, add a header which shows how long this message
	   is. This header, regardless how long the real message is, will always be
	   of the length MSG_HEADER_LEN.
	*/

	char msgLength[MSG_HEADER_LEN+1];
    sprintf_s(msgLength,strlen(msgLength),"%6d",message.size());
	string sendMsg = string(msgLength);
    sendMsg += message;

	// Sends the message to the connected host
	try 
	{
		if (numBytes = send(m_nSocketId,sendMsg.c_str(),sendMsg.size(),0) == -1)
		{
			#ifdef WINDOWS_XP
				int errorCode = 0;
				string errorMsg = "error calling send():\n";
				detectErrorSend(&errorCode,errorMsg);
				MyException socketSendException(errorCode,errorMsg);
				throw socketSendException;
			#endif

			#ifdef UNIX
				MyException unixSocketSendException(0,"unix: error calling send()");
				throw unixSocketSendException;
			#endif
        }
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}

	return numBytes;
}

#ifdef WINDOWS_XP
/*
int MyTcpSocket::XPrecieveMessage(string& message)
{
	int numBytes = 0;                 // The number of bytes received
    int currentSize = MSG_HEADER_LEN; // The number of bytes wanted to receive
    int offsetSize = 0;               // The number of bytes currently recieved

	// retrieve the length of the message received

	char msgLength[MSG_HEADER_LEN+1];
	memset(msgLength,0,sizeof(msgLength));

	try
	{
		while ( numBytes < currentSize )
		{
			numBytes = recv(socketId,msgLength+offsetSize,currentSize,MSG_PEEK);
			if (numBytes == -1)
			{
				int errorCode = 0;
				string errorMsg = "error calling recv():\n";
				detectErrorRecv(&errorCode,errorMsg);
				MyException socketRecvException = new MyException(errorCode,errorMsg);
				throw socketRecvException;
			}
			else if ( numBytes < currentSize )
			{
				offsetSize += numBytes;
				currentSize -= numBytes;
			}
		}

	}
	catch(MyException excp)
	{
		excp.response();
		exit(1);
	}

	// recieve the real message
	currentSize = atoi(msgLength);
	offsetSize = 0;

	cout   << "[RECV:message length] " << msgLength << endl;
	winLog << "[RECV:message length] " << msgLength << endl;

	try
	{
		while ( numBytes < currentSize )
		{
			numBytes = recv(socketId,(char*)(message.c_str())+offsetSize,currentSize,0);
			if (numBytes == -1)
			{
				int errorCode = 0;
				string errorMsg = "error calling recv():\n";
				detectErrorRecv(&errorCode,errorMsg);
				MyException socketRecvException = new MyException(errorCode,errorMsg);
				throw socketRecvException;
			}
			else if ( numBytes < currentSize )
			{
				offsetSize += numBytes;
				currentSize -= numBytes;
			}
		}

	}
	catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}

	cout   << "[RECV:message] " << message << endl;
	winLog << "[RECV:message] " << message << endl;

    return atoi(msgLength);   
}
*/


int MyTcpSocket::XPrecieveMessage(string& message)
{
	int received = 0;                 // The number of bytes received
    int msgSize = MAX_RECV_LEN;       // The number of bytes wanted to receive
    int numBytes = 0;                 // The number of bytes currently recieved
	int totalRecvNum = 0;
	bool headerFinished = false;

	char charMsg[MAX_RECV_LEN+1];
	char msgLength[MSG_HEADER_LEN+1];
	memset(charMsg,0,sizeof(charMsg));
	memset(msgLength,0,sizeof(msgLength));

	try
	{
		while ( received < msgSize )
		{
			numBytes = recv(m_nSocketId,charMsg+received,1,0);
			if (numBytes == -1)
			{
				int errorCode = 0;
				string errorMsg = "error calling recv():\n";
				detectErrorRecv(&errorCode,errorMsg);
				MyException socketRecvException(errorCode,errorMsg);
				throw socketRecvException;
			}

			if ( !headerFinished )
			{
				msgLength[received] = *(charMsg+received);
				received ++;

				if ( received == MSG_HEADER_LEN )
				{
					headerFinished = true;
					received = 0;
					memset(charMsg,0,sizeof(charMsg));
					msgSize = atoi(msgLength);
				}
			}
			else 
				received ++;
		}
	}
	catch(MyException& excp)
	{
		if ( excp.getErrCode() == WSAECONNRESET )
		{
			//cout   << "!! your party has shut down the connection... \n";
			//winLog << "!! your party has shut down the connection... \n";
			return -99; //不停止服务器
		}
		excp.response();
		exit(1);
	}

	message.append(string(charMsg));
    return msgSize;
}

#endif

/************************************************************************
 * return	received length                                             *
************************************************************************/
int MyTcpSocket::recieveMessage(string& message)
{
	int numBytes;  // The number of bytes recieved

	#ifdef WINDOWS_XP
		return XPrecieveMessage(message);
	#endif

	// retrieve the length of the message received

	char msgLength[MSG_HEADER_LEN+1];
	memset(msgLength,0,sizeof(msgLength));
	try
	{
		numBytes = recv(m_nSocketId,msgLength,MSG_HEADER_LEN,0);
        if (numBytes == -1)
        {
			MyException unixSocketRecvException(0,"unix: error calling recv()");
			throw unixSocketRecvException;
		}
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1); //退出进程
	}

	// receive the real message
	try
	{
		numBytes = recv(m_nSocketId,(char*)(message.c_str()),atoi(msgLength),0);
        if (numBytes == -1)
        {
			MyException unixSocketRecvException(0,"unix: error calling recv()");
			throw unixSocketRecvException;
		}
	}
    catch(MyException& excp)
	{
		excp.response();
		exit(1);
	}

    return numBytes;
}


