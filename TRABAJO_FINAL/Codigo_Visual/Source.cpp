#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include"SerialClass/SerialClass.h"
#define MAX_BUFFER 200
#define PAUSA_MS 20
#define Q 8
#define P 100
#define LONGCAD 100

struct piezas {


    char Peon;
    char Rey;
    char Dama;
    char Alfil;
    char Torre;
    char Caballo;

};

struct movimiento{

    char pieza;
    char ent_mov[3];
    char sal_mov[3];
    int tiempo_duracion;

};



typedef struct {

    char nombre_jugador1[LONGCAD];
    char nombre_jugador2[LONGCAD];
    char codigo_busqueda_partida[LONGCAD];
    movimiento jugadas_jugador1[LONGCAD];
    movimiento jugadas_jugador2[LONGCAD];
    
}almacenar;


int mostrar_datos_usuario(almacenar u);
void listado_usuarios(almacenar* usuario, int n);
almacenar leer_datos_usuario(void);
int alta_usuario(almacenar** plista, int* pn, int* pl);
void imprimir_tablero(char tab[8][8]);
char generar_movimiento(char tab[8][8], char ent_mov[3], char sal_mov[3]);
int Enviar_y_Recibir(Serial* Arduino, const char* mensaje_enviar, char* mensaje_recibir);
void modelo_partida(Serial* Arduino, int tiempo_partida);
int leer_jugada(char tab[8][8], char ent_mov[3], char sal_mov[3]);
int menu_ppal1(void);
int menu_ppal2(void);
void solicitar_tiempos_movimientos(Serial* Arduino, almacenar* alm);
int escribir_fichero(almacenar* lista, int n);
void guardar_movimientos(char entrada[3], char salida[3], char pieza_movida, almacenar* palm, int n);



int main()
{
    Serial* Arduino;
    char puerto[] = "COM4";

    Arduino = new Serial((char*)puerto);

    struct piezas neg = { 'P','R','D','A','T','C' };//Se introducen las piezas negras
    struct piezas bln = { 'p','r','d','a','t','c' };//Se introducen las piezas blancas
    struct piezas* N;
    struct piezas* B;
    
    N = &neg;
    B = &bln;
    char otro_movimiento = 'S';
    char pieza;
    int tiempo_minutos_partida = 0;
    
    int i, j, k;
    int cont = 0, contador_malloc = 0;

    char tablero[Q][Q];
    char ent_mov[3];
    char sal_mov[3];

    int camb_ent1, camb_ent2;
    int camb_sal1, camb_sal2;
    int clave_salida = 0;
    
    almacenar alm[P];
    almacenar* palm;
    palm = alm;
   
   
    int longitud = 0, n = 0;

    tablero[0][0] = B->Torre;
    tablero[0][1] = B->Caballo;
    tablero[0][2] = B->Alfil;
    tablero[0][3] = B->Rey;
    tablero[0][4] = B->Dama;
    tablero[0][5] = B->Alfil;
    tablero[0][6] = B->Caballo;
    tablero[0][7] = B->Torre;

    tablero[7][0] = N->Torre;
    tablero[7][1] = N->Caballo;
    tablero[7][2] = N->Alfil;
    tablero[7][3] = N->Rey;
    tablero[7][4] = N->Dama;
    tablero[7][5] = N->Alfil;
    tablero[7][6] = N->Caballo;
    tablero[7][7] = N->Torre;



    for (i = 0; i < Q; i++)
    {
        tablero[1][i] = B->Peon;

        for (j = 2; j < 6; j++)
        {
            tablero[j][i] = ' ';
        }
        tablero[6][i] = N->Peon;
    }

    do {
        setlocale(LC_ALL, "ES-es");

        menu_ppal1();
        Sleep(PAUSA_MS);

        switch (menu_ppal1())
        {
        case 1:
            menu_ppal2();
            Sleep(PAUSA_MS);
            switch (menu_ppal2())
            {

            case 1:
                tiempo_minutos_partida = 3;
                break;
            case 2:
                tiempo_minutos_partida = 5;
                break;
            case 3:
                tiempo_minutos_partida = 10;
                break;
            case 4:
                tiempo_minutos_partida = 20;
                break;
            case 5:
                tiempo_minutos_partida = 30;
                break;
            default: printf("No es posible esa opcion\n");
                break;

            }

            imprimir_tablero(tablero);

            modelo_partida(Arduino, tiempo_minutos_partida);
            Sleep(PAUSA_MS);

           alta_usuario(&palm, &n, &longitud);

           
           do {


                do {
                    getchar();
                    printf("Introduzca la casilla de la que parte la pieza\n");
                    gets_s(ent_mov);

                    getchar();
                    printf("Introduzca la casilla donde desea colocar la pieza\n");
                    gets_s(sal_mov);
                    leer_jugada(tablero, ent_mov, sal_mov);

                    if (leer_jugada(tablero, ent_mov, sal_mov) == 0)
                    {

                        printf("No es posible realizar ese movimiento tendrá que volver a introducirlo\n");

                    }


                } while (leer_jugada(tablero, ent_mov, sal_mov) == 1);


                pieza = generar_movimiento(tablero, ent_mov, sal_mov);
                cont++;

                guardar_movimientos(ent_mov, sal_mov, pieza, palm, cont);

                imprimir_tablero(tablero);


                printf("Desea realizar otro movimiento:S/N?\n");
                scanf_s("%c", &otro_movimiento);

            }while (otro_movimiento != 'N');
            escribir_fichero(palm, cont);

            break;

        case 2:

                listado_usuarios(palm, cont);
           
            break;
        case 3:

            clave_salida = 1;

            break;


        default: printf("No es posible esa opcion\n");
            break;

        }

    } while (clave_salida != 1);

    solicitar_tiempos_movimientos(Arduino, palm);

}

  






   

