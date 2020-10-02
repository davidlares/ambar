# Ambar

This repository contains three source scripts, the `main.c` that acts as a backdoor for Windows clients and a `server.c` as the attacker's `C&C` for sending commands and gather information, and a `keylogger.h` library for writing `.txt` files with an ASCII HEX comparison

The main functionality of this project is to perform a `TCP` reverse shell with C native sockets library. After the connection is made, you can move into directories, generate a Windows Registry key for malware persistence, and also a simple key-logger in a hard-coded `.txt` file.

Both machines need to be under the same LAN network, but with some port-forwarding, you can extend the LAN range to the internet.

## C Windows libraries

1. `HINSTANCE` is a handle to instance or module, the Windows OS uses it to identify the executable when is in memory
2. `hPrev` is used in 64bit Windows
4. `nCmdShow` is a flag to identify when a program window will be minimized or maximized
5. `sockaddr_in` (Struct element), pretty similar to a C Class. The `sockaddr_in` specifies the server address.
6. `WSADATA` is a structure that contains information about the windows socket. A pointer to the `WSADATA` data structure that is to receive details of the Windows Sockets implementation.

7. The `WSAStartup` function initiates the use of the `Winsock DLL` by a process.
8. `memset` is a C library function void that copies the character `c` (an unsigned char) to the first `n` characters of the string pointed to, by the argument `str`. This is widely used for filling default memory access with zeros.

## How to compile

Make sure first that the `IPs` and the ports set are correct according to your LAN and your attacking machine IP.

To compile your `server.c` you will need to run: `gcc server.c -o server.o`

The backdoor or `main.c` is intended for Windows machines, so, the process is a little different. Just: `i686-w64-mingw32-gcc -o malware.exe main.c -lwsock32 -lwininet`. Where:

- `mingw32-gcc`, the script is compiled to 32, because can run under 64 and 32bits OS
- `malware.exe` is the `.exe` output
- `main.c`, the source code
- the following flags are libraries for the Windows functionality on the compiler (`lwsock32` - `lwininet`)


## Credits

 - [David E Lares](https://twitter.com/davidlares3)

## License

 - [MIT](https://opensource.org/licenses/MIT)
