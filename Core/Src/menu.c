/*
 * menu.c
 *
 *  Created on: Dec 26, 2023
 *      Author: ezema
 */


#include "menu.h"

menu_t menu;


menu_t *getMenu(){

	return &menu;
}


void menuInit(){

	//Se inicializan las posiciones iniciales del player y de los aliens.
	playerInit();
	InvaderInit();
	disparoInit();

	//Se inicializa el cursor de la pantalla principal.
	getMenu()->menuActual = guardar_nombre;
	getMenu()->posicion_MenuPrincipal = POSICION_CURSOR_JUGAR;


}

void menuActualizar(uint8_t x, uint8_t y, uint8_t boton){


	switch(getMenu()->menuActual){

//--------------------------------------------------------------MENU PRINCIPAL-----------------------------------------------------------------------------
	case menu_principal:

		SSD1306_DrawBitmap(35, 0, SpaceInvaders, 64, 32, 1);

		SSD1306_GotoXY(45, 35);
		SSD1306_Puts("JUGAR", &Font_7x10, 1);
		SSD1306_GotoXY(45, 45);
		SSD1306_Puts("PUNTAJES", &Font_7x10, 1);


		if(y == arriba){
			getMenu()->posicion_MenuPrincipal = POSICION_CURSOR_JUGAR;
		}
		else if(y == abajo){
			getMenu()->posicion_MenuPrincipal = POSICION_CURSOR_PUNTAJES;
		}

		SSD1306_DrawCircle(POSICION_CURSOR_X, getMenu()->posicion_MenuPrincipal, 3, 1);


		switch(boton){
		case true:
			if(getMenu()->posicion_MenuPrincipal == POSICION_CURSOR_JUGAR){
				getMenu()->menuActual = juego;
			}
			else if(getMenu()->posicion_MenuPrincipal == POSICION_CURSOR_PUNTAJES){
				getMenu()->menuActual = puntajes;
			}
			break;
		case false:
			break;
		default:
			break;
		}

		break;

//--------------------------------------------------------------  JUEGO -----------------------------------------------------------------------------

	case juego:

		//Se grafican el player, aliens y disparo.
		plotPlayer(x, getPlayer());
		plotAliens();

		disparar();


		//Prender led si se apretó el boton
		if(boton == true){

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
			//boton_apretado = 1;
			getDisparo()->numero_disparos = getDisparo()->numero_disparos + 1;
		}
		else{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET);
		}

		break;

//-------------------------------------------------------------- PUNTAJES-----------------------------------------------------------------------------
	case puntajes:

		//Titulo 'Puntajes'
		SSD1306_GotoXY(35, 0);
		SSD1306_Puts("PUNTAJES", &Font_7x10, 1);

		uint8_t posicion;
		uint8_t buffer[2];

		osMutexAcquire(mutexPuntajesHandle, osWaitForever);

		//char buff_retorno[5][6];

		//Nombres

		for(uint8_t j=0; j<5;j++){

			posicion = j*10 + 13;

			SSD1306_GotoXY(15, posicion);
			//strcpy(buff_retorno[j], "jdsa");
			SSD1306_Puts(getPuntajes(j)->nombre, &Font_7x10, 1);
		}


		//Puntuaciones
		for(uint8_t j=0; j<5;j++){

			posicion = j*10 + 13;

			SSD1306_GotoXY(90, posicion);
			itoa(getPuntajes(j)->puntaje,(char*)buffer,10);
			SSD1306_Puts((char *)buffer, &Font_7x10, 1);
		}

		osMutexRelease(mutexPuntajesHandle);


		switch(y){
		case arriba:
				getMenu()->menuActual = menu_principal;
			break;
		default:
			break;
		}
		break;

//-------------------------------------------------------------- GUARDADO DEL NOMBRE-----------------------------------------------------------------------------
	case guardar_nombre:


		char buff_qwerty[] = "qwertyuiop";
		char buff_asdf[] = "asdfghjkl";
		char buff_zxc[] = "zxcvbnm";

		uint8_t posicion_x;

		SSD1306_DrawRectangle(35, 0, 50, 15, 1);
		SSD1306_GotoXY(37, 5);
		SSD1306_Puts("capaz", &Font_7x10, 1);


			for(uint8_t i=0;i<10;i++){

				posicion_x = i*(12) + 6;

				SSD1306_GotoXY(posicion_x, 23);
				SSD1306_Putc(buff_qwerty[i], &Font_7x10, 1);
				SSD1306_GotoXY(posicion_x, 38);
				SSD1306_Putc(buff_asdf[i], &Font_7x10, 1);

			}


			for(uint8_t i=0;i<7;i++){

				posicion_x = i*(12) + 18;

				SSD1306_GotoXY(posicion_x, 53);
				SSD1306_Putc(buff_zxc[i], &Font_7x10, 1);
			}

			SSD1306_DrawLine(6, 23 + 10, 12, 23+10, 1);


			switch(y){
				case arriba:

					break;

				case abajo:
					break;
				default:
					break;
				}
				break;

			switch(x){
				case izquierda:

					break;
				case derecha:
					break;
				default:
					break;
				}
				break;


				if(boton == true){

				}
				else{

				}

		break;


//-------------------------------------------------------------- GAME OVER -----------------------------------------------------------------------------
	case game_over:

		SSD1306_GotoXY(35, 25);
		SSD1306_Puts("GAME OVER", &Font_7x10, 1);


		break;
	default:
		break;
	}

}











