import subprocess


if __name__ == "__main__":
    
    directorio = "archivos_prueba/"
    # Borramos todos los archivos de pruebas anteriores
    subprocess.run("rm -r archivos_prueba/*.out", shell=True)

    with open(directorio + "base.txt", "r") as f:
        caso_base = f.read()


    # Primero hacemos una prueba normal para comprobar que todo funciona
    ruta = directorio + "inicial.out"
    comando = "./cliente > " + ruta
    resultado = subprocess.run(comando, shell=True)
    
    if resultado.returncode < 0:
        print("Ocurrió un error con el primer archivo")
        exit()
    with open(ruta) as f:
        caso_inicial = f.read()
    if caso_inicial != caso_base:
        print("Los archivos inicial y base no coinciden")
        exit()

    # Comprobamos con 10 clientes (el numero máximo de mensajes en cola es 10)
    for i in range(10):
        ruta = directorio + str(i) + ".out"
        comando = "./cliente > " + ruta
        resultado = subprocess.run(comando, shell=True)
    
        if resultado.returncode < 0:
            print("Ocurrió un error con el primer archivo")
            exit()
        with open(ruta) as f:
            caso_intermedio = f.read()
        if caso_intermedio != caso_base:
            print(f"Los archivos {i} y base no coinciden")
            exit()
    # Por último, para probar la concurrencia, ejecutamos 100 clientes a la vez y comprobamos que 
    # se atienden todas las peticiones
    for i in range(10, 110):
        ruta = directorio + str(i) + ".out"
        comando = "./cliente > " + ruta
        resultado = subprocess.run(comando, shell=True)
    
        if resultado.returncode < 0:
            print("Ocurrió un error con el primer archivo")
            exit()
        with open(ruta) as f:
            caso_intermedio = f.read()
        if caso_intermedio != caso_base:
            print(f"Los archivos {i} y base no coinciden")
            exit()

    print('\033[92m' + "Test superado correctamente")