almacenar leer_datos_usuario(void)
{
    almacenar alm;
    printf("Alta de nueva partida\n");
    printf("========================\n");
    printf("Nombre jugador1:\n");
    gets_s(alm.nombre_jugador1, LONGCAD);
    getchar();
    printf("Nombre jugador2:\n");
    gets_s(alm.nombre_jugador2, LONGCAD);
    getchar();
    printf("Introduzca un codigo para rastrearla partida:\n");
    printf("Recomendamos fecha y nombres jugadores\n");
    gets_s(alm.codigo_busqueda_partida, LONGCAD);


    return alm;

}





int alta_usuario(almacenar**plista,int*pn,int*pl)
{
    int n = *pn;
    int l = *pl;
    almacenar* lista = *plista;
    almacenar* lista_antigua;
    int cod_error = 0;
    if (l == 0) // Si la longitud es cero hay que crear la lista por primera vez
    {
        lista = (almacenar*)malloc(sizeof(almacenar) * P);
        if (lista == NULL) //Si falla la solicitud de memoria para los N primeros
        {
            printf("No se ha podido reservar memoria\n");
            cod_error = 1;
        }
        else
            l = P; // Si se logra reservar memoria actualizamos tamaño de la lista
    }
    else //Si ya existe memoria previamente reservada
    {
        if (l == n) // Si la lista actual está completa
        {
            lista_antigua = lista;
            lista = (almacenar*)realloc(lista, sizeof(almacenar) * (P + 1));
            if (lista == NULL) // No es posible el incremento de l+N
            {
                printf("No se ha podido incrementar el espacio para nuevos usuarios");
                lista = lista_antigua;
                cod_error = 2;
            }
            else //Si el incremento de l+N ha sido posible
                l = l + P;
        }
    }
    if (cod_error == 0) // Si no hemos tenido problemas con la reserva de memoria
    {
        *(lista + n) = leer_datos_usuario(); // Lee los datos del nuevo usuario
        n++; // Incrementa la cantidad de usuarios
        *pn = n;  // Actualiza los valores de los argumentos pasados por referencia
        *pl = l;
        *plista = lista;
    }
    return cod_error;
}



