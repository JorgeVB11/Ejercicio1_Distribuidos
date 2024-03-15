/*Desarrollar el código del servidor (servidor.c) encargado de gestionar las estructuras de datos
que almacenan los elementos clave-valor1-valor2-valor3. Puede elegirse la estructura de datos
que se estime oportuno, siempre que no imponga un límite en el número de elementos que se
pueden almacenar. El servidor desarrollado debe ser concurrente.*/
// Created by jvinas on 2/26/24.
//
#include "proxy_server.h"
// Mutex para el acceso a archivos
pthread_mutex_t mutex_archivos;
pthread_cond_t cond_archivos;
int ocupado;

int main()
{
    struct mq_attr atributos;


    atributos.mq_flags = 0;
    atributos.mq_maxmsg = 10; // no sé si hay nº max de mensajes pongo este por poner algo. */
    atributos.mq_curmsgs = 0;
    atributos.mq_msgsize = sizeof(Mensaje);
    // Primero abrimos la cola
    mqd_t server_queue = mq_open(MQ_NAME, O_CREAT | O_RDONLY, 0666, &atributos);
    if (server_queue == -1)
    {
        perror("mq_open");
        return -1;
    }
    // Borramos alguna cola que puediera haber de anteriores ejecuciones
    mq_unlink(MQ_NAME);
    server_queue = mq_open(MQ_NAME, O_CREAT | O_RDONLY, 0666, &atributos);
    if (server_queue == -1)
    {
        perror("mq_open");
        return -1;
    }

    // Inicializamos el mutex y la variable de condicion
    pthread_mutex_init(&mutex_archivos, NULL);
    pthread_cond_init(&cond_archivos, NULL);
    ocupado = 1;
    
    // Atributos para los hilos (no joinables)
    pthread_attr_t atributos_hilo;

    // Inicializamos los atributos
    pthread_attr_init(&atributos_hilo);

    // Cambiamos los hilos a detached para que no sea necesario recogerlos
    pthread_attr_setdetachstate(&atributos_hilo, PTHREAD_CREATE_DETACHED);


    while (1)
    {
        char buffer[sizeof(Mensaje)];
        if(mq_receive(server_queue, buffer, sizeof(Mensaje), NULL)==-1){
            perror("mq_receive");
            return -1;
        }
        Mensaje mensaje;
        memcpy(&mensaje, buffer, sizeof(Mensaje));

        // Creamos el hilo
        pthread_t hilo;
        int ret = pthread_create(&hilo, &atributos_hilo, gestionar_peticion,(void *) &mensaje);
        if (ret == -1) {
            printf("Error al crear el hilo: %d" ,ret);
            return -1;
        }
        printf("-----------------\n");
    }
    pthread_mutex_destroy(&mutex_archivos);
    pthread_cond_destroy(&cond_archivos);
    pthread_attr_destroy(&atributos_hilo);
    return 0;
}