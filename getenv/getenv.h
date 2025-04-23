/*
------------------------------------------------------------------------------
GETENV LIB FOR C - ASTOLFI Vincent 2025

Usage:

    #define GETENV_LIB_IMPLEMENTATION
    #include "getenv.h"

API:

    int getenv_load_env(char* path);
    char* getenv_get_env(char* key);
    char* getenv_strerror();
    void getenv_free();

MACROS:
    
    if they are not already defined you can have access to macros to shortten function name

    load_env ->  getenv_load_env
    get_env  ->  getenv_get_env

    macros also exist to improve interface

    GETENV_OK -> 1 if last error is GETENV_ERR_NONE, 0 otherwise

RETURNS:

    - getenv_load_env()
    return 1 on success, 0 on error

    - getenv_get_env()
    return value assiocated with a key, NULL if key not found

    - getenv_strerror()
    return string corresponding to the last error

ERRORS:
  
  GETENV_ERR_NONE          
  GETENV_ERR_KEY_NOT_FOUND
  GETENV_ERR_FILE_NOT_FOUND

EXAMPLE:

    ```c
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

      if(c == NULL || d == NULL || !GETENV_OK)
      {
        fprintf(stderr, "Missing key: %s\n", getenv_strerror());
        return 1;
      }

      printf("EXPECT: BAR, GET : %s\n", c);
      printf("EXPECT: QUX, GET : %s\n", d);

      return 0;
    }
    ```
------------------------------------------------------------------------------
*/

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

#ifndef GETENV_FREE
#include <stdlib.h>
#define GETENV_FREE free
#endif // GETENV_FREE

#include <stdio.h>
#include <string.h>

typedef enum
{
  GETENV_ERR_NONE = 0,
  GETENV_ERR_KEY_NOT_FOUND,
  GETENV_ERR_FILE_NOT_FOUND,
  GETENV_ERR_MEMORY
} getenv_err_t;

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

#ifndef get_env
#define get_env getenv_get_env
#endif // get_env

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
GETENV_LIB void getenv_free();

GETENV_LIB char* getenv_strerror();

#ifdef GETENV_LIB_IMPLEMENTATION

#define GETENV_OK (getenv_last_error == GETENV_ERR_NONE)

static getenv_err_t getenv_last_error = GETENV_ERR_NONE;

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
    return NULL;

  char* s = (char*) GETENV_MALLOC(i + 1);
  if(s == NULL)
    return NULL;

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
    getenv_last_error = GETENV_ERR_FILE_NOT_FOUND;
    return 0;
  }

  fseek(f, 0, SEEK_END);
  length = ftell(f);
  fseek(f, 0, SEEK_SET);
  env.getenv_content = GETENV_MALLOC(length + 1);
  if(env.getenv_content)
  {
    size_t bytes_read = fread(env.getenv_content, 1, length, f);
    env.getenv_content[bytes_read] = '\0';
  }
  else
  {
    getenv_last_error = GETENV_ERR_MEMORY; 
    fclose(f);
    return 0;
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
            GETENV_FREE(dump);
            dump = NULL;
            continue;
        }

        if(c.value == '#')
        {
          dump = _getenv_consume();
          GETENV_FREE(dump);
          dump = NULL;

          int comment_length = 0;
          while((c = _getenv_peek(comment_length)).has_value)
          {
            if(c.value == '\n')
              break;
            
            ++comment_length;
          }

          dump = _getenv_consume(comment_length);
          GETENV_FREE(dump);
          dump = NULL;

          continue;
        }

        env.vars_count++;
        env.vars = (getenv_var_t*) GETENV_REALLOC(env.vars, env.vars_count * sizeof(getenv_var_t));
        if(env.vars == NULL)
        {
          getenv_last_error = GETENV_ERR_MEMORY;
          return 0;
        }
        key_length = 0;
        value_length = 0;

        while((c = _getenv_peek(key_length)).has_value)
        {
            if(c.value == '=')
                break;
            key_length++;
        }

        env.vars[env.vars_count - 1].key = GETENV_MALLOC(key_length + 1);
        if(env.vars[env.vars_count - 1].key == NULL)
        {
          getenv_last_error = GETENV_ERR_MEMORY;
          return 0;
        }
        strcpy(env.vars[env.vars_count - 1].key, _getenv_consume(key_length));

        // we consume the '='
        {
            dump = _getenv_consume();
            GETENV_FREE(dump);
            dump = NULL;
        }

        while((c = _getenv_peek(value_length)).has_value)
        {
            if(c.value == '\n')
                break;

            value_length++;
        }

        env.vars[env.vars_count - 1].value = GETENV_MALLOC(value_length + 1);
        if(env.vars[env.vars_count - 1].value == NULL)
        {
          getenv_last_error = GETENV_ERR_MEMORY;
          return 0;
        }
        strcpy(env.vars[env.vars_count - 1].value, _getenv_consume(value_length));
    }
  }

  return 1;
}

GETENV_LIB char* getenv_get_env(char* key)
{
  for(int i = 0; i < env.vars_count; ++i)
  {
      if(strcmp(env.vars[i].key, key) == 0)
          return env.vars[i].value;
  }

  getenv_last_error = GETENV_ERR_KEY_NOT_FOUND;
  return NULL;
}

GETENV_LIB char* getenv_strerror()
{
  switch(getenv_last_error)
  {
    case GETENV_ERR_NONE: return "No error";
    case GETENV_ERR_KEY_NOT_FOUND: return "Key not found";
    case GETENV_ERR_FILE_NOT_FOUND: return "Env file not found";
    case GETENV_ERR_MEMORY: return "Memory related error";
  }
}

GETENV_LIB void getenv_free()
{
  for (int i = 0; i < env.vars_count; ++i) {
    GETENV_FREE(env.vars[i].key);
    GETENV_FREE(env.vars[i].value);
  }
  
  GETENV_FREE(env.vars);
  GETENV_FREE(env.getenv_content);

  env = (getenv_t){0};
}

#endif // GETENV_LIB_IMPLEMENTATION

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // GETENV_H
