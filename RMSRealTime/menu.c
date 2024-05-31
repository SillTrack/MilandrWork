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
#include "adc.h"
#include <math.h>
extern xQueueHandle stdin_queue;
extern xQueueHandle stdout_queue;
extern xSemaphoreHandle SemaphoreDMA;

//2 Буфера для хранения результатов АЦП
extern uint16_t ADC_Buffer_ping [];
extern uint16_t ADC_Buffer_pong [];
extern uint32_t ping_pong;
extern void create_scale(void);
#define WINDOW_SIZE 1200
// Выбранный пункт меню
uint8_t U_MENU_Item;
uint16_t window[WINDOW_SIZE];
uint8_t rms_iterator =0;
double rms_arr[15];
double rms;
uint8_t p_bits[4] = 
{
		 0xc0, 0x30,0xc,  0x3,
};
// Строка для представления среднего напряжения
char message[64];  

// Указатель на сообщение, выводимое в качестве пункта меню
char* U_MENU_Message;

// Подготовить строку для выбранного пункта меню
char* U_MENU_Prepare_Item(uint8_t item); 

void draw_rms(double rms, uint8_t position)
{
	uint8_t page, chp, adr;
	uint16_t temp;
	uint8_t i;
	uint8_t reduced = 0;
	temp = rms*10;
	
	//определить chp page adr по position
	if (position == 1)
	{
		position = position * 8 +2;
		chp = 1;
		reduced = 2;
	}
	else if (position <=7)
	{
		chp = 1;
		position = position * 8;
	}
	else
	{
		position -=7;
		position = position * 8;
		chp=2;
	}
	page = 7-1-(rms*10)/4;
	if ((temp % 4) == 0 )
		page+=1;
//	if (page == 0)
//	{
//		
//		U_MLT_Set_Page (chp, page);
//		U_MLT_Set_Address (chp, position*4);
//		while (U_MLT_Read_Status (chp) & 0x80);
//		U_MLT_Write_Data (chp, 0x3);
//		
//		U_MLT_Set_Page (chp, page);
//		U_MLT_Set_Address (chp, position*4+1);
//		while (U_MLT_Read_Status (chp) & 0x80);
//		U_MLT_Write_Data (chp, 0x3);
//	}
	adr = (temp ) -((temp)/4)*4;
		for (i = 0; i < (8 - reduced); i++)
	{
		U_MLT_Set_Page (chp, page);
		U_MLT_Set_Address (chp, position + i);
		while (U_MLT_Read_Status (chp) & 0x80);
		U_MLT_Write_Data (chp,  p_bits[adr-1]);
	}
};

// Инициализировать меню
void U_MENU_Init (void)
{
  	
	// Выбрать начальный пункт меню
	U_MENU_Item = 0;
	// Отобразить начальный пункт меню
	 // Подготовить строку для выбранного пункта меню
     U_MENU_Message = U_MENU_Prepare_Item (U_MENU_Item); 			
	// Вывести на ЖКИ строку пункта меню
}

// Задача по работе с меню
void U_MENU_Task_Function (void)
{
	/*Переменная, которая будет хранить значение счётчика квантов*/
	portTickType xLastTime;
	uint16_t iterator = 0;
	int32_t temp=0;
	uint8_t i, t;
	uint32_t average = 0;
	double summator = 0;
	uint16_t max = 0;
	uint8_t draw_position = 0;
	double rms;
	char message[10];
	//Инициализация переменной, хранящей значение счётчика квантов
	xLastTime=xTaskGetTickCount();
	while(1)
	{
		summator = 0;
		draw_position = 0;
		average = 0;

		/* по нажатию кнопки UP	произыести вычисления rms у каждых 80 точек в 
		массиве window. Вывести эти значения на LCD экран 
*/		vTaskDelay (20);
//		if (check_button(&SB_DOWN) == CLICK)
//		{
//			sprintf(message , "H1");
//			U_MLT_Put_String (message, 2);
//		}


//			sprintf(message , "H1");
//			U_MLT_Put_String (message, 2);
//			for (iterator = 0; iterator < WINDOW_SIZE; iterator++)
//		{
//			if (window[iterator] > max)
//				max = window[iterator];
//			
//			if ((  (iterator+1) % (ADC_BUFFER_SIZE)) == 0)
//			{
//				rms = sqrt(summator);
//				
//				if ((max / sqrt(2)) - rms <(15) && ((max / sqrt(2)) - rms >(-15)))
//				{
//					draw_rms(rms, draw_position);
//				}
//				else if(((max / sqrt(3)) - rms <(25)) && ((max / sqrt(3)) - rms >(-25)) )
//				{
//					draw_rms(rms, draw_position);
//				}
//				else 
//				{
//					// синус тут	
//					i = iterator +1 - 80;
//					while ( iterator < i)
//					{
//						temp = window[i] - average;
//						summator=(temp * temp/ADC_BUFFER_SIZE)+summator;
//						i++;
//					}
//					rms = sqrt(summator);
//					draw_rms(rms, draw_position);
//					
//				}
//				draw_position+=1;
//				summator = 0;
//			}
//			summator += window[i]*window[i]/ADC_BUFFER_SIZE;
//			average += window[i]/ADC_BUFFER_SIZE;
//		}

		
    
		
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
	}
}
//Задача по выводу на ЖКИ информации полученной с терминала по UART
void U_MENU_Output (void)
{ 
	 /* Буфер для полученного сообщения по UART */
    char Message_UART[16 + 1]; 
	while(1)
	{	
	 /* Ожидание ввода сообщения по UART */
   scanf("%16s", Message_UART);
	 // Передача полученного ссобщения на дисплей ЖКИ в 4-ю строку
	  U_MLT_Put_String (Message_UART, 4); 
	 // Передача в стандартный поток вывода, что сообщение успешно передано на ЖКИ
	 printf("Transmit Message To LCD - Success\r\n");
	}
}

