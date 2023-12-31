/*
 * memoria.h
 *
 *  Created on: Dec 29, 2023
 *      Author: ezema
 */

#ifndef INC_MEMORIA_H_
#define INC_MEMORIA_H_


#include "stdint.h"
#include "main.h"
#include "string.h"
#include <stdlib.h>


typedef struct{

	char nombre[6];
	uint16_t puntaje;

}puntajes_t;


extern SPI_HandleTypeDef hspi1;

//opcodes
#define WREN  6
#define WRDI  4
#define RDSR  5
#define WRSR  1
#define READ  3
#define WRITE 2

#define PIN_CS GPIO_PIN_5
#define MEMORIA_ADDRESS 0

#define MEMORIA_TAMANO_PUNTAJE 8	//6 bytes de nombre y 2 de puntaje


puntajes_t *getPuntajes(uint8_t indice);
uint8_t Write_Memoria(uint16_t address, uint8_t value);
uint8_t Read_memoria(uint16_t address);

void memoriaInit();


#endif /* INC_MEMORIA_H_ */
