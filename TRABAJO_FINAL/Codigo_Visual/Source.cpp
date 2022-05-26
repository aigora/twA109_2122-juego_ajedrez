#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include"SerialClass/SerialClass.h"
#define Q 8
#define MAX_ALM 30000


void gotoxy(int x, int y)
{

    HANDLE hcon;
    hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon, dwPos);



}


struct piezas_negras {


    char Peon;
    char Rey;
    char Dama;
    char Alfil;
    char Torre;
    char Caballo;

};

struct piezas_blancas {


    char Peon;
    char Rey;
    char Dama;
    char Alfil;
    char Torre;
    char Caballo;

};

typedef struct {

    char datos_partidas;//Estructura necesaria para la gestion de memoria

}almacenar;



void imprimir_tablero(char tab[8][8]);
char generar_movimiento(char tab[8][8], char ent_mov[3], char sal_mov[3]);
void crear_fichero(almacenar* alm, int cont);

int menu_ppal(void)
{
    int opcion;
    gotoxy(45, 5); printf("Menu principal:\n"); 
    gotoxy(45, 6); printf("-Si desea jugar una partida pulse 1\n");
    gotoxy(45, 7); printf("-Si desea salir del programa pulse 2\n");
    scanf_s("%d", &opcion);

    return opcion;
}

int main()
{

    
    char jugador1[] = "BLANCO";
    char jugador2[] = "NEGRO";
    char fin_programa[] = "FIN";
    char puerto[6] = "COM4";

    Serial* Arduino = new Serial(puerto); //Inicializamos el puntero de Arduino haciendo que apunte al puerto
    

    int menu;
    struct piezas_negras neg = { 'P','R','D','A','T','C' };//Se introducen las piezas negras
    struct piezas_blancas bln = { 'p','r','d','a','t','c' };//Se introducen las piezas blancas
    struct piezas_negras* N;
    struct piezas_blancas* B;
    almacenar* alm;
    N = &neg;
    B = &bln;
    char otro_movimiento = 'S';
    char space = ' ';
    int i, j, k;
    int cont = 0, contador_malloc = 0;

    char tablero[Q][Q];
    char ent_mov[3];
    char sal_mov[3];

    int camb_ent1, camb_ent2;
    int camb_sal1, camb_sal2;

    alm = (almacenar*)malloc(MAX_ALM* sizeof(almacenar));//Solo permitirá 5000 movimientos entre los dos jugadores


    //Inicializamos el tablero

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

    menu = menu_ppal();


    switch (menu)
    {


    case 1:

        imprimir_tablero(tablero);
        gotoxy(35, 8); printf("_________________________________________________\n");
        gotoxy(35, 9); printf("|Recuerde que es importante que conozca las     |\n");
        gotoxy(35, 10);printf("|normas de movimiento de cada pieza             |\n");
        gotoxy(35, 11); printf("|porque este programa no le pondra restricciones|\n");
        gotoxy(35, 12); printf("|si realiza un movimento incorrecto             |\n");
        gotoxy(35, 13); printf("_________________________________________________\n");


        gotoxy(2, 25); printf("\n");
        printf("Empiezan a jugar las blancas(piezas conformadas por las letras en minusculas)\n");

        for (k = 0; otro_movimiento != 'N'; k++)
        {
            if (alm == NULL)
            {
                //Para cuando sobrepasemos los 5000 movientos nos informe al respecto
                printf("Se ha sobrepasado el numero de datos que permite la capacidad del portatil\n");

            }

            getchar();
            printf("Introduzca la casilla de la que parte la pieza\n");
            gets_s(ent_mov);

            getchar();
            printf("Introduzca la casilla donde desea colocar la pieza\n");
            gets_s(sal_mov);

            if (Arduino->IsConnected())
            {
                printf("Arduino conectado\n");
                if (contador_malloc % 2 == 0)
                {
                    //Intercambio de datos entre Arduino y visual cuando juega el blanco
                    printf("Enviando:%s\n", jugador1);
                    Arduino->WriteData(jugador1, sizeof(jugador1));
                    Sleep(500);
                    Arduino->ReadData(jugador1, sizeof(jugador1));
                    printf("Recibiendo:%s\n", jugador1);
                    Sleep(500);

                    printf("Cambio de turno:juegan negras\n");
                }

                else {
                    //Intercambio de datos entre Arduino y visual cuando juega el negro
                    Arduino->WriteData(jugador2, sizeof(jugador2));
                    printf("Enviando:%s\n", jugador2);
                    Sleep(500);
                    Arduino->ReadData(jugador2, sizeof(jugador2));
                    printf("Recibiendo:%s\n", jugador2);
                    Sleep(500);

                    printf("Cambio de turno:juegan las blancas\n");
                }
            }

            // Gestion de datos

            (alm + cont)->datos_partidas = ent_mov[0];

            (alm + cont + 1)->datos_partidas = ent_mov[1];

            (alm + cont + 3)->datos_partidas = sal_mov[0];

            (alm + cont + 4)->datos_partidas = sal_mov[1];

            (alm + cont + 5)->datos_partidas = space;

            (alm + cont + 2)->datos_partidas = generar_movimiento(tablero, ent_mov, sal_mov);

            imprimir_tablero(tablero);

            printf("Desea realizar otro movimiento:S/N?\n");
            scanf_s("%c", &otro_movimiento);



            cont = cont + 6;

            contador_malloc++;
        }

        if (Arduino->IsConnected())
        {

            printf("Arduino esta conectado\n");

            Arduino->WriteData(fin_programa, sizeof(fin_programa));
            printf("Enviando:%s\n", fin_programa);
            Sleep(500);
            Arduino->ReadData(fin_programa, sizeof(fin_programa));
            printf("Recibiendo:%s\n",fin_programa);
            
           
            Sleep(500);

            printf("El programa ha terminado\n");


        }
        

        contador_malloc = 6 * contador_malloc;

        crear_fichero(alm, contador_malloc);

        break;

    case 2:
        gotoxy(2, 15); printf("Saliendo del programa\n");
        break;
    default: printf("No es posible esa instruccion al menu"); break;
    }
    return 0;


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

void crear_fichero(almacenar* alm, int cont)
{
    FILE* f1;
    errno_t e1;
    int i;
    e1 = fopen_s(&f1, "analisis_partida.txt", "w");
    if (f1 == NULL)
    {
        printf("No ha sido posible abrir el fichero\n");

    }
    else {

        fprintf(f1, "MOV_BLANCAS   MOV_NEGRAS\n");//Dividimos el resultado en dos columnas una para el movimiento de las negras y otra para el de las blancas

        for (i = 0; i < cont; i++)
        {
            if (i % 6 == 0 && i % 12 != 0)
            {
                fprintf(f1,"\t\t");

            }

            if (i % 12 == 0 && i != 0)
            {
                fprintf(f1, "\n");
            }


            fprintf(f1, "%c", (alm + i)->datos_partidas);


        }

        fclose(f1);
        free(alm);
    }

}
