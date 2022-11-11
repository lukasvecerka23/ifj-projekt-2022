#include <stdbool.h>
#include <string.h>

typedef enum { DTYPE_INT, DTYPE_STRING, DTYPE_FLOAT, DTYPE_NULL } DTYPE;

typedef enum {
    RETTYPE_INT,
    RETTYPE_FLOAT,
    RETTYPE_STRING,
    RETTYPE_VOID
} RETURN_TYPE;

typedef const char* htab_key_t;

typedef struct htab_func_data {
    bool defined;
    char* name;
    RETURN_TYPE ret_type;
    htab_item_t** params;

} htab_func_data_t;

typedef struct htab_var_data {
    char* name;
    bool init;
    DTYPE data_type;
} htab_var_data_t;

typedef struct htab_item {
    htab_key_t key;
    union {
        htab_func_data_t func_data;
        htab_var_data_t var_data;
    };
    htab_item_t* next;
} htab_item_t;

typedef struct htab {
    size_t size;
    size_t arr_size;
    htab_item_t** arr_ptr;
} htab_t;