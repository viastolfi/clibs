#ifndef GETENV_H
#define GETENV_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef GETENV_LIB
  #define GETENV_LIB static inline
#endif // GETENV_LIB

#ifndef GETENV_MALLOC
#include <stdlib.h>
#define GETENV_MALLOC malloc
#endif // GETENV_MALLOC

#ifndef GETENV_REALLOC
#include <stdlib.h>
#define GETENV_REALLOC realloc
#endif // GETENV_REALLOC

#include <stdio.h>
#include <errno.h>
#include <string.h>

typedef struct
{
  int has_value;
  char value;
} getenv_optionnal_char_t;

typedef struct
{
  char *key, *value;
} getenv_var_t;

typedef struct
{
  getenv_var_t* vars;
  char *getenv_content, *getenv_buffer;
  int vars_count, getenv_index;
} getenv_t;

#ifndef load_env
#define load_env getenv_load_env
#endif // load_env

#define _getenv_peek(...) _getenv_peek_impl ## __VA_OPT__(1)(__VA_ARGS__)
#define _getenv_peek_impl1(...) getenv_peek(__VA_ARGS__)
#define _getenv_peek_impl(...) getenv_peek(0)

#define _getenv_consume(...) _getenv_consume_impl ## __VA_OPT__(1)(__VA_ARGS__)
#define _getenv_consume_impl1(...) getenv_consume(__VA_ARGS__)
#define _getenv_consume_impl(...) getenv_consume(1)

GETENV_LIB getenv_optionnal_char_t getenv_peek(int i);
GETENV_LIB char* getenv_consume(int i);

GETENV_LIB int getenv_load_env(char* path);
GETENV_LIB char* getenv_get_env(char* key);

#ifdef GETENV_LIB_IMPLEMENTATION

getenv_t env = {.vars = NULL, .getenv_content = NULL, .getenv_buffer = NULL, .vars_count = 0, .getenv_index = 0};

GETENV_LIB getenv_optionnal_char_t getenv_peek(int i)
{
  if(env.getenv_index+ i >= strlen(env.getenv_content))
    return (getenv_optionnal_char_t) {.has_value = 0};
  else
    return (getenv_optionnal_char_t) {.value = env.getenv_content[env.getenv_index + i], .has_value = 1};
}

GETENV_LIB char* getenv_consume(int i)
{
  if(i == 0)
      return "ERROR"; // to handle properly

  char* s = (char*) GETENV_MALLOC(i + 1);
  for(int j = 0; j < i; ++j)
  {
      s[j] = env.getenv_content[env.getenv_index];
      ++(env.getenv_index);
  }

  s[i] = '\0';
  return s;
}

GETENV_LIB int load_env(char* path)
{
  FILE* f;
  long length;
  getenv_optionnal_char_t c;

  f = fopen(path, "rb");
  if(f == NULL)
  {
      perror("Error reading .env file");
      return 1;
  }

  fseek(f, 0, SEEK_END);
  length = ftell(f);
  fseek(f, 0, SEEK_SET);
  env.getenv_content = malloc(length + 1);
  if(env.getenv_content)
  {
      size_t bytes_read = fread(env.getenv_content, 1, length, f);
      env.getenv_content[bytes_read] = '\0';
  }
  else
  {
      fclose(f);
      return 1;
  }

  fclose(f);
  {
    char* dump = NULL;
    int key_length, value_length;
    while((c = _getenv_peek()).has_value)
    {
        if(c.value == '\n')
        {
            dump = _getenv_consume();
            free(dump);
            dump = NULL;
            continue;
        }

        env.vars_count++;
        env.vars = (getenv_var_t*) GETENV_REALLOC(env.vars, env.vars_count * sizeof(getenv_var_t));
        key_length = 0;
        value_length = 0;

        while((c = _getenv_peek(key_length)).has_value)
        {
            if(c.value == '=')
                break;
            key_length++;
        }

        env.vars[env.vars_count - 1].key = malloc(key_length + 1);
        strcpy(env.vars[env.vars_count - 1].key, _getenv_consume(key_length));

        // we consume the '='
        {
            dump = _getenv_consume();
            free(dump);
            dump = NULL;
        }

        while((c = _getenv_peek(value_length)).has_value)
        {
            if(c.value == '\n')
                break;

            value_length++;
        }

        env.vars[env.vars_count - 1].value = malloc(value_length + 1);
        strcpy(env.vars[env.vars_count - 1].value, _getenv_consume(value_length));
    }
  }
}

GETENV_LIB char* getenv_get_env(char* key)
{
  for(int i = 0; i < env.vars_count; ++i)
  {
      if(strcmp(env.vars[i].key, key) == 0)
          return env.vars[i].value;
  }

  perror("env key not found");
  exit(errno);
}

#endif // GETENV_LIB_IMPLEMENTATION

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // GETENV_H
