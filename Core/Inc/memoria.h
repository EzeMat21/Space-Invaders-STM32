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
#include "cmsis_os.h"


#define true 1
#define false 0

typedef struct{

	char nombre[6];
	uint16_t puntaje;

}puntajes_t;

extern osMutexId_t mutexPuntajesHandle;
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
#define TAMANO_PAGINA 64

#define MEMORIA_TAMANO_PUNTAJE 8	//6 bytes de nombre y 2 bytes de puntaje
#define TAMANO_TOTAL_PUNTAJES 40


puntajes_t *getPuntajes(uint8_t indice);
void Write_Memoria(uint16_t address, uint8_t value);
uint8_t Read_memoria(uint16_t address);
void memoriaInit();


void Ordenamiento_Puntajes();
void guardarNuevosPuntaje();
void writeNuevosPuntajes(uint8_t cambios);
//void EEPROM_ReadPage_DMA(uint16_t pageNumber, uint8_t *dataBuffer);



#endif /* INC_MEMORIA_H_ */
