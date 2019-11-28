#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estructura que almacena los datos del laberinto a generar.
typedef struct
{
    int dimension, cantParedesRandom, cantParedesDefinidas, cantParedesRandomColocadas;
    int posInicial[2], posFinal[2];
    int **listaParedes;
    int **listaObstaculosRandom;
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
        fscanf(fp, "%d,%d)\n", &datos->listaParedes[cantParedes][0], &datos->listaParedes[cantParedes][1]);
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
    fscanf(fp, "(%d,%d)\n", &datos->posInicial[0], &datos->posInicial[1]);
    datos->posInicial[0] --;
    datos->posInicial[1] --;
    fscanf(fp, "%s\n", linea);
    fscanf(fp, "(%d,%d)\n", &datos->posFinal[0], &datos->posFinal[1]);
    datos->posFinal[0]--;
    datos->posFinal[1]--;
    // Se cierra el archivo y retorna datos.
    fclose(fp);
    return datos;
}

// Funcion que resume la creacion de numeros random para que sea mas limpio su uso en el codigo.
int generarRandom(int min, int max){
    // Retornamos un numero random entre el minimo y el maximo.
    return rand() % max + min;
}

// Funcion encargada de determinar el caracter que ira en la posicion x, y.
// Ademas es usada en la generacion de randoms para descartar la posibilidad de que una nueva pared random sea colocada en
// la posicion de otro elemento.
char comparativa(int x, int y, datosLaberinto * datos){
    // Devulve I si x, y corresponden a la posicion incial.
    if(x == datos->posInicial[0] && y == datos->posInicial[1])
        return 'I';
    // Devulve X si x, y corresponden a la posicion final.
    if(x == datos->posFinal[0] && y == datos->posFinal[1])
        return 'X';
    // Devulve 1 en caso de que x, y corresponden a una pared, ya sea random o no.
    for(int i = 0; i < datos->cantParedesRandomColocadas; i++) {
        if (x == datos->listaObstaculosRandom[i][0] && y == datos->listaObstaculosRandom[i][1])
            return '1';
    }
    for(int i = 0; i < datos->cantParedesDefinidas; i++){
        if (x == datos->listaParedes[i][0] && y == datos->listaParedes[i][1])
            return '1';
    }
    // 0 en el caso de que no corresponda a ningun elemento.
    return '0';
}

// Funcion auxiliar utilizada para intercambiar el contenido de dos punteros a ints. (caso particular)
void swap(int *x, int *y){
    int temp[2];
    temp[0] = x[0];
    temp[1] = x[1];
    x[0] = y[0];
    x[1] = y[1];
    y[0] = temp[0];
    y[1] = temp[1];
}

// Fucnion encargada de generar los obstaculos random.
void generadorObstaculosRandom(datosLaberinto * datos){
    // Seed de generacion seteada con respecto al tiempo.
    srand(time(NULL));
    // Variables utiles durante el desarollo.
    int tam = datos->dimension*datos->dimension, puestas = 0, cont = 0;
    // Array bidemencional que contendra todas las posibles convinaciones de (i,j).
    int listaPosicionesPosibles[tam][2];
    // Se setea la array ^.
    for(int j = 0; j < datos->dimension; j++){
        for(int i = 0; i < datos->dimension; i++){
            listaPosicionesPosibles[puestas][0] = j;
            listaPosicionesPosibles[puestas][1] = i;
            puestas ++;
        }
    }
    // Se intercambian de manera aleatoria el contenido de la array, para que las posiciones sean aleatorias.
    for(int i = 0; i < tam; i++){
        swap(listaPosicionesPosibles[i], listaPosicionesPosibles[generarRandom(0, tam)]);
    }
    // Se setea el puntero en NULL para que el realloc pueda inicializar su tamaño.
    datos->listaObstaculosRandom = NULL;
    // Se comienza a obtener los randoms pedidos.
    while(datos->cantParedesRandomColocadas < datos->cantParedesRandom){
        // Se utiliza comparativa para determinar si en esa posicion hay algun otro objeto.
        if(comparativa(listaPosicionesPosibles[cont][0], listaPosicionesPosibles[cont][1], datos) =='0'){
            // Si esta vacia, se pide mamoria y se almacena la posicion del random.
            datos->listaObstaculosRandom = (int **)realloc(datos->listaObstaculosRandom, (datos->cantParedesRandomColocadas+1)* sizeof(int*));
            datos->listaObstaculosRandom[datos->cantParedesRandomColocadas] = (int *)calloc(2, sizeof(int));
            datos->listaObstaculosRandom[datos->cantParedesRandomColocadas][0] = listaPosicionesPosibles[cont][0];
            datos->listaObstaculosRandom[datos->cantParedesRandomColocadas][1] = listaPosicionesPosibles[cont][1];
            // Printf auxiliar (sacar)
            printf("(%d, %d)", listaPosicionesPosibles[cont][0], listaPosicionesPosibles[cont][1]);
            datos->cantParedesRandomColocadas++;
        }
        // Contador aumentado, para seguir recorriendo la array de posibles.
        cont++;
    }
}


// Funcion que se encarga de generar el archivo que contiene al laberinto.
// Recibe la estructura de datos y escribe con ellos el archivo.
void generadorDeLaberinto(datosLaberinto * datos, char * path){
    FILE *fp;
    // Se abre el archivo en modo escritura +, para reemplazar el archivo que tenga ese nombre.
    fp = fopen(path, "w+");
    // Se generan los obstaculos random.
    generadorObstaculosRandom(datos);
    // Se recorre toda la matriz que representa el laberinto, escribiendo en el archivo segun corresponda.
    for(int x = 0; x < datos->dimension; x++){
        for(int y = 0; y < datos->dimension; y++){
            // Se utiliza nuevamente comparativa.
            fprintf(fp,"%c",comparativa(x, y, datos));
        }
        // Se tiene en cuenta no colocar un \n al final, para facilitar el parseo en la siguiente parte.
        if((x + 1) != datos->dimension)
            fprintf(fp, "%s","\n");
    }
    // Se cierra el archivo.
    fclose(fp);
}

void liberarMemoria(datosLaberinto * datos){
    for(int i = 0; i < datos->cantParedesDefinidas; i++){
        free(datos->listaParedes[i]);
    }
    free(datos->listaParedes);
    for(int i = 0; i < datos->cantParedesRandom; i++){
        free(datos->listaObstaculosRandom[i]);
    }
    free(datos->listaObstaculosRandom);
    free(datos);
}

int main() {
    datosLaberinto * datos = parserArchivo("entrada.txt");
    generadorDeLaberinto(datos, "salida.txt");
    liberarMemoria(datos);
    return 0;
}
