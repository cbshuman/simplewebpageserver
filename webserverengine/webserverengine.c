#include <stdio.h>
#include <string.h>
#include "../http/http.h"

struct generated_response
  {
    char* headers;
    char* content;
  };

struct generated_response GenerateResponse(struct ClientInformation clientInfo)
  {
  struct generated_response response = {};

  printf("method: %s\n", clientInfo.method);
  printf("path: %s\n", clientInfo.path);
  printf("protocol: %s\n", clientInfo.protocol);

  if(strcmp(clientInfo.method,"get") == 0)
    {
    printf("Getting some stuff\n");
    }

  response.headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html";

  return response;
  }
