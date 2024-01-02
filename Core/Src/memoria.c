/*
 * memoria.c
 *
 *  Created on: Dec 29, 2023
 *      Author: ezema
 */

#include "memoria.h"

puntajes_t Puntajes[5];

puntajes_t *getPuntajes(uint8_t indice){

	//puntajes_t *(puntajes_Ptr) = malloc(sizeof(puntajes_t));
	return &Puntajes[indice];
}

void Write_Memoria(uint16_t address, uint8_t value){

	uint8_t data[3];
	data[0] = WRITE;
	data[1] = address>>8;
	data[2] = address;
	//data[4] = value;

	uint8_t wren = WREN;

	HAL_GPIO_WritePin (GPIOB, PIN_CS, GPIO_PIN_RESET);  // pull the cs pin low
	HAL_SPI_Transmit (&hspi1, &wren, 1, 100);  // write data to register

	HAL_GPIO_WritePin (GPIOB, PIN_CS, GPIO_PIN_SET);  // pull the cs pin high

	HAL_Delay(1);

	HAL_GPIO_WritePin (GPIOB, PIN_CS, GPIO_PIN_RESET);  // pull the cs pin low
	HAL_SPI_Transmit (&hspi1, data, 3, 100);  // write data to register
	HAL_SPI_Transmit (&hspi1, &value, 1, 100);  // write data to register

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


		puntajesActualizar();

		char buff_nombre[7];

		uint16_t address = MEMORIA_ADDRESS;
		uint8_t j = 0;
		uint8_t permiso = true;
		uint8_t i=0;

		uint8_t puntaje_lsb;
		uint8_t puntaje_msb;
		uint8_t byte_dumb;


		buff_nombre[0]  = Read_memoria(address);
		HAL_Delay(3);

		strcpy(buff_nombre, "      ");

		do{

			if( (i==6) || (i==14) || (i==22) || (i==30) || (i==38) ){

				puntaje_msb = Read_memoria(address);
				HAL_Delay(3);
			}
			else if((i==7) || (i==15) || (i==23) || (i==31) || (i==39)){

				puntaje_lsb = Read_memoria(address);
				HAL_Delay(3);

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
					buff_nombre[j]  = Read_memoria(address);
					HAL_Delay(3);

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

				else{
					byte_dumb = Read_memoria(address);
					HAL_Delay(3);
				}

			}
			i++;
			address++;
			//HAL_Delay(3);

		}while(i != 40);



}


void puntajesActualizar(){		//Actualizar los puntajes y guardarlos en memoria


	uint16_t address = MEMORIA_ADDRESS+24;

	char buffer[6];


	strcpy(buffer, "boris");


	uint16_t puntaje1 = 121;

	uint8_t i=0;

	Write_Memoria(address, buffer[i]);

	do{

		if(i== 6){
			Write_Memoria(address, puntaje1>>8);
		}
		if(i==7){
			Write_Memoria(address, puntaje1);
		}
		else{
			Write_Memoria(address, buffer[i]);
		}

		i++;
		address++;
		HAL_Delay(10);

	}while(i != 9);

}


//Hay que corregir el ordenamiento de puntajes.

void Ordenamiento_Puntajes(){

	//Tengo que hacer un ordenamiento de un vector de 5 para acomodar los puntajes de mayor a menor.
	//En principio el ordenamiento lo realizará el menú (dentro de la tarea menu/pantalla)


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

					}

			}

		indice_subvector++;

	}while(indice_subvector != 5);




	char buff_retorno[5][6];


	for(uint8_t k=0;k<5;k++){

		uint8_t j=0;
		do{

			if(getPuntajes(j)->puntaje == vector[k]){

				//strcpy(buff_retorno[j], buff_nombre[k]);
				strcpy(getPuntajes(k)->nombre, getPuntajes(j)->nombre);
				strcpy(getPuntajes(j)->nombre, buff_nombre[k]);

				getPuntajes(j)->puntaje = getPuntajes(k)->puntaje;
				getPuntajes(k)->puntaje = vector[k];
			}
			j++;

		}while(j!=k);

	}


}




















