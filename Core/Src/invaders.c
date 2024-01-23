/*
 * invaders.c
 *
 *  Created on: Dec 18, 2023
 *      Author: ezema
 */


#include "invaders.h"

//----------------------------------------------------------------------------------------------------------------------------------------------

//Variables del jugador Player
player_t Player;
disparo_t Disparo;

//Variables de los aliens
movimiento_aliens_t movimiento_Aliens;
alien_t Alien[NUM_ALIEN_FILA][NUM_ALIEN_COLUMNA];
disparo_t Disparo_Aliens;


dificultad_t dificultad;


//----------------------------------------------------------------------------------------------------------------------------------------------


player_t *getPlayer(){

	return &Player;
}

alien_t *getAlien(uint8_t fila, uint8_t columna){
	return &Alien[fila][columna];
}

movimiento_aliens_t *getMovAliens(){
	return &movimiento_Aliens;
}

disparo_t *getDisparo(){
	return &Disparo;
}

disparo_t *getDisparoAliens(){
	return &Disparo_Aliens;
}

dificultad_t *getDificultad(){

	return &dificultad;
}

//----------------------------------------------------------------------------------------------------------------------------------------------


void actualizarPantalla(){

    SSD1306_UpdateScreen(); // update screen
    SSD1306_Fill(0);
}


void playerInit(){

	getPlayer()->posicion_X = 64;
	getPlayer()->vivo = true;
	getPlayer()->puntaje = 900;
	getPlayer()->vidas = 3;
	getPlayer()->nivel = 1;
}

void plotPlayer(uint8_t direccion, player_t *player){

	if(getPlayer()->vivo == true){

		if(direccion == derecha){

			if(player->posicion_X >= 120){
				player->posicion_X = 120;
			}
			else{
				player->posicion_X = player->posicion_X + 4;
			}

		}
		else if(direccion == izquierda){

			if(player->posicion_X <= 0){
				player->posicion_X = 0;
			}
			else{
				player->posicion_X = player->posicion_X - 4;
			}
		}

		SSD1306_DrawBitmap(player->posicion_X, 56, nave, 8,8,1);
	}

	else{	//Si el jugador ha sido eliminado.

		SSD1306_DrawBitmap(player->posicion_X, 56, nave_explotada, 8,8,1);
	}


}



void InvaderInit(){

	getMovAliens()->orientacion = mov_derecha;
	getMovAliens()->alien_columna_derecha = NUM_ALIEN_COLUMNA - 1;
	getMovAliens()->alien_columna_izquierda = 0;

	for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
		for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){
			getAlien(y,x)->posicion_X  =  OFFSET_INICIO + x*(TAMANO_ALIEN+ ESPACIO_ENTRE_COLUMNAS);
			getAlien(y,x)->posicion_Y = y*(TAMANO_ALIEN+ ESPACIO_ENTRE_FILAS);
			getAlien(y, x)->vivo = true;
			getAlien(y, x)->conteo_explosion = 2;
		}
	}

}

