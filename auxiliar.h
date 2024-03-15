#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MQ_NAME "/SERVER_QUEUE_075"

// SERVER
#define FILES_PATH "./server_files/"
#define FILE_ENDING ".tupla"

// CLIENTE

typedef struct{
    int cod_operacion;
    int clave;
    char value1[256];
    int N_value2;
    double V_value2[32];
    char cola_respuesta[128];
}Mensaje;
typedef struct{
    int clave;
    int resultado;
    char value1[256];
    int N_value2;
    double V_value2[32];
}Respuesta;
// Estructura para que sea más fácil guardar la información
struct tupla
{
    int key;
    char value1[256];
    int N_value2;
    double V_value2[32];
};


#endif