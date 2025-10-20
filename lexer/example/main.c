#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LEXER_LIB_IMPLEMENTATION
#include "../lexer.h"

int main(void) 
{
  const char* text = "let test _var const int9 u9";
  int len = strlen(text);

  lexer_t lexer;
  lexer_init_lexer(&lexer, text, text + len, malloc(255), 255);

  while (lexer_get_token(&lexer)) {
    if (lexer.token == LEXER_token_parse_error) {
      printf("\n<<<PARSE ERROR>>>\n");
      break; 
    }
    lexer_print_token(&lexer);
    printf("   ");
  } 
  
  // explicitely print the 'EOF' token for better debugging
  lexer_print_token(&lexer);

  return 0;
}
