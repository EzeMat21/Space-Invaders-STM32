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


//Dificultad, progresion de niveles
dificultad_t dificultad;

//Variables para las bases
uint8_t bases[3][16];
const uint8_t limites_izquierda[] = {LIMITE_IZQ_X_BASE1, LIMITE_IZQ_X_BASE2, LIMITE_IZQ_X_BASE3};
const uint8_t limites_derecha[] = {LIMITE_DER_X_BASE1, LIMITE_DER_X_BASE2, LIMITE_DER_X_BASE3};


//----------------------------------------------------------------------------------------------------------------------------------------------


player_t getPlayer(){

	return Player;
}

uint8_t getDisparodisparo(){
	return Disparo.disparo;
}

uint8_t getCantidad_Aliens_Vivos(){
	return movimiento_Aliens.cantidad_aliens_vivos;
}

//----------------------------------------------------------------------------------------------------------------------------------------------


void actualizarPantalla(){

    SSD1306_UpdateScreen(); // update screen
    SSD1306_Fill(0);
}


void playerInit(){

	Player.posicion_X = 64;
	Player.vivo = true;
	Player.puntaje = 2000;
	Player.vidas = 3;
	Player.nivel = 0;

}

void plotPlayer(uint8_t direccion){

	if(Player.vivo == true){

		if(direccion == derecha){

			if(Player.posicion_X >= 120){
				Player.posicion_X = 120;
			}
			else{
				Player.posicion_X = Player.posicion_X + 4;
			}

		}
		else if(direccion == izquierda){

			if(Player.posicion_X <= 0){
				Player.posicion_X = 0;
			}
			else{
				Player.posicion_X = Player.posicion_X - 4;
			}
		}

		SSD1306_DrawBitmap(Player.posicion_X, 56, nave, 8,8,1);
	}

	else{	//Si el jugador ha sido eliminado.

		SSD1306_DrawBitmap(Player.posicion_X, 56, nave_explotada, 8,8,1);
	}


}



void InvaderInit(){

	movimiento_Aliens.orientacion = mov_derecha;
	movimiento_Aliens.alien_columna_derecha = NUM_ALIEN_COLUMNA - 1;
	movimiento_Aliens.alien_columna_izquierda = 0;

	//Inicializacion cantidad aliens vivos.
	movimiento_Aliens.cantidad_aliens_vivos = 21;

	for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
		for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){
			Alien[y][x].posicion_X  =  OFFSET_INICIO + x*(TAMANO_ALIEN+ ESPACIO_ENTRE_COLUMNAS);
			Alien[y][x].posicion_Y = y*(TAMANO_ALIEN+ ESPACIO_ENTRE_FILAS);
			Alien[y][x].vivo = true;
			Alien[y][x].explosion = false;
			Alien[y][x].conteo_explosion = 2;
		}
	}


	//Reinicio velocidad horizontal de los aliens
	dificultad.velocidad_horizontal = 8;
	dificultad.aumento_posicion_pixeles = AUMENTO_POSICION_PIXELES_2;


}

