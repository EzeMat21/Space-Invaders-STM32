/*
 * menu.c
 *
 *  Created on: Dec 26, 2023
 *      Author: ezema
 */


#include "menu.h"
#include "invaders.h"

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

		SSD1306_GotoXY(35, 25);
		SSD1306_Puts("PUNTAJES", &Font_7x10, 1);


		switch(boton){
		case true:
				getMenu()->menuActual = menu_principal;
			break;
		case false:
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

