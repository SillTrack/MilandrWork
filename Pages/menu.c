/************************************************************************************
  Микроконтроллер: K1986ВЕ92QI
  Устройство: Evaluation Board For MCU MDR32F2Q
  Файл: menu.с
  Назначение: Формирование меню
  Компилятор:  Armcc 5.06.0 из комплекта Keil uVision 5.20.0
************************************************************************************/

#include "menu.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "uart_io.h"
#include "string.h"
extern xQueueHandle stdin_queue;
extern xQueueHandle stdout_queue;
extern xQueueHandle task_queue;
extern int current_page;



// Выбранный пункт меню
uint8_t U_MENU_Item;

// Строка для представления времени
uint8_t U_MENU_Time_String[8];  

// Указатель на сообщение, выводимое в качестве пункта меню
char* U_MENU_Message;

// Подготовить строку для выбранного пункта меню
char* U_MENU_Prepare_Item(uint8_t item); 

// Инициализировать меню
void U_MENU_Init (void)
{
  	
	// Выбрать начальный пункт меню
	U_MENU_Item = 0;
	// Отобразить начальный пункт меню
	 // Подготовить строку для выбранного пункта меню
     U_MENU_Message = U_MENU_Prepare_Item (U_MENU_Item); 			
	// Вывести на ЖКИ строку пункта меню
	if (U_MENU_Message)
	U_MLT_Put_String (U_MENU_Message, 3);
}

// Задача по работе с меню
void U_MENU_Task_Function (void)
{
	int jig;
	char str[16];
	char cmdb[4] = "";
	
	//Переменная, которая будет хранить значение счётчика квантов
	portTickType xLastTime;
	uint8_t flag_button;
	//Инициализация переменной, хранящей значение счётчика квантов
	xLastTime=xTaskGetTickCount();
				
	flag_button=OFF;

	while(1)
	{

		// Пауза 20мс, чтобы по-напрасну не грузить процессор
    vTaskDelay (20);

//task_queue
		 if (check_button(&SB_LEFT) == CLICK)
		{
			
			
			reset_click(&SB_LEFT);
			
				cmdb[0] = '?';
				cmdb[1] ='l';
				cmdb[2] = 'b';
				cmdb[3] = '!';
				
				for (jig = 0; jig<5; jig++)
				xQueueSend(task_queue, &cmdb[jig], 0);
			// запись в очередь 
			//xQueueSendToBack(task_queue, &decrement_page, 1);
		}
		if (check_button(&SB_RIGHT) == CLICK)
		{
			reset_click(&SB_RIGHT);
				
				cmdb[0] = '?';
				cmdb[1] ='r';
				cmdb[2] = 'b';
				cmdb[3] = '!';
				
				for (jig = 0; jig<5; jig++)
				xQueueSend(task_queue, &cmdb[jig], 0);

			// запись в очередь 
			//xQueueSendToBack(task_queue, &increment_page, 1);
			
	}
}
}

// Подготовить строку для выбранного пункта меню
char* U_MENU_Prepare_Item (uint8_t item) 
{
	switch (item)
	{
		// Пункт 0;
		case 0:
			
			return "\xCF\xF3\xED\xEA\xF2 0";  // Пункт 0
		
		// Пункт 1;
		case 1:
			
			return "\xCF\xF3\xED\xEA\xF2 1";  // Пункт 1

		// Пункт 2;
		case 2:
			
			return "\xCF\xF3\xED\xEA\xF2 2";  // Пункт 2

		// Пункт 3;
		case 3:
		
			return "\xCF\xF3\xED\xEA\xF2 3";  // Пункт 3
		
		default:
			
			return 0;
	}
}

// Задача по выводу бегущей строки на ЖКИ
void U_MENU_Running_String_Task_Function (void)
{
	uint32_t k = 0;
	
	// Строка, выводимая на индикатор 
//	const char s[] =  "\xD0\xE0\xE1\xEE\xF2\xE0 \xF1 \xEA\xED\xEE\xEF\xEA\xE0\xEC\xE8   ";  // "Работа с кнопками"
	const char s[]="Work with buttons";
	while(1)
	{
		// Пауза в тиках системного таймера. Здесь 1 тик = 1 мс
    vTaskDelay(1000);
		
		// Вывести строку c прокруткой
		// s - выводимая строка
		// 6 - номер строки на дисплее
		// k++ - смещение строки на количество символов
		U_MLT_Scroll_String (s, 6, k++);
	}
}






