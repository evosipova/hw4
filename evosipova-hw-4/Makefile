CC = gcc
CFLAGS = -g -lpthread
CFLAGS_OBJ = -g -c
FILES = main.c
TARGET = hw-4

TEMP_FOLDER = temp
LDLIBS = -lm temp/argument_resolver.a temp/customer.a temp/io.a temp/shop.a

.PHONY: all

all: clean prepare build

prepare:
	mkdir $(TEMP_FOLDER)

build_argument_resolver:
	mkdir $(TEMP_FOLDER)/argument_resolver

	$(CC) $(CFLAGS_OBJ) argument_resolver/count_flags.c -o $(TEMP_FOLDER)/argument_resolver/count_flags.o
	$(CC) $(CFLAGS_OBJ) argument_resolver/has_argument.c -o $(TEMP_FOLDER)/argument_resolver/has_argument.o
	$(CC) $(CFLAGS_OBJ) argument_resolver/resolve_argument.c -o $(TEMP_FOLDER)/argument_resolver/resolve_argument.o
	$(CC) $(CFLAGS_OBJ) argument_resolver/resolve_enter_type.c -o $(TEMP_FOLDER)/argument_resolver/resolve_enter_type.o
	$(CC) $(CFLAGS_OBJ) argument_resolver/resolve_file_descriptor.c -o $(TEMP_FOLDER)/argument_resolver/resolve_file_descriptor.o

	ar -rcsT $(TEMP_FOLDER)/argument_resolver.a $(TEMP_FOLDER)/argument_resolver/*

build_customer:
	mkdir $(TEMP_FOLDER)/customer

	$(CC) $(CFLAGS_OBJ) customer/clean_customers.c -o $(TEMP_FOLDER)/customer/clean_customers.o
	$(CC) $(CFLAGS_OBJ) customer/generate_customers.c -o $(TEMP_FOLDER)/customer/generate_customers.o
	$(CC) $(CFLAGS_OBJ) customer/print_generated_customers.c -o $(TEMP_FOLDER)/customer/print_generated_customers.o
	$(CC) $(CFLAGS_OBJ) customer/resolve_customers.c -o $(TEMP_FOLDER)/customer/resolve_customers.o

	ar -rcsT $(TEMP_FOLDER)/customer.a $(TEMP_FOLDER)/customer/* $(TEMP_FOLDER)/argument_resolver/count_flags.o

build_io:
	mkdir $(TEMP_FOLDER)/io

	$(CC) $(CFLAGS_OBJ) io/close_file_descriptor.c -o $(TEMP_FOLDER)/io/close_file_descriptor.o

	ar -rcsT $(TEMP_FOLDER)/io.a $(TEMP_FOLDER)/io/*

build_shop:
	mkdir $(TEMP_FOLDER)/shop

	$(CC) $(CFLAGS_OBJ) shop/convert_customers.c -o $(TEMP_FOLDER)/shop/convert_customers.o
	$(CC) $(CFLAGS_OBJ) shop/shop.c -o $(TEMP_FOLDER)/shop/shop.o

	ar -rcsT $(TEMP_FOLDER)/shop.a $(TEMP_FOLDER)/shop/*

build: build_argument_resolver build_customer build_io build_shop
	$(CC) $(FILES) $(CFLAGS) -o $(TARGET) $(LDLIBS)

clean:
	rm -rf $(TEMP_FOLDER)
	rm -rf $(TARGET)
