//
// Created by jvinas on 2/26/24.
//


#ifndef EJERCICIO1_DISTRIBUIDOS_CLAVES_H
#define EJERCICIO1_DISTRIBUIDOS_CLAVES_H

// Codigo Operacion -> 0

// Inicializa el servicio y se elimina lo guardado anteriormente
// Es necesario llamar a esta función antes que a cualquier otra

// Devuelve:
//  0  -> Exito
//  -1 -> Error

int init();



// Codigo Operacion -> 1

// Inserta el elemento <key, value1, value2>, donde key es la clave que identifica,
// value1 es una string de menos de 255 caracteres y value2 es un vector de doubles
// de menos de 32 elementos.

// El vector viene dado por dos parametros: N_value2, que indica el tamaño, y
// V_value_2, que apunta a los elementos en si

// Se considera error insertar en una clave ya existente o que el valor N_value2
// este fuera de rango

// Devuelve:
// 0  -> Exito
// -1 -> Error

int set_value(int key, char *value1, int N_value2, double *V_value_2);



// Codigo Operacion -> 2

// Permite obtener el valor almacenado en la clave "key"
// La cadena de caracteres se devuelve en value1, la dimension del vector en
// N_value2 y los elementos del vector en V_Value2

// Es necesario que *value1 y *V_value2 tengan suficiente memoria asignada para guardar
// los elementos. Si no, se devolvera un error

// Devuelve:
// 0  -> Exito
// -1 -> Error

int get_value(int key, char *value1, int N_value2, double *V_value2);



// Codigo Operacion -> 3

// Elimina la clave y los datos asociados a ella

// Devuelve error en caso de que no exista la clave

// Devuelve:
// 0  -> Exito
// -1 -> Error
int delete_key(int key);



// Codigo Operacion -> 4

// Permite cambiar el valor almacenado en la clave "key" por el indiciado en
// value1 y value2.

// Hará uso de las funciones delete_key y set_value para lograr este cometido

// Devuelve: error si la clave no existe o  si el valor de N_value2 está fuera de rango

// Devuelve:
// 0  -> Exito
// -1 -> Error

int modify_value(int key, char *value1, int N_value2, double *V_value2);



// Codigo Operacion -> 5
// Permite conocer si existe un elemento con la clave key

// Devuelve:
// 0  -> La clave no existe
// 1  -> La clave si existe
// -1 -> Error
int exist(int key);
#endif //EJERCICIO1_DISTRIBUIDOS_CLAVES_H
