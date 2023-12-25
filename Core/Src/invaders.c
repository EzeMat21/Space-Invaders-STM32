/*
 * invaders.c
 *
 *  Created on: Dec 18, 2023
 *      Author: ezema
 */


#include "invaders.h"


player_t Player;

alien_t Alien[NUM_ALIEN_FILA][NUM_ALIEN_COLUMNA];

disparo_t Disparo;

extern dificultad_t dificultad;

player_t *getPlayer(){

	return &Player;
}

alien_t *getAlien(uint8_t fila, uint8_t columna){
	return &Alien[fila][columna];
}

disparo_t *getDisparo(){
	return &Disparo;
}

void playerInit(){

	Player.posicion_X = 64;

}

void plotPlayer(uint16_t direccion, player_t *player){

	if( direccion > 2000){

		if(player->posicion_X >= 120){
			player->posicion_X = 120;
		}
		else{
			player->posicion_X = player->posicion_X + 4;
		}

	}
	else if(direccion < 1800){

		if(player->posicion_X <= 0){
			player->posicion_X = 0;
		}
		else{
			player->posicion_X = player->posicion_X - 4;
		}
	}

	SSD1306_DrawBitmap(player->posicion_X, 56, pato, 8,8,1);
}



void InvaderInit(){

	for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
		for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){
			getAlien(y,x)->posicion_X  =  OFFSET_INICIO + x*(TAMANO_ALIEN_CHICO + ESPACIO_ENTRE_COLUMNAS);
			getAlien(y,x)->posicion_Y = y*(TAMANO_ALIEN_CHICO + ESPACIO_ENTRE_FILAS);
			getAlien(y, x)->vivo = true;
		}
	}

}

void plotAliens(orientacion_t *orientacion_horizontal, uint8_t vel, uint8_t *conteo){


	if(vel % dificultad.velocidad_horizontal == 0){
			switch(*orientacion_horizontal){
				case mov_derecha:

						for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
							for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){
								getAlien(y,x)->posicion_X  = getAlien(y,x)->posicion_X + 1;


							}
						}

					break;

				case mov_izquierda:

						for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
							for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){

								getAlien(y,x)->posicion_X  = getAlien(y,x)->posicion_X - 1;

							}
						}

						break;
				default:
					break;
		}


			if( getAlien(0,7)->posicion_X >= 39){

				*orientacion_horizontal = mov_izquierda;
			}
			if( getAlien(0,0)->posicion_X <= 0){
				*orientacion_horizontal = mov_derecha;
				*conteo = *conteo + 1;
			}



		//Esta parte se encarga de ir bajando los aliens

			if(*conteo == 10){
				for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
					for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){

						getAlien(y,x)->posicion_Y  = getAlien(y,x)->posicion_Y + dificultad.velocidad_bajada;
						*conteo = 0;

					}
				}
			}

	}


	//Acá se grafican los aliens en la pantalla

		for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
			for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){

					switch(y){

					case 0:
						if(getAlien(y,x)->vivo == true){
							SSD1306_DrawBitmap(getAlien(y,x)->posicion_X, getAlien(y,x)->posicion_Y,InvaderMiddleGfx,16,8,1);
						}
						else{

						}
						break;
					case 1:
						if(getAlien(y,x)->vivo == true){
							SSD1306_DrawBitmap(getAlien(y,x)->posicion_X, getAlien(y,x)->posicion_Y,InvaderMiddleGfx,16,8,1);
						}
						else{

						}
						break;
					default:
						if(getAlien(y,x)->vivo == true){
							SSD1306_DrawBitmap(getAlien(y,x)->posicion_X, getAlien(y,x)->posicion_Y,InvaderBottomGfx,16,8,1);
						}
						else{

						}
						break;
					}
			}

		}

}


void disparoInit(){

	Disparo.posicion_x = 255;
	Disparo.posicion_y = 58;
	Disparo.disparo = false;
}


void disparar(uint8_t *numero_disparos){

	//Para el disparo
	//if(disparoOK == 1){		//estamos disparando


		//if(disparo[i].disparo == false){
			//disparo[i].posicion_x = getPlayer()->posicion_X;
			switch (*numero_disparos) {
				case 1:
					if(getDisparo()->disparo == false){
						getDisparo()->disparo = true;
						getDisparo()->posicion_x = getPlayer()->posicion_X;
					}
					break;
					/*case 2:
					if(disparo[1].disparo == false){
						disparo[1].disparo = true;
						disparo[1].posicion_x = getPlayer()->posicion_X;
					}
					break;
				case 3:
					if(disparo[2].disparo == false){
						disparo[2].disparo = true;
						disparo[2].posicion_x = getPlayer()->posicion_X;
					}
					break;
				case 4:
					if(disparo[3].disparo == false){
						disparo[3].disparo = true;
						disparo[3].posicion_x = getPlayer()->posicion_X;
					}
					break;
				case 5:
					if(disparo[4].disparo == false){
						disparo[4].disparo = true;
						disparo[4].posicion_x = getPlayer()->posicion_X;
					}*/
					break;
				default:
					break;
			}



		//for(uint8_t i=0; i<1;i++){

			if(getDisparo()->disparo == true){
				SSD1306_DrawLine(getDisparo()->posicion_x + 4, getDisparo()->posicion_y, getDisparo()->posicion_x+4, getDisparo()->posicion_y + 3, 1);
				//disparar(disparo[i].posicion_x, &disparo[i].posicion_y);
				getDisparo()->posicion_y = getDisparo()->posicion_y - 8;


				if(getDisparo()->posicion_y <= 0){
					getDisparo()->disparo = false;
					getDisparo()->posicion_y = 58;
					*numero_disparos = *numero_disparos - 1;
				}
			}
			else{

			}
		//}


}














