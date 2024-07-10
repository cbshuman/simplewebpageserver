#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int BUFFER_SIZE = 1024;

struct ClientInformation 
  {
  int client_socket;
  char* path;
  char* protocol;
  char* method;
  };

struct ClientInformation GetClientConnection(int server_socket)
  {
  struct ClientInformation clientInfo = {};
  
  struct sockaddr_in client_address;
  socklen_t client_len = sizeof(client_address);

  int client_sock = accept(server_socket, (struct sockaddr *)&client_address, &client_len);

  char buffer[BUFFER_SIZE];

  memset(buffer, 0, BUFFER_SIZE);
  read(client_sock, buffer, BUFFER_SIZE - 1);

  char method[16], path[256], protocol[16];
  sscanf(buffer, "%s %s %s", method, path, protocol);

  char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/html";

  clientInfo.client_socket = client_sock;
  clientInfo.path = path;
  clientInfo.protocol = protocol;
  clientInfo.method = method;

  return clientInfo;
  }

void respond(int client_sock, const char *header, const char *content) 
  {
  char response[BUFFER_SIZE];
  snprintf(response, BUFFER_SIZE, "%s\r\nContent-Length: %zu\r\n\r\n%s", header, strlen(content), content);
  send(client_sock, response, strlen(response), 0);
  }
