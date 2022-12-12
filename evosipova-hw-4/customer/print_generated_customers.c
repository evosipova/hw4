#include <stdio.h>

#include "customer.h"

/**
 * Prints randomly generated customers' items
 *
 * @param output Output FD
 * @param customers Pointer to customers struct
 */
void print_generated_customers(FILE* output, customers_t *customers) {
    fprintf(output, "Generated %zu customers with given items list:\n\n", customers->count);
    for (size_t it = 0; it < customers->count; ++it) fprintf(output, "- %s\n", customers->customers[it]);
    fprintf(output, "\n");
}
