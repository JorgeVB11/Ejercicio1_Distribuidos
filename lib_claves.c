#include "claves.h"
#include "auxiliar.h"

/*Desarrollar el código que implementa los servicios anteriores (init, set_value, get_value,
delete_key, modify_value y exist). El código se desarrollará sobre el archivo con nombre
claves.c. Este es el código que ofrece la interfaz a los clientes y se encarga de implementar
los servicios anteriores (del lado del cliente) contactando con el servidor anterior. A partir de
dicha implementación se deberá crear una biblioteca dinámica denominada libclaves.so.
Esta será la biblioteca que utilizarán las aplicaciones de usuario que para usar el servicio. Debe
investigar y buscar la forma de crear dicha biblioteca.*/
// Created by jvinas on 2/26/24.

int longitud = 0;
int init()
{
    atributos.mq_flags = 0;
    atributos.mq_maxmsg = 10; // no sé si hay nº max de mensajes pongo este por poner algo. */
    atributos.mq_curmsgs = 0;
    atributos.mq_msgsize = sizeof(Mensaje);

    mqd_t queue = mq_open(MQ_NAME, O_CREAT | O_WRONLY, 0666, NULL);
    if (queue == -1)
    {
        perror("mq_open");
        return -1;
    }
    // Si he entendido bien los flags de la 2ª posicion son mis permisos y los de la 3ª posicion son los permisos para el resto de usuarios.
    Mensaje struct_to_send;
    struct_to_send.cod_operacion = 0;
    mq_send(queue, (char *)&struct_to_send, sizeof(struct_to_send), 1);
    mq_close(queue);
    // mq_unlink(MQ_NAME);
    return 0;
}

int set_value(int key, char *value1, int N_value2, double *V_value2)
{
    if (N_value2 < 1 || N_value2 > 32)
    {
        return -1;
    }
    mqd_t queue = mq_open(MQ_NAME, O_WRONLY);
    if (queue == -1)
    {
        perror("mq_open");
        return -1;
    }
    Mensaje struct_to_send;
    struct_to_send.cod_operacion = 1;
    struct_to_send.clave = key;
    strcpy(struct_to_send.value1, value1);

    struct_to_send.N_value2 = N_value2;
    for (int i = 0; i < N_value2; i++)
    {
        struct_to_send.V_value2[i] = V_value2[i];
    }
    mq_send(queue, (char *)&struct_to_send, sizeof(struct_to_send), 1);
    mq_close(queue);
    //mq_unlink(MQ_NAME);
    return 0;
}
int get_value(int key, char *value1, int *N_value2, double *V_value2)
{
    mqd_t queue = mq_open(MQ_NAME, O_CREAT | O_WRONLY);
    if (queue == -1)
    {
        perror("mq_open");
        return -1;
    }
    Mensaje struct_to_send;
    struct_to_send.cod_operacion = 2;
    struct_to_send.clave = key;
    strcpy(struct_to_send.value1, value1);
    struct_to_send.N_value2 = *N_value2;
    for (int i = 0; i < *N_value2; i++)
    {
        struct_to_send.V_value2[i] = V_value2[i];
    }
    mq_send(queue, (char *)&struct_to_send, sizeof(struct_to_send), 1);
    mq_close(queue);
    mq_unlink(MQ_NAME);
    return 0;
}
int delete_key(int key)
{
    mqd_t queue = mq_open(MQ_NAME, O_WRONLY);
    if (queue == -1)
    {
        perror("mq_open");
        return -1;
    }
    Mensaje struct_to_send;
    struct_to_send.cod_operacion = 3;
    struct_to_send.clave = key;
    mq_send(queue, (char *)&struct_to_send, sizeof(struct_to_send), 1);
    mq_close(queue);
    //mq_unlink(MQ_NAME);
    return 0;
}

int modify_value(int key, char *value1, int N_value2, double *V_value2)
{
    if (N_value2 < 1 || N_value2 > 32)
    {
        return -1;
    }
    mqd_t queue = mq_open(MQ_NAME, O_WRONLY);
    if (queue == -1)
    {
        perror("mq_open");
        return -1;
    }
    Mensaje struct_to_send;
    struct_to_send.cod_operacion = 4;
    struct_to_send.clave = key;
    strcpy(struct_to_send.value1, value1);
    struct_to_send.N_value2 = N_value2;
    for (int i = 0; i < N_value2; i++)
    {
        struct_to_send.V_value2[i] = V_value2[i];
    }
    mq_send(queue, (char *)&struct_to_send, sizeof(struct_to_send), 1);
    mq_close(queue);
    //mq_unlink(MQ_NAME);
    return 0;
}
int exist(int key)
{
    mqd_t queue = mq_open(MQ_NAME, O_WRONLY);
    if (queue == -1)
    {
        perror("mq_open");
        return -1;
    }
    Mensaje struct_to_send;
    struct_to_send.cod_operacion = 5;
    struct_to_send.clave = key;
    mq_send(queue, (char *)&struct_to_send, sizeof(struct_to_send), 1);
    mq_close(queue);
    //mq_unlink(MQ_NAME);
    return 0;
}
