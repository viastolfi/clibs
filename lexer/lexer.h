/*
------------------------------------------------------------------------------
LEXER LIB FOR C - ASTOLFI Vincent 2025

/!\ Warning

This lib is greatly inspired by the stb_c_lexer lib 
This is just a try from me to understand it and rewrite it only using what I understand of it 
This is clear that some part of it are exactly the same as it is in the stb_c_lexer lib
There is still no copy past from it at all. Otherwise it would make no sence to rewrite it

*/

#ifndef LEXER_H
#define LEXER_H

#ifdef LEXER_LIB_IMPLEMENTATION
#ifndef LEXER_LIB_DEFINITION

// We need Y and N for later #if usage so the lib can be used in multiple context
#if defined(Y) || defined(N)
#error "You can only use this lexer lib if Y and N are not defined yet"
#endif

// BASIC INT IN DECIMAL FORM (ex: 12)
#define LEXER_LIB_DECIMAL_INTS Y // "0|[1-9][0-9]*"   LEX_intlit

// TODO: add all other possible token

#define LEXER_LIB_DEFINITION

#endif // LEXER_LIB_IMPLEMENTATION
#endif // LEXER_LIB_DEFINITION

typedef struct 
{
  const char* input_stream;
  const char* eof;

  // actual point where we are at in the parsing
  const char* parse_point;

  // Token variables
  long token;
  long int_number;
} lexer_t;

// We start at 256 since this is the end of the ASCII table
enum 
{
  LEXER_eof = 256,
  LEXER_intlit,
};

// So we can #if on each token definition
#define Y(x) 1
#define N(x) 0

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern int lexer_get_token(lexer_t* l);
extern void lexer_init_lexer(lexer_t* l, const char* input_stream, const char* end_input_stream);

#ifdef __cplusplus
  }
#endif // __cplusplus

#ifdef LEXER_LIB_IMPLEMENTATION

#ifndef LEXER_STDLIB
#define LEXER_STDLIB
#include <stdlib.h>
#endif // LEXER_STDLIB

static int lexer_is_white(char c) 
{
  return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f'; 
}

void lexer_init_lexer(lexer_t* l, const char* input_stream, const char* end_input_stream) 
{
  l->input_stream = input_stream;
  l->eof = end_input_stream;
  l->parse_point = input_stream;
}

int lexer_get_token(lexer_t* l) 
{
  const char* p = l->parse_point;

  for (;;) {
    while (p != l->eof && lexer_is_white(*p)) {
      ++p;
    }
  }
}

#endif // LEXER_LIB_IMPLEMENTATION
#endif // LEXER_H
