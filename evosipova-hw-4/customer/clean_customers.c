#include "customer.h"

#include <stdlib.h>

/**
 * Clean dynamically-allocated customers struct
 *
 * @param customers Pointer to customers struct
 */
void clean_customers(customers_t *customers) {
    for (size_t it = 0; it < customers->count; ++it) free(customers->customers[it]);
    free(customers->customers);
}
