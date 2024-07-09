#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "serversettings/serversettings.h"

const int BUFFER_SIZE = 1020;

void respond(int client_sock, const char *header, const char *content) 
  {
    char response[BUFFER_SIZE];
    snprintf(response, BUFFER_SIZE, "%s\r\nContent-Length: %zu\r\n\r\n%s", header, strlen(content), content);
    send(client_sock, response, strlen(response), 0);
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

  char buffer[BUFFER_SIZE];

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

  int client_sock;

  while(1)
    {
    FD_ZERO(&readfds);
    FD_SET(server_socket,&readfds);

    int selection = select(server_socket+1, &readfds, NULL, NULL, &timeout);

    if ((selection < 0) && (errno != EINTR)) 
      {
      printf("select error");
      }

    if (selection == 0) 
      {
      continue;
      }

    if(FD_ISSET(server_socket, &readfds))
      {
      client_sock = accept(server_socket, (struct sockaddr *)&client_address, &client_len);
      memset(buffer, 0, BUFFER_SIZE);
      read(client_sock, buffer, BUFFER_SIZE - 1);

      char method[16], path[256], protocol[16];
      sscanf(buffer, "%s %s %s", method, path, protocol);


      char *header = "HTTP/1.1 200 OK\r\nContent-Type: text/html";
      //const char *content = "<html><body> <b>Hello to the World!</b></body></html>";

      char *content = malloc(100);

      strcat(content, "<html><body><b>");

      strcat(content, path);
      strcat(content, "<b></body></html>");


      respond(client_sock, header, content);

      printf("%s, %s, %s", method,path,protocol);
      }
    }

  printf("We outa here");

  close(server_socket);
  return 0;
  }
