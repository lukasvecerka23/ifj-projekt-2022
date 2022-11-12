#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_HT_SIZE 101

int HT_SIZE;

#define TEST(NAME, DESCRIPTION) \
    void NAME() {               \
        printf("[%s] %s\n", #NAME, DESCRIPTION);

#define ENDTEST                 \
    printf("\n");               \
    ht_print_table(test_table); \
    htab_free(test_table);      \
    printf("\n");               \
    }

htab_item_t* uninitialized_item;

void ht_print_table(htab_t* table) {
    int max_count = 0;
    int sum_count = 0;

    printf("------------HASH TABLE--------------\n");
    for (int i = 0; i < table->arr_size; i++) {
        printf("%i: ", i);
        int count = 0;
        htab_item_t* item = table->arr_ptr[i];
        while (item != NULL) {
            if (item->data.func_data != NULL)
                printf("(%s, %d)", item->key, item->data.func_data->ret_type);
            else
                printf("(%s)", item->key);
            if (item != uninitialized_item) {
                count++;
            }
            item = item->next;
        }
        printf("\n");
        if (count > max_count) {
            max_count = count;
        }
    }

    printf("------------------------------------\n");
    printf("Total items in hash table: %i\n", sum_count);
    printf("Maximum hash collisions: %i\n", max_count == 0 ? 0 : max_count - 1);
    printf("------------------------------------\n");
}

void init_uninitialized_item() {
    uninitialized_item = (htab_item_t*)malloc(sizeof(htab_item_t));
    uninitialized_item->key = "*UNINITIALIZED*";
    uninitialized_item->data.func_data = NULL;
    uninitialized_item->next = NULL;
}

void init_test_table(htab_t* table) {
    table = (htab_t*)malloc(sizeof(htab_t));
    for (int i = 0; i < MAX_HT_SIZE; i++) {
        table->arr_ptr[i] = uninitialized_item;
    };
}

void ht_insert_many(htab_t* table, const htab_item_t items[], int count) {
    for (int i = 0; i < count; i++) {
        htab_search_insert(table, items[i].key, items[i].data);
    }
}

void init_test() {
    printf("Hash Table - testing script\n");
    printf("---------------------------\n");
    HT_SIZE = 13;
    printf("\nSetting HT_SIZE to prime number (%i)\n", HT_SIZE);
    printf("\n");
}

TEST(test_table_init, "Initialize the table")
htab_t* test_table = htab_init(HT_SIZE);
ENDTEST

TEST(test_search_nonexist, "Search for a non-existing item")
htab_t* test_table = htab_init(HT_SIZE);
if (htab_search(test_table, "Ethereum")) {
    printf("found");
} else {
    printf("not found");
}
ENDTEST

TEST(test_insert_simple, "Insert a new item")
htab_t* test_table = htab_init(HT_SIZE);
htab_func_data_t* func_item =
    (htab_func_data_t*)malloc(sizeof(htab_func_data_t));
func_item->defined = false;
func_item->params = NULL;
func_item->ret_type = RETTYPE_VOID;
htab_item_data_type_t new_data;
new_data.func_data = func_item;

htab_search_insert(test_table, "funkce", new_data);
ENDTEST

TEST(test_search_exist, "Search for an existing item")
htab_t* test_table = htab_init(HT_SIZE);
htab_func_data_t* func_item =
    (htab_func_data_t*)malloc(sizeof(htab_func_data_t));
func_item->defined = false;
func_item->params = NULL;
func_item->ret_type = RETTYPE_VOID;
htab_item_data_type_t new_data;
new_data.func_data = func_item;

htab_search_insert(test_table, "funkce", new_data);
if (htab_search(test_table, "funkce")) {
    printf("found");
} else {
    printf("not found");
}
htab_delete(test_table, "funkce");
ENDTEST

int main(int argc, char* argv[]) {
    init_uninitialized_item();
    init_test();

    test_table_init();
    test_search_nonexist();
    test_insert_simple();
    test_search_exist();
}
