#pragma once
#include <stdio.h> 
#include <stdlib.h> 

#include "SDL.h" 
#include "SDL_ttf.h"


//хранит кол-во ресурсов
struct Resyrs
{
	int gold; int eat; int metl;
};

//строени€ на карте
struct Stroenie
{
	int market; int kazarm; int krepoct; int mine;
};

//воины на карте
typedef struct Voin
{
	int bronz; int serebr; int gold;
}voin;

//Maths functions
int znakomesto(int k);	//вычисл€ет знакоместо дл€ коректного вывода цифр

//Drawing functions
void kartinka_start_menu(SDL_Window* window, SDL_Renderer* &renderer);		//отрисовка стартового меню
void kartinka_start(SDL_Window* window, SDL_Renderer* &renderer);			//отрисовка игрового меню
void pokaz_4icel(SDL_Window*window, SDL_Renderer* &renderer, int k, int i);	//отрисовка цифр на игровом меню