void listado_usuarios(almacenar*usuario,int n)
{
    int i;
    if (n == 0)
        printf("En este momento no hay usuarios en la aplicación\n");
    else
    {
        printf("Codigodebusqueda\n");
        printf("Nombre_jugador1\tdatosmovimientos\n");
        printf("Nombrejugador2\tdatosmovimientos\n");
        for (i = 0; i < n; i++)
            mostrar_datos_usuario(usuario[i]);
    }
}

int mostrar_datos_usuario(almacenar u)
{
    int i, j;
    puts(u.codigo_busqueda_partida);
    printf("\n");
    puts(u.nombre_jugador1);
    printf("\t");
    for (i = 0; (u.jugadas_jugador1->pieza + i) != NULL; i++)
    {
        puts(u.jugadas_jugador1->ent_mov);
        printf("%c\t", u.jugadas_jugador1->pieza);
        puts(u.jugadas_jugador1->sal_mov);
        printf("%d\t", u.jugadas_jugador1->tiempo_duracion);
    }
    for (j = 0; (u.jugadas_jugador2->pieza + j) != NULL; j++)
    {
        puts(u.jugadas_jugador2->ent_mov);
        printf("%c\t", u.jugadas_jugador2->pieza);
        puts(u.jugadas_jugador2->sal_mov);
        printf("%d\t", u.jugadas_jugador2->tiempo_duracion);

    }

    return 0;

    
}



void guardar_movimientos(char entrada[3], char salida[3], char pieza_movida, almacenar* palm,int n)
{
    if (n % 2 == 0)
    {
        palm->jugadas_jugador1[n / 2].ent_mov[0] = entrada[0];
        palm->jugadas_jugador1[n / 2].ent_mov[1] = entrada[1];

        palm->jugadas_jugador1[n /2].pieza = pieza_movida;
        palm->jugadas_jugador1[n / 2].sal_mov[0] = salida[0];
        palm->jugadas_jugador1[n / 2].sal_mov[1] = salida[1];
    }

    else {
        palm->jugadas_jugador2[n % 2].ent_mov[0] = entrada[0];
        palm->jugadas_jugador2[n % 2].ent_mov[1] = entrada[1];
        palm->jugadas_jugador2[n % 2].pieza = pieza_movida;
        palm->jugadas_jugador2[n % 2].sal_mov[0] = salida[0];
        palm->jugadas_jugador2[n % 2].sal_mov[1] = salida[1];
    }

 }















int menu_ppal1(void)
{

    int opcion;
    printf("\n");
    printf("Menú Principal\n");
    printf("==============\n");
    printf("1 - Jugar partida\n");
    printf("2 - Datos partidas anteriores\n");
    printf("3 - Salir de la partida\n");
    printf("Opción:\n");
    scanf_s("%d", &opcion);
    return opcion;

}

int menu_ppal2(void)
{
    int opcion;
    printf("\n");
    printf("Formato de partida:\n");
    printf("(Constituira el tiempo maximo de juego de los jugadores)\n");
    printf("==============\n");
    printf("1 - 3 minutos\n");
    printf("2 - 5 minutos\n");
    printf("3- 10 minutos\n");
    printf("4- 20 minutos\n");
    printf("5- 30 minutos\n");
    printf("Opción:");
    scanf_s("%d", &opcion);
    return opcion;

}


void imprimir_tablero(char tab[Q][Q])
{

    int k, m, l;


    printf("\n\n   ");
    for (k = 0; k < 8; k++)
    {
        printf("%d ", k);

    }
    printf("\n");
    printf("  +---------------+\n");

    for (m = 0; m < 8; m++) {
        printf("%c |", 'a' + m);
        for (l = 0; l < 8; l++)
        {
            printf("%c ", tab[m][l]);
        }
        printf("\b|\n");

    }
    printf("  +---------------+\n");



}

