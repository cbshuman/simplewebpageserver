#ifndef WEBSERVERENGINE_H
#define WEBSERVERENGINE_H

#include "../http/http.h"

struct generated_response
  {
    char* headers;
    char* content;
  };

struct generated_response GenerateResponse(struct ClientInformation clientInfo);

#endif