void plotAliens(){


	AumentoVelocidadAliens();

	//Esta parte se encarga de ir moviendo los aliens de izquierda a derecha, derecha a izquierda.
	if(movimiento_Aliens.conteo_horizontal % dificultad.velocidad_horizontal == 0){

		movimiento_Aliens.animacion = !movimiento_Aliens.animacion;

			switch(movimiento_Aliens.orientacion){
				case mov_derecha:

						for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
							for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){

								Alien[y][x].posicion_X  = Alien[y][x].posicion_X + dificultad.aumento_posicion_pixeles;


							}
						}

					break;

				case mov_izquierda:

						for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
							for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){

								Alien[y][x].posicion_X  -= dificultad.aumento_posicion_pixeles;

							}
						}

						break;
				default:
					break;
		}


	//Aqui se cambia la orientacion de los aliens: cuando se llega al borde de la pantalla los aliens pasan a moverse hacia la derecha o hacia la izquierda.

			if( (Alien[0][movimiento_Aliens.alien_columna_derecha].posicion_X + TAMANO_ALIEN_BOTTOM_ANCHURA) >= LIMITE_PANTALLA_DER){

				movimiento_Aliens.orientacion = mov_izquierda;
			}


			if( (Alien[0][movimiento_Aliens.alien_columna_izquierda].posicion_X) <= LIMITE_PANTALLA_IZQ){
				movimiento_Aliens.orientacion = mov_derecha;
				movimiento_Aliens.conteo_bajada++;
			}



		//Esta parte se encarga de ir bajando los aliens.
			if(movimiento_Aliens.conteo_bajada == 10){
				for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
					for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){

						Alien[y][x].posicion_Y  = Alien[y][x].posicion_Y + dificultad.velocidad_bajada;
						movimiento_Aliens.conteo_bajada = 0;

					}
				}
			}

	}

	movimiento_Aliens.conteo_horizontal++;


	uint8_t conteo_columna_derecha = 0;
	uint8_t conteo_columna_izquierda = 0;

	//Acá se grafican los aliens en la pantalla.
	//Se grafican los aliens que solo estan vivos.
		for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){

			if(movimiento_Aliens.alien_columna_derecha != movimiento_Aliens.alien_columna_izquierda){

				if(Alien[y][movimiento_Aliens.alien_columna_derecha].vivo == false){
					conteo_columna_derecha++;
				}

				if(Alien[y][movimiento_Aliens.alien_columna_izquierda].vivo == false){
					conteo_columna_izquierda++;
				}

			}

			for(uint8_t x = 0; x < NUM_ALIEN_COLUMNA; x++) {
			    if(Alien[y][x].vivo == true) {
			        const uint8_t *gfx;
			        if(movimiento_Aliens.animacion == true) {
			            gfx = (y == 0) ? InvaderTopGfx : (y == 1) ? InvaderMiddleGfx : InvaderBottomGfx;
			        } else {
			            gfx = (y == 0) ? InvaderTopGfx2 : (y == 1) ? InvaderMiddleGfx2 : InvaderBottomGfx2;
			        }
			        SSD1306_DrawBitmap(Alien[y][x].posicion_X, Alien[y][x].posicion_Y, gfx, (y == 0) ? 8 : 16, 8, 1);
			    }
			}

		}

		if(conteo_columna_derecha == NUM_ALIEN_FILA){
			movimiento_Aliens.alien_columna_derecha--;

		}

		if(conteo_columna_izquierda == NUM_ALIEN_FILA){
			movimiento_Aliens.alien_columna_izquierda++;
		}


}


void disparoInit(){

	//Inicializacion del disparo del Player
	Disparo.posicion_x = 0;
	Disparo.posicion_y = POSICION_INICIAL_DISPARO;
	Disparo.disparo = false;

}


void disparar(){

			if(Disparo.disparo == true){
				SSD1306_DrawLine(Disparo.posicion_x + 4, Disparo.posicion_y, Disparo.posicion_x + 4, Disparo.posicion_y + 2, 1);
				Disparo.posicion_y = Disparo.posicion_y - VELOCIDAD_DISPARO_PLAYER;


				if(Disparo.posicion_y <= 0){
					//Se reinician los valores del disparo
					Disparo.disparo = false;
					Disparo.posicion_y = POSICION_INICIAL_DISPARO;
				}
			}


			//Eliminar los aliens si se produce un disparo. Se fija si el disparo se encuentra en el area comprendido por cada alien.
			for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){

				uint8_t tamano_alien = TAMANO_ALIEN_TOP_ANCHURA;

				switch(y){
				case 0:
					tamano_alien = TAMANO_ALIEN_TOP_ANCHURA;
					break;
				case 1:
					tamano_alien = TAMANO_ALIEN_MIDDLE_ANCHURA;
					break;
				case 2:
					tamano_alien = TAMANO_ALIEN_BOTTOM_ANCHURA;
					break;

				}

				for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){

					if( (Disparo.posicion_x + 1 >= Alien[y][x].posicion_X) &  (Disparo.posicion_x + 1 <= (Alien[y][x].posicion_X + tamano_alien))
							& ((Disparo.posicion_y) >= Alien[y][x].posicion_Y) & (Disparo.posicion_y <= Alien[y][x].posicion_Y + TAMANO_ALIEN_ALTURA)){

								if(Alien[y][x].vivo == true){
									Alien[y][x].vivo = false;
									Alien[y][x].explosion = true;

									//Se reinician los valores del disparo
									Disparo.disparo = false;
									Disparo.posicion_y = POSICION_INICIAL_DISPARO;
									//Disparo.numero_disparos = 0;


									//Conteo de la cantidad de aliens eliminados
									movimiento_Aliens.cantidad_aliens_vivos -=1;


									//Puntajes del Player
									switch(y){
									case 0:
										Player.puntaje = (Player.puntaje) + (PUNTAJE_ALIEN_TOP + Player.nivel);
										break;
									case 1:
										Player.puntaje = (Player.puntaje) + (PUNTAJE_ALIEN_MIDDLE + Player.nivel);
										break;
									case 2:
										Player.puntaje = (Player.puntaje) + (PUNTAJE_ALIEN_BOTTOM + Player.nivel);
										break;

									}


								}


					}

					//Se grafican las explosiones, como la pantalla se refresca muy rapido, las se utiliza una variable 'conteo_explosiones' para
					//que se grafique la explosion por mas tiempo y cuando conteo_explosiones = 0 ahi se deja de graficar las explosiones.
					//Tambien se definio una variable explosion, si es true se entra a este if y se decrementa conteo_explosiones.
					if(Alien[y][x].explosion == true){
						SSD1306_DrawBitmap(Alien[y][x].posicion_X, Alien[y][x].posicion_Y,explosion,16,8,1);
						Alien[y][x].conteo_explosion--;

						if(Alien[y][x].conteo_explosion == 0){
							Alien[y][x].explosion = false;
						}
					}



				}
			}
}


