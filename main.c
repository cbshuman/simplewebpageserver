#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "serversettings/serversettings.h"
#include "http/http.h"
#include "webserverengine/webserverengine.h"

struct ThreadArgs {
    int server_socket;
    struct serverSettings server;
};

void* HandleClientRequest(void *args)
  {
  int client_socket = *(int *)args;

  struct ClientInformation clientInfo = GetClientConnection(client_socket);

  printf("Responding to client sock %d \n", client_socket);
  printf("Method: %s \n", clientInfo.method);

  return NULL;
  }


int main(int argc, char* argv[]) 
  {
  // ---- Run through arguments ------ \\\
 
  printf("Starting Server . . .\n");
  
  struct serverSettings _server = readParams(argc, argv);

  int errorCode = validateServerSettings(_server);
  if(errorCode != 0)
    {
      return(errorCode);
    }

  // ---- Bind and get the server up and running ------ \\\

  int server_socket = socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in server_address, client_address;
  socklen_t client_len = sizeof(client_address);


  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(_server.port);

  fd_set readfds;
  struct timeval timeout;
  timeout.tv_sec = 5; // Set a timeout of 5 seconds
  timeout.tv_usec = 0;

  if(server_socket < 0)
    {
      printf("Problem opening socket");
      return -1;
    }

  if(bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
    printf("Problem binding on Port %d Error Code: %s \n", _server.port,strerror(errno));
    return -1;
    }

  listen(server_socket, _server.max_connections);
  printf("Listening on port %d\n", _server.port);

  while(1)
    {
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    int *client_fd = malloc(sizeof(int));

    if((*client_fd = accept(server_socket,(struct sockaddr*)&client_address,&client_len)) < 0) 
      {
      continue;
      }

    pthread_t thread;
    pthread_create(&thread, NULL, HandleClientRequest,(void *)client_fd);
    pthread_detach(thread);
    }

  close(server_socket);
  return 0;
  }
