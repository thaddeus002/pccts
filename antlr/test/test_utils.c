/**
 * Tests of "utils" module.
 */

#include <string.h>
#include <stdio.h>
#include "../utils.h"

int main() {

    char quoted[20] = "\"quoted string\"";
    char *result = strip_quotes(quoted);

    if(strcmp(result, "quoted string")) {
        fprintf(stderr, "Unexpected result for strip_quotes() : %s\n", result);
        return 1;
    }

    return 0;
}
