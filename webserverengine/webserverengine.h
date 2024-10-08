#ifndef WEBSERVERENGINE_H
#define WEBSERVERENGINE_H

#include "../http/http.h"

struct generated_response
  {
    char* headers;
    char* content;
    long contentLength;
  };

struct generated_response GenerateResponse(char* fileLocation, struct ClientInformation clientInfo);

#endif
