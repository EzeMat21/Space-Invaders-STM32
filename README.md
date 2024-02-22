
### SPACE INVADERS UTILIZANDO RTOS

Juego Space Invaders desarrollado en un STM32F103C8T6. Se utilizó RTOS para el programa, asignando 4 tareas diferentes: Joystick, Pantalla, Memoria y Sonido. Se utilizaron los siguientes mecanismos de comunicación: 2 colas de mensajes y 1 notificación.

Se utilizaron los siguientes componentes: 1 pantalla OLED, un Joystick, un parlante, un filtro RC simple y una memoria EEPROM 93C86.
A su vez, se utilizaron los siguientes periféricos del microcontrolador: 1 interrupción por Timer y el PWM para el Parlante, conversores ADC para el Joystick, comunicación SPI para la Memoria y comunicación I2C para la Pantalla. 



#### DIAGRAMA DE TAREAS

- JoystickTask: Lee los datos analógicos del Joystick a través del ADC. Envía una cola de mensaje a la tarea Pantalla cada 50ms.
- PantallaTask: Actualiza la pantalla, toda la lógica de los menúes se ubica aquí. Se comunica con la Tarea Memoria para el guardado de los nuevos puntajes y con la Tarea Sonido para la generación de los diferentes sonidos.
- SonidoTask: Se encarga de generar el sonido junto con la rutina de interrupción por Timer cuya frecuencia es de 8kHz. Se generan los sonidos del disparo del jugador, las explosiones de los invaders y la música del game-over.
- MemoriaTask: Se encarga de almacenar los nuevos puntajes en la memoria EEPROM.

![Diagrama en blanco](https://github.com/sistemas-embebidos-ing-unlp/SE2024-698043-TE2/assets/126365409/fbf1257d-f8ad-4cd8-bfa7-2c24c60d1801)




### Menúes del Juego:
- Pantalla Principal
- Progresión de niveles
- Puntajes
- Game Over
- Guardado del Nombre


<img src="https://github.com/sistemas-embebidos-ing-unlp/SE2024-698043-TE2/assets/126365409/16db1cc6-c041-4bb9-ade8-47af01c00de0" width="200"/>
<img src="https://github.com/EzeMat21/Space-Invaders/assets/126365409/d2ab4b51-5467-426a-bfa0-6bca5952eaeb" width="200"/>
<img src="https://github.com/EzeMat21/Space-Invaders/assets/126365409/a18693bd-61da-48fa-81de-00796d787953" width="200"/>

<img src="https://github.com/sistemas-embebidos-ing-unlp/SE2024-698043-TE2/assets/126365409/e5e40ddd-59a2-4a24-891e-2e880de54e07" width="200"/>
<img src="https://github.com/EzeMat21/Space-Invaders/assets/126365409/1a87f3f8-44d9-4632-a522-726cd72ca1c2" width="200"/>
<img src="https://github.com/sistemas-embebidos-ing-unlp/SE2024-698043-TE2/assets/126365409/82762a5a-9f8e-4f9c-89db-1dc7c74b1f7a" width="200"/>
<img src="https://github.com/sistemas-embebidos-ing-unlp/SE2024-698043-TE2/assets/126365409/f5849e6b-0f87-450a-b2c4-f804d75af0d5" width="200"/>

#### LINKS CONSULTADOS
Bitmaps de las figuras:
https://www.xtronical.com/projects/space-invaders/parts-1-7/part-2-invaders-invaders-everywhere-plotting-invaders-display/

Musica de game-over: https://goodarduinocode.com/projects/simon

Sonidos de explosión y disparos:
https://www.classicgaming.cc/classics/space-invaders/sounds
https://mixkit.co/free-sound-effects/space-shooter/
