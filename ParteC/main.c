#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estructura que almacena los datos del laberinto a generar.
typedef struct
{
    int dimension, cantParedesRandom, cantParedesDefinidas;
    int posInicial[2], posFinal[2];
    int **listaParedes;
    char ** laberinto;
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
        if(datos->listaParedes[cantParedes][0] <=0 || datos->listaParedes[cantParedes][1] > datos->dimension){
            // Se testea que la entrada tenga sentido.
            printf("Alguna de las coordenadas ingresadas exede los limites del laberinto");
            return NULL;
        }
        datos->listaParedes[cantParedes][0] --;
        datos->listaParedes[cantParedes][1] --;
        // Se incrementa la cantidad de paredes.
        cantParedes ++;
    }
    // De la misma manera que anteriormente, se guarda la informacion correspondiente a posicion inicial y final.
    datos->cantParedesDefinidas = cantParedes;
    fscanf(fp, "%s %s\n", linea, linea);
    fscanf(fp, "%d\n", &datos->cantParedesRandom);
    // Se testea que la entrada tenga sentido.
    if((datos->cantParedesRandom + cantParedes + 2) > (datos->dimension*datos->dimension)){
        printf("La cantidad de paredes ingresadas es mayor que la cantidad maxima admitida");
        return NULL;
    }
    fscanf(fp, "%s %s\n", linea, linea);
    fscanf(fp, "(%d,%d)\n", &datos->posInicial[0], &datos->posInicial[1]);
    if(datos->posInicial[0] <=0 || datos->posInicial[1] > datos->dimension){
        // Se testea que la entrada tenga sentido.
        printf("Las coordenadas de la posicion inicial no pertenecen al laberinto");
        return NULL;
    }
    datos->posInicial[0] --;
    datos->posInicial[1] --;

    fscanf(fp, "%s\n", linea);
    fscanf(fp, "(%d,%d)\n", &datos->posFinal[0], &datos->posFinal[1]);
    if(datos->posFinal[0] <=0 || datos->posFinal[1] > datos->dimension){
        // Se testea que la entrada tenga sentido.
        printf("Las coordenadas de la posicion final no pertenecen al laberinto");
        return NULL;
    }
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
// Funcion generadora de obstaculos aleatorios, que agrega al laberinto, cuidando que no se "pise" ningun otro elemento.
void generarObstaculosRandom(datosLaberinto * datos){
    // Se da una propuesta de posicion
    int i = generarRandom(0, datos->dimension);
    int j = generarRandom(0, datos->dimension);
    // Se testea si coincide con otro elemento
    while(datos->laberinto[i][j] != '0'){
        i = generarRandom(0, datos->dimension);
        j = generarRandom(0, datos->dimension);
    }
    // Se setea el obstaculo.
    datos->laberinto[i][j] = '1';
}
// Similar a la funcion anterior, solo que genera los espacios (0). Funcionamiento similar.
void generarEspaciosRandom(datosLaberinto * datos){
    int i = generarRandom(0, datos->dimension);
    int j = generarRandom(0, datos->dimension);
    while(datos->laberinto[i][j] != '1'){
        i = generarRandom(0, datos->dimension);
        j = generarRandom(0, datos->dimension);
    }
    datos->laberinto[i][j] = '0';
}

