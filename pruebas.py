import subprocess
import unittest

class TestCliente(unittest.TestCase):
    
    
    def test_inicial(self):
        directorio = "archivos_prueba/"
        # Primero hacemos una prueba normal para comprobar que todo funciona
        ruta = directorio + "inicial.out"
        comando = "./cliente > " + ruta
        with open(directorio + "base.txt", "r") as f:
            caso_base = f.read()
        subprocess.run(comando, shell=True)
        with open(ruta) as f:
            caso_inicial = f.read()
        self.assertEqual(caso_base, caso_inicial)

    def test_diez(self):
        directorio = "archivos_prueba/"
        with open(directorio + "base.txt", "r") as f:
            caso_base = f.read()
        for i in range(10):
            ruta = directorio + str(i) + ".out"
            comando = "./cliente > " + ruta
            subprocess.run(comando, shell=True)
            with open(ruta) as f:
                caso_intermedio = f.read()
            self.assertEqual(caso_base,caso_intermedio)
    def test_cien(self):
        directorio = "archivos_prueba/"
        with open(directorio + "base.txt", "r") as f:
            caso_base = f.read()
        for i in range(10, 110):
            ruta = directorio + str(i) + ".out"
            comando = "./cliente > " + ruta
            subprocess.run(comando, shell=True)
            with open(ruta) as f:
                caso_intermedio = f.read()
            self.assertEqual(caso_base,caso_intermedio)

if __name__ == "__main__":
    subprocess.run("rm -r archivos_prueba/*.out", shell=True)
    unittest.main()