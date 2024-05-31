/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "init.h"
#include "menu.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* Подключение функций стандартного ввода/вывода (printf и scanf) */
#include <stdio.h>

/* На отладочной плате модуль UART микроконтроллера подключен 
   к преобразователю UART - RS-232.
   Персональный компьютер подключается к разъему RS-232 отладочной
   платы через преобразователь RS-232 - USB.
   На персональном компьютере в программе терминале выбирается 
   COM порт с параметрами: 115200 8N1 (скорость передачи 115200 бод, 
   8 бит данных, без контроля четности и 1 стоп бит).
   В данном проекте ввод и вывод символов производится
   с помощью стандартной библиотеки ввода/вывода (stdio.h).
   Для использования стандартной библиотеки ввода/вывода
   переопределены функции stdin_getchar и stdout_putchar 
   для приема и передачи символов по UART (uart_io.h, uart_io.c).
*/
#include "uart_io.h"





#define PAGES 10
#define LINES 8
#define LINE_LENGTH 16

char page_data[PAGES][LINES][LINE_LENGTH];


int current_page = 0;
/* Объявить переменную-дескриптор очереди. Эта переменная
* будет использоваться для ссылки на очередь после ее создания. */
 xQueueHandle stdin_queue,stdout_queue, task_queue;
// Функция обработчика прерывания по UART2
void UART2_IRQHandler(void)
{
	static uint8_t ReceiveByte=0x00; // данные для приема
	static BaseType_t xHigherPriorityTaskWoken;
	if (UART_GetITStatusMasked(MDR_UART2, UART_IT_RX) == SET)
	//проверка установки флага прерывания по окончании приема данных
	{
	UART_ClearITPendingBit(MDR_UART2, UART_IT_RX);//очистка флага прерывания по приему данных
	}
	ReceiveByte=UART_ReceiveData(UART_IO); // записать принятый данный в статическую переменную
  //Отправить принятый байт в очередь приема данных
	xQueueSendToBackFromISR(stdin_queue, 
        &ReceiveByte, &xHigherPriorityTaskWoken);
	// Принудительно переключить контекст, вызов планировщика для передачи управления другой задаче
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}







