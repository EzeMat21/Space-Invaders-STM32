/*
 * defines.h
 *
 *  Created on: Jan 21, 2024
 *      Author: ezema
 */

#ifndef INC_DEFINES_H_
#define INC_DEFINES_H_

#define true 1
#define false 0


//Esto para interpretar los botones del joystick.
enum{

	arriba,
	abajo,
	derecha,
	izquierda,
	nulo

};


//Para la cola de notificacion.
#define NOTIFICATION_VALUE 1

#define UMBRAL_DERECHA 2200
#define UMBRAL_IZQUIERDA 1600
#define UMBRAL_ABAJO 2200
#define UMBRAL_ARRIBA 1600



#endif /* INC_DEFINES_H_ */
