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
    if (init() == -1) {
        return -1;
    }
    
    printf("------------\n");
    char value1[] = "Sociedad";
    double V_value2[] = {1.324, 22.2, 432.1};
    if (set_value(13, value1, 3, V_value2) == -1) {
        return -1;
    }
    printf("------------\n");
    char value1_2 [] = "Pepperoni";
    double V_value2_2[] = {32.2, 32.1, 3232.12, -12.0};
    set_value(17, value1_2, 4, V_value2_2);
    printf("------------\n");

    delete_key(17);
    
    /*char value1_3[256];
    int N_value2_3;
    double V_value2_3[32];
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