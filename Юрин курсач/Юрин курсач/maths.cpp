#include "help.h"


/*ф-я вычисляет знакоместо для коректного вывода цифр
принимаем число k - кол-во любого ресурса*/
int znakomesto(int k)
{
	int a = 0;
	do
	{
		k = k / 10;
		a++;
	} while (k > 0);
	return a;
}