void plotAliens(){

	//Esta parte se encarga de ir moviendo los aliens de izquierda a derecha, derecha a izquierda.
	if(getMovAliens()->conteo_horizontal % dificultad.velocidad_horizontal == 0){

		/*getMovAliens()->animacion = getMovAliens()->animacion + 1;
		if(getMovAliens()->animacion >= 8){
			getMovAliens()->animacion = 0;
		}*/

		getMovAliens()->animacion = !getMovAliens()->animacion;

			switch(getMovAliens()->orientacion){
				case mov_derecha:

						for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
							for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){
								getAlien(y,x)->posicion_X  = getAlien(y,x)->posicion_X + 2;


							}
						}

					break;

				case mov_izquierda:

						for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
							for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){

								getAlien(y,x)->posicion_X  = getAlien(y,x)->posicion_X - 2;

							}
						}

						break;
				default:
					break;
		}


	//Aqui se cambia la orientacion de los aliens: cuando se llega al borde de la pantalla los aliens pasan a moverse hacia la derecha o hacia la izquierda.

			if( (getAlien(0,getMovAliens()->alien_columna_derecha)->posicion_X + TAMANO_ALIEN_BOTTOM) >= 127){

				getMovAliens()->orientacion = mov_izquierda;
			}


			if( (getAlien(0,getMovAliens()->alien_columna_izquierda)->posicion_X) <= 0){
				getMovAliens()->orientacion = mov_derecha;
				getMovAliens()->conteo_bajada = getMovAliens()->conteo_bajada + 1;
			}



		//Esta parte se encarga de ir bajando los aliens.
			if(getMovAliens()->conteo_bajada == 10){
				for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
					for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){

						getAlien(y,x)->posicion_Y  = getAlien(y,x)->posicion_Y + dificultad.velocidad_bajada;
						getMovAliens()->conteo_bajada = 0;

					}
				}
			}

	}

	getMovAliens()->conteo_horizontal = getMovAliens()->conteo_horizontal + 1;


	uint8_t conteo_columna_derecha = 0;
	uint8_t conteo_columna_izquierda = 0;

	//Acá se grafican los aliens en la pantalla.
	//Se grafican los aliens que solo estan vivos.
		for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){

			if(getMovAliens()->alien_columna_derecha != getMovAliens()->alien_columna_izquierda){

				if(getAlien(y,getMovAliens()->alien_columna_derecha)->vivo == false){
					conteo_columna_derecha++;
				}

				else if(getAlien(y,getMovAliens()->alien_columna_izquierda)->vivo == false){
					conteo_columna_izquierda++;
				}

			}

			for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){

					switch(y){

					case 0:
						if(getAlien(y,x)->vivo == true){

							if(getMovAliens()->animacion == true ){
								SSD1306_DrawBitmap(getAlien(y,x)->posicion_X, getAlien(y,x)->posicion_Y,InvaderTopGfx,8,8,1);
								//getMovAliens()->animacion = false;
							}
							else{
								SSD1306_DrawBitmap(getAlien(y,x)->posicion_X, getAlien(y,x)->posicion_Y,InvaderTopGfx2,8,8,1);
								//getMovAliens()->animacion = true;
							}

						}
						break;
					case 1:
						if(getAlien(y,x)->vivo == true){
							if(getMovAliens()->animacion == true ){
								SSD1306_DrawBitmap(getAlien(y,x)->posicion_X, getAlien(y,x)->posicion_Y,InvaderMiddleGfx,16,8,1);
								//getMovAliens()->animacion = false;
							}
							else{
								SSD1306_DrawBitmap(getAlien(y,x)->posicion_X, getAlien(y,x)->posicion_Y,InvaderMiddleGfx2,16,8,1);
								//getMovAliens()->animacion = true;
							}
						}
						break;
					default:
						if(getAlien(y,x)->vivo == true){
							if(getMovAliens()->animacion == true ){
								SSD1306_DrawBitmap(getAlien(y,x)->posicion_X, getAlien(y,x)->posicion_Y,InvaderBottomGfx,16,8,1);
							}
							else{
								SSD1306_DrawBitmap(getAlien(y,x)->posicion_X, getAlien(y,x)->posicion_Y,InvaderBottomGfx2,16,8,1);
							}
						}
						break;
					}
			}

		}

		uint8_t debug1, debug2;

		if(conteo_columna_derecha == 3){
			getMovAliens()->alien_columna_derecha= getMovAliens()->alien_columna_derecha - 1;

		}

		if(conteo_columna_izquierda == 3){
			getMovAliens()->alien_columna_izquierda= getMovAliens()->alien_columna_izquierda + 1;
		}

		if(getMovAliens()->alien_columna_izquierda == getMovAliens()->alien_columna_derecha){

			debug1 = getMovAliens()->alien_columna_derecha;
			debug2 = getMovAliens()->alien_columna_izquierda;
		}


}


void disparoInit(){

	//Inicializacion del disparo del Player
	getDisparo()->posicion_x = 0;
	getDisparo()->posicion_y = POSICION_INICIAL_DISPARO;
	getDisparo()->disparo = false;
	getDisparo()->numero_disparos = 0;

	//Inicializacion del Disparo de los Aliens
	getDisparoAliens()->numero_disparos = 21;
}


