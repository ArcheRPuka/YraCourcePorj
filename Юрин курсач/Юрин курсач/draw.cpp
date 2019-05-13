#include "help.h"
//#define _CRT_SECURE_NO_WARNINGS 

/*itoa ���������!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

//��������� ���������� ����
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

//��������� �������� ����
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

//��������� ���� �� ������� ����
//k-����� ��� ������, i-������� �����
void pokaz_4icel(SDL_Window*window, SDL_Renderer* &renderer, int k, int i)
{
	int a;
	int b;
	b = znakomesto(k);
	a = 6 - b;
	char *text = new char[10];
	_itoa_s(k, text, 10, 10);
	//_itoa(k, text, 10);

	TTF_Init();
	TTF_Font* my_font = TTF_OpenFont("Text.ttf", 100);
	SDL_Color fore_color = { 0,0,0 };
	SDL_Color back_color = { 255,255,255 };
	SDL_Rect *rect = new SDL_Rect[10];
	//������
	rect[1] = { 1129 + 23 * a,79,23 * b,40 };
	//���
	rect[2] = { 1129 + 23 * a,199,23 * b,40 };
	//������
	rect[3] = { 1129 + 23 * a,317,23 * b,40 };
	//1-�����
	rect[4] = { 171 + 23 * a,641,23 * b,58 };
	//2-�����
	rect[5] = { 561 + 23 * a,641,23 * b,58 };
	//3-�����
	rect[6] = { 951 + 23 * a,641,23 * b,58 };
	//��������� �����
	rect[7] = { 300 + 20 * a,645,20 * b,45 };
	//��������� �����
	rect[8] = { 172 + 20 * a,645,20 * b,45 };
	SDL_Surface* textSurface = NULL;
	textSurface = TTF_RenderText_Shaded(my_font, text, fore_color, back_color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_RenderCopy(renderer, texture, NULL, &rect[i]);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(my_font);
	TTF_Quit();
}