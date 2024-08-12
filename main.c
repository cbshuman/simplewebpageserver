#include <signal.h>
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

int server_socket;
int BUFFER_SIZ = 3081960;

struct ThreadArgs {
    int client_socket;
    struct serverSettings server;
};

void* HandleClientRequest(void *args) {
    struct ThreadArgs* arguments = (struct ThreadArgs *)args;

    printf("\n - - New Request - - Responding to client sock %d \n", arguments->client_socket);

    struct ClientInformation clientInfo = GetClientConnection(arguments->client_socket);

    if(strcmp(clientInfo.method, "") == 0)
      {
      return NULL;
      }

    struct generated_response generatedResponse = GenerateResponse(arguments->server.serverPath, clientInfo);
    char response[BUFFER_SIZ];

    printf("%s", generatedResponse.content);
    memcpy(response, generatedResponse.headers, strlen(generatedResponse.headers));
    memcpy(response + strlen(generatedResponse.headers), generatedResponse.content, generatedResponse.contentLength);
    printf("%s", response);
   // memcpy(response, BUFFER_SIZ, "%s%s", generatedResponse.headers, generatedResponse.content);
    
    int sendResponse = send(arguments->client_socket, response, generatedResponse.contentLength + strlen(generatedResponse.headers), 0);

    if(sendResponse < 0)
      {
      printf("Error");
      }

    //printf("Count: %i Sending: %s\n", sendResponse, response);

    close(arguments->client_socket);
    free(arguments);
    //Need to find a better way to do this. . . 
    if(generatedResponse.contentLength > 100)
      {
      free(generatedResponse.content);
      }
    free(generatedResponse.headers);

    printf(" - - - Done - - - \n");
    return NULL;
}

void closeSigHandler(int sig)
  {
  close(server_socket);
  printf("\r\n -- Closing -- \r\n");
  exit(0);
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
  
  signal(SIGINT, closeSigHandler);

  // ---- Bind and get the server up and running ------ \\\

  server_socket = socket(AF_INET,SOCK_STREAM,0);
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
    int client_fd;

    if((client_fd = accept(server_socket,(struct sockaddr*)&client_address,&client_len)) < 0) 
      {
      continue;
      }

    struct ThreadArgs* args = (struct ThreadArgs *)malloc(sizeof(struct ThreadArgs));
    args->client_socket = client_fd;
    args->server = _server;

    pthread_t thread;
    pthread_create(&thread, NULL, HandleClientRequest,(void *)args);
    pthread_detach(thread);
    }

  close(server_socket);
  return 0;
  }
