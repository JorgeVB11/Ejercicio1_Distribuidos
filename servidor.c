/*Desarrollar el código del servidor (servidor.c) encargado de gestionar las estructuras de datos
que almacenan los elementos clave-valor1-valor2-valor3. Puede elegirse la estructura de datos
que se estime oportuno, siempre que no imponga un límite en el número de elementos que se
pueden almacenar. El servidor desarrollado debe ser concurrente.*/
// Created by jvinas on 2/26/24.
//
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "claves.h"

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

char* get_file_name(int key) {
    
    // Primero convertimos la clave en string para poder concaternala luego y
    // obtener el nombre del archivo
    
    char string_key[12]; // El numero maximo de caracteres en un int es 11
    sprintf(string_key, "%d", key);
    size_t file_name_size = strlen(FILES_PATH) + strlen(string_key) + strlen(FILE_ENDING) + 1;
    // Ahora creamos una string lo suficientemente grande para guardar el path completo
    char *file_name = (char*) malloc(file_name_size);
    // En caso de error cortamos la escritura
    if (file_name == NULL) {
	printf("Ha ocurrido un error al crear el nombre del archivo");
	perror("malloc()");
	return NULL;
    }
    // Concatenamos todo el path
    strcat(file_name, FILES_PATH);
    strcat(file_name, string_key);
    strcat(file_name, FILE_ENDING);

    return file_name;
}


int write_file (char* file_name, struct tupla *tupla)
{
    // Abrimos el archivo para escribir
    FILE *file = fopen(file_name, "w");
    // Comprobamos que el archivo se haya abierto correctamente
    if (file == NULL) {
	printf("Ocurrió un error al abrir el archivo");
	perror("fopen()");
	return -1;
    }

    // Usamos fwrite para pasar la estructura a binario y escribirla
    if (fwrite(tupla, sizeof(struct tupla), 1, file) == 0) {
	printf("Ocurrió un error al escribir el archivo");
	perror("fwrite");
	fclose(file);
	return -1;
    }

    fclose(file);
    return 0;
}

int init() 
{
    // Primero comprobamos que la carpeta de los archivos existe y si no, la creamos
    DIR *server_dir;
    server_dir = opendir(FILES_PATH);

    // Si el directorio no existe el puntero será NULL
    // y errno indica el tipo de error
    if (server_dir == NULL)
    {
        // En este caso el directorio no existe y tenemos que crearlo
        if (errno == ENOENT)
        {
            // Lo creamos con todos los permisos solo para el usuario
            // También hacemos comprobación de errores
            if (mkdir(FILES_PATH, 0700) == -1) 
            {
                printf("Ocurrió un error al crear el directorio");
		return -1;
            }

            // Volvemos a abrir el directorio
            server_dir = opendir(FILES_PATH);
        }
        // Si el error es otro es que algo salió mal
        else {
            printf("Ocurrió un error al acceder al directorio");
	    closedir(server_dir);
            return -1;
        }

    } 

    // Una vez tenemos el directorio creado y abierto, eliminamos los archivos que haya dentro

    struct dirent *dir;
        // Readdir devuelve una struct dirent por cada archivo y NULL cuando llega al final
        // Cuando da error tambien devuelve NULL, pero cambia el errno, por lo que se pone a 0
        // para poder identificarlo.
        errno = 0;

        dir = readdir(server_dir);

        while (dir != NULL) 
        {
            // Nos aseguramos que el archivo eliminado no es ni "." ni ".."
            if (strcmp(".", dir->d_name) != 0 && strcmp("..", dir->d_name) != 0 )
            {
                char path[256];
                strcpy(path, FILES_PATH);
                strcat(path, dir->d_name);
                if (remove(path) == -1)
                {
                    printf("Ocurrió un error al borrar los archivos: \n");
                    perror("remove()");
		    return -1;
                }
            }
            
            dir = readdir(server_dir);
        }
	closedir(server_dir);
        return 0;
}

int set_value(int key, char *value1, int N_value2, double *V_value2){
    if (N_value2 > 32) {
	printf("Numero de elementos de value2 inválido");
	return -1;
    }

    // Primero obtenemos el nombre del archivo
    char *file_name = get_file_name(key);

    // Comprobamos que no haya errores
    if (file_name == NULL) {
	return -1;
    }

    // Comprobamos que el fichero no existe
    if (access(file_name, F_OK) == 0) {
	printf("La tupla ya existe");
	return -1;
    }

    // Creamos la tupla y escribimos el archivo
    struct tupla tupla;
    // Primero copiamos los valores normales
    tupla.key = key;
    tupla.N_value2 = N_value2;

    // Y luego los elementos de los arrays
    strcpy(tupla.value1, value1);
    
    for (size_t i = 0; i < N_value2; i++) {
	tupla.V_value2[i] = V_value2[i];
    }

    
    // Una vez tenemos la estructura y el nombre del archivo, lo escribimos
    if (write_file(file_name, &tupla) == -1) {
	printf("Ocurrió un error al escribir el fichero");
	return -1;
    }
    

    printf("%s escrito\n", file_name);
    return 0;
}

int main() {
    char value1[] = "Sociedad";
    double V_value2[] = {1.324, 22.2, 432.1};
    set_value(13, value1, 3, V_value2);
}
