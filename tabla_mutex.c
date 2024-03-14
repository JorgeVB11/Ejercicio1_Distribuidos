#include "tabla_mutex.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

tb_mtx_item* create_item(char *file_name) {
    tb_mtx_item *item = (tb_mtx_item*) malloc(sizeof(tb_mtx_item));
    strcpy(item->file_name, file_name);
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    item->mutex = &mutex;
    item->var_cond = &cond;
    item->ocupado = 0; // 0 significa no ocupado
    pthread_mutex_init(item->mutex, NULL);
    pthread_cond_init(item->var_cond, NULL);
    return item;
}

tabla_mutex* create_table() {
    tabla_mutex* tabla = (tabla_mutex*) malloc(sizeof(tabla_mutex));
    tabla->count = 0;
    return tabla;
}

void free_item(tb_mtx_item* item) {
    pthread_mutex_destroy(item->mutex);
    pthread_cond_destroy(item->var_cond);
    free(item->file_name);
    free(item);
}
void free_table(tabla_mutex* tabla) {
    for (int i = 0; i < tabla->count; i++){
        free_item(tabla->items[i]);
    }
    free(tabla->items);
    free(tabla);
}
int tb_mtx_search(tabla_mutex* tabla, char* file_name) {
    int indice = -1;
    for (int i = 0; i < tabla->count; i++){
        if (strcmp(tabla->items[i]->file_name, file_name) == 0) {
            indice = i;
        }
    }
    return indice;
}
int tb_mtx_insert(tabla_mutex* tabla, char* file_name) {
    if (tb_mtx_search(tabla, file_name) == -1) {
        return -1;
    }
    tb_mtx_item *item = create_item(file_name);

    tabla->items = realloc(tabla->items, sizeof(tb_mtx_item*)); 
    tabla->items[tabla->count] = item;
    tabla->count++;
    return 0;
}

int main() {
    tabla_mutex *tabla = create_table();
    tb_mtx_insert(tabla, "patata");
    tb_mtx_insert(tabla, "pollo");
}

