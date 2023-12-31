/*
 * memoria.c
 *
 *  Created on: Dec 29, 2023
 *      Author: ezema
 */

#include "memoria.h"


puntajes_t puntajes[5];


puntajes_t *getPuntajes(uint8_t indice){

	//puntajes_t *(puntajes_Ptr) = malloc(sizeof(puntajes_t));
	return &puntajes[indice];
}

uint8_t Write_Memoria(uint16_t address, uint8_t value){

	uint8_t data[3];
	data[0] = WRITE;
	data[1] = address>>8;
	data[2] = address;
	//data[4] = value;

	uint8_t wren = WREN;
	//uint8_t write = WRITE;

	uint8_t error;

	HAL_GPIO_WritePin (GPIOB, PIN_CS, GPIO_PIN_RESET);  // pull the cs pin low
	error = HAL_SPI_Transmit (&hspi1, &wren, 1, 100);  // write data to register

	if(error != HAL_OK){
		return 1;
	}

	HAL_GPIO_WritePin (GPIOB, PIN_CS, GPIO_PIN_SET);  // pull the cs pin high

	HAL_Delay(1);

	HAL_GPIO_WritePin (GPIOB, PIN_CS, GPIO_PIN_RESET);  // pull the cs pin low

	error = HAL_SPI_Transmit (&hspi1, data, 3, 100);  // write data to register
	if(error != HAL_OK){
		return 1;
	}
	error = HAL_SPI_Transmit (&hspi1, &value, 1, 100);  // write data to register
	if(error != HAL_OK){
		return 1;
	}
	HAL_GPIO_WritePin (GPIOB, PIN_CS, GPIO_PIN_SET);  // pull the cs pin high


}

uint8_t Read_memoria(uint16_t address)
{
	uint8_t data_rec;
	uint8_t data[3];
	data[0] = READ;
	data[1] = address>>8;
	data[2] = address;

	HAL_GPIO_WritePin (GPIOB, PIN_CS, GPIO_PIN_RESET);  // pull the pin low
	HAL_SPI_Transmit (&hspi1, data, 3, 100);  // send address
	HAL_SPI_Receive (&hspi1, &data_rec, 1, 100);  // receive 6 bytes data
	HAL_GPIO_WritePin (GPIOB, PIN_CS, GPIO_PIN_SET);  // pull the pin high

	return data_rec;
}



void memoriaInit(){

	//Tengo que hacer un ordenamiento de un vector de 5 para acomodar los puntajes de mayor a menor.

	//------------------------------------------------------------
		uint16_t address = MEMORIA_ADDRESS;

		char buffer[40];
		char buffer_retorno[6];
		uint8_t puntaje[2];
		strcpy(buffer, "linda");

		uint8_t i=0;

		address = MEMORIA_ADDRESS;

		getPuntajes(0)->nombre[i] = Read_memoria(address);

		do{

			if(i>=0 && i<6){
				getPuntajes(0)->nombre[i] = Read_memoria(address);
			}
			else if(i>=6 && i<8){

				for(uint8_t j=0; j<2;j++){

					puntaje[j] = Read_memoria(address);
				}

				getPuntajes(0)->puntaje = (puntaje[0]<<8) + puntaje[1];
			}

			i++;
			address;

		}while(i == 9);



}
















