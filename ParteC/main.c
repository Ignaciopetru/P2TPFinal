#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define INDICE_CHAR_TO_INT 48


// Estructura que almacena los datos del laberinto a generar.
typedef struct
{
    int dimension, cantParedesRandom, cantParedesDefinidas, cantParedesRandomColocadas;
    int posInicial[2], posFinal[2];
    int **listaParedes; // Se declara una cantidad de obstaculos exagerada, para no tenes que trabajar con memoria dinamica, reallocando cada vez que sea necesario.
} datosLaberinto;

// Funcion que se encarga de leer el archivo de entrada con los datos del laberinto y los almacena en la estructura definida anteriormente,
// para luego ser procesados.
datosLaberinto * parserArchivo(char * path){
    FILE *fp;
    // Se abre el archivo en modo lectura.
    fp = fopen(path, "r");
    datosLaberinto * datos = calloc(1, sizeof(datosLaberinto));
    // Variables auxiliares de la funcion.
    char linea[80];
    int cantLineasL = 0, indiceParedes = 0, indicadorTerminoParedes = 0, indicadorContinua = 0;
    datos->listaParedes = NULL;
    // While que recorre palabra por palabra el archivo.
    // Se parsea teniendo en cuenta la dispocicion del archivo entrada.
    while(fscanf(fp, "%s", linea)!=EOF) {
        // Segunda linea almacena la dimension del laberinto
        if(cantLineasL == 1){
            datos->dimension = atoi(linea);
        }
        // Cuando la linea comienza con parentesis.
        if(linea[0] == '(' && indicadorContinua == 0){
            // Almaceno la ubicacion de la pared en la lista dentro de la estructura.
            datos->listaParedes = (int **) realloc(datos->listaParedes, sizeof(int*) * (indiceParedes+1));
            datos->listaParedes[indiceParedes] = (int *)malloc(sizeof(int)*2);
            datos->listaParedes[indiceParedes][0] = linea[1] - INDICE_CHAR_TO_INT - 1;
            datos->listaParedes[indiceParedes][1] = linea[3] - INDICE_CHAR_TO_INT - 1;
            indicadorTerminoParedes = 1;
            // Incremento la cantidad de paredes determinadas en el archivo.
            indiceParedes++;
        }
        // Se saltea las lineas siguientes a la utlima posicion de obstaculo determinado.
        if(indicadorTerminoParedes == 1 && linea[0] != '('){
            indicadorContinua ++;
        }
        // Se saltean 2, una para cada palabra (obstaculos y random).
        if(indicadorContinua == 3){
            datos->cantParedesRandom = atoi(linea);
        }
        // Se almacena la ubicacion de inicio y de fin.
        if(indicadorContinua == 5 && linea[0] == '('){
            datos->posInicial[0] = linea[1] - INDICE_CHAR_TO_INT - 1;
            datos->posInicial[1] = linea[3] - INDICE_CHAR_TO_INT - 1;
        }
        if(indicadorContinua == 6 && linea[0] == '('){
            datos->posFinal[0] = linea[1] - INDICE_CHAR_TO_INT - 1;
            datos->posFinal[1] = linea[3] - INDICE_CHAR_TO_INT - 1;
        }
        cantLineasL ++;
    }
    // Guardo la cantidad de obstaculos definidos.
    datos->cantParedesDefinidas = indiceParedes;
    datos->cantParedesRandomColocadas = 0;
    // Cierro el archivo y retorno el puntero a la estructura.
    fclose(fp);
    return datos;
}

// Funcion que resume la creacion de numeros random para que sea mas limpio su uso en el codigo.
int generarRandom(int min, int max){
    // Seteamos la semilla de generacion con respecto a la hora.

    // Retornamos un numero random entre el minimo y el maximo.
    return rand() % max + min;
}

// Funcion encargada de determinar el caracter que ira en la posicion x, y.
// Ademas es usada en la generacion de randoms para descartar la posibilidad de que una nueva pared random sea colocada en
// la posicion de otro elemento.
char comparativa(int x, int y, datosLaberinto * datos, int ** obstaculosRandom){
    // Devulve I si x, y corresponden a la posicion incial.
    if(x == datos->posInicial[0] && y == datos->posInicial[1])
        return 'I';
    // Devulve X si x, y corresponden a la posicion final.
    if(x == datos->posFinal[0] && y == datos->posFinal[1])
        return 'X';
    // Devulve 1 en caso de que x, y corresponden a una pared, ya sea random o no.
    for(int i = 0; i < datos->cantParedesRandomColocadas; i++) {
        if (x == obstaculosRandom[i][0] && y == obstaculosRandom[i][1])
            return '1';
    }
    for(int i = 0; i < datos->cantParedesDefinidas; i++){
        if (x == datos->listaParedes[i][0] && y == datos->listaParedes[i][1])
            return '1';
    }
    // 0 en el caso de que no corresponda a ningun elemento.
    return '0';
}

int ** generadorObstaculosRandom(datosLaberinto * datos){
    srand(time(NULL));
    int ** obstaculosRandom = (int **) calloc(datos->cantParedesRandom, sizeof(int *));
    int i = 0, x, y;
    while( i < datos->cantParedesRandom){
        x = generarRandom(0, datos->dimension);
        y = generarRandom(0, datos->dimension);
        if(comparativa(x, y, datos, obstaculosRandom) == '0'){
            obstaculosRandom[i] = (int *)calloc(2, sizeof(int));
            obstaculosRandom[i][0] = x;
            obstaculosRandom[i][1] = y;
            datos->cantParedesRandomColocadas ++;
            i++;
            printf("(%d, %d)",x,y);
        }
    }
    return obstaculosRandom;
}



// Funcion que se encarga de generar el archivo que contiene al laberinto.
// Recibe la estructura de datos y escribe con ellos el archivo.
void generadorDeLaberinto(datosLaberinto * datos, char * path){
    FILE *fp;
    fp = fopen(path, "w+");
    int **  obstaculosRandom = generadorObstaculosRandom(datos);

    for(int y = 0; y < datos->dimension; y++){
        for(int x = 0; x < datos->dimension; x++){
            fprintf(fp,"%c",comparativa(x, y, datos, obstaculosRandom));
        }
        if((y + 1) != datos->dimension)
            fprintf(fp, "%s","\n");
    }
    fclose(fp);

}

int main(int argc, char ** argv) {
    printf("gola");
    datosLaberinto * datos = parserArchivo(argv[1]);
    generadorDeLaberinto(datos, "salida.txt");

    return 0;
}
