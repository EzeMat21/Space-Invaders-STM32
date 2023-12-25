/*
 * invaders.h
 *
 *  Created on: Dec 18, 2023
 *      Author: ezema
 */


#ifndef INC_INVADERS_H_
#define INC_INVADERS_H_


#include "ssd1306.h"

//---------------------------------------------------------------Player-------------------------------------------------------

#define true 1
#define false 0

typedef struct{

	uint8_t nombre;
	uint8_t posicion_X;
	uint16_t score;

}player_t;

//enum Input{Izq, Der, Disparo, Exit};

static const unsigned char pato[]={
		14, 11, 14, 60, 252, 124, 28, 8
};

//---------------------------------------------------------------Aliens-------------------------------------------------------


#define NUM_ALIEN_COLUMNA 7
#define NUM_ALIEN_FILA 3
#define ESPACIO_ALIENS 1


#define OFFSET_INICIO 10
#define TAMANO_ALIEN_CHICO 8
#define ESPACIO_ENTRE_COLUMNAS 5
#define ESPACIO_ENTRE_FILAS 3

typedef struct{

	uint8_t posicion_X;
	uint8_t posicion_Y;
	uint8_t vivo;

}alien_t;


typedef struct{

	uint8_t velocidad_horizontal;
	uint8_t velocidad_bajada;

}dificultad_t;


typedef enum{

	mov_derecha,
	mov_izquierda

}orientacion_t;


static const unsigned char InvaderMiddleGfx[]=	//SSD1306_DrawBitmap(0,0,InvaderMiddleGfx,16,8,1);
{
  0b00100000,0b10000000,
  0b00010001,0b00000000,
  0b00111111,0b10000000,
  0b01101110,0b11000000,
  0b11111111,0b11100000,
  0b10111111,0b10100000,
  0b10100000,0b10100000,
  0b00011011,0b00000000
};

static const unsigned char InvaderBottomGfx[] = {	//SSD1306_DrawBitmap(0,0,InvaderBottomGfx,16,8,1);
  0b00001111,0b00000000,
  0b01111111,0b11100000,
  0b11111111,0b11110000,
  0b11100110,0b01110000,
  0b11111111,0b11110000,
  0b00111001,0b11000000,
  0b01100110,0b01100000,
  0b00110000,0b11000000
};


static const unsigned char InvaderTopGfx[]= {	//SSD1306_DrawBitmap(0,0,InvaderMiddleGfx,8,8,1);
  0b00011000,
  0b00111100,
  0b01111110,
  0b11011011,
  0b11111111,
  0b00100100,
  0b01011010,
  0b10100101
};

//----------------------------------------------------------------------Disparo----------------------------------------------------------------------


typedef struct{

	uint8_t posicion_x;
	int8_t posicion_y;
	uint8_t disparo;


}disparo_t;



//----------------------------------------------------------------------------------------------------------------------------------------------------


//Funciones Player
player_t *getPlayer();
void playerInit();
void plotPlayer(uint16_t direccion, player_t *player);

//Funciones Aliens
alien_t *getAlien(uint8_t fila, uint8_t columna);
void InvaderInit();
void plotAliens(orientacion_t *orientacion_horizontal, uint8_t vel, uint8_t *conteo);

//Funciones disparo Player
disparo_t *getDisparo();
void disparoInit();
void disparar(uint8_t *numero_disparos);

#endif /* INC_INVADERS_H_ */
