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
	getMenu()->menuActual = menu_principal;
	getMenu()->posicion_MenuPrincipal = POSICION_CURSOR_JUGAR;


}

void menuActualizar(uint8_t x, uint8_t y, uint8_t boton){


	uint8_t buffer[2];

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

		//Nombres
		SSD1306_GotoXY(15, 13);
		SSD1306_Puts("eze", &Font_7x10, 1);

		SSD1306_GotoXY(15, 23);
		//osMutexAcquire(mutexPuntajesHandle, osWaitForever);

		SSD1306_Puts(getPuntajes(0)->nombre, &Font_7x10, 1);
		//osMutexRelease(mutexPuntajesHandle);


		SSD1306_GotoXY(15, 33);
		SSD1306_Puts("xd", &Font_7x10, 1);
		SSD1306_GotoXY(15, 43);
		SSD1306_Puts("que onda", &Font_7x10, 1);
		SSD1306_GotoXY(15, 53);
		SSD1306_Puts("fiufiu", &Font_7x10, 1);

		//Puntuaciones
		SSD1306_GotoXY(90, 13);
		itoa(getPuntajes(0)->puntaje,(char*)buffer,10);
		SSD1306_Puts((char *)buffer, &Font_7x10, 1);
		SSD1306_GotoXY(90, 23);
		SSD1306_Puts("100", &Font_7x10, 1);
		SSD1306_GotoXY(90, 33);
		SSD1306_Puts("200", &Font_7x10, 1);
		SSD1306_GotoXY(90, 43);
		SSD1306_Puts("300", &Font_7x10, 1);
		SSD1306_GotoXY(90, 53);
		SSD1306_Puts("34", &Font_7x10, 1);


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

