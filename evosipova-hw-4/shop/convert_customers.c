#include "shop.h"

/**
 * Converts customers struct to another customers type
 *
 * @param customers Pointer to customers struct
 * @param result Pointer to new customers array
 */
void convert_customers(customers_t* customers, customer_t* result) {
    for (size_t it = 0; it < customers->count; ++it) {
        result[it].id = it + 1;
        result[it].items = customers->customers[it];
    }
}
