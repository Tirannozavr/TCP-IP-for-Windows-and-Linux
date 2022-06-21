// Copyright 2022 <Tirannozavr>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Error input\n");
    return 1;
  }

  int port = atoi(argv[1]);
  int sock;
  struct sockaddr_in addr;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(1);
  }

  struct hostent* server = {0};
  server = gethostbyname(argv[2]);
  if (server == NULL) {
    perror("server");
    exit(1);
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  bcopy(reinterpret_cast<char*>(server->h_addr),
        reinterpret_cast<char*>(&addr.sin_addr.s_addr), server->h_length);

  if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("connect");
    exit(2);
  }
  int bytesRecv;
  char recvbuf[64] = "";
	std::string s;
  char message[261];
  std::cin.getline(message, 260, '\n');
  send(sock, message, sizeof(message), 0);

  bytesRecv = recv(sock, recvbuf, 64, 0);
  if (bytesRecv == 0) {
    printf("Connection Closed.\n");
    exit(1);
  }

  printf("Bytes Recv: %d\n", bytesRecv);

  char* ptr;
  char* p = strtok_r(recvbuf, " ", &ptr);
  char* p2 = strtok_r(NULL, " ", &ptr);
  if (p[0] == 'e') {
    std::cout << "error name disk!" << std::endl;
  } else {
    std::cout << "free space = " << p << ", occupied space = " << p2
              << std::endl;
  }
  close(sock);
  return 0;
}
