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
base_t bases[3];
//uint8_t bases[3][16];

//----------------------------------------------------------------------------------------------------------------------------------------------


player_t getPlayer(){

	return Player;
}

disparo_t *getDisparo(){
	return &Disparo;
}

disparo_t getDisparoAliens(){
	return Disparo_Aliens;
}

//----------------------------------------------------------------------------------------------------------------------------------------------


void actualizarPantalla(){

    SSD1306_UpdateScreen(); // update screen
    SSD1306_Fill(0);
}


void playerInit(){

	Player.posicion_X = 64;
	Player.vivo = true;
	Player.puntaje = 0;
	Player.vidas = 5;
	Player.nivel = 1;

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

	for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
		for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){
			Alien[y][x].posicion_X  =  OFFSET_INICIO + x*(TAMANO_ALIEN+ ESPACIO_ENTRE_COLUMNAS);
			Alien[y][x].posicion_Y = y*(TAMANO_ALIEN+ ESPACIO_ENTRE_FILAS);
			Alien[y][x].vivo = true;
			Alien[y][x].conteo_explosion = 2;
		}
	}

}

void plotAliens(){

	//Esta parte se encarga de ir moviendo los aliens de izquierda a derecha, derecha a izquierda.
	if(movimiento_Aliens.conteo_horizontal % dificultad.velocidad_horizontal == 0){

		movimiento_Aliens.animacion = !movimiento_Aliens.animacion;

			switch(movimiento_Aliens.orientacion){
				case mov_derecha:

						for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
							for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){

								Alien[y][x].posicion_X  = Alien[y][x].posicion_X + 2;


							}
						}

					break;

				case mov_izquierda:

						for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){
							for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){

								Alien[y][x].posicion_X  = Alien[y][x].posicion_X - 2;

							}
						}

						break;
				default:
					break;
		}


	//Aqui se cambia la orientacion de los aliens: cuando se llega al borde de la pantalla los aliens pasan a moverse hacia la derecha o hacia la izquierda.

			if( (Alien[0][movimiento_Aliens.alien_columna_derecha].posicion_X + TAMANO_ALIEN_BOTTOM) > 127){

				movimiento_Aliens.orientacion = mov_izquierda;
			}


			if( (Alien[0][movimiento_Aliens.alien_columna_izquierda].posicion_X) < 0){
				movimiento_Aliens.orientacion = mov_derecha;
				movimiento_Aliens.conteo_bajada = movimiento_Aliens.conteo_bajada + 1;
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

	movimiento_Aliens.conteo_horizontal = movimiento_Aliens.conteo_horizontal + 1;


	uint8_t conteo_columna_derecha = 0;
	uint8_t conteo_columna_izquierda = 0;

	//Acá se grafican los aliens en la pantalla.
	//Se grafican los aliens que solo estan vivos.
		for(uint8_t y=0; y<NUM_ALIEN_FILA ;y++){

			if(movimiento_Aliens.alien_columna_derecha != movimiento_Aliens.alien_columna_izquierda){

				if(Alien[y][movimiento_Aliens.alien_columna_derecha].vivo == false){
					conteo_columna_derecha++;
				}

				else if(Alien[y][movimiento_Aliens.alien_columna_izquierda].vivo == false){
					conteo_columna_izquierda++;
				}

			}

			for(uint8_t x=0; x<NUM_ALIEN_COLUMNA; x++){

					switch(y){

					case 0:
						if(Alien[y][x].vivo == true){

							if(movimiento_Aliens.animacion == true ){
								SSD1306_DrawBitmap(Alien[y][x].posicion_X, Alien[y][x].posicion_Y,InvaderTopGfx,8,8,1);
								//movimiento_Aliens.animacion = false;
							}
							else{
								SSD1306_DrawBitmap(Alien[y][x].posicion_X, Alien[y][x].posicion_Y,InvaderTopGfx2,8,8,1);
								//movimiento_Aliens.animacion = true;
							}

						}
						break;
					case 1:
						if(Alien[y][x].vivo == true){
							if(movimiento_Aliens.animacion == true ){
								SSD1306_DrawBitmap(Alien[y][x].posicion_X, Alien[y][x].posicion_Y,InvaderMiddleGfx,16,8,1);
								//movimiento_Aliens.animacion = false;
							}
							else{
								SSD1306_DrawBitmap(Alien[y][x].posicion_X, Alien[y][x].posicion_Y,InvaderMiddleGfx2,16,8,1);
								//movimiento_Aliens.animacion = true;
							}
						}
						break;
					default:
						if(Alien[y][x].vivo == true){
							if(movimiento_Aliens.animacion == true ){
								SSD1306_DrawBitmap(Alien[y][x].posicion_X, Alien[y][x].posicion_Y,InvaderBottomGfx,16,8,1);
							}
							else{
								SSD1306_DrawBitmap(Alien[y][x].posicion_X, Alien[y][x].posicion_Y,InvaderBottomGfx2,16,8,1);
							}
						}
						break;
					}
			}

		}

		//uint8_t debug1, debug2;

		if(conteo_columna_derecha == NUM_ALIEN_FILA){
			movimiento_Aliens.alien_columna_derecha = movimiento_Aliens.alien_columna_derecha - 1;

		}

		if(conteo_columna_izquierda == NUM_ALIEN_FILA){
			movimiento_Aliens.alien_columna_izquierda = movimiento_Aliens.alien_columna_izquierda + 1;
		}

		/*if(movimiento_Aliens.alien_columna_izquierda == movimiento_Aliens.alien_columna_derecha){

			debug1 = movimiento_Aliens.alien_columna_derecha;
			debug2 = movimiento_Aliens.alien_columna_izquierda;
		}*/


}


