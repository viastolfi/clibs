#define GETENV_LIB_IMPLEMENTATION
#include "../getenv.h"

#include <stdio.h>

int main(void)
{
  load_env("./.env");
  char *c = get_env("FOO");
  char *d = get_env("BAZ");
  
  printf("EXPECT: BAR, GET : %s\n", c);
  printf("EXPECT: QUX, GET : %s\n", d);

  return 0;
}
