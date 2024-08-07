#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "../http/http.h"

struct generated_response
  {
    char* headers;
    char* content;
  };

//Might need to move this to utilities
char* ReadFile(char* filePath)
  {
  printf("reading in file: %s\n", filePath);
  char *source = NULL; 
  FILE *filePointer = fopen(filePath,"r");
  printf("Got my file pointer: ");

  if (filePointer != NULL) 
    {
    if(fseek(filePointer, 0L, SEEK_END) == 0)
      {
      long buffer_size = ftell(filePointer);
      printf("Buffer size %ld\n", buffer_size);
      if(buffer_size == -1)
        {
        printf("Problem setting pointer to end\n");
        return NULL;
        }
      
      if(fseek(filePointer, 0L, SEEK_SET) != 0)
        {
        printf("Problem setting pointer back\n");
        return NULL;
        }

      source = malloc(sizeof(char) * (buffer_size+1));
      size_t newLen = fread(source, sizeof(char), buffer_size, filePointer);
      
      if(ferror(filePointer) != 0)
        {
        printf("ERROR from fread\n");
        return NULL;
        }
      else 
        {
        source[newLen++] = '\0';
        }
      }
    fclose(filePointer);
    }
  else 
    {
    printf("Cannot read file %s\n", filePath);
    }

  //printf("%s", source);
  return source;
  }

const char* GetMimeType(char * filePath)
  {
  char *extension = strrchr(filePath, '.') + 1;
  if(strcasecmp(extension,"html") == 0)
    {
    return "text/html";
    }
  else if(strcasecmp(extension,"css") == 0)
    {
    return "text/css";
    }
  else if(strcasecmp(extension,"ttf") == 0)
    {
    return "application/x-font-ttf";
    }
  return "text/html";
  }


struct generated_response GenerateResponse(char* fileLocation, struct ClientInformation clientInfo)
  {
  struct generated_response response = {};

  printf("method: %s\n", clientInfo.method);
  printf("path: %s\n", clientInfo.path);
  printf("protocol: %s\n", clientInfo.protocol);

  if(strcmp(clientInfo.method,"get") == 0)
    {

    //Need to find a way to differentiate between an API call and regular ol' file requests.
    if(strcmp(clientInfo.path, "/") == 0)
      {
      strcpy(clientInfo.path, "/index.html");
      }

    char fullPath[1024];
    snprintf(fullPath, sizeof(fullPath), "%s%s", fileLocation, clientInfo.path);
    response.content = ReadFile(fullPath);

    response.headers = malloc(2048);
    if(response.content == NULL)
      {
      response.content = "404 - File not found!";
      snprintf(response.headers, 2048, "HTTP/1.1 400 NOT FOUND\r\nContent-Type:%s\r\nConnection:close\r\n", GetMimeType(fullPath));
      }
    else
      {
      //header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n";
      snprintf(response.headers, 2048, "HTTP/1.1 200 OK\r\nContent-Type:%s\r\nConnection:close\r\n", GetMimeType(fullPath));
      }
    }

  printf("returning: %s\n", response.headers);
  return response;
  }

