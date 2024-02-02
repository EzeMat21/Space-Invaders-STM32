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


#define TAMANO_NAVE_Y 8
#define TOTAL_PIXELES_Y 64


#define NUM_ALIEN_COLUMNA 7
#define NUM_ALIEN_FILA 3
#define ESPACIO_ALIENS 1

#define TAMANO_ALIEN_TOP 8
#define TAMANO_ALIEN_MIDDLE 11
#define TAMANO_ALIEN_BOTTOM 12



#define OFFSET_INICIO 11
#define TAMANO_ALIEN 8
#define ESPACIO_ENTRE_COLUMNAS 5
#define ESPACIO_ENTRE_FILAS 3


//Puntajes por cada aline
#define PUNTAJE_ALIEN_TOP 30
#define PUNTAJE_ALIEN_MIDDLE 20
#define PUNTAJE_ALIEN_BOTTOM 10

#define PUNTAJE_ADICIONAL 5


//Defines para la base

#define POS_OFF_DISPARO_PLAYER 4
#define LIMITE_IZQ_X_BASE1 16
#define LIMITE_DER_X_BASE1 31
#define LIMITE_IZQ_X_BASE2 56
#define LIMITE_DER_X_BASE2 71
#define LIMITE_IZQ_X_BASE3 96
#define LIMITE_DER_X_BASE3 111
#define LIMITE_ARRIBA_BASE 38
#define LIMITE_ABAJO_BASE 45

#define OFFSET_DESPLAZAMIENTO 15
#define RANDOM_MAX_2 2



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
//Bases

typedef struct{

	uint8_t buffer[16];
	uint8_t lim_der;
	uint8_t lim_izq;
	uint8_t lim_arriba;
	uint8_t lim_abajo;

}base_t;



//----------------------------------------------------------------------------------------------------------------------------------------------------


//Pantalla
void actualizarPantalla();

//Funciones Player
player_t getPlayer();
void playerInit();
void plotPlayer(uint8_t direccion);

//Funciones Aliens
void InvaderInit();
void plotAliens();
disparo_t getDisparoAliens();
void disparoAliens();

//Funciones disparo Player
disparo_t *getDisparo();
void disparoInit();
void disparar();

//Dificultades
//dificultad_t *getDificultad();
void AumentoNivel();
void Dificultad_Init();

//Bases defensivas
void plotBases();
void BasesInit();

#endif /* INC_INVADERS_H_ */
