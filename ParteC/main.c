#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define INDICE_CHAR_TO_INT 48

// Estructura que almacena los datos del laberinto a generar.
typedef struct
{
    int dimension, cantParedesRandom, cantParedesDefinidas, cantParedesRandomColocadas;
    int posInicial[2], posFinal[2];
    int **listaParedes;
    // Se declara una cantidad de obstaculos exagerada, para no tenes que trabajar con memoria dinamica, reallocando cada vez que sea necesario.
} datosLaberinto;

// Funcion que se encarga de leer el archivo de entrada con los datos del laberinto y los almacena en la estructura definida anteriormente,
// para luego ser procesados.
datosLaberinto * parserArchivo(char * path){
    FILE *fp;
    // Se abre el archivo en modo lectura.
    fp = fopen(path, "r");
    // Se reserva memoria para la estructura que almacena la informacion.
    datosLaberinto * datos = calloc(1, sizeof(datosLaberinto));
    // Variable utilizada para almacenar las palabras de modo auxiliar.
    char linea[20];
    // Se comienza a leer el archivo
    // Se descarta la palabra dimension
    fscanf(fp, "%s\n", linea);
    // Se almacena el numero dimension
    fscanf(fp, "%s", linea);
    datos->dimension = atoi(linea);
    // Es descartada la siguiente linea
    fscanf(fp, "%s %s\n", linea, linea);
    // Contador de la cantidad de paredes que se ingresa.
    int cantParedes = 0;
    // Bucle que se utiliza para leer todas las posiciones de las paredes.
    while(fgetc(fp) == '('){
        // Se atualiza la memoria necesaria para almacenar las paredes.
        datos->listaParedes = (int **) realloc(datos->listaParedes, sizeof(int*) * (cantParedes+1));
        datos->listaParedes[cantParedes] = (int *)malloc(sizeof(int)*2);
        // Se almacenan correspondientemente.
        fscanf(fp, "%d,%d)\n", &datos->listaParedes[cantParedes][1], &datos->listaParedes[cantParedes][0]);
        datos->listaParedes[cantParedes][0] --;
        datos->listaParedes[cantParedes][1] --;
        // Se incrementa la cantidad de paredes.
        cantParedes ++;
    }
    // De la misma manera que anteriormente, se guarda la informacion correspondiente a posicion inicial y final.
    datos->cantParedesDefinidas = cantParedes;
    fscanf(fp, "%s %s\n", linea, linea);
    fscanf(fp, "%d\n", &datos->cantParedesRandom);
    fscanf(fp, "%s %s\n", linea, linea);
    fscanf(fp, "(%d,%d)\n", &datos->posInicial[1], &datos->posInicial[0]);
    datos->posInicial[0] --;
    datos->posInicial[1] --;
    fscanf(fp, "%s\n", linea);
    fscanf(fp, "(%d,%d)\n", &datos->posFinal[1], &datos->posFinal[0]);
    datos->posFinal[0]--;
    datos->posFinal[1]--;
    // Se cierra el archivo y retorna datos.
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


// Funcion que genera la pocicion de los obstaculos random.
int ** generadorObstaculosRandom(datosLaberinto * datos){
    // Seteada la semilla de generacion con respecto al tiempo.
    srand(time(NULL));
    // Se reserva la memoria necesaria para almacenar todas las posiciones.
    int ** obstaculosRandom = (int **) calloc(datos->cantParedesRandom, sizeof(int *));
    int i = 0, x, y;
    // Ciclo que itera sobre la cantidad de obstaculos generados.
    while( i < datos->cantParedesRandom){
        // Se generan.
        x = generarRandom(0, datos->dimension);
        y = generarRandom(0, datos->dimension);
        // Se comprueba mediante la funcion comparativa, si la posicion coincide con otro elemento.
        if(comparativa(x, y, datos, obstaculosRandom) == '0'){
            // Si la posicion no esta ocupada, se coloca un muro random.
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

int main() {

    datosLaberinto * datos = parserArchivo("entrada.txt");
    generadorDeLaberinto(datos, "salida.txt");
    //printf("%d", datos->posFinal[0]);
    return 0;
}