// Задача обработчик буфера оцифрованных значений
void Task_DSP(void)
{	
	uint16_t window_counter = 0;
	uint16_t time_counter = 0;
	while(1)
	{
		// Объявление указателя на буфер обработки данных 
		uint16_t *buffer_ADC;
		uint32_t i,average,summa,sum_of_square;
		uint16_t max = 0;
		uint16_t min =0;
		int32_t temp;

		float U;
		/* Реализовано ожидание события с помощью двоичного
		семафора. Семафор после создания становится
		доступен (так, как будто его кто-то отдал).
		Поэтому сразу после запуска планировщика задача
		захватит его. Второй раз сделать это ей не удастся,
		и она будет ожидать, находясь в блокированном
		состоянии, пока семафор не отдаст обработчик
		прерывания. Время ожидания задано равным
		бесконечности, поэтому нет необходимости проверять
		возвращаемое функцией xSemaphoreTake() значение. */
		xSemaphoreTake(SemaphoreDMA, portMAX_DELAY);
		//Задача разблокирована, семафор взят
		if(ping_pong==PING)
		{
			//Заполняется DMA буфер пинг
				// В указатель buffer_ADC записывается начальный адрес массива буфера понг
			buffer_ADC=ADC_Buffer_pong;
		}
		if(ping_pong==PONG)
		{
			//Заполняется DMA буфер понг
			// В указатель buffer_ADC записывается начальный адрес массива буфера пинг
			buffer_ADC=ADC_Buffer_ping;
		}
		// Обработка буфера оцифрованных значений
		summa=0;
		sum_of_square = 0;
		max = buffer_ADC[0];
		for(i=0;i<ADC_BUFFER_SIZE;i++)
		{
			if (max <= buffer_ADC[i])
			{
				max = buffer_ADC[i];
			}
			summa=buffer_ADC[i]+summa;
			window[window_counter] = buffer_ADC[i];
			sum_of_square = buffer_ADC[i] * buffer_ADC[i]/ADC_BUFFER_SIZE + sum_of_square;
			window_counter+=1;
		}
		if (window_counter >= 1200)
		{
			window_counter =0;
		}
		rms = sqrt(sum_of_square);
		
		if ((max / sqrt(2)) - rms <(15) && ((max / sqrt(2)) - rms >(-15)))
		{
			average = summa/ADC_BUFFER_SIZE;
			
		}
		else if(((max / sqrt(3)) - rms <(25)) && ((max / sqrt(3)) - rms >(-25)) )
		{
			average = summa/ADC_BUFFER_SIZE;
		}
		else 
		{
			average = summa/ADC_BUFFER_SIZE;
			// синус тут	
			for(i=0;i<ADC_BUFFER_SIZE;i++)
			{
				temp = buffer_ADC[i] - average;
				sum_of_square=(temp * temp/ADC_BUFFER_SIZE)+sum_of_square;
			}
			rms = sqrt(sum_of_square);	
			
		}
		
		// Преобразование среднего значения АЦП в измеренное напряжение
		// U_ADC_U равно 3.3, напряжение питания
		// U_ADC_D равно 0x1000 (212=4096) – максимальное значение 12-разрядного АЦП
		// U_ADC_U и U_ADC_D определены в файле adc.h
		rms = (U_ADC_U / U_ADC_D) * rms;
	
		
				if (check_button(&SB_DOWN) == CLICK)
		{
			reset_click(&SB_DOWN);
			
			vTaskDelay (20);
			sprintf(message , "Urms=%3.2fV", rms);
			U_MLT_Put_String (message, 7);
		}
		if (rms_iterator++ == 15)
		{
			rms_iterator = 14;
			

			if (time_counter++ > 5)
			{
				for (i=1; i < 15; i++)
				{
					rms_arr[i-1] = rms_arr[i];
				}
				rms_arr[rms_iterator] = rms;
				create_scale();
				time_counter=0;
				for (i = 0; i < 15; i++)
				{
				draw_rms(rms_arr[i], i+1);
				}
			}

		}
		

		// Вывести результат измерения напряжения на ЖКИ
		// формирует строку символов с вставкой 5-символьного поля - вещественное значение с 3 цифрами после запятой 

	}
}	
