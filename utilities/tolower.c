#include "tolower.h"
#include <ctype.h>

void toLower(char* str)
  {
  for(int i = 0; str[i]; i++)
    {
    str[i] = tolower((unsigned char)str[i]);
    }
  }
