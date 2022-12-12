#include "customer.h"

#include <stdlib.h>
#include <time.h>

#define CUSTOMERS_MIN_COUNT 2
#define CUSTOMERS_MAX_COUNT 10
#define CUSTOMER_ITEM_MIN_COUNT 5
#define CUSTOMER_ITEM_MAX_COUNT 15

/**
 * Random generation of integer number in range [start; end]
 *
 * @param start Left bound
 * @param end Right bound
 * @return Random number
 */
static int randint(int start, int end) {
    return rand() % (end - start + 1) + start;
}

/**
 * Randomized generation of customers
 *
 * @param customers Pointer to customers struct
 */
void generate_customers(customers_t *customers) {
    srand(time(0));
    customers->count = randint(CUSTOMERS_MIN_COUNT, CUSTOMERS_MAX_COUNT);
    customers->customers = malloc(sizeof(char*) * customers->count);

    for (size_t it = 0; it < customers->count; ++it) {
        size_t items_count = randint(CUSTOMER_ITEM_MIN_COUNT, CUSTOMER_ITEM_MAX_COUNT);
        customers->customers[it] = calloc(items_count + 1, sizeof(char));

        for (size_t item = 0; item < items_count; ++item) {
            customers->customers[it][item] = (char) randint('A', 'B');
        }
    }
}
