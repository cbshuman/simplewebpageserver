#ifndef HTTPHELPER_H
#define HTTPHELPER_H

struct ClientInformation 
  {
  int client_socket;
  char method[16], path[256], protocol[16];
  };

void respond(int client_sock, const char *header, const char *content);
struct ClientInformation GetClientConnection(int server_socket);


#endif
