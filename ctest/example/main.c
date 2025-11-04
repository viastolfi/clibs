#include "../ctest.h"

ct_test(simple, test) 
{
  ct_assert(1);
}

ct_test(simple, fail)
{
  ct_assert(0);
}
