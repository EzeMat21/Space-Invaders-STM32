/*
 * menu.h
 *
 *  Created on: Dec 26, 2023
 *      Author: ezema
 */


#ifndef INC_MENU_H_
#define INC_MENU_H_

#include "ssd1306.h"
#include "invaders.h"
#include "memoria.h"


#define true 1
#define false 0


typedef struct{

	uint8_t menuActual;
	uint8_t posicion_MenuPrincipal;

}menu_t;

enum{

	menu_principal,
	juego,
	puntajes,
	guardar_nombre,
	game_over

};

enum{

	arriba,
	abajo,
	derecha,
	izquierda,
	nulo

};


//Posicion del cursor en el Menú Principal.
#define POSICION_CURSOR_JUGAR 38
#define POSICION_CURSOR_PUNTAJES 48
#define POSICION_CURSOR_X 39


//Funciones
menu_t *getMenu();
void menuInit();
void menuActualizar(uint8_t x, uint8_t y, uint8_t boton);



#endif /* INC_MENU_H_ */
