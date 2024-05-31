/************************************************************************************
  Микроконтроллер: K1986ВЕ92QI
  Устройство: Evaluation Board For MCU MDR32F2Q
  Файл: button.h 
  Назначение: Управление кнопкой   
  Компилятор:  Armcc 5.06.0 из комплекта Keil uVision 5.20.0
************************************************************************************/


#ifndef __U_BUTTON
 #define __U_BUTTON

#include "common.h"
#include "portmacro.h"



// Порт кнопки
#define U_BTN_PORT MDR_PORTB
// Линия для кнопки
#define U_BTN_PIN PORT_Pin_5

// Инициализировать кнопку
void U_BTN_Init (void);

// Получить состояние кнопки
uint8_t U_BTN_Read_Button (void);

#define ANTIBOUNCE_DELAY 20


#define BUT_UP 1
#define BUT_UP_DOWN 2 
#define BUT_DOWN 3
#define BUT_DOWN_UP 4
//событие клика на кнопку
#define UNCLICK 0
#define CLICK 1


typedef struct {
	MDR_PORT_TypeDef* PORTx;
	uint32_t MDR_PORT_Pin;
	uint8_t BUTTON_STATE; 
	uint8_t BUTTON_CLICK; //событие клик по кнопке нажата и после отжата
	portTickType TIME_UP_DOWN; // нажатие кнопки 
	portTickType TIME_DOWN_UP; // отжатие кнопки
	portTickType TIME_DOWN; // длительность нажатого состояния 
} button_t;


// объявление используемых функций
void Init_button(void);
uint8_t check_button(button_t *button);
void reset_click(button_t *button);


extern button_t SB_SELECT, SB_UP, SB_DOWN, SB_RIGHT, SB_LEFT;
#endif 
