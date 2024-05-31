/************************************************************************************
  Микроконтроллер: K1986ВЕ92QI
  Устройство: Evaluation Board For MCU MDR32F2Q
  Файл: button.с
  Назначение: Управление кнопкой
  Компилятор:  Armcc 5.06.0 из комплекта Keil uVision 5.20.0
************************************************************************************/
#include "button.h"
#include "common.h"


button_t SB_SELECT, SB_UP, SB_DOWN, SB_RIGHT, SB_LEFT;

void Init_button(void)
{
	// Структура для инициализации портов	
	PORT_InitTypeDef PortInitStructure;
	
	RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTB, ENABLE);
	RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTC, ENABLE);
	RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTE, ENABLE);
	
	SB_SELECT.PORTx = MDR_PORTC;
	SB_SELECT.MDR_PORT_Pin = PORT_Pin_2;
	SB_SELECT.BUTTON_STATE = BUT_UP;
	SB_SELECT.BUTTON_CLICK = UNCLICK;
	
	SB_UP.PORTx = MDR_PORTB;
	SB_UP.MDR_PORT_Pin = PORT_Pin_5;
	SB_UP.BUTTON_STATE = BUT_UP;
	SB_UP.BUTTON_CLICK = CLICK;
	
	SB_DOWN.PORTx = MDR_PORTE;
	SB_DOWN.MDR_PORT_Pin = PORT_Pin_1;
	SB_DOWN.BUTTON_STATE = BUT_UP;
	SB_DOWN.BUTTON_CLICK = UNCLICK;
	
	SB_RIGHT.PORTx = MDR_PORTB;
	SB_RIGHT.MDR_PORT_Pin = PORT_Pin_6;
	SB_RIGHT.BUTTON_STATE = BUT_UP;
	SB_RIGHT.BUTTON_CLICK = UNCLICK;
	
	SB_LEFT.PORTx = MDR_PORTE;
	SB_LEFT.MDR_PORT_Pin = PORT_Pin_3;
	SB_LEFT.BUTTON_STATE = BUT_UP;
	SB_LEFT.BUTTON_CLICK = UNCLICK;
	

	
  // Сделать входом линию, к которой подключены кнопки
	PORT_StructInit (&PortInitStructure);
	PortInitStructure.PORT_Pin   = SB_SELECT.MDR_PORT_Pin;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PortInitStructure.PORT_OE    = PORT_OE_IN;
	PortInitStructure. PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInitStructure. PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init (SB_SELECT.PORTx, &PortInitStructure);
	
	 // Сделать входом линию, к которой подключены кнопки
	PORT_StructInit (&PortInitStructure);
	PortInitStructure.PORT_Pin   = SB_UP.MDR_PORT_Pin;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PortInitStructure.PORT_OE    = PORT_OE_IN;
	PortInitStructure. PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInitStructure. PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init (SB_UP.PORTx, &PortInitStructure);
	
	 // Сделать входом линию, к которой подключены кнопки
	PORT_StructInit (&PortInitStructure);
	PortInitStructure.PORT_Pin   = SB_DOWN.MDR_PORT_Pin;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PortInitStructure.PORT_OE    = PORT_OE_IN;
	PortInitStructure. PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInitStructure. PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init (SB_DOWN.PORTx, &PortInitStructure);
	
	 // Сделать входом линию, к которой подключены кнопки
	PORT_StructInit (&PortInitStructure);
	PortInitStructure.PORT_Pin   = SB_RIGHT.MDR_PORT_Pin;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PortInitStructure.PORT_OE    = PORT_OE_IN;
	PortInitStructure. PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInitStructure. PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init (SB_RIGHT.PORTx, &PortInitStructure);
	
		 // Сделать входом линию, к которой подключены кнопки
	PORT_StructInit (&PortInitStructure);
	PortInitStructure.PORT_Pin   = SB_LEFT.MDR_PORT_Pin;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PortInitStructure.PORT_OE    = PORT_OE_IN;
	PortInitStructure. PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInitStructure. PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init (SB_LEFT.PORTx, &PortInitStructure);
}






// Инициализировать кнопки
void U_BTN_Init (void)
{
	// Структура для инициализации портов	
	PORT_InitTypeDef PortInitStructure;

	// Разрешить тактирование порта B
	RST_CLK_PCLKcmd (RST_CLK_PCLK_PORTB, ENABLE);

  // Сделать входом линию, к которой подключены кнопки
	PORT_StructInit (&PortInitStructure);
	PortInitStructure.PORT_Pin   = U_BTN_PIN;
	PortInitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PortInitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PortInitStructure.PORT_OE    = PORT_OE_IN;
	PortInitStructure. PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInitStructure. PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PORT_Init (U_BTN_PORT, &PortInitStructure);
}

// Получить состояние кнопки (0 - отпущена, 1 - нажата)
uint8_t U_BTN_Read_Button (void)
{
	// Прочитать состояние входа, к которому подключена кнопка
	if(PORT_ReadInputDataBit (U_BTN_PORT, U_BTN_PIN))
	  // Кнопка отпущена
	  return 0;
	else
	  // Кнопка нажата
		return 1;
}

void reset_click(button_t *button)
{
	button -> BUTTON_CLICK = UNCLICK;
}

//цифровой автомат опроса кнопки
uint8_t check_button(button_t *button){
	
	// проверяем состояние цифрового автомата
	switch (button->BUTTON_STATE)
	{
		case BUT_UP:
		{
			if(PORT_ReadInputDataBit(button->PORTx, button->MDR_PORT_Pin) == 0)
			{	
				button -> BUTTON_STATE = BUT_UP_DOWN;
				button -> TIME_UP_DOWN = xTaskGetTickCount();
			}
			break;
		}
		case BUT_UP_DOWN:
		{
			if (((xTaskGetTickCount()) - (button -> TIME_UP_DOWN)) > ANTIBOUNCE_DELAY)
				// прошла временная задержка
			{
				button -> BUTTON_STATE = BUT_DOWN;
			}
			break;
		}
		case BUT_DOWN:
		{
			if(PORT_ReadInputDataBit(button->PORTx, button->MDR_PORT_Pin) == 1)
			{	
				button -> BUTTON_STATE = BUT_DOWN_UP;
				button -> TIME_DOWN_UP = xTaskGetTickCount();
			}
			break;
		}
		case BUT_DOWN_UP:
		{
			if (((xTaskGetTickCount()) - (button -> TIME_DOWN_UP)) > ANTIBOUNCE_DELAY)
				// прошла временная задержка
			{
				button -> BUTTON_STATE = BUT_UP;
				// длительность нажатия кнопки 
				button -> TIME_DOWN =  button -> TIME_DOWN_UP - button -> TIME_UP_DOWN;
				button -> BUTTON_CLICK = CLICK;
			}
			break;
		}
	}		
	return button -> BUTTON_CLICK;
}