void disparoAliens(){


	if(movimiento_Aliens.cantidad_aliens_vivos != 0){	//Si no hay aliens vivos.

				if(Disparo_Aliens.disparo == true){

					Disparo_Aliens.posicion_y = Disparo_Aliens.posicion_y + dificultad.velocidad_disparo_aliens;


					if( (Disparo_Aliens.posicion_x + 1 >= Player.posicion_X) &  (Disparo_Aliens.posicion_x + 1 <= Player.posicion_X + TAMANO_PLAYER_ANCHURA)
							& ((Disparo_Aliens.posicion_y) >= TOTAL_PIXELES_Y-TAMANO_NAVE_Y) & (Disparo.posicion_y <= TOTAL_PIXELES_Y)){

						Disparo_Aliens.disparo = false;
						Player.vidas = Player.vidas - 1;


						//Si me quedo sin vidas, el player se elimina.
						if(Player.vidas == 0){
							Player.vivo = false;
						}
					}

					else if(Disparo_Aliens.posicion_y >= 60){
						Disparo_Aliens.disparo = false;
					}

				}
				else{

					uint8_t flag = false;

					while(flag != true){


						uint8_t fila = rand() % NUM_ALIEN_FILA;
						uint8_t columna = rand() % NUM_ALIEN_COLUMNA;

						if(Alien[fila][columna].vivo == true){
							flag = true;
							Disparo_Aliens.disparo = true;

							Disparo_Aliens.posicion_x = Alien[fila][columna].posicion_X + (TAMANO_ALIEN/2);
							Disparo_Aliens.posicion_y = Alien[fila][columna].posicion_Y + (TAMANO_ALIEN/2);
						}

					}
				}

				SSD1306_DrawBitmap(Disparo_Aliens.posicion_x, Disparo_Aliens.posicion_y, AlienBombGfx, 2, 4, 1);

	}

}


void plotBases(){

	SSD1306_DrawBitmap(LIMITE_IZQ_X_BASE1, LIMITE_ABAJO_BASE, bases[0], 16, 8, 1);
	SSD1306_DrawBitmap(LIMITE_IZQ_X_BASE2, LIMITE_ABAJO_BASE, bases[1], 16, 8, 1);
	SSD1306_DrawBitmap(LIMITE_IZQ_X_BASE3, LIMITE_ABAJO_BASE, bases[2], 16, 8, 1);

	//Colisiones
	BasesProcesarDisparo(&Disparo_Aliens, 0);
	BasesProcesarDisparo(&Disparo, 1);


}


