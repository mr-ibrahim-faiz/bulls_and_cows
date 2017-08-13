#include "network.h"

#include<iostream>
using std::cout;
using std::endl;

// run a server on the host computer
SOCKET server()
// the server listens on TCP port 27015
// returns the socket on which the connection with the client is made
// if no error occurs, WSAStartup, getaddrinfo, bind and listen return zero
{
	SOCKET client_socket { INVALID_SOCKET };
	SOCKET listen_socket { INVALID_SOCKET };
	
	WSADATA wsadata;

	addrinfo *result = NULL, hints;

	// initializes winsock
	int error_result = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (error_result != 0) {
		cout << "\nWSAStartup failed with error " << error_result << "." << endl;
		WSACleanup();
		return INVALID_SOCKET;
	}

	// confirms that the WinSock DLL supports the version 2.2
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2) {
		cout << "\nUnable to find a usable version of Winsock.dll." << endl;
		WSACleanup();
		return INVALID_SOCKET;
	}

	// provides hints about the socket
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// resolves local address and port to be used by the server
	error_result = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (error_result != 0) {
		cout << "\ngetaddrinfo failed with error " << error_result << "." << endl;
		WSACleanup();
		return INVALID_SOCKET;
	}

	// create a socket to listen on the client connection
	listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listen_socket == INVALID_SOCKET) {
		cout << "\nsocket failed with error " << WSAGetLastError() << "." << endl;
		freeaddrinfo(result);
		WSACleanup();
		return INVALID_SOCKET;
	}

	// bind the local address with the socket
	error_result = bind(listen_socket, result->ai_addr, (int)result->ai_addrlen);
	if (error_result != 0) {
		cout << "\nbind failed with error " << WSAGetLastError() << "." << endl;
		freeaddrinfo(result);
		closesocket(listen_socket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	freeaddrinfo(result);

	// listen on the socket
	error_result = listen(listen_socket, SOMAXCONN);
	if (error_result != 0) {
		cout << "\nlisten failed with error " << WSAGetLastError() << "." << endl;
		closesocket(listen_socket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	cout << "\nWaiting for friend..." << endl;

	// accept client connection
	client_socket = accept(listen_socket, NULL, NULL);
	if (client_socket == INVALID_SOCKET) {
		cout << "accept failed with error " << WSAGetLastError() << "." << endl;
		closesocket(listen_socket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	// cleanup
	closesocket(listen_socket);

	return client_socket;
}

// run a client on the host computer
SOCKET client(const string& string_ip_adress)
// the client tries to connect to the server on TCP port 27015
// returns the socket on which the connection with the server is made
// if no error occurs, WSAStartup, getaddrinfo and connect return zero
{
	SOCKET connect_socket { INVALID_SOCKET };

	WSADATA wsadata;

	addrinfo *result = NULL, *ptr = NULL, hints;

	// initializes winsock
	int error_result = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (error_result != 0) {
		cout << "\nWSAStartup failed with error " << error_result << "." << endl;
		WSACleanup();
		return INVALID_SOCKET;
	}

	// confirms that the WinSock DLL supports the version 2.2
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2) {
		cout << "\nUnable to find a usable version of Winsock.dll." << endl;
		WSACleanup();
		return INVALID_SOCKET;
	}

	// provides hints about the socket
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	PCSTR ip_address = string_ip_adress.c_str();

	// resolves the server address and port
	error_result = getaddrinfo(ip_address, DEFAULT_PORT, &hints, &result);
	if (error_result != 0) {
		cout << "\ngetaddrinfo failed with error " << error_result << "." << endl;
		WSACleanup();
		return INVALID_SOCKET;
	}

	// attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// create a socket to connect to server
		connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (connect_socket == INVALID_SOCKET) {
			cout << "\nsocket failed with error " << WSAGetLastError() << "." << endl;
			freeaddrinfo(result);
			WSACleanup();
			return INVALID_SOCKET;
		}

		// connect to server.
		error_result = connect(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (error_result != 0) {
			closesocket(connect_socket);
			connect_socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (connect_socket == INVALID_SOCKET) {
		cout << "\nUnable to conect to server." << endl;
		WSACleanup();
		return INVALID_SOCKET;
	}

	return connect_socket;
}

// shutdown the connection
int shutdown_connection(const SOCKET& socket)
// disable sends and recieves on a socket
// socket must nbe valid
// returns 0 if no errors occur
{
	if (socket == INVALID_SOCKET) {
		cout << "\nThe socket is invalid." << endl;
		WSACleanup();
		return 1;
	}

	int error_result = shutdown(socket, SD_BOTH);
	if (error_result != 0) {
		cout << "\nshutdown failed with error " << WSAGetLastError() << "." << endl;
		closesocket(socket);
		WSACleanup();
		return 2;
	}

	// cleanup
	closesocket(socket);
	WSACleanup();

	return 0;
}