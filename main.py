def parserArchivo(pathF):
    file = open(pathF, 'r')
    archivo = file.read()
    Alab = [[]]
    linea = 0
    for carac in archivo:
        if carac == '\n':
            Alab.append([])
            linea+=1
        else:
            Alab[linea].append(ord(carac)-48)
    file.close()
    return Alab

def recorrido(i, j, laberinto): 
    if laberinto[i][j] == 2:
        return [(j, i)]
 
    if laberinto[i][j] == 1:
        return []
 
    laberinto[i][j] = -1
 
    if i > 0 and laberinto[i - 1][j] in [0, 2]:                     # Norte
        camino = recorrido(i - 1, j, laberinto)
        if camino: return [(j, i)] + camino
 
    if j < len(laberinto[i]) - 1 and laberinto[i][j + 1] in [0, 2]: # Este
        camino = recorrido(i, j + 1, laberinto)
        if camino: return [(j, i)] + camino
 
    if i < len(laberinto) - 1 and laberinto[i + 1][j] in [0, 2]:    # Sur
        camino = recorrido(i + 1, j, laberinto)
        if camino: return [(j, i)] + camino
 
    if j > 0 and laberinto[i][j - 1] in [0, 2]:                     # Oeste
        camino = recorrido(i, j - 1, laberinto) 
        if camino: return [(j, i)] + camino
 
    return []
 

laberinto = parserArchivo("hola.txt")
ruta = recorrido(0,0, laberinto)
if ruta == []:
    print("No se puede llegar al objetivo")
else:
    for x in ruta: print(x)
