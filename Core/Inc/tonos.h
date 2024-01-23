/*
 * tonos.h
 *
 *  Created on: Jan 21, 2024
 *      Author: ezema
 */

#ifndef INC_TONOS_H_
#define INC_TONOS_H_

#include <stdint.h>

#define VECES_622HZ 170
#define VECES_587HZ 160
#define VECES_554HZ 150
#define VECES_TONOS_VARIABLES 10

#define TAMANO_BUFF_AUDIO_DISPARO 634
#define TAMANO_BUFF_AUDIO_EXPLOSION 640
#define TAMANO_BUFF_TONOS_VARIABLES 861
#define TAMANO_BUFF_TONO_622 14
#define TAMANO_BUFF_TONO_587 15
#define TAMANO_BUFF_TONO_554 16

typedef enum{

	disparo_,
	explosion_,
	gameover_

}musica_t;

typedef enum{
	f622,
	f587,
	f554,
	variables
}tonos_t ;

extern uint8_t tonos_variables[];
extern uint8_t tono_622hz[];
extern uint8_t tono_587hz[];
extern uint8_t tono_554hz[];
extern uint8_t audio_disparo[];
extern uint8_t audio_explosion[];

#endif /* INC_TONOS_H_ */
