#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "shop.h"

#define SELLERS_COUNT 2

typedef struct {
    unsigned seller_id;
} seller_args_t;

typedef struct customer_in_queue {
    customer_t* current;
    struct customer_in_queue* next;
} customer_in_queue_t;

static char is_finished = 0;
static FILE* OUTPUT;
static customer_in_queue_t* queues[SELLERS_COUNT] = {};
static customer_in_queue_t* queues_end[SELLERS_COUNT] = {};
static pthread_mutex_t queue_locks[SELLERS_COUNT] = {};
static pthread_mutex_t* customer_locks;

/**
 * Places customer in queue
 *
 * @param customer Pointer to customer struct
 * @param queue_id Queue (equals to seller's) ID
 */
static void allocate_customer_in_queue(customer_in_queue_t* customer, unsigned queue_id) {
    if (queues[queue_id] == NULL) {
        queues_end[queue_id] = customer;
        queues[queue_id] = customer;
    } else {
        queues_end[queue_id]->next = customer;
        queues_end[queue_id] = customer;
    }
}

/**
 * Seller entity
 *
 * @param args Pointer to seller arguments
 * @return NULL via pthread_join
 */
static void* run_seller(seller_args_t* args) {
    unsigned seller_id = args->seller_id;
    unsigned queue_id = seller_id - 1;
    fprintf(OUTPUT, "[SELLER %u] Initialized\n", seller_id);

    while (!is_finished) {
        if (queues[queue_id] != NULL) {
            unsigned sold_items = 0;
            customer_in_queue_t* customer = queues[queue_id];
            customer_t* current_customer = customer->current;
            char target_type = current_customer->items[0];

            fprintf(OUTPUT, "[SELLER %u] Maintaining client #%u\n", seller_id, current_customer->id);

            //
            // When seller is processing customer, customer thread is 'awaiting' in 'while' loop
            // After customer maintaining seller should obtain next customer from current struct, but customer may be
            // make this field NULL faster. This situation breaks queue, therefore we use mutex here which locks
            // customer that tries to make this field NULL
            //

            // Processes customer's item

            pthread_mutex_lock(customer_locks + (current_customer->id - 1));
            while (current_customer->items[0] == target_type) {
                current_customer->items++;
                sold_items++;
            }
            fprintf(OUTPUT, "[SELLER %u] Client #%u maintained. Sold items count: %u\n", seller_id, current_customer->id, sold_items);

            queues[queue_id] = queues[queue_id]->next;
            pthread_mutex_unlock(customer_locks + (current_customer->id - 1));
        }
    }

    fprintf(OUTPUT, "[SELLER %u] Finished\n", args->seller_id);
    return NULL;
}

/**
 * Customer entity
 *
 * @param customer_in_queue Pointer to customers struct
 * @return NULL via pthread_join
 */
static void* run_customer(customer_in_queue_t* customer_in_queue) {
    customer_t* customer = customer_in_queue->current;
    fprintf(OUTPUT, "[CUSTOMER %u] Initialized\n", customer->id);

    while (customer->items[0]) {
        // Reinitialize of customers

        pthread_mutex_lock(customer_locks + (customer->id - 1));
        customer_in_queue->next = NULL;
        pthread_mutex_unlock(customer_locks + (customer->id - 1));

        //
        // 1. Allocates customer in queue
        // 2. Awaiting for result
        //

        switch (customer->items[0]) {
            case 'A':
                pthread_mutex_lock(queue_locks);
                allocate_customer_in_queue(customer_in_queue, 0);
                fprintf(OUTPUT, "[CUSTOMER %u] Stood up in the first queue\n", customer->id);
                pthread_mutex_unlock(queue_locks);

                while (customer->items[0] == 'A');
                fprintf(OUTPUT, "[CUSTOMER %u] Left first queue\n", customer->id);

                break;
            case 'B':
                pthread_mutex_lock(queue_locks + 1);
                allocate_customer_in_queue(customer_in_queue, 1);
                fprintf(OUTPUT, "[CUSTOMER %u] Stood up in the second queue\n", customer->id);
                pthread_mutex_unlock(queue_locks + 1);

                while (customer->items[0] == 'B');
                fprintf(OUTPUT, "[CUSTOMER %u] Left second queue\n", customer->id);

                break;
        }
    }

    fprintf(OUTPUT, "[CUSTOMER %u] Finished\n", customer->id);
    return NULL;
}

