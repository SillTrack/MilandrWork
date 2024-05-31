/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "init.h"
#include "menu.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "button.h"

// Задача бездействие
void vApplicationIdleHook (void)
{
}


int main(void)
{
	// Инициализация тактирования МК на частоте 80МГц
	init ();
	// Инициализация кнопки
	//U_BTN_Init ();
		// Инициализация кнопок
	Init_button();
	// Инициализация ЖКИ
	U_MLT_Init ();	
	// Инициализация меню
	U_MENU_Init ();
	  // Задача по работе с меню 
    xTaskCreate(vMenu,(char *) "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
	  // Задача по выводу бегущей строки на ЖКИ 
    xTaskCreate(vScroll_String, (char *) "Task2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    
	// Запуск планировщика задач 	
    vTaskStartScheduler();
}

