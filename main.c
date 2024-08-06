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
int BUFFER_SIZ = 9080;

struct ThreadArgs {
    int client_socket;
    struct serverSettings server;
};

void* HandleClientRequest(void *args) {
    struct ThreadArgs* arguments = (struct ThreadArgs *)args;

    // Log the new request
    printf("\n - - New Request - - Responding to client sock %d \n", arguments->client_socket);

    // Extract client information
    struct ClientInformation clientInfo = GetClientConnection(arguments->client_socket);

    // Generate the response
    GenerateResponse(arguments->server.serverPath, clientInfo);

    // Respond to the client
    const char* header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n";
    const char* content = "<b>hello World</b>\r\n";
    char response[BUFFER_SIZ];
    snprintf(response, BUFFER_SIZ, "%sContent-Length: %zu\r\n\r\n%s", header, strlen(content), content);
    
    if(send(arguments->client_socket, response, strlen(response), 0) < 0)
      {
        printf("Error");
      }

    // Log the response
    printf("Sending: %s\n", response);

    // Clean up
    close(arguments->client_socket);
    free(arguments);

    // Indicate the thread is done
    printf("Done\n");

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
