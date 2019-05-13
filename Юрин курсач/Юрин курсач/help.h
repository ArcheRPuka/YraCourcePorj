#pragma once
#include "SDL.h" 
#include "SDL_ttf.h"

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

//Drawing functions
void kartinka_start_menu(SDL_Window* window, SDL_Renderer* &renderer);		//отрисовка стартового меню