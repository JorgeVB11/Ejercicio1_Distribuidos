#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>
#include <pthread.h>
#include "auxiliar.h"

char *get_file_name(int key);
int write_file(char *file_name, struct tupla *tupla);
int read_file(char *file_name, struct tupla *tupla);
int init();
int set_value(int key, char *value1, int N_value2, double *V_value2);
int get_value(int key, char *value1, int *N_value2, double *V_value2);
int delete_key(int key);
int modify_value(int key, char *value1, int N_value2, double *V_value2);
int exist(int key);
void* gestionar_peticion(void *argumento);

// Mutex para el acceso a archivos
extern pthread_mutex_t mutex_archivos;
extern pthread_cond_t cond_archivos;
extern int ocupado;

#endif