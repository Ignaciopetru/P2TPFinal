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
    Alab = [[]]
    linea = 0
    inicio = (0,0)
    # Se recorren todos los caracteres del archivo, almacenandolos linea por array.
    for y,line in enumerate(file):
        for x,carac in enumerate(line):
            if carac == 'I':
                inicio = (x, y)
            if carac == '\n':
                Alab.append([])
            else:
                Alab[y].append(ord(carac)-48)
    
    # Se cierra el archivo y se retorna una tupla con el array doble y las coordenadas de inicio.
    file.close()
    return Alab, inicio


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
            return [(x, y)] + camino
 
    # Derecha.
    if x < len(laberinto[y]) - 1 and laberinto[y][x + 1] in [0, 1, X]:
        camino = recorrido(y, x + 1, laberinto)
        if camino:
            return [(x, y)] + camino
 
    # Abajo.
    if y < len(laberinto) - 1 and laberinto[y + 1][x] in [0, 1, X]:
        camino = recorrido(y + 1, x, laberinto)
        if camino:
            return [(x, y)] + camino
 
    # Izquierda.
    if x > 0 and laberinto[y][x - 1] in [0, 1, X]:
        camino = recorrido(y, x - 1, laberinto)
        if camino:
            return [(x, y)] + camino
 
    # En el caso de no poder ir en ninguna direccion se retorna la lista vacia.
    return []
 

def main():
    response = subprocess.run(["../ParteC/a.out", "entrada.txt"])
    laberinto, inicio = parserArchivo("salida.txt")
    ruta = recorrido(inicio[1],inicio[0], laberinto)
    while(ruta == []):
        response = subprocess.run(["../ParteC/a.out", "entrada.txt"])
        laberinto, inicio = parserArchivo("salida.txt")
        ruta = recorrido(inicio[1],inicio[0], laberinto)
    for paso in ruta: print(paso)
    
 
if __name__ == "__main__":
    main()