/**
 * Initializes seller thread
 *
 * @param thid Pointer to pthread struct
 * @param seller_args Pointer to seller arguments struct
 */
static void init_seller(pthread_t *thid, seller_args_t* seller_args) {
    if (pthread_create(thid, NULL, (void *(*)(void *)) run_seller, seller_args) != 0) {
        fprintf(stderr, "Unable to initialize thread!\n");
        exit(-1);
    }
}

/**
 * Initializes customer thread
 *
 * @param thid Pointer to pthread struct
 * @param customer Pointer to customer struct
 */
static void init_customer(pthread_t *thid, customer_in_queue_t* customer) {
    if (pthread_create(thid, NULL, (void *(*)(void *)) run_customer, customer) != 0) {
        fprintf(stderr, "Unable to initialize customer!\n");
        exit(-1);
    }
}

/**
 * Gives identification number to seller
 *
 * @param sellers Pointer to sellers array
 */
static void initialize_sellers_args(seller_args_t* sellers) {
    for (unsigned it = 0; it < SELLERS_COUNT; ++it) sellers[it].seller_id = it + 1;
}

/**
 * Converts customers array to queue type array
 *
 * @param customers_in_queue Pointer to queue type array
 * @param customers Pointer to customers array
 * @param customers_count Customers count
 */
static void initialize_customers_in_queue(customer_in_queue_t* customers_in_queue, customer_t* customers, size_t customers_count) {
    for (unsigned it = 0; it < customers_count; ++it) customers_in_queue[it].current = customers + it;
}

/**
 * Entry point to shop entity
 *
 * @param output Output FD
 * @param customers Pointer to customers array
 * @param customers_count Customers count
 */
void start_shop(FILE* output, customer_t* customers, size_t customers_count) {
    OUTPUT = output;
    customer_locks = calloc(customers_count, sizeof(pthread_mutex_t));

    // Allocation

    pthread_t sellers[SELLERS_COUNT] = {};
    pthread_t* customers_threads = calloc(customers_count, sizeof(pthread_t));

    seller_args_t sellers_args[SELLERS_COUNT];
    initialize_sellers_args(sellers_args);

    customer_in_queue_t* customer_in_queue = calloc(customers_count, sizeof(customer_in_queue_t));
    initialize_customers_in_queue(customer_in_queue, customers, customers_count);

    // Mutexes initialize

    for (size_t it = 0; it < SELLERS_COUNT; ++it) pthread_mutex_init(queue_locks + it, NULL);
    for (size_t it = 0; it < customers_count; ++it) pthread_mutex_init(customer_locks + it, NULL);

    // Runs entities

    for (size_t it = 0; it < SELLERS_COUNT; ++it) init_seller(sellers + it, sellers_args + it);
    for (size_t it = 0; it < customers_count; ++it) init_customer(customers_threads + it, customer_in_queue + it);

    // Awaiting threads end

    for (size_t it = 0; it < customers_count; ++it) pthread_join(customers_threads[it], NULL);
    is_finished = 1;
    for (size_t it = 0; it < SELLERS_COUNT; ++it) pthread_join(sellers[it], NULL);

    // Destroys mutexes

    for (size_t it = 0; it < SELLERS_COUNT; ++it) pthread_mutex_destroy(queue_locks + it);
    for (size_t it = 0; it < customers_count; ++it) pthread_mutex_destroy(customer_locks + it);

    // Deallocate

    free(customer_locks);
    free(customers_threads);
    free(customer_in_queue);
}
