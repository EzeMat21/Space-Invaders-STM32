/*
 * invaders.h
 *
 *  Created on: Dec 18, 2023
 *      Author: ezema
 */


#ifndef INC_INVADERS_H_
#define INC_INVADERS_H_


#include "ssd1306.h"
#include "menu.h"
#include "figuras.h"

//---------------------------------------------------------------Player-------------------------------------------------------

typedef struct{

	uint8_t nombre;
	uint8_t posicion_X;
	uint16_t score;

}player_t;


//---------------------------------------------------------------Aliens-------------------------------------------------------


#define NUM_ALIEN_COLUMNA 7
#define NUM_ALIEN_FILA 3
#define ESPACIO_ALIENS 1


#define OFFSET_INICIO 10
//#define TAMANO_ALIEN_CHICO 8
#define TAMANO_ALIEN 8
#define ESPACIO_ENTRE_COLUMNAS 5
#define ESPACIO_ENTRE_FILAS 3

typedef struct{

	uint8_t posicion_X;
	uint8_t posicion_Y;
	uint8_t vivo;
	uint8_t explosion;
	uint8_t conteo_explosion;

}alien_t;


typedef struct{

	uint8_t velocidad_horizontal;
	uint8_t velocidad_bajada;

}dificultad_t;


typedef enum{

	mov_derecha,
	mov_izquierda

}orientacion_t;


typedef struct{

	orientacion_t orientacion;
	uint8_t conteo_horizontal;
	uint8_t conteo_bajada;


}movimiento_aliens_t;


//----------------------------------------------------------------------Disparo----------------------------------------------------------------------


#define VELOCIDAD_DISPARO_PLAYER 6
#define POSICION_INICIAL_DISPARO 60

typedef struct{

	uint8_t posicion_x;
	int8_t posicion_y;
	uint8_t disparo;
	uint8_t numero_disparos;

}disparo_t;



//----------------------------------------------------------------------------------------------------------------------------------------------------


//Funciones Player
player_t *getPlayer();
void playerInit();
void plotPlayer(uint8_t direccion, player_t *player);

//Funciones Aliens
alien_t *getAlien(uint8_t fila, uint8_t columna);
movimiento_aliens_t *getMovAliens();
void InvaderInit();
void plotAliens();

//Funciones disparo Player
disparo_t *getDisparo();
void disparoInit();
void disparar();

void actualizarPantalla();

#endif /* INC_INVADERS_H_ */
