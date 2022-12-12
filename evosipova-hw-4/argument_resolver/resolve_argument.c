#include <string.h>

#include "argument_resolver.h"

/**
 * Resolves CLI argument
 *
 * @param argc Arguments count
 * @param argv Arguments array
 * @param arg_name Argument name
 * @return Argument value
 */
char* resolve_argument(int argc, char **argv, const char* arg_name) {
    for (int it = 1; it < argc - 1; ++it) {
        if (!strcmp(arg_name, argv[it])) return argv[it + 1];
    }

    return NULL;
}

/**
 * Resolves argument values
 *
 * @param argc Arguments count
 * @param argv Arguments array
 * @param arg_name Argument name
 * @param result Target array where values will be placed
 * @param size Flag entries count
 */
void resolve_arguments(int argc, char **argv, const char* arg_name, char** result, size_t size) {
    size_t current_iterator = 0;
    for (int it = 1; it < argc - 1 && current_iterator < size; ++it) {
        if (!strcmp(argv[it], arg_name)) result[current_iterator++] = argv[++it];
    }
}
