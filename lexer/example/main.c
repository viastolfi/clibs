#include <stdio.h>
#include <string.h>
#define LEXER_LIB_IMPLEMENTATION
#include "../lexer.h"

int main(void) 
{
  const char* text = "13 1 13 42 + += ++ - -- -= -> = == === => > < >= <= != !== !";  
  int len = strlen(text);

  lexer_t lexer;
  lexer_init_lexer(&lexer, text, text + len);

  while (lexer_get_token(&lexer)) {
    lexer_print_token(&lexer);
    printf("   ");
  } 
  
  // explicitely print the 'EOF' token for better debugging
  lexer_print_token(&lexer);

  return 0;
}
