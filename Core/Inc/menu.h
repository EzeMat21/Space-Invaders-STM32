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
#include "task.h"

#include "tonos.h"


typedef struct{

	uint8_t posicion_x;
	uint8_t posicion_y;
	char nombre[6];
	int8_t indice;
	TickType_t xLastWakeTime_x;
	TickType_t xLastWakeTime_y;
	TickType_t xLastWakeTime_boton;

}cursor_t;


typedef struct{

	TickType_t retrasoJuego_GameOver;
	uint8_t flag;

}menuJuego_t;

typedef struct{

	uint8_t menuActual;
	uint8_t posicion_MenuPrincipal;
	cursor_t GuardarNombre;
	menuJuego_t juego;

	uint8_t musica_gameover;


}menu_t;

//Los diferentes menu en los que accede menuActual.
enum{

	menu_principal,
	juego,
	progresion_niveles,
	puntajes,
	guardar_nombre,
	game_over

};



//Posicion del cursor en el menú Guardado de nombre
#define GUARDADO_OFFSET_X_CURSOR 12
#define GUARDADO_POSICION_X_INICIAL 9
#define GUARDADO_POSICION_X_FINAL 117
#define GUARDADO_POSICION_X3_INICIAL 28
#define GUARDADO_POSICION_X3_FINAL GUARDADO_POSICION_X3_INICIAL + 6*GUARDADO_OFFSET_X_CURSOR
//#define GUARDADO_POSICION_X3_FINAL 117
#define GUARDADO_POSICION_X_BORRAR 11
#define GUARDADO_POSICION_X_ENTER GUARDADO_POSICION_X_FINAL-1
#define GUARDADO_POSICION_Y1 27
#define GUARDADO_POSICION_Y2 42
#define GUARDADO_POSICION_Y3 57



//Posicion del cursor en el Menú Principal.
#define POSICION_CURSOR_JUGAR 38
#define POSICION_CURSOR_PUNTAJES 48
#define POSICION_CURSOR_X 39



//Funciones
menu_t *getMenu();
void menuInit();
void menuReset();
void GuardarNombreReset();
void menuActualizar(uint8_t x, uint8_t y, uint8_t boton);



#endif /* INC_MENU_H_ */
