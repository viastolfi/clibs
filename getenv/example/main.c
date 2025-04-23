#define GETENV_LIB_IMPLEMENTATION
#include "../getenv.h"

#include <stdio.h>

int main(void)
{
  if(!load_env("./.env") || !GETENV_OK)
  {
    fprintf(stderr, "Load failed : %s\n", getenv_strerror());
    return 1;
  }

  char *c = get_env("FOO");
  char *d = get_env("BAZ");

  if(c == NULL || d == NULL)
  {
    fprintf(stderr, "Missing key: %s\n", getenv_strerror());
    return 1;
  }
  
  printf("EXPECT: BAR, GET : %s\n", c);
  printf("EXPECT: QUX, GET : %s\n", d);

  getenv_free();

  return 0;
}
