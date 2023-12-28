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

	getMenu()->menuActual = menu_principal;

	SSD1306_GotoXY(50, 35);
	SSD1306_Puts("JUGAR", &Font_7x10, 1);
	SSD1306_GotoXY(50, 45);
	SSD1306_Puts("PUNTAJES", &Font_7x10, 1);

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

		/*
		if(x == izquierda){
			SSD1306_DrawCircle(43, 37, 3, 1);
		}
		else{
			SSD1306_DrawCircle(43, 48, 3, 1);
		}*/

		switch (getMenu()->accion) {
			case enter:

				if(opciones_principal == principal_jugar ){

					getMenu()->menuActual = juego;
				}

				else if(opciones_principal == principal_puntajes ){

					getMenu()->menuActual = puntajes;
				}

				break;
			case arriba:
				break;
			case abajo:
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
		if(boton == enter){

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

		break;

//-------------------------------------------------------------- GUARDADO DEL NOMBRE-----------------------------------------------------------------------------
	case guardar_nombre:
		break;


//-------------------------------------------------------------- GAME OVER -----------------------------------------------------------------------------
	case game_over:

		SSD1306_GotoXY(35, 25);
		SSD1306_Puts("GAME OVER", &Font_7x10, 1);

		getMenu()->accion = boton;

		switch(getMenu()->accion){
			case enter:
				getMenu()->menuActual = puntajes;
				break;
			default:
				break;
		}

		break;
	default:
		break;
	}

}

