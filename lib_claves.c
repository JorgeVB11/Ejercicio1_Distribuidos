#include "claves.h"
#include "auxiliar.h"
#include <pthread.h>
#include <unistd.h>

/*Desarrollar el código que implementa los servicios anteriores (init, set_value, get_value,
delete_key, modify_value y exist). El código se desarrollará sobre el archivo con nombre
claves.c. Este es el código que ofrece la interfaz a los clientes y se encarga de implementar
los servicios anteriores (del lado del cliente) contactando con el servidor anterior. A partir de
dicha implementación se deberá crear una biblioteca dinámica denominada libclaves.so.
Esta será la biblioteca que utilizarán las aplicaciones de usuario que para usar el servicio. Debe
investigar y buscar la forma de crear dicha biblioteca.*/
// Created by jvinas on 2/26/24.

int init()
{
    mqd_t queue = mq_open(MQ_NAME, O_WRONLY);
    if (queue == -1)
    {
        perror("mq_open");
        return -1;
    }
    Mensaje struct_to_send; //creamos el mensaje que queremos enviar
    struct_to_send.cod_operacion = 0; //le asignamos un 0 porque queremos hacer un init()
    sprintf(struct_to_send.cola_respuesta, "/cliente_%d_%ld", getpid(), pthread_self());
    //Para crear la cola de respuesta necesitamos un nombre único,
    //por eso concatenamos el identificador de hilo y el identificador de proceso.
    if (mq_send(queue, (char *)&struct_to_send, sizeof(struct_to_send), 1) == -1)
    {
        perror("mq_send");
        return -1;
    }
    mq_close(queue);
    struct mq_attr atributos_respuesta;
    atributos_respuesta.mq_flags = 0;
    atributos_respuesta.mq_maxmsg = 10; // no sé si hay nº max de mensajes pongo este por poner algo. */
    atributos_respuesta.mq_curmsgs = 0;
    atributos_respuesta.mq_msgsize = sizeof(Respuesta);
    //abrimos la cola en la que vamos a recibir la respuesta
    mqd_t cola_cliente = mq_open(struct_to_send.cola_respuesta, O_RDONLY | O_CREAT, 0666, &atributos_respuesta);
    if (cola_cliente == -1)
    {
        perror("mq_open");
        return -1;
    }
    char buffer[sizeof(Respuesta)];
    //Recibimos
    if (mq_receive(cola_cliente, buffer, sizeof(Respuesta), NULL) == -1)
    {
        perror("mq_receive");
        return -1;
    }
    mq_close(cola_cliente);
    Respuesta respuesta;
    memmove(&respuesta, buffer, sizeof(Respuesta));
    return respuesta.resultado;
}

