import subprocess

# Valor ascii de X - 48
X = 40
 
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
                inicio = (i, j) #Ajuste de representacion
            if carac == '\n':
                laberintoArray.append([])
            else:
                laberintoArray[i].append(ord(carac)-48)
    
    # Se cierra el archivo y se retorna una tupla con el array doble y las coordenadas de inicio.
    file.close()
    return laberintoArray, inicio


def outOfRange(nodo, laberinto):
    largo = len(laberinto)
    return nodo[0] >= 0 and nodo[1] >= 0 and nodo[0] < largo and nodo[1] < largo

def obtenerVecinos(nodo, laberinto):
    vecinos = []
    direcciones = [(1,0),(0,1),(-1,0),(0,-1)]
    for direccion in direcciones:
        if (outOfRange(((nodo[0]+direccion[0]),(nodo[1]+direccion[1])), laberinto) and laberinto[nodo[0]+direccion[0]][nodo[1]+direccion[1]] != 1):
            vecinos.append((nodo[0]+direccion[0],nodo[1]+direccion[1]))
    return vecinos


def resolverLab(inicio, laberinto):
    explorado = []
    cola = []
    cola.append([inicio])
    while cola:
        ruta = cola.pop()
        nodo = ruta[-1]
        if nodo not in explorado:
            vecinos = obtenerVecinos(nodo, laberinto)
        for vecino in vecinos:
            if vecino not in explorado:
                nuevaRuta = list(ruta)
                nuevaRuta.append(vecino)
                cola.append(nuevaRuta)
                if laberinto[vecino[0]][vecino[1]] == X:
                    return nuevaRuta
        explorado.append(nodo)
    return []
    



# Funcion que se encargar de generar la ruta para llegar desde el inicio (en el caso del enunciado propuesto desde arriba a la izquiera.)
# Se hace uso de la fuerza bruta, teniendo en cuenta todos los casos posibles recursivamente, quedandonos con la opcion correcto.
def recorrido(y, x, laberinto): 
    # Caso base, si en la posicion de inicio es el objetivo se retorna la posicion final de la ruta.
    if laberinto[y][x] == 40:
        return [(x, y)]
    # Caso en el que se intente llegar al objetivo a traves de una "pared". Necesario para dar la salida de lista vacia, cuando no se puede llegar al objetivo.
    if laberinto[y][x] == 1:
        return []
    
    # Se "pisa" la posicion que ta pertenece al camino de la ruta, para no pasar por encima otra vez.
    laberinto[y][x] = -1
    
    # A continuacion se comienza a analizar cada caso de camino posible, en las 4 direcciones que se aceptan para formar parte de la ruta.
    # Cada if testea si la proxima posicion existe en el laberinto, ademas de constatar que no sea una posicion "pisada".

    # Arriba.
    if y > 0 and laberinto[y - 1][x] in [0, 1, X]:
        camino = recorrido(y - 1, x, laberinto)
        # Se testea si existe un posible camino en esa direccion, si lo hay se agraga a la ruta esta posicion. Y se sigue a partir de esta direccion.
        if camino:
            return [(y, x)] + camino
 
    # Derecha.
    if x < len(laberinto[y]) - 1 and laberinto[y][x + 1] in [0, 1, X]:
        camino = recorrido(y, x + 1, laberinto)
        if camino:
            return [(y, x)] + camino
 
    # Abajo.
    if y < len(laberinto) - 1 and laberinto[y + 1][x] in [0, 1, X]:
        camino = recorrido(y + 1, x, laberinto)
        if camino:
            return [(y, x)] + camino
 
    # Izquierda.
    if x > 0 and laberinto[y][x - 1] in [0, 1, X]:
        camino = recorrido(y, x - 1, laberinto)
        if camino:
            return [(y, x)] + camino
 
    # En el caso de no poder ir en ninguna direccion se retorna la lista vacia.
    return []

laberinto, inicio = parserArchivo("salida.txt")
ruta2 = recorrido(inicio[0],inicio[1], laberinto)
ruta = resolverLab(inicio, laberinto)
for c in ruta:
    print(c)
print "-----------------"
for c in ruta2:
    print(c)




#print(inicio)
#ruta = recorrido(inicio[0],inicio[1], laberinto)
#for paso in ruta: print(paso)

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
