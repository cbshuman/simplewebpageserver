#ifndef HTTPHELPER_H
#define HTTPHELPER_H

struct ClientInformation 
  {
  int client_socket;
  char* path;
  char* protocol;
  char* method;
  };

void respond(int client_sock, const char *header, const char *content);
struct ClientInformation GetClientConnection(int server_socket);


#endif
