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
#include <mqueue.h>
#include "claves.h"
#include "auxiliar.h"



// Después de usar el nombre del archivo es importante hacer un free para liberar memoria
char* get_file_name(int key) {
    
    // Primero convertimos la clave en string para poder concaternala luego y
    // obtener el nombre del archivo

    int string_key_size = snprintf(NULL, 0, "%d", key);
    char *string_key = (char*) malloc(string_key_size + 1); // +1 para el caracter final
    snprintf(string_key, string_key_size + 1, "%d", key);
    
    size_t file_name_size = strlen(FILES_PATH) + strlen(string_key) + strlen(FILE_ENDING) + 3;
    // Ahora creamos una string lo suficientemente grande para guardar el path completo
    char *file_name = (char*) malloc(file_name_size);
    // En caso de error cortamos la escritura
    if (file_name == NULL) {
	printf("Ha ocurrido un error al crear el nombre del archivo");
	perror("malloc()");
	return NULL;
    }
    // Concatenamos todo el path
    strcpy(file_name, FILES_PATH);
    strcat(file_name, string_key);
    strcat(file_name, FILE_ENDING);
    free(string_key);
    return file_name;
}


int write_file (char* file_name, struct tupla *tupla)
{
    // Abrimos el archivo para escribir
    FILE *file = fopen(file_name, "w");
    // Comprobamos que el archivo se haya abierto correctamente
    if (file == NULL) {
	fprintf(stderr, "Error en la linea %d:\n", __LINE__);
	fprintf(stderr, "No se pudo abrir el archivo %s: %s\n", file_name, strerror(errno));
	return -1;
    }

    // Usamos fwrite para pasar la estructura a binario y escribirla
    if (fwrite(tupla, sizeof(struct tupla), 1, file) == 0) {
	fprintf(stderr, "Error en la linea %d:\n", __LINE__);
	fprintf(stderr, "Ocurrió un error al escribir el archivo %s: %s\n", file_name, strerror(errno));
	fclose(file);
	return -1;
    }

    fclose(file);
    return 0;
}

int read_file(char *file_name, struct tupla *tupla) {

    // Abrimos el archivo para leer
    FILE *file = fopen(file_name, "rb");
    // Comprobamos que el archivo se haya abierto correctamente
    if (file == NULL) {
	fprintf(stderr, "Error en la linea %d:\n", __LINE__);
	fprintf(stderr, "No se pudo abrir el archivo %s: %s\n", file_name, strerror(errno));
	return -1;
    }

    
    if (fread(tupla, sizeof(struct tupla), 1, file) == 0) {
	fprintf(stderr, "Error en la linea %d:\n", __LINE__);
	fprintf(stderr, "Ocurrió un error al leer el archivo %s: %s\n", file_name, strerror(errno));
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
		printf("Eliminado: %s\n", path);
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
    printf("File_name: %s\n", file_name);
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
	fprintf(stderr, "Error en la linea %d:\n", __LINE__);
	printf("Ocurrió un error al escribir el fichero %s\n", file_name);
	return -1;
    }
    
    printf("%s escrito\n", file_name);
    free(file_name); // Importante liberar la memoria del nombre del archivo
    return 0;
}


int get_value(int key, char *value1, int *N_value2, double *V_value2) {
    char *file_name = get_file_name(key);
    // Guardamos la información del fichero en la estructura
    struct tupla tupla;

    if (read_file(file_name, &tupla) == -1) {
	fprintf(stderr, "Error en la linea %d:\n", __LINE__);
	printf("Ocurrió un error al leer el fichero %s\n", file_name);
	return -1;
    }

    // Copiamos los valores de la estructura a sus variables correspondientes
    strcpy(value1, tupla.value1);
    *N_value2 = tupla.N_value2;
    for (int i = 0; i < *N_value2; i ++) {
	V_value2[i] = tupla.V_value2[i];
    }
    // Liberamos el fichero
    free(file_name);
    return 0;
}

int delete_key(int key) {
    // Se borra el archivo con el nombre de clave asociado
    char *file_name = get_file_name(key);
    if (access(file_name, F_OK) == -1) {
        perror("No existe el archivo");
        free(file_name);
        return -1;
    }
    if (remove(file_name) == -1) {
        perror("remove()");
        free(file_name);
        return -1;
    }
    return 0;


}

int modify_value(int key, char *value1, int N_value2, double *V_value2) {
    if (delete_key(key) == -1) {
        perror("delete_key");
        return -1;
    }
    if (set_value(key, value1, N_value2, V_value2) == -1){
        perror("set_value");
        return -1;
    }
    return 0;
}

int exist(int key) {
    char *file_name = get_file_name(key);
    if (file_name == NULL) {
        perror("file_name");
        return -1;
    }
    // Comprobamos que el fichero existe
    if (access(file_name, F_OK) == 0) {
        free(file_name);
        return 1;
    }
    free(file_name);
    return 0;
}

int main() {
    atributos.mq_flags = 0;
    atributos.mq_maxmsg = 10; //no sé si hay nº max de mensajes pongo este por poner algo. */
    atributos.mq_curmsgs = 0;
    atributos.mq_msgsize = sizeof(Mensaje);
    mqd_t server_queue = mq_open(MQ_NAME, O_CREAT |O_RDONLY, S_IRUSR|S_IWUSR, &atributos);


    
    return 0;
}