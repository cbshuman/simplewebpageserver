#ifndef SERVERSETTINGS_H
#define SERVERSETTINGS_H

struct ClientInformation 
  {
  int client_socket; 
  };

const int BUFFER_SIZE = 1024;
void respond(int client_sock, const char *header, const char *content);
struct ClientInformation GetClientConnection(int server_socket);


#endif
