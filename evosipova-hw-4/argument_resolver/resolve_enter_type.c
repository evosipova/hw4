#include <string.h>

#include "argument_resolver.h"

/**
 * Resolve enter mode
 *
 * @param argc Arguments count
 * @param argv Arguments array
 * @return Entry type
 */
enter_mode_e resolve_enter_mode(int argc, char **argv) {
    if (argc == 1) return STD; // Neither flags do not presented

    char* arg_value_long = resolve_argument(argc, argv, "--mode");
    char* arg_value_short = resolve_argument(argc, argv, "-m");

    if (arg_value_long && arg_value_short) return UNKNOWN; // Both flags present
    if (!arg_value_long && !arg_value_short) return STD; // Neither flags do not presented

    char* arg_value = arg_value_long == NULL ? arg_value_short : arg_value_long;

    if (!strcmp(arg_value, "std")) return STD;
    if (!strcmp(arg_value, "cli")) return CLI;
    if (!strcmp(arg_value, "file")) return TEXT_FILE;
    if (!strcmp(arg_value, "random")) return RANDOM;

    return UNKNOWN; // Undefined value
}
