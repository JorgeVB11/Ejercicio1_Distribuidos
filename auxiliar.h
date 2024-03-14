#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MQ_NAME "/SERVER_QUEUE_075"

// SERVER
const char *FILES_PATH = "./server_files/";
const char *FILE_ENDING = ".tupla";

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
char* get_file_name(int key);
int write_file (char* file_name, struct tupla *tupla);
int read_file(char *file_name, struct tupla *tupla);


void gestionar_peticion(Mensaje mensaje);

struct mq_attr atributos;//lo declaro aquí para que sea global y asigno valores en el main() para que no haya problemas.
struct mq_attr atributos_respuesta;