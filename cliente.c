/*
 *
 * 3. Desarrollar un ejemplo de código de un cliente (cliente.c) que utilice las funciones anteriores.
El ejecutable de este programa tiene que generarse empleando la biblioteca desarrollada en el
apartado anterior, es decir, el código de este cliente debe enlazarse con la biblioteca dinámica
anterior. Este cliente se utilizará para probar el servicio desarrollado y deberá realizar las
invocaciones al API de tuplas que considere oportuno. El código incluido en cliente.c solo
podrá incluir llamadas a los servicios implementados y descritos anteriormente. En él no puede
haber ninguna referencia a colas de mensajes.*/
// Created by jvinas on 2/26/24.
//
#include "claves.h"
#include "auxiliar.h"
#include <stdio.h>

int main(){
    // Varios init para comprobar que funciona correctamente la concurrencia
    for (int i = 0; i < 50; i++) {
        if (init() == -1) {
            printf("Fallo en el init\n");
            return -1;
        }
    char value1[] = "Sociedad";
    double V_value2[] = {1.324, 22.2, 432.1};
    if (set_value(13, value1, 3, V_value2) == -1) {
        printf("Error en el set_value");
        return -1;
    }
    char value1_2 [] = "Pepperoni";
    double V_value2_2[] = {32.2, 32.1, 3232.12, -12.0};
    if (set_value(17, value1_2, 4, V_value2_2) == -1) {
        printf("Error en el set_value");
        return -1;
    }

    if (delete_key(17) == -1) {
        printf("Fallo en delete_key\n");
        return -1;
    }
    int exist17 = exist(17); // Debería dar 0
    int exist13 = exist(13); // Debería dar 1
    if ( exist17== -1 || exist13 == -1) {
        printf("Fallo en el exist\n");
        return -1;
    }
    printf("Exist 17: %d\n", exist17);
    printf("Exist 13: %d\n", exist13);

    //  Aqui es donde guardaremos los valores de prueba
    char value1_buffer[256];
    int N_value2_buffer;
    double V_value2_buffer[32];
    if (get_value(13, value1_buffer, &N_value2_buffer, V_value2_buffer) == -1) {
        printf("Error en el get_value\n");
        return -1;
    }
    printf("------------------------\n");
    printf("Valor de value1: %s\n", value1_buffer);
    printf("Valor de N_value2: %d\n", N_value2_buffer);
    for (int i = 0; i < N_value2_buffer; i++) {
        printf("Elemento %d de V_value2: %f\n", i, V_value2_buffer[i]);
    }
    


    char value1_3[] = "Leganes";
    int N_value2_3 = 2;
    double V_value2_3[] = {6.5, 3.1223};

    if (modify_value(13, value1_3, N_value2_3, V_value2_3) == -1) {
        printf("Error en el modify_value\n");
    }

    if (get_value(13, value1_buffer, &N_value2_buffer, V_value2_buffer) == -1) {
        printf("Error en el get_value\n");
        return -1;
    }
    printf("------------------------\n");
    printf("Valor de value1: %s\n", value1_buffer);
    printf("Valor de N_value2: %d\n", N_value2_buffer);
    for (int i = 0; i < N_value2_buffer; i++) {
        printf("Elemento %d de V_value2: %f\n", i, V_value2_buffer[i]);
    }
    printf("*****************\n");
    }
    
    /*
    get_value(13, value1_3, &N_value2_3, V_value2_3);
    printf("value1: %s\n", value1_3);
    printf("N_value2: %d\n", N_value2_3);
    for (int i = 0; i < N_value2_3; i++) {
        printf("Elemento %d de V_value2: %f\n", i, V_value2_3[i]);
    }

    


    double V_value2_4[] = {3.21, 75.2, 342.2};
    modify_value(13, "JUAN", 3, V_value2_4);
    char value1_4[256];
    int N_value2_4;
    double V_value2_5[32];
    get_value(13, value1_4, &N_value2_4, V_value2_5);
    printf("value1: %s\n", value1_4);
    printf("N_value2: %d\n", N_value2_4);
    for (int i = 0; i < N_value2_4; i++) {
        printf("Elemento %d de V_value2: %f\n", i, V_value2_5[i]);
    }
    printf("La tupla %d existe: %d\n", 17, exist(13));
    exist(17);
    */
    return 0;
};