char generar_movimiento(char tab[Q][Q], char ent_mov[3], char sal_mov[3])
{
    char pieza = '0';

    int camb_ent1, camb_ent2;
    int camb_sal1, camb_sal2;

    //Algoritmos para transformar casillas en intercambio de piezas

    camb_ent1 = ent_mov[0] - 'a';
    camb_ent2 = ent_mov[1] - '0';
    camb_sal1 = sal_mov[0] - 'a';
    camb_sal2 = sal_mov[1] - '0';



    pieza = tab[camb_ent1][camb_ent2];
    tab[camb_ent1][camb_ent2] = ' ';
    tab[camb_sal1][camb_sal2] = pieza;




    return pieza;
}






int reverso(char valor)
{
    int resultado;
    
    resultado = (int)valor - '0';

    return resultado;
}

void solicitar_tiempos_movimientos(Serial* Arduino,almacenar*alm)
{
    int bytesRecibidos;
    char mensaje_recibido[MAX_BUFFER];
    int i;

    bytesRecibidos = Enviar_y_Recibir(Arduino, "ENVIAR TIEMPOS", mensaje_recibido);
    if (bytesRecibidos <= 0)
    {
        printf("\nNo se ha recibido respuesta a la petición\n");
    }
    else
    {
        printf("\nLa respuesta recibida tiene %d bytes. Recibido=", bytesRecibidos);
        puts(mensaje_recibido);

        for (i = 0; i < 200; i+2)
        {

            alm->jugadas_jugador1[i/2].tiempo_duracion = reverso(mensaje_recibido[i]);
            alm->jugadas_jugador2[i/2].tiempo_duracion = reverso(mensaje_recibido[i + 1]);
        }

    }

}











void modelo_partida(Serial* Arduino, int tiempo_partida)
{

    int bytesRecibidos;
    char mensaje_recibido[MAX_BUFFER];
    switch (tiempo_partida)
    {
    case 3:

        int bytesRecibidos;
        char mensaje_recibido[MAX_BUFFER];
        bytesRecibidos = Enviar_y_Recibir(Arduino, "3", mensaje_recibido);
        if (bytesRecibidos <= 0)
        {
            printf("\nNo se ha recibido respuesta a la petición\n");
        }
        else
        {
            printf("\nLa respuesta recibida tiene %d bytes. Recibido=", bytesRecibidos);
            puts(mensaje_recibido);
        }


        break;


    case 5:

        bytesRecibidos = Enviar_y_Recibir(Arduino, "5", mensaje_recibido);
        if (bytesRecibidos <= 0)
        {
            printf("\nNo se ha recibido respuesta a la petición\n");
        }
        else
        {
            printf("\nLa respuesta recibida tiene %d bytes. Recibido=", bytesRecibidos);
            puts(mensaje_recibido);
        }

        break;

    case 10:

        bytesRecibidos = Enviar_y_Recibir(Arduino, "10", mensaje_recibido);
        if (bytesRecibidos <= 0)
        {
            printf("\nNo se ha recibido respuesta a la petición\n");
        }
        else
        {
            printf("\nLa respuesta recibida tiene %d bytes. Recibido=", bytesRecibidos);
            puts(mensaje_recibido);
        }

        break;
    case 20:

        bytesRecibidos = Enviar_y_Recibir(Arduino, "20", mensaje_recibido);
        if (bytesRecibidos <= 0)
        {
            printf("\nNo se ha recibido respuesta a la petición\n");
        }
        else
        {
            printf("\nLa respuesta recibida tiene %d bytes. Recibido=", bytesRecibidos);
            puts(mensaje_recibido);
        }

        break;
    case 30:

        bytesRecibidos = Enviar_y_Recibir(Arduino, "30", mensaje_recibido);
        if (bytesRecibidos <= 0)
        {
            printf("\nNo se ha recibido respuesta a la petición\n");
        }
        else
        {
            printf("\nLa respuesta recibida tiene %d bytes. Recibido=", bytesRecibidos);
            puts(mensaje_recibido);
        }

        break;

    default:
        printf("Hay un error\n");
        break;
    }

}







