#define _CRT_SECURE_NO_WARNINGS 
#include "SDL.h" 
//#include "SDL_ttf.h"

#include <math.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <locale.h> 
#include <time.h>

//хранит кол-во ресурсов
struct Resyrs
{
	int gold; int eat; int metl;
};
//строения на карте
struct Stroenie
{
	int market; int kazarm; int krepoct; int mine;
};
//воины на карте
typedef struct Voin
{
	int bronz; int serebr; int gold;
}voin;
//отрисовка стартового меню
void kartinka_start_menu(SDL_Window* window, SDL_Renderer* &renderer)
{
	SDL_Surface *myImage = SDL_LoadBMP("textures/FON1.bmp");
	SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 0, 0, 0));
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
	SDL_Rect rect = { 0,0,1280,720 };
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_FreeSurface(myImage);
	SDL_DestroyTexture(texture);
}
//отрисовка игрового меню
void kartinka_start(SDL_Window* window, SDL_Renderer* &renderer)
{
	SDL_Surface *myImage = SDL_LoadBMP("textures/Menu_game.bmp");
	SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 2, 2, 2));
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
	SDL_Rect rect = { 0,0,1280,720 };
	SDL_RenderCopy(renderer, texture, NULL, &rect);
	SDL_FreeSurface(myImage);
	SDL_DestroyTexture(texture);
}
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
//отрисовка цифр на игровом меню
//k-число для показа, i-кодовый номер
void pokaz_4icel(SDL_Window*window, SDL_Renderer* &renderer, int k, int i)
{
	int a;
	int b;
	b = znakomesto(k);
	a = 6 - b;
	char *text = new char[10];
	_itoa(k, text, 10);
	TTF_Init();
	TTF_Font* my_font = TTF_OpenFont("Text.ttf", 100);
	SDL_Color fore_color = { 0,0,0 };
	SDL_Color back_color = { 255,255,255 };
	SDL_Rect *rect = new SDL_Rect[10];
	//золото
	rect[1] = { 1129 + 23 * a,79,23 * b,40 };
	//еда
	rect[2] = { 1129 + 23 * a,199,23 * b,40 };
	//металл
	rect[3] = { 1129 + 23 * a,317,23 * b,40 };
	//1-воины
	rect[4] = { 171 + 23 * a,641,23 * b,58 };
	//2-воины
	rect[5] = { 561 + 23 * a,641,23 * b,58 };
	//3-воины
	rect[6] = { 951 + 23 * a,641,23 * b,58 };
	//отправить всего
	rect[7] = { 300 + 20 * a,645,20 * b,45 };
	//отправить выбор
	rect[8] = { 172 + 20 * a,645,20 * b,45 };
	SDL_Surface* textSurface = NULL;
	textSurface = TTF_RenderText_Shaded(my_font, text, fore_color, back_color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_RenderCopy(renderer, texture, NULL, &rect[i]);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(my_font);
	TTF_Quit();
}
//ф-я создает ромб из координаты x и y размером t
void romb(SDL_Renderer*renderer, SDL_Rect rect, int t, int k)
{
	int r, g, b;
	switch (k)
	{
	case(1): {r = 255; g = 0; b = 0; break; }//красный
	case(2): {r = 0; g = 255; b = 0; break; }//зеленый
	case(3): {r = 0; g = 0; b = 255; break; }//синий
	case(4): {r = 255; g = 128; b = 0; break; }//оранжевый
	case(5): {r = 255; g = 0; b = 170; break; }//розовый
	case(6): {r = 170; g = 0; b = 255; break; }//фиолетовый
	case(7): {r = 255; g = 128; b = 255; break; }//бледно розовый
	case(8): {r = 128; g = 255; b = 255; break; }//бирюзовый
	case(9): {r = 255; g = 255; b = 128; break; }//желтый
	case(10): {r = 0; g = 0; b = 0; break; }//1
	default: {r = 128; g = 128; b = 128; break; }
	}
	SDL_SetRenderDrawColor(renderer, r, g, b, 0);
	for (int i = 0; i < t; i++)
	{
		SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.w, rect.h);
		rect.x++;
		rect.y++;
		rect = { rect.x,rect.y,rect.x + t,rect.y - t };
	}
}
//создает игровое поле 
//t-постоянное координатное число
void map_ren(SDL_Window* window, SDL_Renderer* renderer, int t, int colr[])
{
	FILE*tf;
	int map[1000][2];
	int kol;
	fopen_s(&tf, "map.txt", "r");
	fscanf_s(tf, "%d", &kol);
	for (int j = 1; j <= kol; j++)
	{
		fscanf_s(tf, "%d %d ", &map[j][0], &map[j][1]);
	}
	fclose(tf);
	for (int j = 1; j <= kol; j++)
	{
		SDL_Rect rect = { map[j][0] , map[j][1] , map[j][0] + t , map[j][1] - t };
		romb(renderer, rect, t, colr[j]);
	}
}
//обводит выбранный часть карты 
void obvodka(SDL_Renderer* renderer, int x, int y, int t)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderDrawLine(renderer, x, y, x + t, y - t);
	SDL_RenderDrawLine(renderer, x, y, x + t, y + t);
	SDL_RenderDrawLine(renderer, x + t, y - t, x + t + t, y);
	SDL_RenderDrawLine(renderer, x + t, y + t, x + t + t, y);
}
//индикатор состояния, k-номер игрока r-состояние
void ind_sost(SDL_Renderer* renderer, int k, int r)
{
	//координаты точек
	SDL_Rect *cord_color = new SDL_Rect[10];
	cord_color[1] = { 1169,385,8,8 };
	cord_color[2] = { 1169,414,8,8 };
	cord_color[3] = { 1169,442,8,8 };
	cord_color[7] = { 1169,471,8,8 };
	cord_color[9] = { 1169,498,8, 8 };
	cord_color[4] = { 1237,385,8,8 };
	cord_color[5] = { 1237,412,8,8 };
	cord_color[6] = { 1237,441,8,8 };
	cord_color[8] = { 1237,470,8,8 };
	if (r == 0)
	{
		SDL_Surface *myImage = SDL_LoadBMP("ind_st0.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 0, 0, 0));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord_color[k]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	if (r == 1)
	{
		SDL_Surface *myImage = SDL_LoadBMP("ind_st1.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 0, 0, 0));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord_color[k]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	if (r == 2)
	{
		SDL_Surface *myImage = SDL_LoadBMP("ind_st2.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 0, 0, 0));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord_color[k]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
}
//выводит меню построек
void Menu_obchenia(SDL_Renderer* renderer, int ap, int market, int kazarm, int krepost, int mine)
{
	SDL_Rect *cord = new SDL_Rect[20];
	cord[1] = { 20,580,1080,120 };
	cord[2] = { 116,570,187,140 };
	cord[3] = { 343,570,187,140 };
	cord[4] = { 570,570,187,140 };
	cord[5] = { 797,570,187,140 };
	cord[6] = { 697,570,187,140 };
	cord[7] = { 470,570,187,140 };
	cord[8] = { 243,570,187,140 };
	cord[9] = { 120,580,50,60 };
	cord[10] = { 120,640,50,60 };
	cord[11] = { 510,580,50,60 };
	cord[12] = { 510,640,50,60 };
	cord[13] = { 900,580,50,60 };
	cord[14] = { 900,640,50,60 };
	cord[15] = { 110,580,1080,120 };
	if (ap == 1)
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Menu_v_z.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[1]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	if (ap == 2)
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Menu_x_n.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[1]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	if (ap == 3)
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Menu_s_y.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[1]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	if (ap == 4)
	{
		if (market == 0)
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/st_mark1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[2]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
		}
		else
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/st_mark0.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[2]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
		}
		if (krepost == 0)
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/st_krep1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[4]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
		}
		else
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/st_krep0.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[4]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
		}
		if (mine == 0)
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/st_mine1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[5]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
		}
		else
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/st_mine0.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[5]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
		}
		if (kazarm == 0)
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/st_kaz1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[3]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
		}
		else
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/st_kaz0.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[3]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
		}
	}
	if (ap == 5)
	{
		if (krepost == 3 || krepost == 0)
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/st_krep0.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[7]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
		}
		else
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/st_krep1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[7]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
		}
		if (mine == 3 || mine == 0)
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/st_mine0.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[8]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
		}
		else
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/st_mine1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[8]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
		}
		if (kazarm == 3 || kazarm == 0)
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/st_kaz0.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[6]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
		}
		else
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/st_kaz1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[6]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
		}
	}
	if (ap == 6)
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Menu_nv.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 0, 255, 255));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[1]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
		int p = 0;
		if (kazarm == 0)
		{
			p = 1;
			SDL_Surface *myImage = SDL_LoadBMP("textures/Str_u.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[9]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			SDL_Surface *myImage1 = SDL_LoadBMP("textures/Str_d.bmp");
			SDL_SetColorKey(myImage1, SDL_TRUE, SDL_MapRGB(myImage1->format, 255, 255, 255));
			SDL_Texture *texture1 = SDL_CreateTextureFromSurface(renderer, myImage1);
			SDL_RenderCopy(renderer, texture1, NULL, &cord[10]);
			SDL_FreeSurface(myImage1);
			SDL_DestroyTexture(texture1);
		}
		if (kazarm == 1 || p == 1)
		{
			p = 1;
			SDL_Surface *myImage = SDL_LoadBMP("textures/Str_u.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[11]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			SDL_Surface *myImage1 = SDL_LoadBMP("textures/Str_d.bmp");
			SDL_SetColorKey(myImage1, SDL_TRUE, SDL_MapRGB(myImage1->format, 255, 255, 255));
			SDL_Texture *texture1 = SDL_CreateTextureFromSurface(renderer, myImage1);
			SDL_RenderCopy(renderer, texture1, NULL, &cord[12]);
			SDL_FreeSurface(myImage1);
			SDL_DestroyTexture(texture1);
		}
		if (kazarm == 2 || p == 1)
		{
			p = 1;
			SDL_Surface *myImage = SDL_LoadBMP("textures/Str_u.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[13]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			SDL_Surface *myImage1 = SDL_LoadBMP("textures/Str_d.bmp");
			SDL_SetColorKey(myImage1, SDL_TRUE, SDL_MapRGB(myImage1->format, 255, 255, 255));
			SDL_Texture *texture1 = SDL_CreateTextureFromSurface(renderer, myImage1);
			SDL_RenderCopy(renderer, texture1, NULL, &cord[14]);
			SDL_FreeSurface(myImage1);
			SDL_DestroyTexture(texture1);
		}
	}
	if (ap == 7)
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Menu_x1.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[15]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	if (ap == 8)
	{
		SDL_Surface *myImage1 = SDL_LoadBMP("textures/Str_d.bmp");
		SDL_SetColorKey(myImage1, SDL_TRUE, SDL_MapRGB(myImage1->format, 255, 255, 255));
		SDL_Texture *texture1 = SDL_CreateTextureFromSurface(renderer, myImage1);
		SDL_RenderCopy(renderer, texture1, NULL, &cord[12]);
		SDL_FreeSurface(myImage1);
		SDL_DestroyTexture(texture1);
	}
	if (ap == 9)
	{
		SDL_Surface *myImage1 = SDL_LoadBMP("textures/Str_d.bmp");
		SDL_SetColorKey(myImage1, SDL_TRUE, SDL_MapRGB(myImage1->format, 255, 255, 255));
		SDL_Texture *texture1 = SDL_CreateTextureFromSurface(renderer, myImage1);
		SDL_RenderCopy(renderer, texture1, NULL, &cord[14]);
		SDL_FreeSurface(myImage1);
		SDL_DestroyTexture(texture1);
	}
	if (ap == 10)
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Str_u.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[9]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	if (ap == 11)
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Str_u.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[11]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	if (ap == 12)
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Str_u.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[13]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	if (ap == 13)
	{
		SDL_Surface *myImage1 = SDL_LoadBMP("textures/Str_d.bmp");
		SDL_SetColorKey(myImage1, SDL_TRUE, SDL_MapRGB(myImage1->format, 255, 255, 255));
		SDL_Texture *texture1 = SDL_CreateTextureFromSurface(renderer, myImage1);
		SDL_RenderCopy(renderer, texture1, NULL, &cord[10]);
		SDL_FreeSurface(myImage1);
		SDL_DestroyTexture(texture1);
	}
	if (ap == 14)
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Menu_v_m.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[1]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	if (ap == 15)
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Menu_xod1.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 128, 128, 128));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[1]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	if (ap == 16)
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Menu_xod2.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 128, 128, 128));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[1]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	if (ap == 17)
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Menu_xod3.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 128, 128, 128));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[1]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
}
//рисует постройки
void risovka_3danij(SDL_Renderer* renderer, int market, int kazarm, int krepost, int mine)
{
	SDL_Rect cord = { 959,0,161,560 };
	SDL_Surface *myImage = SDL_LoadBMP("textures/Otdel_yr.bmp");
	SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
	SDL_RenderCopy(renderer, texture, NULL, &cord);
	SDL_FreeSurface(myImage);
	SDL_DestroyTexture(texture);
	SDL_Rect cordmar = { 960,1,159,141 };
	SDL_Rect cordkazr = { 960,143,159,138 };
	SDL_Rect cordkrep = { 960,284,159,138 };
	SDL_Rect cordmine = { 960,421,159,138 };
	switch (market)
	{
	case(1): {SDL_Surface *myImage1 = SDL_LoadBMP("textures/market.bmp");
		SDL_SetColorKey(myImage1, SDL_TRUE, SDL_MapRGB(myImage1->format, 255, 255, 255));
		SDL_Texture *texture1 = SDL_CreateTextureFromSurface(renderer, myImage1);
		SDL_RenderCopy(renderer, texture1, NULL, &cordmar);
		SDL_FreeSurface(myImage1);
		SDL_DestroyTexture(texture1); break; }
	}
	switch (kazarm)
	{
	case(1): {SDL_Surface *myImage2 = SDL_LoadBMP("textures/kazarma_1.bmp");
		SDL_SetColorKey(myImage2, SDL_TRUE, SDL_MapRGB(myImage2->format, 255, 255, 255));
		SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, myImage2);
		SDL_RenderCopy(renderer, texture2, NULL, &cordkazr);
		SDL_FreeSurface(myImage2);
		SDL_DestroyTexture(texture2); break; }
	case(2): {SDL_Surface *myImage2 = SDL_LoadBMP("textures/kazarma_2.bmp");
		SDL_SetColorKey(myImage2, SDL_TRUE, SDL_MapRGB(myImage2->format, 255, 255, 255));
		SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, myImage2);
		SDL_RenderCopy(renderer, texture2, NULL, &cordkazr);
		SDL_FreeSurface(myImage2);
		SDL_DestroyTexture(texture2); break; }
	case(3): {SDL_Surface *myImage2 = SDL_LoadBMP("textures/kazarma_3.bmp");
		SDL_SetColorKey(myImage2, SDL_TRUE, SDL_MapRGB(myImage2->format, 255, 255, 255));
		SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, myImage2);
		SDL_RenderCopy(renderer, texture2, NULL, &cordkazr);
		SDL_FreeSurface(myImage2);
		SDL_DestroyTexture(texture2); break; }
	}
	switch (krepost)
	{
	case(1): {SDL_Surface *myImage3 = SDL_LoadBMP("textures/krepost_1.bmp");
		SDL_SetColorKey(myImage3, SDL_TRUE, SDL_MapRGB(myImage3->format, 255, 255, 255));
		SDL_Texture *texture3 = SDL_CreateTextureFromSurface(renderer, myImage3);
		SDL_RenderCopy(renderer, texture3, NULL, &cordkrep);
		SDL_FreeSurface(myImage3);
		SDL_DestroyTexture(texture3); break; }
	case(2): {SDL_Surface *myImage3 = SDL_LoadBMP("textures/krepost_2.bmp");
		SDL_SetColorKey(myImage3, SDL_TRUE, SDL_MapRGB(myImage3->format, 255, 255, 255));
		SDL_Texture *texture3 = SDL_CreateTextureFromSurface(renderer, myImage3);
		SDL_RenderCopy(renderer, texture3, NULL, &cordkrep);
		SDL_FreeSurface(myImage3);
		SDL_DestroyTexture(texture3); break; }
	case(3): {SDL_Surface *myImage3 = SDL_LoadBMP("textures/krepost_3.bmp");
		SDL_SetColorKey(myImage3, SDL_TRUE, SDL_MapRGB(myImage3->format, 255, 255, 255));
		SDL_Texture *texture3 = SDL_CreateTextureFromSurface(renderer, myImage3);
		SDL_RenderCopy(renderer, texture3, NULL, &cordkrep);
		SDL_FreeSurface(myImage3);
		SDL_DestroyTexture(texture3); break; }
	}
	switch (mine)
	{
	case(1): {SDL_Surface *myImage4 = SDL_LoadBMP("textures/mine-1.bmp");
		SDL_SetColorKey(myImage4, SDL_TRUE, SDL_MapRGB(myImage4->format, 255, 255, 255));
		SDL_Texture *texture4 = SDL_CreateTextureFromSurface(renderer, myImage4);
		SDL_RenderCopy(renderer, texture4, NULL, &cordmine);
		SDL_FreeSurface(myImage4);
		SDL_DestroyTexture(texture4); break; }
	case(2): {SDL_Surface *myImage4 = SDL_LoadBMP("textures/mine-2.bmp");
		SDL_SetColorKey(myImage4, SDL_TRUE, SDL_MapRGB(myImage4->format, 255, 255, 255));
		SDL_Texture *texture4 = SDL_CreateTextureFromSurface(renderer, myImage4);
		SDL_RenderCopy(renderer, texture4, NULL, &cordmine);
		SDL_FreeSurface(myImage4);
		SDL_DestroyTexture(texture4); break; }
	case(3): {SDL_Surface *myImage4 = SDL_LoadBMP("textures/mine-3.bmp");
		SDL_SetColorKey(myImage4, SDL_TRUE, SDL_MapRGB(myImage4->format, 255, 255, 255));
		SDL_Texture *texture4 = SDL_CreateTextureFromSurface(renderer, myImage4);
		SDL_RenderCopy(renderer, texture4, NULL, &cordmine);
		SDL_FreeSurface(myImage4);
		SDL_DestroyTexture(texture4); break; }
	}
}
//x-координата y-координата a-какой именно игрок p-сколько уже есть воинов j-вид воина
void risovka_voin_wind(SDL_Renderer* renderer, int x, int y, int p, int a, int j)
{
	SDL_Rect *cord = new SDL_Rect[3];
	cord[0] = { x + 25,y - 35,50,50 };
	cord[1] = { x + 25,y - 25,50,50 };
	cord[2] = { x + 25,y - 15,50,50 };
	switch (a)
	{
	case(1):
	{
		switch (j)
		{
		case(1):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_1_1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(2):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_1_2.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(3):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_1_3.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		}
		break;
	}
	case(2):
	{
		switch (j)
		{
		case(1):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_2_1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(2):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_2_2.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(3):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_2_3.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		}
		break;
	}
	case(3):
	{
		switch (j)
		{
		case(1):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_3_1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(2):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_3_2.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(3):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_3_3.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		}
		break;
	}
	case(4):
	{
		switch (j)
		{
		case(1):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_4_1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(2):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_4_2.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(3):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_4_3.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		}
		break;
	}
	case(5):
	{
		switch (j)
		{
		case(1):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_5_1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(2):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_5_2.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(3):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_5_3.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		}
		break;
	}
	case(6):
	{
		switch (j)
		{
		case(1):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_6_1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(2):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_6_2.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(3):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_6_3.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		}
		break;
	}
	case(7):
	{
		switch (j)
		{
		case(1):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_7_1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(2):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_7_2.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(3):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_7_3.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		}
		break;
	}
	case(8):
	{
		switch (j)
		{
		case(1):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_8_1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(2):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_8_2.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(3):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_8_3.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		}
		break;
	}
	case(9):
	{
		switch (j)
		{
		case(1):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_9_1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(2):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_9_2.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(3):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_9_3.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		}
		break;
	}
	case(10):
	{
		switch (j)
		{
		case(1):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_0_1.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(2):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_0_2.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(3):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/mv_0_3.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[p]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		}
		break;
	}
	}
}
//x-коорд y-коорд a-положение стрелки c-цвет стрелки(1=g,0=r) t-постоянная
void strelka(SDL_Renderer* renderer, int x, int y, int a, int c, int t)
{
	SDL_Rect *cord = new SDL_Rect[4];
	cord[1] = { x,y - t,50,50 };
	cord[2] = { x + t,y - t,50,50 };
	cord[3] = { x + t,y,50,50 };
	cord[4] = { x,y,50,50 };
	if (c == 0)
	{
		switch (a)
		{
		case(1):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/str_r11.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[a]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(2):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/str_r21.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[a]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(3):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/str_r22.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[a]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(4):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/str_r12.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[a]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		}
	}
	if (c == 1)
	{
		switch (a)
		{
		case(1):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/str_g11.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[a]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(2):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/str_g21.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[a]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(3):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/str_g22.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[a]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		case(4):
		{
			SDL_Surface *myImage = SDL_LoadBMP("textures/str_g12.bmp");
			SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 255, 255, 255));
			SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
			SDL_RenderCopy(renderer, texture, NULL, &cord[a]);
			SDL_FreeSurface(myImage);
			SDL_DestroyTexture(texture);
			break;
		}
		}
	}
}
void xod_voin(SDL_Renderer* renderer, int a)
{
	SDL_Rect *cord = new SDL_Rect[20];
	cord[16] = { 110,580,100,120 };
	cord[17] = { 500,580,100,120 };
	cord[18] = { 890,580,100,120 };
	switch (a)
	{
	case(1):
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Menu_x2.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 0, 255, 255));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[16]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	case(2):
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Menu_x2.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 0, 255, 255));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[17]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	case(3):
	{
		SDL_Surface *myImage = SDL_LoadBMP("textures/Menu_x2.bmp");
		SDL_SetColorKey(myImage, SDL_TRUE, SDL_MapRGB(myImage->format, 0, 255, 255));
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, myImage);
		SDL_RenderCopy(renderer, texture, NULL, &cord[18]);
		SDL_FreeSurface(myImage);
		SDL_DestroyTexture(texture);
	}
	}
}
int main(int argc, char** argv)
{
	///////////////////////////////////цена зданий//////////////////////////////
	Resyrs price[4][4];
	////////////////цена постройки///////////////////
	///////////казарма
	price[0][0].gold = 30;	price[0][0].eat = 20;	price[0][0].metl = 10;
	//////////шахта
	price[0][1].gold = 10;	price[0][1].eat = 30;	price[0][1].metl = 10;
	//////////рынок
	price[0][3].gold = 10;	price[0][3].eat = 30;	price[0][3].metl = 40;
	////////////крепость
	price[0][2].gold = 50;	price[0][2].eat = 40;	price[0][2].metl = 50;
	///////////////цена 1-го улучшения///////////////
	/////////////казарма
	price[1][0].gold = 20;  price[1][0].eat = 10;   price[1][0].metl = 20;
	////////////шахта
	price[1][1].gold = 10;  price[1][1].eat = 30;   price[1][1].metl = 20;
	//////////////крепость
	price[1][2].gold = 30;  price[1][2].eat = 40;   price[1][2].metl = 30;
	/////////////////цена 2- улучшения///////////
	//////////////казарма
	price[2][0].gold = 40;  price[2][0].eat = 10;   price[2][0].metl = 30;
	//////////шахта
	price[2][1].gold = 10;  price[2][1].eat = 50;   price[2][1].metl = 10;
	//////крепость
	price[2][2].gold = 50;  price[2][2].eat = 50;   price[2][2].metl = 50;
	////////////////////////////////////////цена воинов//////////////////////////
	////////////1-го уровня//////////////
	price[3][1].gold = 1;  price[3][1].eat = 1;  price[3][1].metl = 1;
	////////////2-го уровня//////////////
	price[3][2].gold = 1;  price[3][2].eat = 1;  price[3][2].metl = 1;
	////////////3-го уровня//////////////
	price[3][3].gold = 1;  price[3][3].eat = 1;  price[3][3].metl = 1;
	/////////////////////////////////////////////////////////////////////////////
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow(u8"ИГРА", 7, 29, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 1);
	SDL_Event event;
	//постоянное коорд число
	int t = 50;
	//координаты карты////////////////////////////////////////
	int map[1000][2];
	int kol;
	FILE*tf;
	fopen_s(&tf, "map.txt", "r");
	fscanf_s(tf, "%d", &kol);
	for (int i = 1; i <= kol; i++)
		fscanf_s(tf, "%d %d ", &map[i][0], &map[i][1]);
	fclose(tf);
	//начало отрисовки игры////////////////////////////////////////////
	bool quit = false;
	while (!quit)
	{
		//отрисовка заглавного меню////////////////////////////
		kartinka_start_menu(window, renderer);
		SDL_RenderPresent(renderer);
		SDL_PollEvent(&event);
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			if (event.button.x >= 426 && event.button.x <= 858 && event.button.y >= 173 && event.button.y <= 281)
			{
				//количество воинов на карте
				voin Player_voin_map[101][11];
				for (int i = 1; i <= 100; i++)
					for (int j = 1; j <= 10; j++)
					{
						Player_voin_map[i][j].bronz = 0;
						Player_voin_map[i][j].serebr = 0;
						Player_voin_map[i][j].gold = 0;
					}
				int coinvoin[3];
				int close = 0;
				int ap = 0;
				int sost = 0;
				//состояние между игроками///////////////////////////////////////////////
				//1-е число номер игрока, 2-е число с каким именно игроком состояние
				//0-нейтральное
				//1-войны
				//2-мира
				int Player_sost[10][10];
				for (int i = 0; i < 10; i++)
				{
					for (int j = 0; j < 10; j++)
					{
						Player_sost[i][j] = 0;
					}
				}
				//массивы ресурсов при старте////////////////////////////////////////////////
				Resyrs Player_res[10];
				for (int i = 0; i < 10; i++)
				{
					Player_res[i].gold = 1000;
					Player_res[i].eat = 1000;
					Player_res[i].metl = 1000;
				}
				//массив построек////////////////////////////////////////////
				Stroenie Player_st[1000];
				for (int i = 1; i <= 100; i++)
				{
					Player_st[i].kazarm = 0;//3-ур
					Player_st[i].krepoct = 0;//3-ур
					Player_st[i].market = 0;//1-ур
					Player_st[i].mine = 0;//3-ур
				}
				//цвета на карте
				int colr[100];
				for (int i = 1; i <= kol; i++)
				{
					colr[i] = 0;
				}
				colr[1] = 1; colr[10] = 2; colr[20] = 3; colr[30] = 4; colr[26] = 5; colr[50] = 6;
				colr[60] = 7; colr[5] = 8; colr[14] = 9; colr[34] = 10;
				//игровой цикл/////////////////////////////
				while (!quit)
				{
					while (!SDL_PollEvent(&event))
					{
						SDL_RenderClear(renderer);
						SDL_PollEvent(&event);
						//отрисовка меню взаимодействий
						kartinka_start(window, renderer);
						//карта игры
						map_ren(window, renderer, t, colr);
						//отрисовка ресурсов на экран
						pokaz_4icel(window, renderer, Player_res[0].gold, 1);
						pokaz_4icel(window, renderer, Player_res[0].eat, 2);
						pokaz_4icel(window, renderer, Player_res[0].metl, 3);
						//отрисовка состояние с др игроками
						for (int j = 1; j < 10; j++)
						{
							ind_sost(renderer, j, Player_sost[0][j]);
						}
						//взаимодействие с картой///////////////////////////
						if (event.type == SDL_MOUSEBUTTONDOWN)
						{
							if (event.button.button == SDL_BUTTON_LEFT)
							{
								if (event.button.x >= 0 && event.button.x <= 1120 && event.button.y >= 0 && event.button.y <= 560)
								{
									close = 0;
									ap = 0;
									sost = 0;
								}
								//выделение клетки карты при нажатии
								for (int j = 1; j <= kol; j++)
								{
									int zx, zy, zw, zh;
									int cx, cy, cw, ch;
									zx = map[j][0]; zy = map[j][1]; zw = map[j][0] + 2 * t; zh = map[j][1];
									cx = zx; cy = zy; cw = zw; ch = zh;
									for (int i = 0; i <= t; i++)
									{
										if ((event.button.x >= zx && event.button.x <= zw && event.button.y >= zy && event.button.y <= zh) || (event.button.x >= cx && event.button.x <= cw && event.button.y >= cy && event.button.y <= ch))
										{
											close = j;
											if (colr[j] == 10)
												ap = 1;
											if (colr[j] < 10 && colr[j] > 0)
											{
												sost = 5;
											}
										}
										zx++; zy--; zw--; zh--;
										cx++; cy++; cw--; ch++;
									}
								}
								//////////////////////////////////////
								if ((ap == 5) && (event.button.x >= 697 && event.button.x <= 884 && event.button.y >= 570 && event.button.y <= 710) && (Player_st[close].kazarm < 3) && (Player_st[close].kazarm > 0))//казарма up	
								{
									if (Player_st[close].kazarm == 2 && (Player_res[0].gold >= price[2][0].gold && Player_res[0].eat >= price[2][0].eat && Player_res[0].metl >= price[2][0].metl))
									{
										Player_res[0].gold = Player_res[0].gold - price[2][0].gold;
										Player_res[0].eat = Player_res[0].eat - price[2][0].eat;
										Player_res[0].metl = Player_res[0].metl - price[2][0].metl;
										Player_st[close].kazarm++;
									}
									if (Player_st[close].kazarm == 1 && (Player_res[0].gold >= price[1][0].gold && Player_res[0].eat >= price[1][0].eat && Player_res[0].metl >= price[1][0].metl))
									{
										Player_res[0].gold = Player_res[0].gold - price[1][0].gold;
										Player_res[0].eat = Player_res[0].eat - price[1][0].eat;
										Player_res[0].metl = Player_res[0].metl - price[1][0].metl;
										Player_st[close].kazarm++;
									}
								}
								if ((ap == 5) && (event.button.x >= 470 && event.button.x <= 657 && event.button.y >= 570 && event.button.y <= 710) && (Player_st[close].krepoct < 3) && (Player_st[close].krepoct > 0))//крепость up
								{
									if (Player_st[close].krepoct == 2 && (Player_res[0].gold >= price[2][2].gold && Player_res[0].eat >= price[2][2].eat && Player_res[0].metl >= price[2][2].metl))
									{
										Player_res[0].gold = Player_res[0].gold - price[2][2].gold;
										Player_res[0].eat = Player_res[0].eat - price[2][2].eat;
										Player_res[0].metl = Player_res[0].metl - price[2][2].metl;
										Player_st[close].krepoct++;
									}
									if (Player_st[close].krepoct == 1 && (Player_res[0].gold >= price[1][2].gold && Player_res[0].eat >= price[1][2].eat && Player_res[0].metl >= price[1][2].metl))
									{
										Player_res[0].gold = Player_res[0].gold - price[1][2].gold;
										Player_res[0].eat = Player_res[0].eat - price[1][2].eat;
										Player_res[0].metl = Player_res[0].metl - price[1][2].metl;
										Player_st[close].krepoct++;
									}
								}
								if ((ap == 5) && (event.button.x >= 243 && event.button.x <= 430 && event.button.y >= 570 && event.button.y <= 710) && (Player_st[close].mine < 3) && (Player_st[close].mine > 0))//шахта up
								{
									if (Player_st[close].mine == 2 && (Player_res[0].gold >= price[2][1].gold && Player_res[0].eat >= price[2][1].eat && Player_res[0].metl >= price[2][1].metl))
									{
										Player_res[0].gold = Player_res[0].gold - price[2][1].gold;
										Player_res[0].eat = Player_res[0].eat - price[2][1].eat;
										Player_res[0].metl = Player_res[0].metl - price[2][1].metl;
										Player_st[close].mine++;
									}
									if (Player_st[close].mine == 1 && (Player_res[0].gold >= price[1][1].gold && Player_res[0].eat >= price[1][1].eat && Player_res[0].metl >= price[1][1].metl))
									{
										Player_res[0].gold = Player_res[0].gold - price[1][1].gold;
										Player_res[0].eat = Player_res[0].eat - price[1][1].eat;
										Player_res[0].metl = Player_res[0].metl - price[1][1].metl;
										Player_st[close].mine++;
									}
								}
								if ((ap == 4) && (event.button.x >= 116 && event.button.x <= 303 && event.button.y >= 570 && event.button.y <= 710)
									&& (Player_res[0].gold >= price[0][3].gold && Player_res[0].eat >= price[0][3].eat && Player_res[0].metl >= price[0][3].metl))
								{
									Player_st[close].market = 1;//строим рынок
									Player_res[0].gold = Player_res[0].gold - price[0][3].gold;
									Player_res[0].eat = Player_res[0].eat - price[0][3].eat;
									Player_res[0].metl = Player_res[0].metl - price[0][3].metl;
								}
								if ((ap == 4) && (event.button.x >= 343 && event.button.x <= 530 && event.button.y >= 570 && event.button.y <= 710)
									&& (Player_res[0].gold >= price[0][0].gold && Player_res[0].eat >= price[0][0].eat && Player_res[0].metl >= price[0][0].metl))
								{
									Player_st[close].kazarm = 1;//строим казарма
									Player_res[0].gold = Player_res[0].gold - price[0][0].gold;
									Player_res[0].eat = Player_res[0].eat - price[0][0].eat;
									Player_res[0].metl = Player_res[0].metl - price[0][0].metl;
								}
								if ((ap == 4) && (event.button.x >= 570 && event.button.x <= 757 && event.button.y >= 570 && event.button.y <= 710)
									&& (Player_res[0].gold >= price[0][2].gold && Player_res[0].eat >= price[0][2].eat && Player_res[0].metl >= price[0][2].metl))
								{
									Player_st[close].krepoct = 1;//строим крепость
									Player_res[0].gold = Player_res[0].gold - price[0][2].gold;
									Player_res[0].eat = Player_res[0].eat - price[0][2].eat;
									Player_res[0].metl = Player_res[0].metl - price[0][2].metl;
								}
								if ((ap == 4) && (event.button.x >= 797 && event.button.x <= 984 && event.button.y >= 570 && event.button.y <= 710)
									&& (Player_res[0].gold >= price[0][1].gold && Player_res[0].eat >= price[0][1].eat && Player_res[0].metl >= price[0][1].metl))
								{
									Player_st[close].mine = 1;//строим шахта
									Player_res[0].gold = Player_res[0].gold - price[0][1].gold;
									Player_res[0].eat = Player_res[0].eat - price[0][1].eat;
									Player_res[0].metl = Player_res[0].metl - price[0][1].metl;
								}
								//////////////////////////////////////////////////////
								if (ap == 6)
								{
									if (Player_st[close].kazarm > 0 && event.button.x >= 120 && event.button.x <= 170 && event.button.y >= 580 && event.button.y < 640 &&
										(price[3][1].gold*coinvoin[1] + 1 <= Player_res[0].gold && price[3][1].eat*coinvoin[1] + 1 <= Player_res[0].eat && price[3][1].metl*coinvoin[1] + 1 <= Player_res[0].metl))//1g
									{
										coinvoin[1]++;
									}
									if (Player_st[close].kazarm > 0 && event.button.x >= 120 && event.button.x <= 170 && event.button.y >= 640 && event.button.y <= 700 && coinvoin[1] > 0)//1r
									{
										coinvoin[1]--;
									}
									if (event.button.x >= 171 && event.button.x <= 321 && event.button.y >= 580 && event.button.y < 640)//присвоение воинов на карту и отнимание цены
									{
										Player_voin_map[close][10].bronz = Player_voin_map[close][10].bronz + coinvoin[1];
										Player_res[0].gold = Player_res[0].gold - price[3][1].gold*coinvoin[1];
										Player_res[0].eat = Player_res[0].eat - price[3][1].eat*coinvoin[1];
										Player_res[0].metl = Player_res[0].metl - price[3][1].metl*coinvoin[1];
										coinvoin[1] = 0;
										coinvoin[2] = 0;
										coinvoin[3] = 0;
									}
									if (Player_st[close].kazarm > 1 && event.button.x >= 510 && event.button.x <= 560 && event.button.y >= 580 && event.button.y < 640 &&
										(price[3][2].gold*coinvoin[2] + 1 <= Player_res[0].gold && price[3][2].eat*coinvoin[2] + 1 <= Player_res[0].eat && price[3][2].metl*coinvoin[2] + 1 <= Player_res[0].metl))//2g
									{
										coinvoin[2]++;
									}
									if (Player_st[close].kazarm > 0 && event.button.x >= 510 && event.button.x <= 560 && event.button.y >= 640 && event.button.y <= 700 && coinvoin[2] > 0)//2r
									{
										coinvoin[2]--;
									}
									if (event.button.x >= 561 && event.button.x <= 711 && event.button.y >= 580 && event.button.y < 640)//присвоение воинов на карту и отнимание цены
									{
										Player_voin_map[close][10].serebr = Player_voin_map[close][10].serebr + coinvoin[2];
										Player_res[0].gold = Player_res[0].gold - price[3][2].gold*coinvoin[2];
										Player_res[0].eat = Player_res[0].eat - price[3][2].eat*coinvoin[2];
										Player_res[0].metl = Player_res[0].metl - price[3][2].metl*coinvoin[2];
										coinvoin[1] = 0;
										coinvoin[2] = 0;
										coinvoin[3] = 0;
									}
									if (Player_st[close].kazarm > 2 && event.button.x >= 900 && event.button.x <= 950 && event.button.y >= 580 && event.button.y < 640 &&
										(price[3][3].gold*coinvoin[3] + 1 <= Player_res[0].gold && price[3][3].eat*coinvoin[3] + 1 <= Player_res[0].eat && price[3][3].metl*coinvoin[3] + 1 <= Player_res[0].metl))//2g
									{
										coinvoin[3]++;
									}
									if (Player_st[close].kazarm > 0 && event.button.x >= 900 && event.button.x <= 950 && event.button.y >= 640 && event.button.y <= 700 && coinvoin[3] > 0)//2r
									{
										coinvoin[3]--;
									}
									if (event.button.x >= 951 && event.button.x <= 1101 && event.button.y >= 580 && event.button.y < 640)//присвоение воинов на карту и отнимание цены
									{
										Player_voin_map[close][10].gold = Player_voin_map[close][10].gold + coinvoin[3];
										Player_res[0].gold = Player_res[0].gold - price[3][3].gold*coinvoin[3];
										Player_res[0].eat = Player_res[0].eat - price[3][3].eat*coinvoin[3];
										Player_res[0].metl = Player_res[0].metl - price[3][3].metl*coinvoin[3];
										coinvoin[1] = 0;
										coinvoin[2] = 0;
										coinvoin[3] = 0;
									}
								}
								////////////////////////////////////////////////////
								if (ap == 7)
								{
									if (event.button.x >= 110 && event.button.x <= 210 && event.button.y >= 580 && event.button.y <= 700 && (Player_voin_map[close][10].bronz > 0))
									{
										ap = 15;
										coinvoin[1] = Player_voin_map[close][10].bronz;
									}
									if (event.button.x >= 500 && event.button.x <= 600 && event.button.y >= 580 && event.button.y <= 700 && (Player_voin_map[close][10].serebr > 0))
									{
										ap = 16;
										coinvoin[1] = Player_voin_map[close][10].serebr;
									}
									if (event.button.x >= 890 && event.button.x <= 990 && event.button.y >= 580 && event.button.y <= 700 && (Player_voin_map[close][10].gold > 0))
									{
										ap = 17;
										coinvoin[1] = Player_voin_map[close][10].gold;
									}
								}
								if (ap == 15 || ap == 16 || ap == 17)
								{
									if (event.button.x >= 120 && event.button.x <= 170 && event.button.y >= 640 && event.button.y <= 700 && coinvoin[1] > 0)
									{
										coinvoin[1]--;
									}
									if (event.button.x >= 170 && event.button.x <= 428 && event.button.y >= 580 && event.button.y <= 640)
									{
										//отправить//
									}
									if (ap == 15)
									{
										if (event.button.x >= 120 && event.button.x <= 170 && event.button.y >= 580 && event.button.y < 640 && coinvoin[1] < Player_voin_map[close][10].bronz)
										{
											coinvoin[1]++;
										}
										if (event.button.x >= 429 && event.button.x <= 499 && event.button.y >= 580 && event.button.y <= 700)
										{
											coinvoin[1] = coinvoin[1] / 2;
										}
										if (event.button.x >= 500 && event.button.x <= 570 && event.button.y >= 580 && event.button.y <= 700)
										{
											coinvoin[1] = coinvoin[1] / 3;
										}
										if (event.button.x >= 570 && event.button.x <= 641 && event.button.y >= 580 && event.button.y <= 700)
										{
											coinvoin[1] = coinvoin[1] / 4;
										}
										if (event.button.x >= 641 && event.button.x <= 712 && event.button.y >= 580 && event.button.y <= 700)
										{
											coinvoin[1] = (coinvoin[1] * 2) / 3;
										}
									}
									if (ap == 16)
									{
										if (event.button.x >= 120 && event.button.x <= 170 && event.button.y >= 580 && event.button.y < 640 && coinvoin[1] < Player_voin_map[close][10].serebr)
										{
											coinvoin[1]++;
										}
										if (event.button.x >= 429 && event.button.x <= 499 && event.button.y >= 580 && event.button.y <= 700)
										{
											coinvoin[1] = coinvoin[1] / 2;
										}
										if (event.button.x >= 500 && event.button.x <= 570 && event.button.y >= 580 && event.button.y <= 700)
										{
											coinvoin[1] = coinvoin[1] / 3;
										}
										if (event.button.x >= 570 && event.button.x <= 641 && event.button.y >= 580 && event.button.y <= 700)
										{
											coinvoin[1] = coinvoin[1] / 4;
										}
										if (event.button.x >= 641 && event.button.x <= 712 && event.button.y >= 580 && event.button.y <= 700)
										{
											coinvoin[1] = (coinvoin[1] * 2) / 3;
										}
									}
									if (ap == 17)
									{
										if (event.button.x >= 120 && event.button.x <= 170 && event.button.y >= 580 && event.button.y < 640 && coinvoin[1] < Player_voin_map[close][10].gold)
										{
											coinvoin[1]++;
										}
										if (event.button.x >= 429 && event.button.x <= 499 && event.button.y >= 580 && event.button.y <= 700)
										{
											coinvoin[1] = coinvoin[1] / 2;
										}
										if (event.button.x >= 500 && event.button.x <= 570 && event.button.y >= 580 && event.button.y <= 700)
										{
											coinvoin[1] = coinvoin[1] / 3;
										}
										if (event.button.x >= 570 && event.button.x <= 641 && event.button.y >= 580 && event.button.y <= 700)
										{
											coinvoin[1] = coinvoin[1] / 4;
										}
										if (event.button.x >= 641 && event.button.x <= 712 && event.button.y >= 580 && event.button.y <= 700)
										{
											coinvoin[1] = (coinvoin[1] * 2) / 3;
										}
									}
								}
								////////////////////////////////////////////////////
								if (sost != 0)
								{
									if (event.button.x >= 20 && event.button.x <= 540 && event.button.y >= 580 && event.button.y <= 700)//война
									{
										Player_sost[0][colr[close]] = 1;
									}
									if (event.button.x >= 580 && event.button.x <= 1100 && event.button.y >= 580 && event.button.y <= 700)//мир
									{
										Player_sost[0][colr[close]] = 2;
									}
								}
								if ((ap == 2) && (event.button.x >= 580 && event.button.x <= 1100 && event.button.y >= 580 && event.button.y <= 700))
								{
									ap = 6;//нанять
									coinvoin[1] = 0;
									coinvoin[2] = 0;
									coinvoin[3] = 0;
								}
								if ((ap == 2) && (event.button.x >= 20 && event.button.x <= 540 && event.button.y >= 580 && event.button.y <= 700))
								{
									ap = 7;//ходить
								}
								if ((ap == 3) && (event.button.x >= 20 && event.button.x <= 540 && event.button.y >= 580 && event.button.y <= 700))
								{
									ap = 4;//строить
								}
								if ((ap == 3) && (event.button.x >= 580 && event.button.x <= 1100 && event.button.y >= 580 && event.button.y <= 700))
								{
									ap = 5;//улучшить
								}
								if ((ap == 1) && (event.button.x >= 20 && event.button.x <= 540 && event.button.y >= 580 && event.button.y <= 700))
								{
									ap = 2;//воины
								}
								if ((ap == 1) && (event.button.x >= 580 && event.button.x <= 1100 && event.button.y >= 580 && event.button.y <= 700))
								{
									ap = 3;//здания
								}
							}
						}
						///////////////стрелки на карте при выборе хода//////////////////////
						if (ap == 7 && (Player_voin_map[close][10].bronz > 0 || Player_voin_map[close][10].serebr > 0 || Player_voin_map[close][10].gold > 0))
						{
							for (int i = 1; i <= kol; i++)
							{
								if (map[close][0] == map[i][0] - t && map[close][1] == map[i][1] + t)
								{
									if (colr[i] != 10 && colr[i] != 0)
									{
										if (Player_sost[0][colr[i]] == 1 || Player_sost[0][colr[i]] == 0)
											strelka(renderer, map[close][0], map[close][1], 2, 0, t);
										else
											strelka(renderer, map[close][0], map[close][1], 2, 1, t);
									}
									if (colr[i] == 0)
										strelka(renderer, map[close][0], map[close][1], 2, 0, t);
									if (colr[i] == 10)
										strelka(renderer, map[close][0], map[close][1], 2, 1, t);
								}
								if (map[close][0] == map[i][0] + t && map[close][1] == map[i][1] + t)
								{
									if (colr[i] != 10 && colr[i] != 0)
									{
										if (Player_sost[0][colr[i]] == 1 || Player_sost[0][colr[i]] == 0)
											strelka(renderer, map[close][0], map[close][1], 1, 0, t);
										else
											strelka(renderer, map[close][0], map[close][1], 1, 1, t);
									}
									if (colr[i] == 0)
										strelka(renderer, map[close][0], map[close][1], 1, 0, t);
									if (colr[i] == 10)
									{
										strelka(renderer, map[close][0], map[close][1], 1, 1, t);
									}
								}
								if (map[close][0] == map[i][0] - t && map[close][1] == map[i][1] - t)
								{
									if (colr[i] != 10 && colr[i] != 0)
									{
										if (Player_sost[0][colr[i]] == 1 || Player_sost[0][colr[i]] == 0)
										{
											strelka(renderer, map[close][0], map[close][1], 3, 0, t);
										}
										else
										{
											strelka(renderer, map[close][0], map[close][1], 3, 1, t);
										}
									}
									if (colr[i] == 0)
									{
										strelka(renderer, map[close][0], map[close][1], 3, 0, t);
									}
									if (colr[i] == 10)
									{
										strelka(renderer, map[close][0], map[close][1], 3, 1, t);
									}
								}
								if (map[close][0] == map[i][0] + t && map[close][1] == map[i][1] - t)
								{
									if (colr[i] != 10 && colr[i] != 0)
									{
										if (Player_sost[0][colr[i]] == 1 || Player_sost[0][colr[i]] == 0)
										{
											strelka(renderer, map[close][0], map[close][1], 4, 0, t);
										}
										else
										{
											strelka(renderer, map[close][0], map[close][1], 4, 1, t);
										}
									}
									if (colr[i] == 0)
									{
										strelka(renderer, map[close][0], map[close][1], 4, 0, t);
									}
									if (colr[i] == 10)
									{
										strelka(renderer, map[close][0], map[close][1], 4, 1, t);
									}
								}
							}
						}
						//отрисовка вонов на карте
						for (int i = 1; i <= kol; i++)
						{
							for (int j = 1; j <= 10; j++)
							{
								int nom = 0;
								if (Player_voin_map[i][j].bronz > 0)
								{
									risovka_voin_wind(renderer, map[i][0], map[i][1], nom, j, 1);
									nom++;
								}
								if (Player_voin_map[i][j].serebr > 0)
								{
									risovka_voin_wind(renderer, map[i][0], map[i][1], nom, j, 2);
									nom++;
								}
								if (Player_voin_map[i][j].gold > 0)
								{
									risovka_voin_wind(renderer, map[i][0], map[i][1], nom, j, 3);
								}
							}
						}
						if (close != 0)
						{
							if (sost == 5)
								Menu_obchenia(renderer, 14, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
							//выделение игровой области
							obvodka(renderer, map[close][0], map[close][1], t);
							//кнопки на панели меню////////////////
							Menu_obchenia(renderer, ap, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
							if (ap == 7)
							{
								if (Player_voin_map[close][10].bronz == 0)
									xod_voin(renderer, 1);
								if (Player_voin_map[close][10].serebr == 0)
									xod_voin(renderer, 2);
								if (Player_voin_map[close][10].gold == 0)
									xod_voin(renderer, 3);
							}
							if (ap != 0)
								//рисовка постройки///////////////
								risovka_3danij(renderer, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
							if (ap == 6)
							{
								pokaz_4icel(window, renderer, coinvoin[1], 4);
								pokaz_4icel(window, renderer, coinvoin[2], 5);
								pokaz_4icel(window, renderer, coinvoin[3], 6);
								if (coinvoin[1] == 0)
								{
									Menu_obchenia(renderer, 13, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
								}
								if (coinvoin[2] == 0)
								{
									Menu_obchenia(renderer, 8, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
								}
								if (coinvoin[3] == 0)
								{
									Menu_obchenia(renderer, 9, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
								}
								if (price[3][1].gold*coinvoin[1] + 1 > Player_res[0].gold || price[3][1].eat*coinvoin[1] + 1 > Player_res[0].eat || price[3][1].metl*coinvoin[1] + 1 > Player_res[0].metl)
								{
									Menu_obchenia(renderer, 10, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
								}
								if (price[3][2].gold*coinvoin[2] + 1 > Player_res[0].gold || price[3][2].eat*coinvoin[2] + 1 > Player_res[0].eat || price[3][2].metl*coinvoin[2] + 1 > Player_res[0].metl)
								{
									Menu_obchenia(renderer, 11, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
								}
								if (price[3][3].gold*coinvoin[3] + 1 > Player_res[0].gold || price[3][3].eat*coinvoin[3] + 1 > Player_res[0].eat || price[3][3].metl*coinvoin[3] + 1 > Player_res[0].metl)
								{
									Menu_obchenia(renderer, 12, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
								}
							}
							if (ap == 15 || ap == 16 || ap == 17)
							{
								pokaz_4icel(window, renderer, coinvoin[1], 8);
								if (ap == 15)
								{
									pokaz_4icel(window, renderer, Player_voin_map[close][10].bronz, 7);
									if (coinvoin[1] == 0)
									{
										Menu_obchenia(renderer, 13, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
									}
									if (coinvoin[1] == Player_voin_map[close][10].bronz)
									{
										Menu_obchenia(renderer, 10, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
									}
								}
								if (ap == 16)
								{
									pokaz_4icel(window, renderer, Player_voin_map[close][10].serebr, 7);
									if (coinvoin[1] == 0)
									{
										Menu_obchenia(renderer, 13, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
									}
									if (coinvoin[1] == Player_voin_map[close][10].serebr)
									{
										Menu_obchenia(renderer, 10, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
									}
								}
								if (ap == 17)
								{
									pokaz_4icel(window, renderer, Player_voin_map[close][10].gold, 7);
									if (coinvoin[1] == 0)
									{
										Menu_obchenia(renderer, 13, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
									}
									if (coinvoin[1] == Player_voin_map[close][10].gold)
									{
										Menu_obchenia(renderer, 10, Player_st[close].market, Player_st[close].kazarm, Player_st[close].krepoct, Player_st[close].mine);
									}
								}
							}
						}
						//закончить ход//////////////////////////////////////////////////
						if ((event.button.button == SDL_BUTTON_LEFT) && (event.button.x >= 1125 && event.button.x <= 1276 && event.button.y >= 566 && event.button.y <= 716))
						{
							close = 0;
							ap = 0;
							//цикл вычисления ботов
						}
						SDL_RenderPresent(renderer);
						if (event.type == SDL_MOUSEBUTTONDOWN)
						{
							SDL_Delay(200);
						}
						if (event.type == SDL_QUIT)
							quit = true;
					}
				}
			}
			//кнопка выхода////////////////////////////////////////////
			if (event.button.x >= 489 && event.button.x <= 798 && event.button.y >= 563 && event.button.y <= 675)
				quit = true;
		}
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}