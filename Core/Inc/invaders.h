/*
 * invaders.h
 *
 *  Created on: Dec 18, 2023
 *      Author: ezema
 */


#ifndef INC_INVADERS_H_
#define INC_INVADERS_H_


#include "ssd1306.h"
#include "defines.h"
#include "figuras.h"



//---------------------------------------------------------------Player-------------------------------------------------------

typedef struct{

	uint8_t posicion_X;
	uint8_t vivo;
	uint8_t vidas;
	uint16_t puntaje;
	uint8_t nivel;


}player_t;


//---------------------------------------------------------------Aliens-------------------------------------------------------


#define NUM_ALIEN_COLUMNA 7
#define NUM_ALIEN_FILA 3
#define ESPACIO_ALIENS 1

#define TAMANO_ALIEN_TOP 8
#define TAMANO_ALIEN_MIDDLE 11
#define TAMANO_ALIEN_BOTTOM 12


#define OFFSET_INICIO 10
#define TAMANO_ALIEN_BOTTOM 12
#define TAMANO_ALIEN 8
#define ESPACIO_ENTRE_COLUMNAS 5
#define ESPACIO_ENTRE_FILAS 3


//Puntajes por cada aline
#define PUNTAJE_ALIEN_TOP 30
#define PUNTAJE_ALIEN_MIDDLE 20
#define PUNTAJE_ALIEN_BOTTOM 10



typedef struct{

	int8_t posicion_X;
	uint8_t posicion_Y;
	uint8_t vivo;
	uint8_t explosion;
	uint8_t conteo_explosion;

}alien_t;


typedef struct{

	uint8_t velocidad_horizontal;
	uint8_t velocidad_bajada;
	uint8_t velocidad_disparo_aliens;
	uint8_t tiempo_entre_disparos;

}dificultad_t;


typedef enum{

	mov_derecha,
	mov_izquierda

}orientacion_t;


typedef struct{

	orientacion_t orientacion;
	uint8_t conteo_horizontal;
	uint8_t conteo_bajada;
	uint8_t animacion;

	uint8_t alien_columna_izquierda;
	uint8_t alien_columna_derecha;


}movimiento_aliens_t;


//----------------------------------------------------------------------Disparo----------------------------------------------------------------------


#define VELOCIDAD_DISPARO_PLAYER 4
#define POSICION_INICIAL_DISPARO 50

typedef struct{

	uint8_t posicion_x;
	int8_t posicion_y;
	uint8_t disparo;
	uint8_t numero_disparos;


}disparo_t;



//----------------------------------------------------------------------------------------------------------------------------------------------------


//Pantalla
void actualizarPantalla();

//Funciones Player
player_t *getPlayer();
void playerInit();
void plotPlayer(uint8_t direccion, player_t *player);

//Funciones Aliens
alien_t *getAlien(uint8_t fila, uint8_t columna);
movimiento_aliens_t *getMovAliens();
void InvaderInit();
void plotAliens();
disparo_t *getDisparoAliens();
void disparoAliens();

//Funciones disparo Player
disparo_t *getDisparo();
void disparoInit();
void disparar();

//Dificultades
dificultad_t *getDificultad();
void AumentoNivel();

//Bases defensivas
void plotBases();

#endif /* INC_INVADERS_H_ */
