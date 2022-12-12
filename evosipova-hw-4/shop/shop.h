#ifndef EVOSIPOVA_HW_4_SHOP
#define EVOSIPOVA_HW_4_SHOP

#include <stdio.h>

#include "../customer/customer.h"

typedef struct {
    unsigned id;
    char* items;
} customer_t;

void convert_customers(customers_t* customers, customer_t* result);
void start_shop(FILE* output, customer_t* customers, size_t customers_count);

#endif
