/*
 * memoria.c
 *
 *  Created on: Dec 29, 2023
 *      Author: ezema
 */

#include "memoria.h"



extern SPI_HandleTypeDef hspi1;

puntajes_t Puntajes[10];

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


		char buff_nombre[NUM_MAX_POR_NOMBRE+1];
		strcpy(buff_nombre, "      ");

		uint16_t address = MEMORIA_ADDRESS;
		uint8_t j = 0;
		uint8_t permiso = true;
		uint8_t i=0;

		uint8_t puntaje_lsb = 0;
		uint8_t puntaje_msb = 0;

		uint8_t buffer[TAMANO_TOTAL_PUNTAJES];


		Read_all(address, TAMANO_TOTAL_PUNTAJES, buffer);

		do{

			if( (i==7) || (i==16) || (i==25) || (i==34) || (i==43) || (i==52) || (i==61) || (i==70) || (i==79) || (i==88) ){

				puntaje_msb = buffer[i];
			}
			else if((i==8) || (i==17) || (i==26) || (i==35) || (i==44) || (i==53) || (i==62) || (i==71) || (i==80) || (i==89) ){

				puntaje_lsb = buffer[i];

				switch(i){

				case 8:
					Puntajes[0].puntaje = (puntaje_msb<<8) + puntaje_lsb;
					break;
				case 17:
					Puntajes[1].puntaje = (puntaje_msb<<8) + puntaje_lsb;
					break;
				case 26:
					Puntajes[2].puntaje = (puntaje_msb<<8) + puntaje_lsb;
					break;
				case 35:
					Puntajes[3].puntaje = (puntaje_msb<<8) + puntaje_lsb;
					break;
				case 44:
					Puntajes[4].puntaje = (puntaje_msb<<8) + puntaje_lsb;
					break;
				case 53:
					Puntajes[5].puntaje = (puntaje_msb<<8) + puntaje_lsb;
					break;
				case 62:
					Puntajes[6].puntaje = (puntaje_msb<<8) + puntaje_lsb;
					break;
				case 71:
					Puntajes[7].puntaje = (puntaje_msb<<8) + puntaje_lsb;
					break;
				case 80:
					Puntajes[8].puntaje = (puntaje_msb<<8) + puntaje_lsb;
					break;
				case 89:
					Puntajes[9].puntaje = (puntaje_msb<<8) + puntaje_lsb;
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

						if((i>=0 && i<7)){
							strcpy(Puntajes[0].nombre, buff_nombre);
						}
						else if((i>=9 && i<16)){
							strcpy(Puntajes[1].nombre, buff_nombre);
						}
						else if((i>=18 && i<25)){
							strcpy(Puntajes[2].nombre, buff_nombre);
						}
						else if((i>=27 && i<34)){
							strcpy(Puntajes[3].nombre, buff_nombre);
						}
						else if((i>=36 && i<43)){
							strcpy(Puntajes[4].nombre, buff_nombre);
						}
						else if((i>=45 && i<52)){
							strcpy(Puntajes[5].nombre, buff_nombre);
						}
						else if((i>=54 && i<61)){
							strcpy(Puntajes[6].nombre, buff_nombre);
						}
						else if((i>=63 && i<70)){
							strcpy(Puntajes[7].nombre, buff_nombre);
						}
						else if((i>=72 && i<79)){
							strcpy(Puntajes[8].nombre, buff_nombre);
						}
						else if((i>=81 && i<88)){
							strcpy(Puntajes[9].nombre, buff_nombre);
						}

						j = 0;
						strcpy(buff_nombre, "       ");
						permiso = false;
					}
					else{
						j++;
					}
				}
			}
			i++;

		}while(i != TAMANO_TOTAL_PUNTAJES);


}