int set_value(int key, char *value1, int N_value2, double *V_value2)
{
    if (N_value2 < 1 || N_value2 > 32)
    {
        return -1;
    }
    mqd_t queue = mq_open(MQ_NAME, O_WRONLY);
    ;
    if (queue == -1)
    {
        perror("mq_open");
        return -1;
    }
    Mensaje struct_to_send;
    //En este caso además del codigo de operacion añadiremos otros campos relevantes a la estructura de envio
    //en concreto clave, value1, N_value2 y V_value2
    struct_to_send.cod_operacion = 1;
    struct_to_send.clave = key;
    strcpy(struct_to_send.value1, value1);

    struct_to_send.N_value2 = N_value2;
    for (int i = 0; i < N_value2; i++)
    {
        struct_to_send.V_value2[i] = V_value2[i];
    }
    sprintf(struct_to_send.cola_respuesta, "/cliente_%d_%ld", getpid(), pthread_self());
    if (mq_send(queue, (char *)&struct_to_send, sizeof(struct_to_send), 1) == -1)
    {
        perror("mq_send");
        return -1;
    }
    mq_close(queue);
    mqd_t cola_cliente = mq_open(struct_to_send.cola_respuesta, O_RDONLY);
    if (cola_cliente == -1)
    {
        perror("mq_open");
        return -1;
    }
    char buffer[sizeof(Respuesta)];
    if (mq_receive(cola_cliente, buffer, sizeof(Respuesta), NULL)==-1){
        perror("mq_receive");
        return -1;
    }
    mq_close(cola_cliente);
    Respuesta respuesta;
    memmove(&respuesta, buffer, sizeof(Respuesta));
    return respuesta.resultado;
}
int get_value(int key, char *value1, int *N_value2, double *V_value2)
{
    mqd_t queue = mq_open(MQ_NAME, O_WRONLY);
    if (queue == -1)
    {
        perror("mq_open");
        return -1;
    }
    Mensaje struct_to_send;
    struct_to_send.cod_operacion = 2;
    struct_to_send.clave = key;
    
    sprintf(struct_to_send.cola_respuesta, "/cliente_%d_%ld", getpid(), pthread_self());
    if (mq_send(queue, (char *)&struct_to_send, sizeof(struct_to_send), 1)== -1){
        perror("mq_send");
        return -1;
    }
    mq_close(queue);
    mqd_t cola_cliente = mq_open(struct_to_send.cola_respuesta, O_RDONLY);
    if (cola_cliente == -1)
    {
        perror("mq_open");
        return -1;
    }
    char buffer[sizeof(Respuesta)];
    if(mq_receive(cola_cliente, buffer, sizeof(Respuesta), NULL)==-1){
        perror("mq_receive");
        return -1;
    }
    mq_close(cola_cliente);
    Respuesta respuesta;

    memmove(&respuesta, buffer, sizeof(Respuesta));

    
    // Copiamos los valores en el sitio correspondiente
    
    strcpy(value1, respuesta.value1);
    *N_value2 = respuesta.N_value2;
    
    for (int i = 0; i < *N_value2; i++) {
        V_value2[i] = respuesta.V_value2[i];
    }
    return respuesta.resultado;
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
    sprintf(struct_to_send.cola_respuesta, "/cliente_%d_%ld", getpid(), pthread_self());
    if (mq_send(queue, (char *)&struct_to_send, sizeof(struct_to_send), 1)==-1){
        perror("mq_send");
        return -1;
    };
    mq_close(queue);
    mqd_t cola_cliente = mq_open(struct_to_send.cola_respuesta, O_RDONLY);
    if (cola_cliente == -1)
    {
        perror("mq_open");
        return -1;
    }
    char buffer[sizeof(Respuesta)];
    if (mq_receive(cola_cliente, buffer, sizeof(Respuesta), NULL)==-1){
        perror("mq_receive");
        return -1;
    }
    mq_close(cola_cliente);
    Respuesta respuesta;
    memmove(&respuesta, buffer, sizeof(Respuesta));
    return respuesta.resultado;
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
    sprintf(struct_to_send.cola_respuesta, "/cliente_%d_%ld", getpid(), pthread_self());
    if(mq_send(queue, (char *)&struct_to_send, sizeof(struct_to_send), 1)==-1){
        perror("mq_send");
        return -1;
    }
    mqd_t cola_cliente = mq_open(struct_to_send.cola_respuesta, O_RDONLY);
    if (cola_cliente == -1)
    {
        perror("mq_open");
        return -1;
    }
    char buffer[sizeof(Respuesta)];
    if(mq_receive(cola_cliente, buffer, sizeof(Respuesta), NULL)==-1){
        perror("mq_receive");
        return -1;
    }
    mq_close(cola_cliente);
    Respuesta respuesta;
    memmove(&respuesta, buffer, sizeof(Respuesta));
    return respuesta.resultado;
}
int exist(int key)
{
    mqd_t queue = mq_open(MQ_NAME, O_WRONLY);
    ;
    if (queue == -1)
    {
        perror("mq_open");
        return -1;
    }
    Mensaje struct_to_send;
    struct_to_send.cod_operacion = 5;
    struct_to_send.clave = key;
    sprintf(struct_to_send.cola_respuesta, "/cliente_%d_%ld", getpid(), pthread_self());
    if(mq_send(queue, (char *)&struct_to_send, sizeof(struct_to_send), 1)==-1){
        perror("mq_send");
        return -1;
    }
    mq_close(queue);
    mqd_t cola_cliente = mq_open(struct_to_send.cola_respuesta, O_RDONLY);
    if (cola_cliente == -1)
    {
        perror("mq_open");
        return -1;
    }
    char buffer[sizeof(Respuesta)];
    if(mq_receive(cola_cliente, buffer, sizeof(Respuesta), NULL)==-1){
        perror("mq_receive");
        return -1;
    }
    mq_close(cola_cliente);
    Respuesta respuesta;
    memmove(&respuesta, buffer, sizeof(Respuesta));
    return respuesta.resultado;
}