void vQHandler(void)
{	
	int ckl =0;
	char buffer;	
	char cmd_msg [2] = "";
	char msg[16] = "";
	int jig =0;
	int iterator = 0;
	int k = 0;
	char str[16];
	cmd_msg[0]='0';
	for (iterator = 0; iterator < PAGES; iterator++)
	{
		for (jig = 0; jig < LINES; jig++)
		{

			for (k = 0; k < LINE_LENGTH; k++)
			{
				str[k] = ' ';
			}
			strcpy(page_data[iterator][jig], str);
		}
		page_data[iterator][7][15]='0'+iterator;
	}
	

	
	
	while(1)
	{
	

	if (xQueueReceive(task_queue, &buffer, 0) == pdPASS)
	{
		if (buffer == '?')
		{			
		ckl=0;
		}
		else if ((ckl < 16+2) && (buffer != '!'))
		{
		if (ckl<2)
		{
			cmd_msg[ckl] = buffer;
			ckl++;
		}
		else{
		msg[ckl-2]=buffer;
		//printf("msg is %s\r\n", &msg);
		//printf("cmd msg is %s\r\n", &cmd_msg);
		ckl++;}
		}
		continue;
	}

		if (!strncmp(cmd_msg, "pg", strlen("pg")))
		{
			current_page = msg[0] - '0';
			printf("current_page is %d\r\n", current_page);
			//printf("cmd msg is %s\r\n", &cmd_msg);
			//printf("\n\rmsg %s\n\r", &msg);
		}
		else if (!strncmp(cmd_msg, "lb", strlen("pg")))
		{
			if (current_page>0)
			{
			current_page--;
			printf("\n\rCurrent page is %d\n\r", current_page);
			}
			else 
			{
			current_page=9;
			printf("\n\rCurrent page is %d\n\r", current_page);
			}
		}
		else if (!strncmp(cmd_msg, "rb", strlen("pg")))
		{
			if (current_page<9)
			{
			current_page++;
			printf("\n\rCurrent page is %d\n\r", current_page);
			}
			else 
			{
			current_page=0;
			printf("\n\rCurrent page is %d\n\r", current_page);
			}
		}
		else if (!strncmp(cmd_msg, "cl", strlen("pg")))
		{
			printf("page IS CLEARED\n\r");
			for (jig = 0; jig < LINES; jig++)
			{
				memset(page_data[current_page][jig], ' ', sizeof(page_data[current_page][jig]));
				//page_data[current_page][jig] = ' ';
			}
			

			for (iterator = 0; iterator < PAGES; iterator++)
				{
					for (jig = 0; jig < LINES; jig++)
					{

						for (k = 0; k < LINE_LENGTH; k++)
						{
							str[k] = ' ';
						}
						strcpy(page_data[iterator][jig], str);
					}
					page_data[iterator][7][15]='0'+iterator;
				}
	
			
			//void U_MLT_Clear_Page (uint8_t Chip, uint8_t Page)
		}
		
		else if (!strncmp(cmd_msg, "ca", strlen("pg")))
		{
			printf("ALL IS CLEARED\n\r");
			for (iterator = 0; iterator < PAGES; iterator++)
			{
				for (jig = 0; jig < LINES; jig++)
				{
					memset(page_data[iterator][jig], ' ', sizeof(page_data[current_page][jig]));
				}
			}
							for (iterator = 0; iterator < PAGES; iterator++)
				{
					for (jig = 0; jig < LINES; jig++)
					{

						for (k = 0; k < LINE_LENGTH; k++)
						{
							str[k] = ' ';
						}
						strcpy(page_data[iterator][jig], str);
					}
					page_data[iterator][7][15]='0'+iterator;
				}
	
			//void U_MLT_Clear_Page (uint8_t Chip, uint8_t Page)
		}
		
		else if (!strncmp(cmd_msg, "pc", strlen("pg")))
		{
			
			printf("CHAR IS SET\n\r");
			
			//page_data[current_page][(msg[2]-'0')][(msg[1]-'0')] = msg[0];
			jig = msg[1]-'0';
			iterator= msg[2]-'0';
			printf("%c", msg[0]);
			printf("%c",msg[1]);
			printf("%c",msg[2]);
			printf("%c",msg[3]);
			printf("%c",msg[4]);
		
			printf("\r\nMESSAGE IS %s\r\nCMD_MSG is %s\r\n", &msg, &cmd_msg);
			if (msg[1] =='1'){
			jig = msg[2]-'0';
			iterator= msg[3]-'0';
			}
			else{
			jig = msg[2]-'0'+10;
			iterator= msg[3]-'0';
			}
			page_data[current_page][iterator][jig] = msg[0];
			//memset(page_data[iterator][jig], ' ', sizeof(page_data[current_page][jig]));
			//strcpy(page_data[current_page][(msg[2]-'0')][(msg[1]-'0')], msg[0]);
		}
		
		else if (!strncmp(cmd_msg, "pl", strlen("pg")))
		{
			printf("LINE PUTED\n\r");
			printf("msg is %s\r\n", &msg);
			printf("cmd msg is %s\r\n", &cmd_msg);
			
			iterator = msg[0]-'0';
			//page_data[current_page][iterator]
			strcpy(page_data[current_page][iterator], &msg[1]);
			
			//U_MLT_Put_String (const char* str, int32_t Y)
		}
		for (jig = 0; jig < LINES; jig++)
		{
			U_MLT_Put_String(page_data[current_page][jig], jig);
		}
cmd_msg[0]='0';
	//printf("\n\r%s\n\r", &cmd_msg);

}
	/*
	int ars=0;
	char buffer[5*STDIN_QUEUE_SIZE] = "";
	// Обработка типа события 
	while(1)
	{
		if (xQueueReceive(task_queue, &buffer, 0) == pdPASS)
			{
				printf(buffer);
				
			}
		else
		{
			continue;
		}
			// работа со страницей
		if (current_page < 9)
			{
				current_page++;			
			}
			else
			{
				current_page = 0;

			}
				*/
				//Очищаем экран
				/* U_MLT_Clear_Chip(1);
				U_MLT_Clear_Chip(2); */
				//вывод в UART номера текущей страницы

				//Заполнение экрнана данными страницы
				/*for( ars = 0; ars < LINES ; ars++)
				{	
					U_MLT_Put_String (page_data[current_page][ars], ars);
						
				}	*/
	//}
	
		
}




// Задача бездействие
void vApplicationIdleHook (void)
{
}


int main(void)
{
	// Инициализация тактирования МК на частоте 80МГц
	init ();
	
	// Инициализация кнопки
	Init_button ();
	// Инициализация ЖКИ
	U_MLT_Init ();	
	// Инициализация меню
	U_MENU_Init ();
	/* Вызов функции инициализации UART для стандартного потока ввода/вывода */
    uart_io_init();

	// Создание очередей для потока ввода/вывода
	stdin_queue = xQueueCreate(STDIN_QUEUE_SIZE*30, sizeof(char));
  stdout_queue = xQueueCreate(STDOUT_QUEUE_SIZE, sizeof(char));
	task_queue = xQueueCreate(STDIN_QUEUE_SIZE*30, sizeof(char));
	   
	  //Создание задачи по работе с меню 
    xTaskCreate(U_MENU_Task_Function,(char *) "Task1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
    //Создание задачи по выводу на ЖКИ информации полученной с терминала по UART
     xTaskCreate(U_MENU_Output, (char *) "Task3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
		 //Создание задачи для работы с выводом по UART
     xTaskCreate(Task_output, (char *) "Task5", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
		 //
		 xTaskCreate(vQHandler,(char *) "Task6",configMINIMAL_STACK_SIZE,NULL, tskIDLE_PRIORITY + 1,NULL);
	// Запуск планировщика задач 	
    vTaskStartScheduler();
}