void disparoInit(){

	//Inicializacion del disparo del Player
	Disparo.posicion_x = 0;
	Disparo.posicion_y = POSICION_INICIAL_DISPARO;
	Disparo.disparo = false;
	Disparo.numero_disparos = 0;

	//Inicializacion del Disparo de los Aliens
	Disparo_Aliens.numero_disparos = 21;
}


void disparar(){


			if(Disparo.numero_disparos == 1){
				Disparo.disparo = true;
				Disparo.posicion_x = Player.posicion_X;
			}


			if(Disparo.disparo == true){
				SSD1306_DrawLine(Disparo.posicion_x + 4, Disparo.posicion_y, Disparo.posicion_x + 4, Disparo.posicion_y + 2, 1);
				Disparo.posicion_y = Disparo.posicion_y - VELOCIDAD_DISPARO_PLAYER;


				if(Disparo.posicion_y <= 0){
					//Se reinician los valores del disparo
					Disparo.disparo = false;
					Disparo.posicion_y = POSICION_INICIAL_DISPARO;
					Disparo.numero_disparos = 0;
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

					if( (Disparo.posicion_x + 1 >= Alien[y][x].posicion_X) &  (Disparo.posicion_x + 1 <= (Alien[y][x].posicion_X + tamano_alien)) & ((Disparo.posicion_y) >= Alien[y][x].posicion_Y) & (Disparo.posicion_y <= Alien[y][x].posicion_Y + tamano_alien)){

								if(Alien[y][x].vivo == true){
									Alien[y][x].vivo = false;
									Alien[y][x].explosion = true;

									//Se reinician los valores del disparo
									Disparo.disparo = false;
									Disparo.posicion_y = POSICION_INICIAL_DISPARO;
									Disparo.numero_disparos = 0;


									//Conteo de la cantidad de aliens eliminados (esto para la funcion disparoAliens())
									//Disparo_Aliens.numero_disparos se interpretará como aliens eliminados.
									Disparo_Aliens.numero_disparos = Disparo_Aliens.numero_disparos - 1;


									//Puntajes del Player
									switch(y){
									case 0:
										Player.puntaje = (Player.puntaje) + (PUNTAJE_ALIEN_TOP+ (PUNTAJE_ADICIONAL*Player.nivel));
										break;
									case 1:
										Player.puntaje = (Player.puntaje) + (PUNTAJE_ALIEN_MIDDLE + (PUNTAJE_ADICIONAL*Player.nivel));
										break;
									case 2:
										Player.puntaje = (Player.puntaje) + (PUNTAJE_ALIEN_BOTTOM+ (PUNTAJE_ADICIONAL*Player.nivel));
										break;

									}


								}


					}

					//Se grafican las explosiones, como la pantalla se refresca muy rapido, las se utiliza una variable 'conteo_explosiones' para
					//que se grafique la explosion por mas tiempo y cuando conteo_explosiones = 0 ahi se deja de graficar las explosiones.
					//Tambien se definio una variable explosion, si es true se entra a este if y se decrementa conteo_explosiones.
					if(Alien[y][x].explosion == true){
						SSD1306_DrawBitmap(Alien[y][x].posicion_X, Alien[y][x].posicion_Y,explosion,16,8,1);
						Alien[y][x].conteo_explosion = Alien[y][x].conteo_explosion - 1;

						if(Alien[y][x].conteo_explosion == 0){
							Alien[y][x].explosion = false;
						}
					}
				}
			}
}


