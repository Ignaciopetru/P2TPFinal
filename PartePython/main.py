import subprocess
import math

# Valor ascii de X - 48
X = 40
DIRECCIONES = [(0,1),(0,-1),(1,0),(-1,0)]
 
# Funcion que se encargar de leer el archivo generado en C.
# Lee el archivo y lo almacena en una array de arrays de chars, donde cada numero puede ser 0, 1, X o I.
def parserArchivo(pathF):
    # Se abre el archivo en modo lectura.
    file = open(pathF, 'r')
    # Se almacena su contenido en archivo (array).
    # Array doble que contendra al laberinto.
    laberintoArray = [[]]
    linea = 0
    # Inicio por defecto en el origen.
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
    
    # Se cierra el archivo y se retorna el array doble y las coordenadas de inicio y fin.
    file.close()
    return laberintoArray, inicio, fin

# Funcion encargda de determinar si unas coordenadas corresponden a una casilla dentro del laberinto.
# Devuelve true en caso de corresponder y false en el contrario
def outOfRange(nodo, laberinto):
    largo = len(laberinto)
    return nodo[0] >= 0 and nodo[1] >= 0 and nodo[0] < largo and nodo[1] < largo

# Funcion que teniendo en cuenta la distancia al fin del laberinto de un nodo, ordena las posibles direcciones
# a tomar. Para que luego el algoritmo logre llegar al final con mayor eficiencia.
def ordenarDirecciones(nodo, fin):
    costos = []
    resultado = []
    # Por cada posible direccion se calcula la distancia al fin de dicho nodo.
    for direccion in DIRECCIONES:
        distancia = (fin[0]-(nodo[0]+direccion[0]))**2+(fin[1]-(nodo[1]+direccion[1]))**2
        if(distancia == 0):
            costos.append((direccion[0], direccion[1], 0.00000001))
        else:
            costos.append((direccion[0], direccion[1], math.sqrt(distancia)))
    # Luego se ordena de forma decreciente, teniendo en cuenta, la 3er "variable" de la tupla.
    costos.sort(key=lambda posible: posible[2], reverse = True)
    # Por ultimo se reorganiza la triupla en un tupla y se retorna.
    for posible in costos:
        resultado.append((posible[0], posible[1]))
    return resultado 
# Notese que las operaciones realizadas en esta funcion son costosas, pero se estan realizadon solo con 4 elementos.

# ObternVecinos, toma un nodo, el fin y el laberinto, y utilizando la funcion anterior calcula los vecinos de una casilla cualesquira.
# Ademas, estos estan en orden, el primero de la lista sera el mas "conveniente" a seguir, sera el mas cercano al origen.
def obtenerVecinos(nodo, fin, laberinto):
    vecinos = []
    # Se ordenan las direcciones para respetar el orden de cercania.
    direcciones = ordenarDirecciones(nodo, fin)
    for direccion in direcciones:
        # Si la casilla esta dentro del rango del laberinto y no es una pared, se agrega como vecino.
        if (outOfRange(((nodo[0]+direccion[0]),(nodo[1]+direccion[1])), laberinto) and laberinto[nodo[0]+direccion[0]][nodo[1]+direccion[1]] != 1):
            vecinos.append((nodo[0]+direccion[0],nodo[1]+direccion[1]))
    return vecinos

# Funcion que se encarga de resolver el laberinto.
def resolverLab(inicio, fin,laberinto):
    # Se utiliza un array como una cola, solo se pueden sacar los elementos de "arriba"
    cola = []
    # Se agrega a la cola a el primer nodo, inicio.
    cola.append([inicio])
    # Siempre que la cola no este vacia.
    while cola:
        # La ruta sera el primer elemento de la cola, (una array con los nodos paso a paso).
        ruta = cola.pop()
        # Tomamos el ultimo elemento de ruta, que sera el nodo que queremos analizar.
        nodo = ruta[-1]
        vecinos = obtenerVecinos(nodo, fin,laberinto)
        for vecino in vecinos:
            # Tomamos el primer vecino, (teniendo en cuenta que para obtener los vecinos se utilizo la lista de direcciones
            # ordenadas, este sera el vecino mas cercano al fin del laberinto).
            if  laberinto[vecino[0]][vecino[1]] != -1:
                # Si no esta pisado, se setea una nueva ruta, que sera la lista de nodos que representa ruta.
                nuevaRuta = list(ruta)
                nuevaRuta.append(vecino)
                # Se agrega al vecino y se lo agrega a la cola para ser tratado luego en las siguientes iteraciones.
                cola.append(nuevaRuta)
                # Chequeamos que el vecino sea el objetivo, si lo es retornamos la ruta.
                if laberinto[vecino[0]][vecino[1]] == X:
                    return nuevaRuta
        # Se setea como "pisado" el nodo analizado.
        laberinto[nodo[0]][nodo[1]] = -1
    # Si el bucle termina, quiere decir que se vacio la cola, es decir que se acabaron los nodos a analizar, osea, no se puede 
    # completar el laberinto.
    return ["No tiene solucion"]
    

laberinto, inicio, fin = parserArchivo("../ParteC/salida.txt")
ruta = resolverLab(inicio, fin, laberinto)
for c in ruta:
    print(c)

#def main():
#    response = subprocess.run(["../ParteC/a.out", "entrada.txt"])
#    laberinto, inicio = parserArchivo("salida.txt")
#    ruta = resolverLab(inicio[1],inicio[0], laberinto)
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