// Funcion encargada de generar el laberinto.
void generarLaberinto(datosLaberinto * datos, char * seed){
    // Se setea la seed, pasada como argumento desde python.
    srand(atoi(seed));
    // Calculo de celdas.
    int cantOcupadas = datos->cantParedesDefinidas + datos->cantParedesRandom + 2;
    int cantLibres = (datos->dimension * datos->dimension) - cantOcupadas;

    // Segun la relacion de cantidad entre celdas vacias, y llenas se decide que metodo usar.
    if(cantLibres >= cantOcupadas){
        // Se pide memoria para almacenar la array que contendra el laberinto.
        datos->laberinto = (char **)calloc(datos->dimension, sizeof(char *));
        // Se inicializa en 0.
        for(int i = 0; i < datos->dimension; i++){
            datos->laberinto[i] = (char *)calloc(datos->dimension + 1, sizeof(char));
            for(int j = 0; j < datos->dimension; j++){
                datos->laberinto[i][j] = '0';
            }
            // Se escribe el salto de linea al final de cada linea para poder escibir por linea luego.
            if((i + 1) != datos->dimension)
                datos->laberinto[i][datos->dimension] = '\n';
        }
        // Se setean los obejtos con posicion particular.
        datos->laberinto[datos->posInicial[0]][datos->posInicial[1]] = 'I';
        datos->laberinto[datos->posFinal[0]][datos->posFinal[1]] = 'X';
        for(int i = 0; i < datos->cantParedesDefinidas;i++){
            datos->laberinto[datos->listaParedes[i][0]][datos->listaParedes[i][1]] = '1';
        }
        // Se generan los obstaulos random
        for (int i = 0; i < datos->cantParedesRandom; ++i) {
            generarObstaculosRandom(datos);
        }
    }else{
        // Se pide memoria para almacenar la array que contendra el laberinto.
        datos->laberinto = (char **)calloc(datos->dimension, sizeof(char *));
        // Se inicializa en '1'
        for(int i = 0; i < datos->dimension; i++){
            datos->laberinto[i] = (char *)calloc(datos->dimension + 1, sizeof(char));
            for(int j = 0; j < datos->dimension; j++){
                datos->laberinto[i][j] = '1';
            }
            // Se escribe el salto de linea al final de cada linea para poder escibir por linea luego.
            if((i + 1) != datos->dimension)
                datos->laberinto[i][datos->dimension] = '\n';
        }
        // Se setean las paredes fijas con un 2, para poder generar los espacios vacios random.
        for(int i = 0; i < datos->cantParedesDefinidas; i++){
            datos->laberinto[datos->listaParedes[i][0]][datos->listaParedes[i][1]] = '2';
        }
        // Posiciones particulares.
        datos->laberinto[datos->posInicial[0]][datos->posInicial[1]] = 'I';
        datos->laberinto[datos->posFinal[0]][datos->posFinal[1]] = 'X';
        // Se generan espacios random.
        for (int i = 0; i < cantLibres; ++i) {
            generarEspaciosRandom(datos);
        }// Paredes fijas otra vez en '1'.
        for(int i = 0; i < datos->cantParedesDefinidas; i++){
            datos->laberinto[datos->listaParedes[i][0]][datos->listaParedes[i][1]] = '1';
        }
    }
}

// Funcion que imprime en el archivo el laberinto.
void imprimirLaberinto(datosLaberinto * datos, char * path){
    FILE *fp = fopen(path, "w+");
    for(int i = 0; i < datos->dimension;i++){
        fprintf(fp,"%s", datos->laberinto[i]);
    }
    fclose(fp);
}


// Funcion que se encarga de liberar la memoria utilizada en el programa.
void liberarMemoria(datosLaberinto * datos){
    // Se libera la array bidemencional listaParedes.
    for(int i = 0; i < datos->cantParedesDefinidas; i++){
        free(datos->listaParedes[i]);
    }
    free(datos->listaParedes);
    // Se libera listaParedesRandom
    for(int i = 0; i < datos->dimension; i++){
        free(datos->laberinto[i]);
    }
    free(datos->laberinto);
    // Y por ultimo el puntero a la estructura.
    free(datos);
}

int main(int argc, char * argv[]) {
    datosLaberinto * datos = parserArchivo(argv[1]);
    // Si la entra es invalidad, datos devuelve NULL, en ese caso no se sigue con el programa.
    if(datos!=NULL){
        generarLaberinto(datos, argv[2]);
        imprimirLaberinto(datos, "salidaC.txt");
        liberarMemoria(datos);
    }
    return 0;
}