void disparoAliens(){


	if(Disparo_Aliens.numero_disparos != 0){	//Si no hay aliens vivos.

				if(Disparo_Aliens.disparo == true){

					Disparo_Aliens.posicion_y = Disparo_Aliens.posicion_y + dificultad.velocidad_disparo_aliens;


					if( (Disparo_Aliens.posicion_x + 1 >= Player.posicion_X) &  (Disparo_Aliens.posicion_x + 1 <= Player.posicion_X + 6) & ((Disparo_Aliens.posicion_y) >= TOTAL_PIXELES_Y-TAMANO_NAVE_Y) & (Disparo.posicion_y <= TOTAL_PIXELES_Y)){

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

	SSD1306_DrawBitmap(LIMITE_IZQ_X_BASE1, LIMITE_ABAJO_BASE, bases[0].buffer, 16, 8, 1);
	SSD1306_DrawBitmap(LIMITE_IZQ_X_BASE2, LIMITE_ABAJO_BASE, bases[1].buffer, 16, 8, 1);
	SSD1306_DrawBitmap(LIMITE_IZQ_X_BASE3, LIMITE_ABAJO_BASE, bases[2].buffer, 16, 8, 1);

	//Colisiones

	uint16_t resultado_or[3];


	for(uint8_t i=0;i<3;i++){
		for(uint8_t j=0; j<16;j++){


			if(j % 2 == 0){	//Me fijo si una columna entera del lado izquierdo esta destruido.

				resultado_or[i] = (resultado_or[i] | (bases[i].buffer[j] <<8)) ;
			}
			else{
				resultado_or[i] = resultado_or[i] | (bases[i].buffer[j]);
			}
		}
	}



	const uint8_t limites_izquierda[] = {LIMITE_IZQ_X_BASE1, LIMITE_IZQ_X_BASE2, LIMITE_IZQ_X_BASE3};
	const uint8_t limites_derecha[] = {LIMITE_DER_X_BASE1, LIMITE_DER_X_BASE2, LIMITE_DER_X_BASE3};

	if((Disparo.posicion_y >= LIMITE_ARRIBA_BASE) &  (Disparo.posicion_y < LIMITE_ABAJO_BASE)){

		// Iterar sobre las bases
		for (int i = 0; i < 3; ++i) {
			// Verificar si el disparo está dentro de la base actual
			if ((Disparo.posicion_x + POS_OFF_DISPARO_PLAYER) >= limites_izquierda[i] &&
				(Disparo.posicion_x + POS_OFF_DISPARO_PLAYER) <= limites_derecha[i]) {

				// Calcular el desplazamiento y el resultado correspondiente
				uint8_t desplazamiento = (Disparo.posicion_x + POS_OFF_DISPARO_PLAYER) - limites_izquierda[i];
				uint16_t resultado = 1 << (OFFSET_DESPLAZAMIENTO - desplazamiento);

				// Verificar si el resultado coincide con algún resultado OR almacenado
				if ((resultado & resultado_or[i]) != 0) {
					// Se reinician los valores del disparo
					Disparo.disparo = false;
					Disparo.posicion_y = POSICION_INICIAL_DISPARO;
					Disparo.numero_disparos = 0;


					//Generar la destruccion de la base
					uint8_t desplazamiento_horiz = (rand() % RANDOM_MAX_2) + 1;

					for(uint8_t j=0;j<desplazamiento_horiz; j++){

						resultado = resultado + (1 << (OFFSET_DESPLAZAMIENTO + j - desplazamiento));
						resultado = resultado + (1 << (OFFSET_DESPLAZAMIENTO - j - desplazamiento));
					}

					uint16_t bases_16bits;
					uint8_t k = 0;
					uint8_t conteo = 0;

					uint16_t debug1, debug2;

					for(uint8_t j=0; j<16;j++){

						if(j % 2 != 0){

							bases_16bits = (bases[i].buffer[15-j]<<8) + (bases[i].buffer[15-j+1] & 0xff);
							k++;

							if( (bases_16bits & resultado) != 0){

								debug1 = ~(resultado >> 8);
								debug2 = ~resultado;
								bases[i].buffer[15-j] = bases[i].buffer[15-j] & ~(resultado>>8);
								bases[i].buffer[15-j+1] = bases[i].buffer[15-j+1] & ~(resultado);
								conteo++;
							}
							if(conteo == 3){
								break;
							}
						}

					}




				}

				// Terminar la iteración una vez que se encuentra una base válida
				break;
			}
		}

	}






}

void BasesInit(){

	for(uint8_t i=0;i<3;i++){
		for(uint8_t j=0; j<16; j++){

			bases[i].buffer[j] = BaseGfx[j];
		}
	}

}

void Dificultad_Init(){

	//Se inicializan las dificultades
	dificultad.velocidad_horizontal = 8;
	dificultad.velocidad_bajada = 1;
	dificultad.velocidad_disparo_aliens = 3;
}


void AumentoNivel(){

	//Incremento de la dificultad (Aumento de la velocidad de los aliens y la velocidad de disparo)
	//Se aumenta la dificultad.
	dificultad.velocidad_horizontal = 6;
	dificultad.velocidad_bajada = 2;
	dificultad.velocidad_disparo_aliens = 3;


	//Se Reinicializa el Player.
	Player.posicion_X = 64;
	Player.vivo = true;
	Player.nivel = Player.nivel + 1;

}






