#ifndef CTEST_H
#define CTEST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COLOR_RESET  "\033[0m"
#define COLOR_SUCCESS "\033[32m"
#define COLOR_FAIL "\033[31m"
#define COLOR_HINT "\033[33m"

typedef void (*ct_func)(void);

typedef struct ct_test ct_test;

struct ct_test
{
  const char* suite;
  const char* name;
  ct_func func;
  ct_test* next;
};

static ct_test* ct_test_head = NULL;

static void add_test(ct_test* t) {
  t->next = ct_test_head;
  ct_test_head = t;
}

#define ct_test(SUITE, NAME) \
  static void SUITE##_##NAME##_impl(void); \
  static ct_test ct_test_##SUITE##_##NAME = { \
    .suite = #SUITE, \
    .name = #NAME, \
    .func = SUITE##_##NAME##_impl, \
    .next = NULL \
  }; \
  __attribute__((constructor)) \
  static void register_##SUITE##_##NAME(void) { add_test(&ct_test_##SUITE##_##NAME); } \
  static void SUITE##_##NAME##_impl(void) 

#define ct_assert(expr) \
  do { \
    printf("(%s:%d) ", __FILE__, __LINE__); \
    if (!expr) \
      printf(COLOR_FAIL "[FAIL] " COLOR_RESET); \
    else \
      printf(COLOR_SUCCESS "[PASS] " COLOR_RESET); \
  } while (0) 

int main(void) 
{
  ct_test* t = ct_test_head;
  if (!t) {
    printf(COLOR_HINT "No tests registered!\n" COLOR_RESET);
    return 1;
  }

  ct_test* reversed = NULL;
  while (t) {
    ct_test* next = t->next;
    t->next = reversed;
    reversed = t;
    t = next;
  }

  t = reversed;
  while (t) {
    printf("[----] %s::%s\n", t->suite, t->name);
    t->func();
    printf("%s::%s\n", t->suite, t->name); 
    t = t->next;
  }

  return 0;
}

#endif // CTEST_H
