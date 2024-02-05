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

	uint16_t posicion_X;
	uint8_t vivo;
	uint8_t vidas;
	uint16_t puntaje;
	uint8_t nivel;


}player_t;


//---------------------------------------------------------------Aliens-------------------------------------------------------


#define NUM_ALIENS_PARA_AUMENTO_VELOCIDAD 3
#define AUMENTO_POSICION_PIXELES_2 2
#define AUMENTO_POSICION_PIXELES_4 4
#define AUMENTO_POSICION_PIXELES_6 6

#define TAMANO_NAVE_Y 8
#define TOTAL_PIXELES_Y 64

#define LIMITE_PANTALLA_IZQ  1
#define LIMITE_PANTALLA_DER  127


#define NUM_ALIEN_COLUMNA 7
#define NUM_ALIEN_FILA 3
#define ESPACIO_ALIENS 1

#define TAMANO_ALIEN_TOP_ANCHURA 8
#define TAMANO_ALIEN_MIDDLE_ANCHURA 11
#define TAMANO_ALIEN_BOTTOM_ANCHURA 12
#define TAMANO_ALIEN_ALTURA 8

#define TAMANO_PLAYER_ANCHURA 6

#define OFFSET_INICIO 11
#define TAMANO_ALIEN 8
#define ESPACIO_ENTRE_COLUMNAS 5
#define ESPACIO_ENTRE_FILAS 3


//Puntajes por cada aliens
#define PUNTAJE_ALIEN_TOP 30
#define PUNTAJE_ALIEN_MIDDLE 20
#define PUNTAJE_ALIEN_BOTTOM 10

#define PUNTAJE_ADICIONAL 5

//-------------------------------------------------------------- Bases ----------------------------------------------------------------

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
#define RANDOM_MAX_3 3
#define DESPLAZAMIENTO_MINIMO 2

#define DESTRUCCION_MAXIMA_VERTICAL 2


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
	uint8_t aumento_posicion_pixeles;

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
	uint8_t cantidad_aliens_vivos;


}movimiento_aliens_t;


//----------------------------------------------------------------------Disparo----------------------------------------------------------------------


#define VELOCIDAD_DISPARO_PLAYER 4
#define POSICION_INICIAL_DISPARO 50

typedef struct{

	uint8_t posicion_x;
	int8_t posicion_y;
	uint8_t disparo;

}disparo_t;


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
uint8_t getCantidad_Aliens_Vivos();
void disparoAliens();

//Funciones disparo Player
uint8_t getDisparo();
void disparoInit();
void disparar();
void disparoTrue();

//Dificultades
void AumentoNivel();
void Dificultad_Init();
void AumentoVelocidadAliens();


//Bases defensivas
void plotBases();
void BasesInit();
void BasesProcesarDisparo(disparo_t *disparo, uint8_t condicion);

#endif /* INC_INVADERS_H_ */
