#include <string.h>

#include "argument_resolver.h"

/**
 * Counts given flag entries
 *
 * @param argc Arguments count
 * @param argv Arguments array
 * @param arg_name Desired argument
 * @return Entries count
 */
size_t count_flags(int argc, char **argv, const char* arg_name) {
    size_t result = 0;
    for (int it = 1; it < argc - 1; ++it) {
        if (!strcmp(argv[it], arg_name)) result++;
    }

    return result;
}
