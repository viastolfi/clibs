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



#endif // LEXER_H
