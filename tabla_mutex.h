#ifndef TABLA_MUTEX_H
#define TABLA_MUTEX_H

#include <pthread.h>

typedef struct tb_mtx_item {
    char file_name[256]; // Nombre del archivo
    pthread_mutex_t *mutex; // Puntero al mutex
    pthread_cond_t *var_cond; // Puntero a la variable de condicion
    int ocupado; // Variable para evitar despertares espureos
    // 0 libre, 1 ocupado
}tb_mtx_item;

typedef struct tabla_mutex {
    tb_mtx_item **items; // Array de punteros (dinámico)
    int count; // Número de elementos
    pthread_mutex_t *mutex; // Puntero al mutex
    pthread_cond_t *var_cond; // Puntero a la variable de condicion
    int ocupado; // Variable para evitar despertares espureos
} tabla_mutex;

tb_mtx_item* create_item(char *file_name);
tabla_mutex* create_table();
void free_item(tb_mtx_item* item);
void free_table(tabla_mutex* tabla);
// Devuelve 0 en caso de exito, -1 en caso de error 
int tb_mtx_insert(tabla_mutex* tabla, char* file_name);

// Devuelve el indice del elemento encontrado, -1 en caso de que no exista y -2 en caso de error
int tb_mtx_search(tabla_mutex* tabla, char* file_name); 

#endif