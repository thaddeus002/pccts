#include <stdio.h>


/** Hash 's' using 'size'. */
static unsigned int hash(char *s, unsigned int size) {

  unsigned int h = 0;

  while ( *s != '\0' ) {
    h = (h<<1) + *s++;
  }
  return (h % size);
}



int main(int argc, char **argv) {

  char *s="A";

  fprintf(stdout, "hash de A = %d\n", hash("A", 2000));
  fprintf(stdout, "hash de B = %d\n", hash("B", 2000));
  fprintf(stdout, "hash de C = %d\n", hash("C", 2000));
  fprintf(stdout, "hash de AA = %d\n", hash("AA", 2000));

  fprintf(stdout, "hash de A = %d - %s\n", hash(s, 2000), s);
  fprintf(stdout, "hash de A = %d - %s\n", hash(s, 2000), s);
	



  return 0;
}
