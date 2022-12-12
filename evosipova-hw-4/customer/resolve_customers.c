#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "customer.h"
#include "../argument_resolver/argument_resolver.h"

static const char CUSTOMER_ITEM_ARG[] = "--items";

/**
 * Resolves customers from CLI arguments
 *
 * @param argc Arguments count
 * @param argv Arguments array
 * @param customers Pointer to customers struct
 */
void resolve_customers_from_cli(int argc, char **argv, customers_t* customers) {
    if ((customers->count = count_flags(argc, argv, CUSTOMER_ITEM_ARG)) == 0) return;
    customers->customers = malloc(customers->count * sizeof(char*));

    char** static_items = malloc(customers->count * sizeof(char*));
    resolve_arguments(argc, argv, CUSTOMER_ITEM_ARG, static_items, customers->count);

    for (size_t it = 0; it < customers->count; ++it) {
        size_t items_count = strlen(static_items[it]);
        customers->customers[it] = calloc(items_count + 1, sizeof(char));
        strcpy(customers->customers[it], static_items[it]);
    }

    free(static_items);
}

/**
 * Resolves customers from input FD
 *
 * @param input Input FD
 * @param customers Pointer to customers struct
 */
void resolve_customers_from_input(FILE* input, customers_t* customers) {
    fscanf(input, "%zu", &customers->count);
    customers->customers = malloc(customers->count * sizeof(char*));

    for (size_t it = 0; it < customers->count; ++it) {
        size_t items_count;
        fscanf(input, "%zu", &items_count);
        customers->customers[it] = calloc(items_count + 1, sizeof(char));
        fscanf(input, "%s", customers->customers[it]);
    }
}
