#ifndef CTEST_H
#define CTEST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define COLOR_RESET  "\033[0m"
#define COLOR_SUCCESS "\033[32m"
#define COLOR_FAIL "\033[31m"
#define COLOR_HINT "\033[33m"
#define COLOR_BOLD  "\e[1m"
#define BOLD_OFF   "\e[m"
#define COLOR_SUM "\033[36m"

static int total = 0;
static int success = 0;

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

#define ct_assert(expr, message) \
  do { \
    total++; \
    if (!expr) { \
      printf(COLOR_FAIL "[FAIL] " COLOR_RESET); \
      printf("(%s:%d) ", __FILE__, __LINE__); \
      printf("%s\n", message); \
      printf(COLOR_HINT "[HINT] " COLOR_RESET); \
      printf(COLOR_SUM COLOR_BOLD "expected :" COLOR_RESET " true\n"); \
      printf(COLOR_HINT "[HINT] " COLOR_RESET); \
      printf(COLOR_SUM COLOR_BOLD "result   :" COLOR_RESET " false\n"); \
    } else {\
      printf(COLOR_SUCCESS "[PASS] " COLOR_RESET); \
      printf("(%s:%d) ", __FILE__, __LINE__); \
      printf("%s\n", message); \
      success++; \
    } \
  } while (0) 

static void ct_assert_eq_int(int x, int y, const char* message, const char* file, int line) 
{
  total++;
  if (x == y) {
    printf(COLOR_SUCCESS "[PASS] " COLOR_RESET);
    printf("(%s:%d) ", file, line); 
    printf("%s\n", message);
    success++; 
  }
  else {
    printf(COLOR_FAIL "[FAIL] " COLOR_RESET);
    printf("(%s:%d) ", file, line); 
    printf("%s", message);
    printf(COLOR_HINT "\n[HINT] " COLOR_RESET);
    printf(COLOR_BOLD COLOR_SUM "expected : " COLOR_RESET BOLD_OFF "%d\n", x);
    printf(COLOR_HINT "[HINT] " COLOR_RESET);
    printf(COLOR_BOLD COLOR_SUM "result   : " COLOR_RESET BOLD_OFF "%d\n", y);
  }
}

static void ct_assert_eq_string(const char* x, const char* y, const char* message, const char* file, int line)
{
  total++;
  if (strcmp(x, y) == 0) {
    printf(COLOR_SUCCESS "[PASS] " COLOR_RESET);
    printf("(%s:%d) ", file, line);
    printf("%s\n", message);
    success++; 
  }
  else {
    printf(COLOR_FAIL "[FAIL] " COLOR_RESET);
    printf("(%s:%d) ", file, line); 
    printf("%s", message);
    printf(COLOR_HINT "\n[HINT] " COLOR_RESET);
    printf(COLOR_BOLD COLOR_SUM "expected : " COLOR_RESET BOLD_OFF "\"%s\"\n", x);
    printf(COLOR_HINT "[HINT] " COLOR_RESET);
    printf(COLOR_BOLD COLOR_SUM "result   : " COLOR_RESET BOLD_OFF "\"%s\"\n", y);
  }
}

#define ct_assert_eq(x, y, message) \
  _Generic((x), \
    int: ct_assert_eq_int, \
    char*: ct_assert_eq_string, \
    const char*: ct_assert_eq_string \
  )(x, y, message, __FILE__, __LINE__)

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
    printf("[" COLOR_SUM "----" COLOR_RESET "] " COLOR_BOLD "%s::%s" BOLD_OFF"\n", t->suite, t->name);
    t->func();
    t = t->next;
  }

  printf("[" COLOR_SUM "====" COLOR_RESET "] " COLOR_BOLD "Summary : " COLOR_HINT "%d " COLOR_RESET COLOR_BOLD "Test | " COLOR_SUCCESS "%d " COLOR_RESET COLOR_BOLD "Success | " COLOR_FAIL "%d " COLOR_RESET COLOR_BOLD "Fail\n" BOLD_OFF, total, success, total - success);

  return 0;
}

#endif // CTEST_H
