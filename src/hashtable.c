#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>

#define AVG_LEN_MAX 1.5
#define AVG_LEN_MIN 0.35

// hashovaci funkce
size_t htab_hash_function(htab_key_t str) {
    uint32_t hash = 0;
    unsigned char* p;

    for (p = (unsigned char*)str; *p != '\0'; p++) {
        hash = 65599 * hash + *p;
    }
    return hash;
}

// inicializace tabulky
htab_t* htab_init(size_t size) {
    if (size == 0) {
        error_exit("Chyba: tabulka musí obsahovat alespoň jeden řádek.");
    }
    htab_t* htab_new = malloc(sizeof(htab_t));
    if (htab_new == NULL) {
        return NULL;
    }
    htab_new->arr_ptr = malloc(sizeof(htab_item_t*) * size);
    if (htab_new->arr_ptr == NULL) {
        return NULL;
    }

    htab_new->size = 0;
    htab_new->arr_size = size;

    for (size_t i = 0; i < size; i++) {
        htab_new->arr_ptr[i] = NULL;
    }

    return htab_new;
}

// funkce pro zjisteni plnosti tabulky
size_t htab_size(htab_t* table) {
    return table->size;
}

// funkce pro zjisteni velikosti tabulky
size_t htab_bucket_count(const htab_t* table) {
    return table->arr_size;
}

// zvetseni tabulky
void htab_resize(htab_t* table, size_t newsize) {
    size_t old_size = table->arr_size;
    table->arr_size = newsize;
    htab_item_t** old_arr_ptr = table->arr_ptr;
    table->arr_ptr = calloc(newsize, sizeof(htab_item_t*));

    for (size_t i = 0; i < old_size; i++) {
        if (old_arr_ptr[i] == NULL) {
            continue;
        } else {
            htab_item_t* tmp = old_arr_ptr[i];
            while (tmp != NULL) {
                htab_item_t* tmp2 = tmp->next;
                htab_lookup_add(table, tmp->key, tmp->data);
                free((char*)tmp->key);
                free(tmp);
                table->size--;
                tmp = tmp2;
            }
            old_arr_ptr[i] = NULL;
        }
    }
    free(old_arr_ptr);
}

htab_item_t* htab_find(htab_t* table, htab_key_t key) {
    size_t index = htab_hash_function(key) % table->arr_size;
    htab_item_t* tmp = table->arr_ptr[index];

    if (tmp == NULL) {
        return NULL;
    }

    while (tmp != NULL) {
        if (!strcmp(tmp->key, key)) {
            return &(tmp->data);
        }
        tmp = tmp->next;
    }

    return NULL;
}

htab_item_t* create_new_item(htab_key_t key, htab_item_data_type_t data) {
    htab_item_t* new_item = malloc(sizeof(htab_item_t));
    if (new_item == NULL) {
        return NULL;
    }

    size_t str_size = strlen(key) + 1;
    new_item->key = malloc(str_size);
    if (new_item->key == NULL) {
        free(new_item);
        return NULL;
    }
    memcpy((char*)new_item->key, key, str_size);
    new_item->next = NULL;
    new_item->data = data;

    return new_item;
}
// try to search item with given key, when key's not found create new element
// with this key
htab_item_t* htab_lookup_add(htab_t* table,
                             htab_key_t key,
                             htab_item_data_type_t data) {
    if (table->size > table->arr_size * AVG_LEN_MAX) {
        htab_resize(table, (table->arr_size * 2));
    }
    size_t index = (htab_hash_function(key) % table->arr_size);
    htab_item_t* tmp = table->arr_ptr[index];
    htab_item_t* p = htab_find(table, key);
    if (p != NULL) {
        return p;
    }

    htab_item_t* new_item = create_new_pair(key, data);
    if (new_item == NULL) {
        return NULL;
    }
    table->size++;

    if (table->arr_ptr[index] == NULL) {
        table->arr_ptr[index] = new_item;
    } else {
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = new_item;
    }
    return &(new_item);
}

// delete item with specified key
bool htab_erase(htab_t* table, htab_key_t key) {
    if (table->size < table->arr_size * AVG_LEN_MIN) {
        htab_resize(table, (table->arr_size / 2));
    }
    size_t index = htab_hash_function(key) % table->arr_size;
    htab_item_t* tmp = table->arr_ptr[index];

    if (tmp == NULL) {
        return false;
    }

    if (!strcmp(tmp->key, key)) {
        if (tmp->next == NULL) {
            free((char*)tmp->key);
            free(tmp);
            table->arr_ptr[index] = NULL;
        } else {
            htab_item_t* tmp_next = tmp->next;
            free((char*)tmp->key);
            free(tmp);
            table->arr_ptr[index] = tmp_next;
        }
        table->size--;
        return true;
    }

    while (tmp->next != NULL) {
        if (!strcmp(tmp->next->key, key)) {
            htab_item_t* tmp_next = tmp->next->next;
            free((char*)tmp->next->key);
            free(tmp->next);
            tmp->next = tmp_next;
        }
        if (tmp->next != NULL) {
            tmp = tmp->next;
        } else {
            continue;
        }
    }
    table->size--;
    return true;
}

// delete all items from table
void htab_clear(htab_t* table) {
    for (size_t i = 0; i < table->arr_size; i++) {
        if (table->arr_ptr[i] != NULL) {
            htab_item_t* pair = table->arr_ptr[i];
            while (pair != NULL) {
                htab_item_t* tmp = pair->next;
                free((char*)pair->key);
                free(pair);
                pair = tmp;
            }
            table->arr_ptr[i] = NULL;
        }
    }
}

// clear table and free memory of hashtable
void htab_free(htab_t* table) {
    htab_clear(table);
    free(table->arr_ptr);
    free(table);
}