//Задача по выводу на ЖКИ информации полученной с терминала по UART
void U_MENU_Output (void)
{ 
	 /* Буфер для полученного сообщения по UART */
    char Message_UART[30];
		int iterator = 0;
		int jig =0;
		char cmd[30] = "\0";
	while(1)
	{	

		for (jig = 0; jig < 30; jig++)
		{
			cmd[jig]=' ';
		}
		
		iterator=0;
		jig =0;
	
	 /* Ожидание ввода сообщения по UART */
   scanf("%30s", Message_UART);
	
		if (!strcmp(Message_UART, "CLEAR_ALL"))
		{
			printf("\"CLEAR ALL\" IS READ\r\n");
			cmd[0] = '?';
			cmd[1] ='c';
			cmd[2] = 'a';
			cmd[3] = '!';
			for (jig = 0; jig<5; jig++)
				xQueueSend(task_queue, &cmd[jig], 0);
			
		}
		else if (!strcmp(Message_UART, "CLEAR"))
		{
			printf("\"CLEAR\" IS READ\r\n");
			cmd[0] = '?';
			cmd[1] ='c';
			cmd[2] = 'l';
			cmd[3] = '!';
			for (jig = 0; jig<5; jig++)
				xQueueSend(task_queue, &cmd[jig], 0);

		}
		else if (!strncmp(Message_UART, "PUTSTRING=", strlen("PUTSTRING=")))
		{
			printf("\"PUTSTRING\" IS READ\r\n");
			cmd[0] = '?';
			cmd[1] ='p';
			cmd[2] = 'l';
			jig = 11;
			iterator = 4;
			while (Message_UART[jig] != '"')
			{
				//printf("CURRENT symbl is %c\r\n", Message_UART[jig]);
				cmd[iterator] =  Message_UART[jig];
				iterator++;
				jig++;
			}
			//printf("PAGE_NUMBER IS %c\r\n", Message_UART[jig+2]);
			jig +=2;
			//printf("iterator + 1 is %d\r\n", iterator);
			//printf("page number is %c\r\n", Message_UART[jig]);
			cmd[3] = Message_UART[jig];
			iterator += 1;
			cmd[iterator] = '!';
			printf("COMMAND IS %s\r\n", &cmd);
			for (jig = 0; jig<iterator+2; jig++)
				xQueueSend(task_queue, &cmd[jig], 0);
			
		}
		else if (!strncmp(Message_UART, "PUTC=", strlen("PUTC=")))
		{
			printf("\"PUTC\" IS READ\r\n");
			cmd[0] = '?';
			cmd[1] ='p';
			cmd[2] = 'c';
			cmd[3] = Message_UART[6];
			if  (Message_UART[10] == ',')
			{
				cmd[4] = '1';
				cmd[5] = Message_UART[9];
				cmd[6] = Message_UART[11];
				cmd[7] = '!';

			}
			else if ((Message_UART[9] == '1')&& (Message_UART[10] >= '0') && (Message_UART[10] <= '15') )
			{
				cmd[4] = '2';
				cmd[5] = Message_UART[10];
				cmd[6] = Message_UART[12];
				cmd[7] = '!';

			}
			else
			{
				printf("NUMBER IS INCORRECT\r\n");
				continue;
			}
			
			printf("COMMAND IS %s\r\n", &cmd);
			for (jig = 0; jig<7; jig++)
				xQueueSend(task_queue, &cmd[jig], 0);
		}
		else if (!strncmp(Message_UART, "PAGE=", strlen("PAGE=")))
		{
			printf("\"PAGE\" IS READ\r\n");
			iterator = 5;
			
			if ((Message_UART [iterator] >= '0')&&(Message_UART [iterator]<='9')&& Message_UART [iterator+1] == '\0')
			{
				// Message_UART [iterator];
				//printf("\n\r mess %c \n\r", Message_UART [iterator]);
				
				cmd[0] = '?';
				cmd[1] ='p';
				cmd[2] = 'g';
				//printf("page number is %c\r\n", Message_UART [iterator]);
				cmd[3] = Message_UART [iterator];
				cmd[4] = '!';
				//printf("command is %s\r\n", cmd);
				for (jig = 0; jig<5; jig++)
					xQueueSend(task_queue, &cmd[jig], 0);
				
			}
			else
			{
				printf("\"PAGE\" error\r\n");
			}
		}
		else
			printf("\n\rCMD ERR\n\r");

		
	 // Передача полученного ссобщения на дисплей ЖКИ в 4-ю строку
	 // U_MLT_Put_String (Message_UART, 4); 
	 // Передача в стандартный поток вывода, что сообщение успешно передано на ЖКИ
	 //printf("Transmit Message To LCD - Success\r\n");
	}
}
// Задача - приветствие
void U_Privet(void)
{
	// Отправка сообщения приветствия
	printf("Work with Queues\r\n");
	// Удаление задачи
	vTaskDelete( NULL );
}
// Задача для работы с выводом по UART
void Task_output(void)
{
	  static uint32_t buffer;
  while(1)
	{
		if (xQueueReceive(stdout_queue, &buffer, portMAX_DELAY) == pdPASS)
    {
				  /* Ожидание флага освобождения буфера передачи (TXFE) */
					while (UART_GetFlagStatus(UART_IO, UART_FLAG_TXFE) != SET);
					// Отправка одного символа по UART
					UART_SendData(UART_IO, buffer);
		}
				
  }
}
