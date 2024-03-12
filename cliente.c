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
        int retorno = init();
        printf("%dxdasdddf\n", retorno);
        return 0;
};