int Enviar_y_Recibir(Serial* Arduino, const char* mensaje_enviar, char* mensaje_recibir)
{
    int bytes_recibidos = 0, total = 0;
    int intentos = 0, fin_linea = 0;
    Arduino->WriteData((char*)mensaje_enviar, strlen(mensaje_enviar));
    Sleep(PAUSA_MS);
    bytes_recibidos = Arduino->ReadData(mensaje_recibir, sizeof(char) * MAX_BUFFER - 1);
    while ((bytes_recibidos > 0 || intentos < 5) && fin_linea == 0)
    {
        if (bytes_recibidos > 0)
        {
            total += bytes_recibidos;
            if (mensaje_recibir[total - 1] == 13 || mensaje_recibir[total - 1] == 10)
                fin_linea = 1;
        }
        else
            intentos++;
        Sleep(PAUSA_MS);
        bytes_recibidos = Arduino->ReadData(mensaje_recibir + total, sizeof(char) * MAX_BUFFER - 1);
    }
    if (total > 0)
        mensaje_recibir[total - 1] = '\0';
    return total;
}



int leer_jugada(char tab[8][8], char ent_mov[3], char sal_mov[3])
{
    int vector_entrada[3];
    int vector_salida[3];
    int res_1;
    int res_2;
    char pieza;



    vector_entrada[0] = ent_mov[0] - 'a';
    vector_entrada[1] = ent_mov[1] - '0';
    vector_salida[0] = sal_mov[0] - 'a';
    vector_salida[1] = sal_mov[1] - '0';

    pieza = tab[vector_entrada[0]][vector_entrada[1]];
    res_1 = vector_salida[0] - vector_entrada[0];
    res_2 = vector_salida[1] - vector_entrada[1];



    switch (pieza)
    {
    case ' ': printf("No hay pieza en esta casilla\n"); return 0; break;
    case 'p':
        if (vector_entrada[0] == 1 && res_1 == 2 && res_2 == 0)
        {
            return 1;
        }
        else if (res_1 == 1 && res_2 == 0)
        {
            return 1;
        }
        else {
            return 0;
        }
        break;
    case 'c':
        if (res_1 < 0)
        {
            res_1 = res_1 * (-1);
        }
        if (res_2 < 0)
        {
            res_2 = res_2 * (-1);
        }

        if (res_1 == 2 && res_2 == 1)
        {
            return 1;
        }
        else if (res_1 == 1 && res_2 == 2)
        {
            return 1;

        }
        else {

            return 0;
        }

        break;
    case 'a':
        if (res_1 < 0)
        {
            res_1 = res_1 * (-1);
        }
        if (res_2 < 0)
        {
            res_2 = res_2 * (-1);
        }

        if (res_1 == res_2)
        {
            return 1;
        }
        else {

            return 0;

        } break;

    case 't':

        if (res_1 == 0 || res_2 == 0)
        {

            return 1;

        }
        else {

            return 0;

        }
        break;

    case 'r':
        if (res_1 < 0)
        {
            res_1 = res_1 * (-1);
        }
        if (res_2 < 0)
        {
            res_2 = res_2 * (-1);
        }

        if (res_1 == 0 && res_2 == 1)
        {
            return 1;
        }
        else if (res_1 == 1 && res_2 == 0)
        {
            return 1;
        }
        else if (res_1 == 1 && res_2 == 1)
        {

            return 1;
        }
        else {

            return  0;
        }

    case 'd':
        if (res_1 < 0)
        {
            res_1 = res_1 * (-1);
        }
        if (res_2 < 0)
        {
            res_2 = res_2 * (-1);
        }

        if (res_1 == res_2)
        {
            return 1;
        }
        else if (res_1 == 0 || res_2 == 0)
        {

            return 1;

        }
        else {

            return 0;

        }
        break;


    case 'P':
        if (vector_entrada[0] == 6 && res_1 == -2 && res_2 == 0)
        {
            return 1;
        }
        else if (res_1 == -1 && res_2 == 0)
        {
            return 1;
        }
        else {
            return 0;
        }
        break;

    case 'C':
        if (res_1 < 0)
        {
            res_1 = res_1 * (-1);
        }
        if (res_2 < 0)
        {
            res_2 = res_2 * (-1);
        }

        if (res_1 == 2 && res_2 == 1)
        {
            return 1;
        }
        else if (res_1 == 1 && res_2 == 2)
        {
            return 1;

        }
        else {

            return 0;
        }

        break;

    case 'A':
        if (res_1 < 0)
        {
            res_1 = res_1 * (-1);
        }
        if (res_2 < 0)
        {
            res_2 = res_2 * (-1);
        }

        if (res_1 == res_2)
        {
            return 1;
        }
        else {

            return 0;

        } break;
    case 'T':

        if (res_1 == 0 || res_2 == 0)
        {

            return 1;

        }
        else {

            return 0;

        }

    case 'R':
        if (res_1 < 0)
        {
            res_1 = res_1 * (-1);
        }
        if (res_2 < 0)
        {
            res_2 = res_2 * (-1);
        }

        if (res_1 == 0 && res_2 == 1)
        {
            return 1;
        }
        else if (res_1 == 1 && res_2 == 0)
        {
            return 1;
        }
        else if (res_1 == 1 && res_2 == 1)
        {

            return 1;
        }
        else {

            return  0;
        }

    case 'D':
        if (res_1 < 0)
        {
            res_1 = res_1 * (-1);
        }
        if (res_2 < 0)
        {
            res_2 = res_2 * (-1);
        }

        if (res_1 == res_2)
        {
            return 1;
        }
        else if (res_1 == 0 || res_2 == 0)
        {
            return 1;
        }
        else {

            return 0;
        }
        break;

    }
}



