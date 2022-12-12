#include <stdio.h>
#include <stdlib.h>

#include "argument_resolver/argument_resolver.h"
#include "customer/customer.h"
#include "io/io.h"
#include "shop/shop.h"

static const char READ_MODE[] = "r";
static const char WRITE_MODE[] = "w";

static const flag_notation_t INPUT_FILE_FLAGS = {"--in", "-i"};
static const flag_notation_t OUTPUT_FILE_FLAGS = {"--out", "-o"};

/**
 * Main function
 *
 * @param argc arguments count
 * @param argv arguments
 * @return
 */
int main(int argc, char **argv) {
    enter_mode_e enter_mode = resolve_enter_mode(argc, argv);

    FILE *input = NULL, *output = NULL;

    switch (enter_mode) {
        case UNKNOWN:
            fprintf(stderr, "Invalid enter mode. Use flag --mode or -m with available modes: std, cli, file, random\n");
            exit(-1);
        case STD:
            input = stdin;
            break;
        case TEXT_FILE:
            if ((input = resolve_file_descriptor(argc, argv, &INPUT_FILE_FLAGS, READ_MODE)) == NULL) {
                fprintf(stderr, "Failed to resolve input file. Use flag --in or -i\n");
                exit(-1);
            }

            break;
    }

    // Output FD

    if (has_argument_by_notation(argc, argv, &OUTPUT_FILE_FLAGS)) {
        output = resolve_file_descriptor(argc, argv, &OUTPUT_FILE_FLAGS, WRITE_MODE);
        if (output == NULL) {
            fprintf(stderr, "Invalid output file. Use flag --out or -o\n");
            exit(-1);
        }
    } else output = stdout;

    customers_t customers;

    switch (enter_mode) {
        case RANDOM:
            generate_customers(&customers);
            print_generated_customers(output, &customers);
            break;
        case CLI:
            resolve_customers_from_cli(argc, argv, &customers);
            break;
        case STD:
        case TEXT_FILE:
            resolve_customers_from_input(input, &customers);
            break;
    }

    customer_t* shop_customers = malloc(sizeof(customer_t) * customers.count);
    convert_customers(&customers, shop_customers);
    start_shop(output, shop_customers, customers.count);

    // Deallocate and clean

    free(shop_customers);

    clean_customers(&customers);
    close_file_descriptor(input);
    close_file_descriptor(output);

    return 0;
}
