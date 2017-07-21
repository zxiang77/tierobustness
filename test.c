#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <igraph.h>

// int main()
// {
//     printf("Hello, World!\n");
//     char *str = (char *)malloc(0);
//     char *str2 = "tutorialspoint";
//     strcat(str, str2);
//     printf("[str] %s, [addr] %p\n", str, str);
//     printf("[str2] %s, [addr] %p\n", str2, str2);
//     // free(str);
//     printf("[str] %s, [addr] %p\n", str, str);
//     return 0;
// }

int main(int argc, char const *argv[]) {
  int b = 66;
  igraph_integer_t a = (igraph_integer_t) b;
  printf("[igraph_integer_t] %d\n", a);
  return 0;
}
