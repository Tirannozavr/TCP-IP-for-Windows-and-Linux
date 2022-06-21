#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>
#pragma comment(lib, "Ws2_32.lib")

#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Error input\n");
    return 1;
  }

  // Initialize Winsock.
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != NO_ERROR) printf("Error at WSAStartup()\n");

  // Create a socket.
  SOCKET m_socket;
  m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (m_socket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
    WSACleanup();
    return 1;
  }

  // Bind the socket.
  struct sockaddr_in service;
  service.sin_family = AF_INET;
  service.sin_port = htons(atoi(argv[1]));
  inet_pton(AF_INET, "192.168.56.1", &service.sin_addr);

  if (bind(m_socket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) ==
      SOCKET_ERROR) {
    printf("bind() failed.\n");
    closesocket(m_socket);
    return 1;
  }

  // Listen on the socket.
  if (listen(m_socket, 1) == SOCKET_ERROR)
    printf("Error listening on socket.\n");

  // Accept connections.
  SOCKET AcceptSocket;

  while (1) {
    printf("Waiting for a client to connect...\n");
    AcceptSocket = SOCKET_ERROR;
    while (AcceptSocket == SOCKET_ERROR) {
      AcceptSocket = accept(m_socket, NULL, NULL);
    }
    printf("Client Connected.\n");

    // Send and receive data.
    int bytesSent;
    int bytesRecv = SOCKET_ERROR;

    char recvbuf[260] = "";

    bytesRecv = recv(AcceptSocket, recvbuf, 260, 0);
    printf("Bytes Recv: %ld\n", bytesRecv);
    printf("mes: %s\n", recvbuf);

    __int64 freeSpace, occupiedSpace;
    DWORD numS, numBinS, numFC, numTC;
    bool err = false; // error disk

    //Getting the disk size
    if (GetDiskFreeSpaceA(recvbuf, &numS, &numBinS, &numFC, &numTC) != 0) {
      __int64 i1 = numS;
      __int64 i2 = numBinS;
      __int64 i3 = numFC;
      __int64 i4 = numTC;
      freeSpace = i1 * i2 * i3;
      occupiedSpace = (i4 - i3) * i1 * i2;
    } else {
      printf("Error name disk!\n");
      err = true;
    }

    if (err == false) {
      std::string str = std::to_string(freeSpace);
      str = str + ' ';
      std::string str2 = std::to_string(occupiedSpace);
      str = str + str2; // concatenate strings with a space

      char* sendbuf = new char[str.length() + 1];
      strcpy_s(sendbuf, str.length() + 1, str.c_str());

      bytesSent = send(AcceptSocket, sendbuf, strlen(sendbuf), 0);
      printf("Bytes Sent: %ld\n", bytesSent);

      delete[] sendbuf;

    } else {
      char sendbuf2[32] = "error name disk!";
      bytesSent = send(AcceptSocket, sendbuf2, strlen(sendbuf2), 0);
    }
    closesocket(AcceptSocket);
  }

  return 0;
}
