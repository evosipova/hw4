#ifndef EVOSIPOVA_HW_4_ARGUMENT_RESOLVER
#define EVOSIPOVA_HW_4_ARGUMENT_RESOLVER

#include <stddef.h>
#include <stdio.h>

typedef enum {
    UNKNOWN, STD, CLI, TEXT_FILE, RANDOM
} enter_mode_e;

typedef struct {
    char* long_form;
    char* short_form;
} flag_notation_t;

size_t count_flags(int argc, char **argv, const char* arg_name);
int has_argument_by_notation(int argc, char **argv, const flag_notation_t* flags);

char* resolve_argument(int argc, char **argv, const char* arg_name);
void resolve_arguments(int argc, char **argv, const char* arg_name, char** result, size_t size);
enter_mode_e resolve_enter_mode(int argc, char **argv);
FILE* resolve_file_descriptor(int argc, char **argv, const flag_notation_t* flags, const char* modes);

#endif
