#include <string.h>

#include "argument_resolver.h"

/**
 * Check CLI argument entry
 *
 * @param argc Arguments count
 * @param argv Arguments array
 * @param flags Flag notation in short/long form
 * @return non-zero value, if argument presents. Otherwise, 0
 */
int has_argument_by_notation(int argc, char **argv, const flag_notation_t* flags) {
    for (int it = 1; it < argc - 1; ++it) {
        if (!strcmp(argv[it], flags->long_form)) return 1;
        if (!strcmp(argv[it], flags->short_form)) return 2;
    }

    return 0;
}
