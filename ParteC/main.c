#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define INDICE_CHAR_TO_INT 48


// Estructura que almacena los datos del laberinto a generar.
typedef struct
{
    int dimension, cantParedesRandom, cantParedesDefinidas;
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
            datos->dimension = linea[0] - INDICE_CHAR_TO_INT;
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
            datos->cantParedesRandom = linea[0] - INDICE_CHAR_TO_INT;
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
     // Cierro el archivo y retorno el puntero a la estructura.
    fclose(fp);
    return datos;
}


int generarRandom(int min, int max){
    return rand() % max + min;
}

int ** generadorObstaculosRandom(int cantR, int dimension){
    srand(time(NULL));
    int ** obstaculosRandom = (int **)calloc(cantR, sizeof(int *));
    for(int i = 0; i < cantR; i++){
        obstaculosRandom[i] = (int *)calloc(2, sizeof(int));
        obstaculosRandom[i][0] = generarRandom(0, dimension);
        obstaculosRandom[i][1] = generarRandom(0, dimension);
    }
    return obstaculosRandom;
}

char comparativa(int x, int y, int ** listaParedes, int cantParedesDefinidas, int ** obstaculosRandom, int cantParedesRandom){
    for(int i = 0; i < cantParedesDefinidas; i++){
        if (x == listaParedes[i][0] && y == listaParedes[i][1])
            return '1';
    }
    for(int i = 0; i < cantParedesRandom; i++) {
        if (x == obstaculosRandom[i][0] && y == obstaculosRandom[i][1])
            return '1';
    }
    return '0';
}

// Funcion que se encarga de generar el archivo que contiene al laberinto.
// Recibe la estructura de datos y escribe con ellos el archivo.
void generadorDeLaberinto(datosLaberinto * datos, char * path){
    FILE *fp;
    fp = fopen(path, "w+");
    int **  obstaculosRandom = generadorObstaculosRandom(datos->cantParedesRandom, datos->dimension);

    for(int y = 0; y < datos->dimension; y++){
        for(int x = 0; x < datos->dimension; x++){
            if(x == datos->posInicial[0] && y == datos->posInicial[1])
                fprintf(fp, "%c", 'I');
            else
            if(x == datos->posFinal[0] && y == datos->posFinal[1])
                fprintf(fp, "%c", 'X');
            else{

                fprintf(fp,"%c",comparativa(x, y, datos->listaParedes, datos->cantParedesDefinidas, obstaculosRandom, datos->cantParedesRandom));
            }

        }
        if((y + 1) != datos->dimension)
            fprintf(fp, "%s","\n");
    }
    fclose(fp);

}

int main() {

    datosLaberinto * datos = parserArchivo("entrada.txt");
    generadorDeLaberinto(datos, "salida.txt");

    return 0;
}