void BasesProcesarDisparo(disparo_t *disparo, uint8_t condicion){

	uint16_t resultado_or[3] = {0x00, 0x00, 0x00};
	uint16_t bases_16bits;
	uint8_t k = 0, conteo = 0;

	for(uint8_t i=0;i<3;i++){
	    for(uint8_t j=0; j<16;j++){
	        resultado_or[i] |= (j % 2 == 0) ? (bases[i][j] << 8) : bases[i][j];

	    }
	}

	if (disparo->posicion_y >= LIMITE_ARRIBA_BASE && disparo->posicion_y < LIMITE_ABAJO_BASE) {

		uint8_t pos_x = disparo->posicion_x;
	    if (condicion == 1) pos_x += POS_OFF_DISPARO_PLAYER;

	    for (int i = 0; i < 3; ++i) {
	        if (pos_x >= limites_izquierda[i] && pos_x <= limites_derecha[i]) {

	        	uint8_t desplazamiento = pos_x - limites_izquierda[i];
	            uint16_t resultado = (1 << (OFFSET_DESPLAZAMIENTO - desplazamiento));

	            if (condicion == 0){
	            	resultado += (1 << (OFFSET_DESPLAZAMIENTO + 1 - desplazamiento));
	            	resultado += (1 << (OFFSET_DESPLAZAMIENTO - 1 - desplazamiento));
	            }

	            if ((resultado & resultado_or[i]) != 0) {

	            	disparo->disparo = false;


	            	uint8_t desplazamiento_horiz = (rand() % RANDOM_MAX_3) + DESPLAZAMIENTO_MINIMO;
	            	uint8_t jmin = 2;

	            	if (condicion != 0) {
	                    disparo->posicion_y = POSICION_INICIAL_DISPARO;
	                    //disparo->numero_disparos = 0;
	                    jmin = 1;
	                }


	                for (uint8_t j=jmin; j<desplazamiento_horiz; j++) {
	                    resultado += (1 << (OFFSET_DESPLAZAMIENTO + j - desplazamiento));
	                    resultado += (1 << (OFFSET_DESPLAZAMIENTO - j - desplazamiento));
	                }


	                for (uint8_t j=0; j<16; j++) {
	                    if (j % 2 != 0) {
	                        if (condicion == 0) {
	                            bases_16bits = (bases[i][j-1]<<8) + (bases[i][j] & 0xff);
	                            k++;
	                            if ((bases_16bits & resultado) != 0) {
	                                bases[i][j-1] &= ~(resultado>>8);
	                                bases[i][j] &= ~resultado;
	                                conteo++;
	                            }
	                        } else {
	                            bases_16bits = (bases[i][15-j]<<8) + (bases[i][15-j+1] & 0xff);
	                            k++;
	                            if ((bases_16bits & resultado) != 0) {
	                                bases[i][15-j] &= ~(resultado>>8);
	                                bases[i][15-j+1] &= ~resultado;
	                                conteo++;
	                            }
	                        }
	                        if (conteo == DESTRUCCION_MAXIMA_VERTICAL) break;
	                    }
	                }
	            }
	            break;
	        }
	    }
	}

}



void BasesInit(){

	for(uint8_t i=0;i<3;i++){
		for(uint8_t j=0; j<16; j++){

			bases[i][j] = BaseGfx[j];
		}
	}

}


void Dificultad_Init(){

	//Se inicializan las dificultades
	dificultad.velocidad_horizontal = 8;
	dificultad.velocidad_bajada = 4;
	dificultad.velocidad_disparo_aliens = 3;
	dificultad.aumento_posicion_pixeles = AUMENTO_POSICION_PIXELES_2;
}


void AumentoNivel(){

	//Incremento de la dificultad (Aumento de la velocidad de los aliens y la velocidad de disparo)
	//Se aumenta la dificultad.
	dificultad.velocidad_bajada += 1;
	if(dificultad.velocidad_bajada > 10){
		dificultad.velocidad_bajada = 10;
	}


	dificultad.velocidad_disparo_aliens = 3;


	//Se Reinicializa el Player.
	Player.posicion_X = 64;
	Player.vivo = true;
	Player.nivel += 1;
	if(Player.nivel > 254){
		Player.nivel = 254;
	}

}

void AumentoVelocidadAliens(){


	//Cuando queden menos de 3 aliens (por ejemplo) que los aliens comiencen a moverse mas rapido.
	switch(movimiento_Aliens.cantidad_aliens_vivos){
	case 3:
		dificultad.velocidad_horizontal = 2;
		dificultad.aumento_posicion_pixeles = AUMENTO_POSICION_PIXELES_4;
		break;
	case 2:
		dificultad.velocidad_horizontal = 1;
		break;
	case 1:
		dificultad.aumento_posicion_pixeles = AUMENTO_POSICION_PIXELES_6;
		break;
	default:
		break;
	}

}


void disparoTrue(){

	Disparo.disparo = true;
	Disparo.posicion_x = Player.posicion_X;

}





