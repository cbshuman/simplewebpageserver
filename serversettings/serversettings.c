#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utilities/tolower.h"

struct serverSettings 
  {
    int port;
    int max_connections;
    char * serverPath;
  };

struct serverSettings readParams(int argCount, char* args[])
  {
  struct serverSettings settings = { 8080, 100 };
  
  for (int i = 0; i < argCount; i++) 
    {
    printf("argv[%d] --> %s\n", i, args[i]);

    toLower(args[i]);

    if(strcmp(args[i],"port") == 0)
      {
      if(i+1 >= argCount)
        {
        return settings;
        }

      i++;
      settings.port = atoi(args[i]);
      printf("Selected Port # %d\n", settings.port);
      }

    if(strcmp(args[i],"connections") == 0)
      {
      if(i+1 >= argCount)
        {
        return settings;
        }
      i++;
      settings.max_connections = atoi(args[i]);
      printf("setting max connections to %d\n", settings.max_connections);
      }

    if(strcmp(args[i],"path") == 0)
      {
      if(i+1 >= argCount)
        {
        return settings;
        }
      i++;
      settings.serverPath = args[i];
      printf("Setting path to '%s'", settings.serverPath);
      }
    }


  return(settings);
  }

int validateServerSettings(struct serverSettings _server)
  {
  if(_server.port == 0)
    {
    printf("Bad settings for server port!\n");
    return 1;
    }

  if(_server.max_connections <= 0)
    {
    printf("Max connections needs to be > 0");
    return(1);
    }

  return(0);
  }

