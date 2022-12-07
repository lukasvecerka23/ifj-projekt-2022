/**
 * Project - IFJ Projekt 2022
 *
 * @author Lukas Vecerka xvecer30
 * @author Jachym Dolezal xdolez0c
 * @author Andrej Nespor xnespo10
 * @author Matej Tomko xtomko06
 *
 * @brief Header file of symtable.c
 */

#ifndef IFJ_SYMTABLE_H
#define IFJ_SYMTABLE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

/**
 * @brief constants for checking htab size when element is added or deleted
 */
#define AVG_LEN_MAX 1.5
#define AVG_LEN_MIN 0.35

/**
 * @enum Function parameter data types
 */
typedef enum { DTYPE_INT, DTYPE_STRING, DTYPE_FLOAT, DTYPE_NULL } DTYPE;

/**
 * @brief typedef for hash
 */
typedef unsigned int uint32_t;

/**
 * @enum Function return types
 */
typedef enum {
    RETTYPE_INT,
    RETTYPE_FLOAT,
    RETTYPE_STRING,
    RETTYPE_VOID
} RETURN_TYPE;

/**
 * @enum Identifier type
 */
typedef enum { ID_FUNC, ID_VAR } ID_TYPE;

typedef const char* htab_key_t;
typedef struct htab_item htab_item_t;

/**
 * @struct variable data
 */
typedef struct htab_var_data {
    bool optional_type;
    DTYPE data_type;
} htab_var_data_t;

/**
 * @struct function data
 */
typedef struct htab_func_data {
    bool defined;
    bool optional_ret_type;
    bool returned;
    RETURN_TYPE ret_type;
    size_t param_count;

} htab_func_data_t;

/**
 * @struct hashtable item data
 */
typedef struct htab_item_data {
    ID_TYPE type;
    char* name;
    union {
        htab_func_data_t func_data;
        htab_var_data_t var_data;
    };
} htab_item_data_t;

/**
 * @struct data for one hashtable item
 */
struct htab_item {
    htab_key_t key;
    htab_item_data_t* data;
    htab_item_t* next;
};

/**
 * @struct of hashtable
 */
typedef struct htab {
    size_t size;
    size_t arr_size;
    htab_item_t** arr_ptr;
} htab_t;

/**
 * Hashing function
 * @param str key which will be hashed
 * @return hash
 */
size_t htab_hash_function(htab_key_t str);

/**
 * Hashtable initialisation
 * @param size initial size of hashtable
 * @return pointer to hashtable
 */
htab_t* htab_init(size_t size);

/**
 * Function which fullness of table
 * @param table pointer to table
 * @return count of table elements
 */
size_t htab_size(htab_t* table);

/**
 * Function which return size of table
 * @param table pointer to table
 * @return number of table size
 */
size_t htab_bucket_count(htab_t* table);

/**
 * Function to resize given table
 * @param table pointer to table
 * @param newsize new size of table
 */
void htab_resize(htab_t* table, size_t newsize);

/**
 * Function which find hashtable element by key
 * @param table pointer to table
 * @param key element key to find
 * @return pointer to item data, if item not found return NULL
 */
htab_item_t* htab_search(htab_t* table, htab_key_t key);

/**
 * Function which insert new item to table, if item is already in table data are
 * updated with given data
 * @param table pointer to table
 * @param key key of item
 * @param data data of item
 * @return pointer to item data
 */
htab_item_t* htab_insert_update(htab_t* table,
                                htab_key_t key,
                                htab_item_data_t* data);

/**
 * Function which delete item by given key
 * @param table pointer to table
 * @param key item's key
 * @return true if item is successfuly deleted, false if item doesn't exits in
 * table
 */
bool htab_delete(htab_t* table, htab_key_t key);

/**
 * Function which clears all data from table and free table items
 * @param table pointer to table
 */
void htab_clear(htab_t* table);

/**
 * Clear table, free hashtable buckets and table itself
 * @param table pointer to table
 */
void htab_free(htab_t* table);
#endif
/*END OF FILE*/