void disparar(){


			if(getDisparo()->numero_disparos == 1){
				getDisparo()->disparo = true;
				getDisparo()->posicion_x = getPlayer()->posicion_X;
			}


			if(getDisparo()->disparo == true){
				SSD1306_DrawLine(getDisparo()->posicion_x + 4, getDisparo()->posicion_y, getDisparo()->posicion_x + 4, getDisparo()->posicion_y + 2, 1);
				getDisparo()->posicion_y = getDisparo()->posicion_y - VELOCIDAD_DISPARO_PLAYER;


				if(getDisparo()->posicion_y <= 0){
					//Se reinician los valores del disparo
					getDisparo()->disparo = false;
					getDisparo()->posicion_y = POSICION_INICIAL_DISPARO;
					getDisparo()->numero_disparos = 0;
				}
			}


			//Eliminar los aliens si se produce un disparo. Se fija si el disparo se encuentra en el area comprendido por cada alien.
			for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){

				uint8_t tamano_alien = TAMANO_ALIEN_TOP;

				switch(y){
				case 0:
					tamano_alien = TAMANO_ALIEN_TOP;
					break;
				case 1:
					tamano_alien = TAMANO_ALIEN_MIDDLE;
					break;
				case 2:
					tamano_alien = TAMANO_ALIEN_BOTTOM;
					break;

				}

				for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){

					if( (getDisparo()->posicion_x + 1 >= getAlien(y,x)->posicion_X) &  (getDisparo()->posicion_x + 1 <= (getAlien(y,x)->posicion_X + tamano_alien)) & ((getDisparo()->posicion_y) >= getAlien(y,x)->posicion_Y) & (getDisparo()->posicion_y <= getAlien(y,x)->posicion_Y + tamano_alien)){

								if(getAlien(y,x)->vivo == true){
									getAlien(y,x)->vivo = false;
									getAlien(y,x)->explosion = true;

									//Se reinician los valores del disparo
									getDisparo()->disparo = false;
									getDisparo()->posicion_y = POSICION_INICIAL_DISPARO;
									getDisparo()->numero_disparos = 0;


									//Conteo de la cantidad de aliens eliminados (esto para la funcion disparoAliens())
									//getDisparoAliens()->numero_disparos se interpretará como aliens eliminados.
									getDisparoAliens()->numero_disparos = getDisparoAliens()->numero_disparos - 1;


									//Puntajes del Player
									switch(y){
									case 0:
										getPlayer()->puntaje = (getPlayer()->puntaje) + (PUNTAJE_ALIEN_TOP*(getPlayer()->nivel));
										break;
									case 1:
										getPlayer()->puntaje = (getPlayer()->puntaje) + (PUNTAJE_ALIEN_MIDDLE*(getPlayer()->nivel));
										break;
									case 2:
										getPlayer()->puntaje = (getPlayer()->puntaje) + (PUNTAJE_ALIEN_BOTTOM*(getPlayer()->nivel));
										break;

									}


								}


					}

					//Se grafican las explosiones, como la pantalla se refresca muy rapido, las se utiliza una variable 'conteo_explosiones' para
					//que se grafique la explosion por mas tiempo y cuando conteo_explosiones = 0 ahi se deja de graficar las explosiones.
					//Tambien se definio una variable explosion, si es true se entra a este if y se decrementa conteo_explosiones.
					if(getAlien(y,x)->explosion == true){
						SSD1306_DrawBitmap(getAlien(y,x)->posicion_X, getAlien(y,x)->posicion_Y,explosion,16,8,1);
						getAlien(y,x)->conteo_explosion = getAlien(y,x)->conteo_explosion - 1;

						if(getAlien(y,x)->conteo_explosion == 0){
							getAlien(y,x)->explosion = false;
						}
					}
				}
			}
}


void disparoAliens(){


	if(getDisparoAliens()->numero_disparos != 0){	//Si no hay aliens vivos.

				if(getDisparoAliens()->disparo == true){

					getDisparoAliens()->posicion_y = getDisparoAliens()->posicion_y + dificultad.velocidad_disparo_aliens;


					if( (getDisparoAliens()->posicion_x + 1 >= getPlayer()->posicion_X) &  (getDisparoAliens()->posicion_x + 1 <= getPlayer()->posicion_X + 6) ){
						getDisparoAliens()->disparo = false;
						getPlayer()->vidas = getPlayer()->vidas - 1;


						//Si me quedo sin vidas, el player se elimina.
						if(getPlayer()->vidas == 0){
							getPlayer()->vivo = false;
						}
					}

					else if(getDisparoAliens()->posicion_y >= 60){
						getDisparoAliens()->disparo = false;
					}

				}
				else{

					uint8_t flag = false;

					while(flag != true){


						uint8_t fila = rand() % NUM_ALIEN_FILA;
						uint8_t columna = rand() % NUM_ALIEN_COLUMNA;

						if(getAlien(fila, columna)->vivo == true){
							flag = true;
							getDisparoAliens()->disparo = true;

							getDisparoAliens()->posicion_x = getAlien(fila, columna)->posicion_X + (TAMANO_ALIEN/2);
							getDisparoAliens()->posicion_y = getAlien(fila, columna)->posicion_Y + (TAMANO_ALIEN/2);
						}

					}
				}

				SSD1306_DrawBitmap(getDisparoAliens()->posicion_x, getDisparoAliens()->posicion_y, AlienBombGfx, 2, 4, 1);

	}

}


void plotBases(){

	SSD1306_DrawBitmap(16, 45, BaseGfx, 16, 8, 1);
	SSD1306_DrawBitmap(56, 45, BaseGfx, 16, 8, 1);
	SSD1306_DrawBitmap(96, 45, BaseGfx, 16, 8, 1);

}


void AumentoNivel(){

	//Incremento de la dificultad (Aumento de la velocidad de los aliens y la velocidad de disparo)
	//Se aumenta la dificultad.
	getDificultad()->velocidad_horizontal = 6;
	getDificultad()->velocidad_bajada = 2;
	getDificultad()->velocidad_disparo_aliens = 3;


	//Se Reinicializa el Player.
	getPlayer()->posicion_X = 64;
	getPlayer()->vivo = true;
	getPlayer()->nivel = getPlayer()->nivel + 1;

}






