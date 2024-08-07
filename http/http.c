#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../utilities/tolower.h"

const int BUFFER_SIZE = 9024;

struct ClientInformation 
  {
  char method[16], path[256], protocol[16];
  };

struct ClientInformation GetClientConnection(int client_sock)
  {
  struct ClientInformation clientInfo = {};

  char buffer[BUFFER_SIZE];
  ssize_t bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0);

  if(bytes_received > 0)
    {
    sscanf(buffer, "%s %s %s",clientInfo.method, clientInfo.path, clientInfo.protocol);

    toLower(clientInfo.path);
    toLower(clientInfo.protocol);
    toLower(clientInfo.method);
    }
  else if (bytes_received == 0)
    {
    printf("Client closed the connection.\n");
    }
  else 
    {
    perror("recv failed");
    }

  return clientInfo;
  }

void respond(int client_sock, const char *header, const char *content) 
  {
  char response[BUFFER_SIZE];
  snprintf(response, BUFFER_SIZE, "%s\r\nContent-Length: %zu\r\n\r\n%s", header, strlen(content), content);
  send(client_sock, response, strlen(response), 0);
  printf("Sending: %s", response);
  }
