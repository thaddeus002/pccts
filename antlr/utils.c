
#include <ctype.h> // toupper()
#include "utils.h"


/**
 * Compare two string case insentive way.
 * \return true if the strings are equal
 */
int ci_strequ(char *a, char *b)
{
  for ( ;*a != 0 && *b != 0; a++, b++) {
    if (toupper(*a) != toupper(*b)) return 0;
  }
  return (*a == *b);
}

