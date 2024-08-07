#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  FILE *filePointer;
  printf("Got my file pointer: ");

  if (filePointer == fopen(filePath,"r")) 
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
    }
  else 
    {
    printf("Cannot read file %s", filePath);
    }
  fclose(filePointer);

  printf("%s", source);
  return source;
  }

struct generated_response GenerateResponse(char* fileLocation, struct ClientInformation clientInfo)
  {
  struct generated_response response = {};

  printf("method: %s\n", clientInfo.method);
  printf("path: %s\n", clientInfo.path);
  printf("protocol: %s\n", clientInfo.protocol);

  //Need to find a way to differentiate between an API call and regular ol' file requests.
  if(strcmp(clientInfo.path, "/") == 0)
    {
    strcpy(clientInfo.path, "/index.html");
    }

  char fullPath[1024];
  snprintf(fullPath, sizeof(fullPath), "%s%s", fileLocation, clientInfo.path);
  response.content = ReadFile(fullPath);

  if (response.content == NULL)
    {
    strcpy(response.content, "File not found!");
    }

  printf("returning: %s\n", response.content);
  return response;
  }

