#ifndef SERVERSETTINGS_H
#define SERVERSETTINGS_H

struct serverSettings 
  {
    int port;
    int max_connections;
    char* serverPath;
  };

struct serverSettings readParams(int argCount, char* args[]);
int validateServerSettings(struct serverSettings _server);

#endif
