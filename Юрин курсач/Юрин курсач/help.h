#pragma once
#include <stdio.h> 
#include <stdlib.h> 

#include "SDL.h" 
#include "SDL_ttf.h"


//������ ���-�� ��������
struct Resyrs
{
	int gold; int eat; int metl;
};

//�������� �� �����
struct Stroenie
{
	int market; int kazarm; int krepoct; int mine;
};

//����� �� �����
typedef struct Voin
{
	int bronz; int serebr; int gold;
}voin;

//Maths functions
int znakomesto(int k);	//��������� ���������� ��� ���������� ������ ����

//Drawing functions
void kartinka_start_menu(SDL_Window* window, SDL_Renderer* &renderer);		//��������� ���������� ����
void kartinka_start(SDL_Window* window, SDL_Renderer* &renderer);			//��������� �������� ����
void pokaz_4icel(SDL_Window*window, SDL_Renderer* &renderer, int k, int i);	//��������� ���� �� ������� ����
