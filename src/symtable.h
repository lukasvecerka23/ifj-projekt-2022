#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

#include <stdbool.h>
#include <string.h>

typedef enum { DTYPE_INT, DTYPE_STRING, DTYPE_FLOAT, DTYPE_NULL } DTYPE;

typedef unsigned int uint32_t;

typedef enum {
    RETTYPE_INT,
    RETTYPE_FLOAT,
    RETTYPE_STRING,
    RETTYPE_VOID
} RETURN_TYPE;

typedef enum { ID_FUNC, ID_VAR } ID_TYPE;

typedef const char* htab_key_t;
typedef struct htab_item htab_item_t;

typedef struct htab_var_data {
    bool init;
    bool optional_type;
    DTYPE data_type;
} htab_var_data_t;

typedef struct htab_func_data {
    bool defined;
    bool optional_ret_type;
    RETURN_TYPE ret_type;
    size_t param_count;

} htab_func_data_t;

typedef struct htab_item_data {
    ID_TYPE type;
    char* name;
    union {
        htab_func_data_t func_data;
        htab_var_data_t var_data;
    };
} htab_item_data_t;

struct htab_item {
    htab_key_t key;
    htab_item_data_t* data;
    htab_item_t* next;
};

typedef struct htab {
    size_t size;
    size_t arr_size;
    htab_item_t** arr_ptr;
} htab_t;

size_t htab_hash_function(htab_key_t str);

htab_t* htab_init(size_t size);
size_t htab_size(htab_t* table);
size_t htab_bucket_count(htab_t* table);
void htab_resize(htab_t* table, size_t newsize);

htab_item_t* htab_search(htab_t* table, htab_key_t key);
htab_item_t* htab_insert_update(htab_t* table,
                                htab_key_t key,
                                htab_item_data_t* data);

bool htab_delete(htab_t* table, htab_key_t key);

void htab_clear(htab_t* table);
void htab_free(htab_t* table);

void ht_print_table(htab_t* table, char* table_type);
#endif
