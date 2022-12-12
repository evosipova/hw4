#include "argument_resolver.h"

/**
 * Resolves FD from arguments
 *
 * @param argc Arguments count
 * @param argv Arguments array
 * @param flags Flag notation in short/long form
 * @param modes File open modes
 * @return FD reference
 */
FILE* resolve_file_descriptor(int argc, char **argv, const flag_notation_t* flags, const char* modes) {
    if (argc <= 2) return NULL;

    char* arg_value_long = resolve_argument(argc, argv, flags->long_form);
    char* arg_value_short = resolve_argument(argc, argv, flags->short_form);

    if ((arg_value_long && arg_value_short) || (!arg_value_long && !arg_value_short)) return NULL;

    char* arg_value = arg_value_long == NULL ? arg_value_short : arg_value_long;

    return fopen(arg_value, modes);
}
