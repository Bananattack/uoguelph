#include <stdio.h>
#include <stdlib.h>

int main() {

   printf("before debug\n");
#if DEBUG
   printf("debugging line\n");
#endif
   printf("after debug\n");

   printf("%d\n", NUMBER);
   printf("%s\n", STR);
}

