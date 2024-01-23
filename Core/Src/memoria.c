/*
 * memoria.c
 *
 *  Created on: Dec 29, 2023
 *      Author: ezema
 */

#include "memoria.h"



extern SPI_HandleTypeDef hspi1;

puntajes_t Puntajes[5];

puntajes_t *getPuntajes(uint8_t indice){

	//puntajes_t *(puntajes_Ptr) = malloc(sizeof(puntajes_t));
	return &Puntajes[indice];
}


void Write_Enable(){


	HAL_GPIO_WritePin(CHIP_SELECT_PORT, CHIP_SELECT_PIN, GPIO_PIN_SET);

	uint8_t comando[2] = { 0x27 , 0xFF};
	HAL_SPI_Transmit(&hspi1, &comando[0], 1, HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1, &comando[1], 1, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(CHIP_SELECT_PORT, CHIP_SELECT_PIN, GPIO_PIN_RESET);


}

void Write_Memoria(uint16_t address, uint8_t dato){

	HAL_GPIO_WritePin(CHIP_SELECT_PORT, CHIP_SELECT_PIN, GPIO_PIN_SET);

	uint8_t comando[3] = { ((uint8_t)WRITE<<3) + (address>>8  & 0xFF), address & 0xFF, dato};
	HAL_SPI_Transmit(&hspi1, comando, 3, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(CHIP_SELECT_PORT, CHIP_SELECT_PIN, GPIO_PIN_RESET);
}


uint8_t Read_memoria(uint16_t address)
{
	uint8_t dato_leido[2];

	HAL_GPIO_WritePin(CHIP_SELECT_PORT, CHIP_SELECT_PIN, GPIO_PIN_SET);

	uint8_t comando[2] = { ((uint16_t)READ<<3) + (address>>8 & 0xFF), address & 0xFF};
	HAL_SPI_Transmit(&hspi1, comando, 2, 100);

	HAL_SPI_Receive(&hspi1, &dato_leido[0], 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, &dato_leido[1], 2, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(CHIP_SELECT_PORT, CHIP_SELECT_PIN, GPIO_PIN_RESET);


	return ((dato_leido[0]<<1) + (dato_leido[1]>>7));
}

void Read_all(uint16_t address, uint16_t tamano, uint8_t *buffer){


	uint8_t dato_actual, dato_anterior;


	HAL_GPIO_WritePin(CHIP_SELECT_PORT, CHIP_SELECT_PIN, GPIO_PIN_SET);

	uint8_t comando[2] = { ((uint16_t)READ<<3) + (address>>8 & 0xFF), address & 0xFF};
	HAL_SPI_Transmit(&hspi1, comando, 2, 100);

	HAL_Delay(10);

	HAL_SPI_Receive(&hspi1, &dato_anterior, 1, HAL_MAX_DELAY);


	for(uint16_t i=0; i< tamano; i++){

		HAL_SPI_Receive(&hspi1, &dato_actual, 1, HAL_MAX_DELAY);
		*buffer = (dato_anterior<<1) + (dato_actual>>7);
		buffer++;
		dato_anterior = dato_actual;
		//HAL_Delay(1);
	}

	HAL_Delay(10);

	HAL_GPIO_WritePin(CHIP_SELECT_PORT, CHIP_SELECT_PIN, GPIO_PIN_RESET);

}



void memoriaInit(){


		char buff_nombre[7];
		strcpy(buff_nombre, "      ");

		uint16_t address = MEMORIA_ADDRESS;
		uint8_t j = 0;
		uint8_t permiso = true;
		uint8_t i=0;

		uint8_t puntaje_lsb;
		uint8_t puntaje_msb;

		uint8_t buffer[TAMANO_TOTAL_PUNTAJES];


		Read_all(address, TAMANO_TOTAL_PUNTAJES, buffer);

		do{

			if( (i==6) || (i==14) || (i==22) || (i==30) || (i==38) ){

				puntaje_msb = buffer[i];
			}
			else if((i==7) || (i==15) || (i==23) || (i==31) || (i==39)){

				puntaje_lsb = buffer[i];

				switch(i){

				case 7:
					getPuntajes(0)->puntaje = (puntaje_msb<<8) + puntaje_lsb;
					break;
				case 15:
					getPuntajes(1)->puntaje = (puntaje_msb<<8) + puntaje_lsb;
					break;
				case 23:
					getPuntajes(2)->puntaje = (puntaje_msb<<8) + puntaje_lsb;
					break;
				case 31:
					getPuntajes(3)->puntaje = (puntaje_msb<<8) + puntaje_lsb;
					break;
				case 39:
					getPuntajes(4)->puntaje = (puntaje_msb<<8) + puntaje_lsb;
					break;

				}

				puntaje_msb = 0;
				puntaje_lsb = 0;
				permiso = true;

			}
			else{

				if( (buff_nombre[j] != '\0') && permiso==true){
					buff_nombre[j]  = buffer[i];;

					if(buff_nombre[j] == '\0'){

						if((i>=0 && i<6)){
							strcpy(getPuntajes(0)->nombre, buff_nombre);
						}
						else if((i>=8 && i<14)){
							strcpy(getPuntajes(1)->nombre, buff_nombre);
						}
						else if((i>=16 && i<22)){
							strcpy(getPuntajes(2)->nombre, buff_nombre);
						}
						else if((i>=24 && i<30)){
							strcpy(getPuntajes(3)->nombre, buff_nombre);
						}
						else if((i>=32 && i<38)){
							strcpy(getPuntajes(4)->nombre, buff_nombre);
						}

						j = 0;
						strcpy(buff_nombre, "      ");
						permiso = false;
					}
					else{
						j++;
					}
				}
			}
			i++;

		}while(i != 40);


}



void writeNuevosPuntajes(uint8_t permutaciones){


	char buff_nuevosPuntajes[TAMANO_TOTAL_PUNTAJES];

	//Primero almaceno los datos en el 'buff_nuevosPuntaje' y luego escribo la memoria con este buffer.

	uint8_t k=0;
	uint16_t address;


	for(uint8_t i=0;i<TAMANO_TOTAL_PUNTAJES;i++){

		if(i>=0 && i<6){

			buff_nuevosPuntajes[i] = getPuntajes(0)->nombre[k];
			k++;
		}
		else if(i>=8 && i<14){

			buff_nuevosPuntajes[i] = getPuntajes(1)->nombre[k];
			k++;
		}
		else if(i>=16 && i<22){

			buff_nuevosPuntajes[i] = getPuntajes(2)->nombre[k];
			k++;
		}
		else if(i>=24 && i<30){

			buff_nuevosPuntajes[i] = getPuntajes(3)->nombre[k];
			k++;
		}
		else if(i>=32 && i<38){

			buff_nuevosPuntajes[i] = getPuntajes(4)->nombre[k];
			k++;
		}




		switch(i){


		case 6:
			buff_nuevosPuntajes[i] = getPuntajes(0)->puntaje >>8;
			k = 0;
			break;
		case 7:
			buff_nuevosPuntajes[i] = getPuntajes(0)->puntaje & 0xFF;
			break;
		case 14:
			buff_nuevosPuntajes[i] = getPuntajes(1)->puntaje >>8;
			k = 0;
			break;
		case 15:
			buff_nuevosPuntajes[i] = getPuntajes(1)->puntaje & 0xFF;
			break;
		case 22:
			buff_nuevosPuntajes[i] = getPuntajes(2)->puntaje >>8;
			k = 0;
			break;
		case 23:
			buff_nuevosPuntajes[i] = getPuntajes(2)->puntaje & 0xFF;
			break;
		case 30:
			buff_nuevosPuntajes[i] = getPuntajes(3)->puntaje >>8;
			k = 0;
			break;
		case 31:
			buff_nuevosPuntajes[i] = getPuntajes(3)->puntaje & 0xFF;
			break;
		case 38:
			buff_nuevosPuntajes[i] = getPuntajes(4)->puntaje >>8;
			k = 0;
			break;
		case 39:
			buff_nuevosPuntajes[i] = getPuntajes(4)->puntaje && 0xFF;
			break;

		}

	}



	//Ahora sí se realiza la escritura.
	//La escritura se realizará siempre que se realice mas de un cambio en las posiciones de los puntajes.
	//La reescritura de la posicion 5 (getPuntajes(4)) siempre se realizará.


	Write_Enable();
	HAL_Delay(10);


		if(permutaciones > 0){

			address = MEMORIA_ADDRESS;

			for(uint8_t i=0; i<TAMANO_TOTAL_PUNTAJES;i++){
				Write_Memoria(address, buff_nuevosPuntajes[i]);
				HAL_Delay(10);
				address++;
			}


		}
		else{

			address = MEMORIA_ADDRESS + 32;
			Write_Memoria(address, buff_nuevosPuntajes[32]);

			for(uint8_t i=32; i<TAMANO_TOTAL_PUNTAJES;i++){
				Write_Memoria(address, buff_nuevosPuntajes[i]);
				HAL_Delay(10);
				address++;

			}

		}

}


//Hay que corregir el ordenamiento de puntajes.

uint8_t Ordenamiento_Puntajes(){

	//Tengo que hacer un ordenamiento de un vector de 5 para acomodar los puntajes de mayor a menor.
	//En principio el ordenamiento lo realizará el menú (dentro de la tarea menu/pantalla)

	uint8_t permutaciones = 0;

	uint16_t vector[5];
	char buff_nombre[5][6];
	uint8_t indice_subvector = 1;
	uint16_t auxiliar;


	//Copio los nombres y los puntajes porque voy a reescribir getPuntajes()
	for(uint8_t i=0;i<5;i++){

		vector[i] = getPuntajes(i)->puntaje;
		strcpy(buff_nombre[i], getPuntajes(i)->nombre);

	}

	//Ordenamiento de mayor a menor xd.
	do{

			for(uint8_t j=indice_subvector; j > 0; j--){

					if(vector[j] > vector[j-1]){

						auxiliar = vector[j-1];
						vector[j-1] = vector[j];
						vector[j] = auxiliar;

						permutaciones++;

					}

			}

		indice_subvector++;

	}while(indice_subvector != 5);



	uint8_t finalizado;
	char buff_aux[6];


	for(uint8_t k=0;k<5;k++){

		finalizado = true;

			uint8_t j=0;
			do{

				if(getPuntajes(j)->puntaje == vector[k]){

					strcpy(buff_aux, getPuntajes(k)->nombre);
					strcpy(getPuntajes(k)->nombre, getPuntajes(j)->nombre);
					strcpy(getPuntajes(j)->nombre, buff_aux);

					//strcpy(buff_retorno[j], buff_nombre[k]);
					//strcpy(getPuntajes(k)->nombre, getPuntajes(j)->nombre);
					//strcpy(getPuntajes(j)->nombre, buff_nombre[k]);

					//getPuntajes(j)->puntaje = getPuntajes(k)->puntaje;
					//getPuntajes(k)->puntaje = vector[k];

					getPuntajes(j)->puntaje = getPuntajes(k)->puntaje;
					getPuntajes(k)->puntaje = vector[k];
					finalizado = false;
				}
				j++;

			}while(finalizado);
	}


	return permutaciones;

}


void Write_PuntajesEjemplos(){

	uint16_t puntaje[5];
	char buffer[5][6];

	strcpy(buffer[0], "lindo");
	puntaje[0] = 2450;

	strcpy(buffer[1], "boris");
	puntaje[1] = 12000;

	strcpy(buffer[2], "eze");
	puntaje[2] = 9999;

	strcpy(buffer[3], "josep");
	puntaje[3] = 1000;

	strcpy(buffer[4], "juan");
	puntaje[4] = 600;

	Write_Enable();
	HAL_Delay(10);

	uint16_t address = MEMORIA_ADDRESS;
	uint8_t j = 0;

	while(j != 5){



		for(uint8_t i=0;i<6;i++){

			Write_Memoria(address, buffer[j][i]);
			address++;
			HAL_Delay(10);
		}

		Write_Memoria(address, puntaje[j]>>8);
		HAL_Delay(10);
		address++;
		Write_Memoria(address, puntaje[j]);
		address++;
		HAL_Delay(10);

		j++;

	}



}



















