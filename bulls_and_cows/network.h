#pragma once

#include<WinSock2.h>
#include<WS2tcpip.h>

#include<string>
using std::string;

// link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "27015"

// run a server on the host computer
SOCKET server();

// run a client on the host computer
SOCKET client(const string&);

// shutdown the connection
int shutdown_connection(const SOCKET&);