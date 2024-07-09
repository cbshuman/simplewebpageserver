#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

const int BUFFER_SIZE = 1024;

void respond(int client_sock, const char *header, const char *content) 
  {
  char response[BUFFER_SIZE];
  snprintf(response, BUFFER_SIZE, "%s\r\nContent-Length: %zu\r\n\r\n%s", header, strlen(content), content);
  send(client_sock, response, strlen(response), 0);
  }
