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
#define LEXER_LIB_DECIMAL_INTS Y // "0|[1-9][0-9]*"   LEXER_token_intlit
#define LEXER_LIB_INCREMENTS   Y // "++"              LEXER_token_plusplus
#define LEXER_LIB_PLUSEQ       Y // "+="              LEXER_token_pluseq

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
  LEXER_token_eof = 256,
  LEXER_token_intlit,
  LEXER_token_plus,
  LEXER_token_plusplus,
  LEXER_token_pluseq
};


// So we can #if on each token definition
#define Y(x) 1
#define N(x) 0

#if LEXER_LIB_DECIMAL_INTS(x)
#define LEXER_decimal_ints
#endif // LEXER_LIB_DECIMAL_INTS

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

// Disable the 'config' usage of Y and N
// Now, Y expand to its content and N to nothing
// This avoid compiler error
#undef Y
#define Y(a) a
#undef N
#define N(a)

static int lexer_is_white(char c) 
{
  return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\f'; 
}

static int lexer_eof(lexer_t* l)
{
  l->token = LEXER_token_eof;
  return 0;
}

// Helper function for creating a token and updating the lexer
static int lexer_create_token(lexer_t* l, int token, const char* end)
{
  l->token = token;

  l->parse_point = end + 1;
  return 1;
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
    break;
  }

  if (p == l->eof)
    return lexer_eof(l);

  switch (*p) {
    default:
      // not implemented
      return 0;
      
    single_char:
      return lexer_create_token(l, *p, p);

    case '+':
      if (p+1 != l->eof) {
        LEXER_LIB_INCREMENTS(if (p[1] == '+') return lexer_create_token(l, LEXER_token_plusplus, p+1);)
        LEXER_LIB_PLUSEQ(if (p[1] == '=') return lexer_create_token(l, LEXER_token_pluseq, p+1);)
      }
      goto single_char;
    case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
    #ifdef LEXER_decimal_ints
    {
      const char* q = p;
      #ifdef LEXER_STDLIB
      /* strtol -> string to long converter
       * params: 
       * * ptr of the string we want to convert
       * * ptr that will indicate the end of the long (ie: '12345Toto q -> 'T')
       * * type of the number (ie: 10 decimal number)
       * return the converted long value
       */
      l->int_number = strtol((const char *) p, (char **) &q, 10);  
      #else
      // TODO: implement case of no stdlib
      return 0;
      #endif // LEXER_STDLIB
      lexer_create_token(l, LEXER_token_intlit, q);
    }
    // TODO: add some suffixe parsing (ie: 42U)
    #endif // LEXER_decimal_ints
  }
}

static void lexer_print_token(lexer_t *l) 
{
  switch (l->token)
  {
    case LEXER_token_eof: printf("EOF"); break;
    case LEXER_token_intlit: printf("#%ld", l->int_number); break;
    case LEXER_token_plus: printf("+"); break;
    case LEXER_token_plusplus: printf("++"); break;
    case LEXER_token_pluseq: printf("+="); break;
    default:
      if (l->token >= 0 && l->token < 256)
        printf("%c", (int) l->token);
      else {
        printf("<<<UNKNOWN TOKEN %ld >>>\n", l->token);
      }
      break;
  }
}

#endif // LEXER_LIB_IMPLEMENTATION
#endif // LEXER_H
