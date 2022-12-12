#ifndef EVOSIPOVA_HW_4_CUSTOMER
#define EVOSIPOVA_HW_4_CUSTOMER

#include <stdio.h>
#include <stddef.h>

typedef struct {
    size_t count;
    char** customers;
} customers_t;

void clean_customers(customers_t *customers);
void generate_customers(customers_t *customers);
void print_generated_customers(FILE* output, customers_t *customers);
void resolve_customers_from_cli(int argc, char **argv, customers_t* customers);
void resolve_customers_from_input(FILE* input, customers_t* customers);

#endif
