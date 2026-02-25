
#include <stdio.h>

#define MAX 80

int main(int argc, char* argv[]) {

  volatile char c;
  char items[80];
  char* prompt;
  prompt = "> ";

  /* parsing item list */
  for (int i = 0; ((c = getchar()) != EOF); i++) {
    items[i] = c;
  }

  printf("%s", prompt);

  printf("newline encountered\n");
  printf("items: %s\n", items);
}
