/*Desarrollar el código del servidor (servidor.c) encargado de gestionar las estructuras de datos
que almacenan los elementos clave-valor1-valor2-valor3. Puede elegirse la estructura de datos
que se estime oportuno, siempre que no imponga un límite en el número de elementos que se
pueden almacenar. El servidor desarrollado debe ser concurrente.*/
// Created by jvinas on 2/26/24.
//
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include "claves.h"

const char *FILES_PATH = "./server_files/";

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
                }
            }
            
            dir = readdir(server_dir);
        }

        
        return 0;
}

int main() {
    init();
}