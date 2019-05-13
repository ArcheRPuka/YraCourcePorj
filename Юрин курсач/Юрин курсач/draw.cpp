#include "help.h"

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