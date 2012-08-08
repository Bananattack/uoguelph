#include <stdio.h>
#include <stdlib.h>

#define STRING1 "Hello 1"
#define STRING2 "Hello 2"
#define STRING3 "Hello 3"

int main() {
	printf(STRING1);
#if 0
	printf(STRING2);
#endif
#if 1
	printf(STRING3);
#endif
}