void writeNuevosPuntajes(uint8_t permutaciones){


	char buff_nuevosPuntajes[TAMANO_TOTAL_PUNTAJES];

	//Primero almaceno los datos en el 'buff_nuevosPuntaje' y luego escribo la memoria con este buffer.

	uint8_t k=0;
	uint16_t address;


	for(uint8_t i=0;i<TAMANO_TOTAL_PUNTAJES;i++){

		if(i>=0 && i<7){

			buff_nuevosPuntajes[i] = Puntajes[0].nombre[k];
			k++;
		}
		else if(i>=9 && i<16){

			buff_nuevosPuntajes[i] = Puntajes[1].nombre[k];
			k++;
		}
		else if(i>=18 && i<25){

			buff_nuevosPuntajes[i] = Puntajes[2].nombre[k];
			k++;
		}
		else if(i>=27 && i<34){

			buff_nuevosPuntajes[i] = Puntajes[3].nombre[k];
			k++;
		}
		else if(i>=36 && i<43){

			buff_nuevosPuntajes[i] = Puntajes[4].nombre[k];
			k++;
		}
		else if(i>=45 && i<52){

			buff_nuevosPuntajes[i] = Puntajes[5].nombre[k];
			k++;
		}
		else if(i>=54 && i<61){

			buff_nuevosPuntajes[i] = Puntajes[6].nombre[k];
			k++;
		}
		else if(i>=63 && i<70){

			buff_nuevosPuntajes[i] = Puntajes[7].nombre[k];
			k++;
		}
		else if(i>=72 && i<79){

			buff_nuevosPuntajes[i] = Puntajes[8].nombre[k];
			k++;
		}
		else if(i>=81 && i<88){

			buff_nuevosPuntajes[i] = Puntajes[9].nombre[k];
			k++;
		}


		switch(i){


		case 7:
			buff_nuevosPuntajes[i] = Puntajes[0].puntaje >>8;
			k = 0;
			break;
		case 8:
			buff_nuevosPuntajes[i] = Puntajes[0].puntaje & 0xFF;
			break;
		case 16:
			buff_nuevosPuntajes[i] = Puntajes[1].puntaje >>8;
			k = 0;
			break;
		case 17:
			buff_nuevosPuntajes[i] = Puntajes[1].puntaje & 0xFF;
			break;
		case 25:
			buff_nuevosPuntajes[i] = Puntajes[2].puntaje >>8;
			k = 0;
			break;
		case 26:
			buff_nuevosPuntajes[i] = Puntajes[2].puntaje & 0xFF;
			break;
		case 34:
			buff_nuevosPuntajes[i] = Puntajes[3].puntaje >>8;
			k = 0;
			break;
		case 35:
			buff_nuevosPuntajes[i] = Puntajes[3].puntaje & 0xFF;
			break;
		case 43:
			buff_nuevosPuntajes[i] = Puntajes[4].puntaje >>8;
			k = 0;
			break;
		case 44:
			buff_nuevosPuntajes[i] = Puntajes[4].puntaje & 0xFF;
			break;
		case 52:
			buff_nuevosPuntajes[i] = Puntajes[5].puntaje >>8;
			k = 0;
			break;
		case 53:
			buff_nuevosPuntajes[i] = Puntajes[5].puntaje & 0xFF;
			break;
		case 61:
			buff_nuevosPuntajes[i] = Puntajes[6].puntaje >>8;
			k = 0;
			break;
		case 62:
			buff_nuevosPuntajes[i] = Puntajes[6].puntaje & 0xFF;
			break;
		case 70:
			buff_nuevosPuntajes[i] = Puntajes[7].puntaje >>8;
			k = 0;
			break;
		case 71:
			buff_nuevosPuntajes[i] = Puntajes[7].puntaje & 0xFF;
			break;
		case 79:
			buff_nuevosPuntajes[i] = Puntajes[8].puntaje >>8;
			k = 0;
			break;
		case 80:
			buff_nuevosPuntajes[i] = Puntajes[8].puntaje & 0xFF;
			break;
		case 88:
			buff_nuevosPuntajes[i] = Puntajes[9].puntaje >>8;
			k = 0;
			break;
		case 89:
			buff_nuevosPuntajes[i] = Puntajes[9].puntaje & 0xFF;
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

			address = MEMORIA_ADDRESS + OFFSET_ULTIMO_PUNTAJE;
			Write_Memoria(address, buff_nuevosPuntajes[OFFSET_ULTIMO_PUNTAJE]);

			for(uint8_t i=OFFSET_ULTIMO_PUNTAJE; i<TAMANO_TOTAL_PUNTAJES;i++){
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

	uint16_t vector[NUM_MAX_PUNTAJES];
	char buff_nombre[NUM_MAX_PUNTAJES][NUM_MAX_POR_NOMBRE];
	uint8_t indice_subvector = 1;
	uint16_t auxiliar;


	//Copio los nombres y los puntajes porque voy a reescribir getPuntajes()
	for(uint8_t i=0;i<NUM_MAX_PUNTAJES;i++){

		vector[i] = Puntajes[i].puntaje;
		strcpy(buff_nombre[i], Puntajes[i].nombre);

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

	}while(indice_subvector != NUM_MAX_PUNTAJES);



	uint8_t finalizado;
	char buff_aux[NUM_MAX_POR_NOMBRE];


	for(uint8_t k=0;k<NUM_MAX_PUNTAJES;k++){

		finalizado = true;

			uint8_t j=0;
			do{

				if(Puntajes[j].puntaje == vector[k]){

					strcpy(buff_aux, Puntajes[k].nombre);
					strcpy(Puntajes[k].nombre, Puntajes[j].nombre);
					strcpy(Puntajes[j].nombre, buff_aux);

					Puntajes[j].puntaje = Puntajes[k].puntaje;
					Puntajes[k].puntaje = vector[k];
					finalizado = false;
				}
				j++;

			}while(finalizado);
	}


	return permutaciones;

}


void Write_PuntajesEjemplos(){

	uint16_t puntaje[NUM_MAX_PUNTAJES];
	char buffer[NUM_MAX_PUNTAJES][NUM_MAX_POR_NOMBRE];

	strcpy(buffer[0], "messi");
	puntaje[0] = 30000;

	strcpy(buffer[1], "boris");
	puntaje[1] = 12000;

	strcpy(buffer[2], "eze");
	puntaje[2] = 9999;

	strcpy(buffer[3], "kafka");
	puntaje[3] = 8888;

	strcpy(buffer[4], "juan");
	puntaje[4] = 7777;

	strcpy(buffer[5], "magnus");
	puntaje[5] = 6666;

	strcpy(buffer[6], "hikaru");
	puntaje[6] = 5555;

	strcpy(buffer[7], "vishy");
	puntaje[7] = 0;

	strcpy(buffer[8], "vishy");
	puntaje[8] = 0;

	strcpy(buffer[9], "noderb");
	puntaje[9] = 0;

	Write_Enable();
	HAL_Delay(10);

	uint16_t address = MEMORIA_ADDRESS;
	uint8_t j = 0;

	while(j != NUM_MAX_PUNTAJES){



		for(uint8_t i=0;i<NUM_MAX_POR_NOMBRE;i++){

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



















