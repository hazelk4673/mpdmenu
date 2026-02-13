#include <stdio.h>

int main() {
  /*
  char c;
  while ((c = getchar()) != EOF) {
    putchar(c);
  }
  return 0;
  */
  char buf[80];
  char c;
  for (int i = 0; ((c = getchar()) != EOF); i++) {
    buf[i] = c;
  }
  printf("success");
  printf("%s", buf);

}
