#include <stdio.h>
#include <stdlib.h>

struct mov {

	char lectura_jugada;
	struct mov* almacenar;
};

typedef struct mov datos;

void crear_fichero(datos* p);

int main()
{
	datos* p;
	mov m;
	int num = 12;
	int i, j = 0;
	int cambio = 0;
	p = (datos*)malloc(num*sizeof(datos)); //Dota inicialamente de espacio para dos movimentos como minimo
	
	while (cambio!= 1)
	{
		if (p == NULL)
		{
			printf("Lectura insuficiente se creara espacio para un movimiento mas\n");
			p = (datos*)realloc(p, sizeof(datos)*6);

		}

		else {

			printf("Introduce la pieza usada en el movimiento(En el caso de querer salir pulse w)\n");
			scanf_s("%c", &(p+j)->lectura_jugada);

			if ((p + j)->lectura_jugada == 'w')
			{
				cambio = 1;

			}
			printf("Introduce un movimiento (ES importante introducir primero la casilla de partida de la pieza y despues la nueva posición Ej. b4 d4\n");

			for (i = 1; i < 7; i++)
			{
				scanf_s("%c", &(p + j + i)->lectura_jugada);
			}
			j = j + 7;

		}


	}
	crear_fichero(p);

}


void crear_fichero(datos* p)
{
	FILE* f1;
	errno_t e1;
	int i;
	e1 = fopen_s(&f1, "resultados.txt", "w");
	if (f1 == NULL)
	{
		printf("No ha sido posible abrir el fichero\n");

	}
	else {

		for (i = 0; (p + i)->lectura_jugada!='\0'; i++)
		{

			fprintf(f1, "%c", (p + i)->lectura_jugada);

		}


		fclose(f1);





	}

}


















