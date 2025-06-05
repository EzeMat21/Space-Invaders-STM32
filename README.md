
### SPACE INVADERS UTILIZANDO RTOS

Juego Space Invaders desarrollado en un STM32F103C8T6. Se utilizó RTOS para el programa, asignando 4 tareas diferentes: Joystick, Pantalla, Memoria y Sonido. Se utilizaron los siguientes mecanismos de comunicación: 2 colas de mensajes y 1 notificación.

Se utilizaron los siguientes componentes: 1 pantalla OLED, un Joystick, un parlante, un filtro RC simple y una memoria EEPROM 93C86.
A su vez, se utilizaron los siguientes periféricos del microcontrolador: 1 interrupción por Timer y el PWM para el Parlante, conversores ADC para el Joystick, comunicación SPI para la Memoria y comunicación I2C para la Pantalla. 



#### DIAGRAMA DE TAREAS

- JoystickTask: Lee los datos analógicos del Joystick a través del ADC. Envía una cola de mensaje a la tarea Pantalla cada 50ms.
- PantallaTask: Actualiza la pantalla, toda la lógica de los menúes se ubica aquí. Se comunica con la Tarea Memoria para el guardado de los nuevos puntajes y con la Tarea Sonido para la generación de los diferentes sonidos.
- SonidoTask: Se encarga de generar el sonido junto con la rutina de interrupción por Timer cuya frecuencia es de 8kHz. Se generan los sonidos del disparo del jugador, las explosiones de los invaders y la música del game-over.
- MemoriaTask: Se encarga de almacenar los nuevos puntajes en la memoria EEPROM.

<img src="assets/diagrama_tareas.jpeg" width=800/>




### Menúes del Juego:
- Pantalla Principal
- Progresión de niveles
- Puntajes
- Game Over
- Guardado del Nombre


<img src="assets/pantalla1.jpg" width="200"/> <img src="assets/pantalla2.jpg" width="200"/> <img src="assets/pantalla3.jpg" width="200"/>

<img src="assets/pantalla4.jpg" width="200"/> <img src="assets/pantalla5.jpg" width="200"/> <img src="assets/pantalla6.jpg" width="200"/>

#### LINKS CONSULTADOS
Bitmaps de las figuras:
https://www.xtronical.com/projects/space-invaders/parts-1-7/part-2-invaders-invaders-everywhere-plotting-invaders-display/

Musica de game-over: https://goodarduinocode.com/projects/simon

Sonidos de explosión y disparos:
https://www.classicgaming.cc/classics/space-invaders/sounds
https://mixkit.co/free-sound-effects/space-shooter/
