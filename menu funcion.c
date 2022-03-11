#include <stdio.h>
#include <stdlib.h>

int menu_ppal(void)
{
	int opcion;
	printf("Menu principal:\n Si desea juga una partida pulse 1\n Si desea recrear una partida pulse 2\n Si desea salir del program pulse 3\n");
	scanf_s("%d", &opcion);
}

int main()
{
	int num;
	num = menu_ppal();


}








