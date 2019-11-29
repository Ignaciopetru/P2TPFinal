import subprocess
import math

# Valor ascii de X - 48
X = 40
DIRECCIONES = [(0,1),(0,-1),(1,0),(-1,0)]
 
# Funcion que se encargar de leer el archivo generado en C.
# Lee el archivo y lo almacena en una array de arrays de enteros, donde cada numero puede ser 0, 1 o 2.
def parserArchivo(pathF):
    # Se abre el archivo en modo lectura.
    file = open(pathF, 'r')
    # Se almacena su contenido en archivo (array).
    # Array doble que contendra al laberinto.
    laberintoArray = [[]]
    linea = 0
    inicio = (0,0)
    # Se recorren todos los caracteres del archivo, almacenandolos linea por array.
    for i,line in enumerate(file):
        for j,carac in enumerate(line):
            if carac == 'I':
                inicio = (i, j)
            if carac == 'X':
                fin = (i, j)
            if carac == '\n':
                laberintoArray.append([])
            else:
                laberintoArray[i].append(ord(carac)-48)
    
    # Se cierra el archivo y se retorna una tupla con el array doble y las coordenadas de inicio.
    file.close()
    return laberintoArray, inicio, fin


def outOfRange(nodo, laberinto):
    largo = len(laberinto)
    return nodo[0] >= 0 and nodo[1] >= 0 and nodo[0] < largo and nodo[1] < largo

def ordenarDirecciones(nodo, fin):
    #print nodo
    costos = []
    resultado = []
    for direccion in DIRECCIONES:
        distancia = (fin[0]-(nodo[0]+direccion[0]))**2+(fin[1]-(nodo[1]+direccion[1]))**2
        if(distancia == 0):
            #print(direccion[0], direccion[1], 0.00001)
            costos.append((direccion[0], direccion[1], 0.00001))
        else:
            costos.append((direccion[0], direccion[1], math.sqrt(distancia)))
            #print(direccion[0], direccion[1], math.sqrt(distancia))

    costos.sort(key=lambda posible: posible[2], reverse = True) 
    for posible in costos:
        resultado.append((posible[0], posible[1]))
    return resultado 


def obtenerVecinos(nodo, fin, laberinto):
    vecinos = []
    direcciones = ordenarDirecciones(nodo, fin)
    #print(direcciones)
    for direccion in direcciones:
        if (outOfRange(((nodo[0]+direccion[0]),(nodo[1]+direccion[1])), laberinto) and laberinto[nodo[0]+direccion[0]][nodo[1]+direccion[1]] != 1):
            vecinos.append((nodo[0]+direccion[0],nodo[1]+direccion[1]))
    return vecinos


def resolverLab(inicio, fin,laberinto):
    cola = []
    cola.append([inicio])
    while cola:
        ruta = cola.pop()
        nodo = ruta[-1]
        if laberinto[nodo[0]][nodo[1]] != -1:
            vecinos = obtenerVecinos(nodo, fin,laberinto)
            #print vecinos
        for vecino in vecinos:
            #print vecino
            if  laberinto[vecino[0]][vecino[1]] != -1:
                nuevaRuta = list(ruta)
                nuevaRuta.append(vecino)
                cola.append(nuevaRuta)
                if laberinto[vecino[0]][vecino[1]] == X:
                    return nuevaRuta
        laberinto[nodo[0]][nodo[1]] = -1
    return []
    

laberinto, inicio, fin = parserArchivo("salida.txt")

ruta = resolverLab(inicio, fin, laberinto)
for c in ruta:
    print(c)
#print(ordenarDirecciones((2,2),(4,3)))

#def main():
#    response = subprocess.run(["../ParteC/a.out", "entrada.txt"])
#    laberinto, inicio = parserArchivo("salida.txt")
#    ruta = recorrido(inicio[1],inicio[0], laberinto)
#    while(ruta == []):
#        print("bababa")
#        response = subprocess.run(["../ParteC/a.out", "entrada.txt"])
#        laberinto, inicio = parserArchivo("salida.txt")
#        ruta = recorrido(inicio[1],inicio[0], laberinto)
#    for paso in ruta: print(paso)
#    
# 
#if __name__ == "__main__":
#    main()
