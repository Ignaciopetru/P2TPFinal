from main import *
LABERINTO, inicio, fin = parserArchivo("ArchivosTest/laberintoEntrada.txt")
# Sistema de tests, se tiene un laberinto, y dos archivos por funcion, uno de entrada y otro salida, cada linea es la entrada
# de la funcion y la salida respectivamente.

def test_OutOfRange():
    entrada = open("ArchivosTest/outOfRange/outOfRangeEntrada.txt", "r")
    salida = open("ArchivosTest/outOfRange/outOfRangeSalida.txt", "r")
    linesIn = entrada.readlines()
    linesOut = salida.readlines()
    for index,lineIn in enumerate(linesIn):
        assert outOfRange(eval(lineIn), LABERINTO) == eval(linesOut[index])

def test_ordenarDirecciones():
    entrada = open("ArchivosTest/ordenarDirecciones/ordenarDireccionesEntrada.txt", "r")
    salida = open("ArchivosTest/ordenarDirecciones/ordenarDireccionesSalida.txt", "r")
    linesIn = entrada.readlines()
    linesOut = salida.readlines()
    for index,lineIn in enumerate(linesIn):
        assert ordenarDirecciones(eval(lineIn)[0], eval(lineIn)[1]) == eval(linesOut[index])

def test_obtenerVecinos():
    print LABERINTO
    entrada = open("ArchivosTest/obtenerVecinos/obtenerVecinosEntrada.txt", "r")
    salida = open("ArchivosTest/obtenerVecinos/obtenerVecinosSalida.txt", "r")
    linesIn = entrada.readlines()
    linesOut = salida.readlines()
    for index,lineIn in enumerate(linesIn):
        assert obtenerVecinos(eval(lineIn)[0], eval(lineIn)[1], LABERINTO) == eval(linesOut[index])