int posicion_usuario(almacenar* u ,int n,char*codigo_de_busqueda)
{
    int i, posicion = 0;
    for (i = 1; i < n && posicion == 0; i++)
    {
        if (strcmp(codigo_de_busqueda, u->codigo_busqueda_partida) == 0)
        {
            posicion = i;
        }
    }
    return posicion;
}

int escribir_fichero(almacenar*lista,int n)
{
    int i, j;
    FILE* fichero;
    errno_t err;
    err = fopen_s(&fichero, "Usuarios.txt", "w");
    if (err == 0) // Si el fichero se ha podido crear
    {
        fprintf(fichero, "%d\n", n); 
        fprintf(fichero, "%s\n", (lista)->codigo_busqueda_partida);

        fprintf(fichero, "Nombre Jugador\n");
        fprintf(fichero, "Movimiento            Tiempo empleado\n");

        fprintf(fichero, "%s\n", (lista)->nombre_jugador1);
        
        for (i = 0; i < n; i++)
        {
            fprintf(fichero, "%s", (lista + i)->jugadas_jugador1->ent_mov);    
            fprintf(fichero, "%c", (lista + i)->jugadas_jugador1->pieza);
            fprintf(fichero, "%s", (lista + i)->jugadas_jugador1->sal_mov);
            fprintf(fichero, "%d\t", (lista + i)->jugadas_jugador1->tiempo_duracion);
            fprintf(fichero, "\n");
        }

        fprintf(fichero, "Nombre Jugador\n");
        fprintf(fichero, "Movimiento            Tiempo empleado\n");

        fprintf(fichero, "%s\n", (lista)->nombre_jugador2);

        for (j = 0; j < n; j++)
        {
            fprintf(fichero, "%s",(lista + j)->jugadas_jugador2->ent_mov);
            fprintf(fichero, "%c", (lista + j)->jugadas_jugador2->pieza);
            fprintf(fichero, "%s", (lista + j)->jugadas_jugador2->sal_mov);
            fprintf(fichero, "%d\t", (lista + j)->jugadas_jugador2->tiempo_duracion);

            fprintf(fichero, "\n");
        }


        fclose(fichero);
    }
    else
        printf("Se ha producido un problema a la hora de grabar el fichero de usuarios\n");
    return err;
}



















