#include "help.h"


/*�-� ��������� ���������� ��� ���������� ������ ����
��������� ����� k - ���-�� ������ �������*/
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