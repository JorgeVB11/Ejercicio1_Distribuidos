#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// SERVER
const char *FILES_PATH = "./server_files/";
const char *FILE_ENDING = ".tupla";

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

// CLIENTE

#define MQ_NAME "server_queue_075"

typedef struct{
    int cod_operacion;
    int clave;
    char value1[256];
    int N_value2;
    double V_value2[32];
}Mensaje;

typedef struct {
        int mq_flags;
        int mq_maxmsg; //no sé si hay nº max de mensajes pongo este por poner algo. */
        int mq_curmsgs;
        int mq_msgsize;
}mq_attr;

mq_attr atributos;//lo declaro aquí para que sea global y asigno valores en el main() para que no haya problemas.
