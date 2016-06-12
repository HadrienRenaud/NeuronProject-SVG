#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "config.h"

using namespace std;

void	menu(SDL_Renderer *ren);
void	keyboard(SDL_Event event, bool* keyboardInput);
void	getOptions(int* length_alphabet, string* geometry);

#endif	// MENU_H_INCLUDED
