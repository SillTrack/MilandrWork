# Задание 
Команды гипертерминала для работы с ЖК индикатором следующая:

PUTC=”A”,X,Y – разместить символ A, по координатам X и Y индикатора.

CLEAR – Очистить содержимое индикатора на текущей странице

PAGE=№ - Номер активной страницы

CLEAR ALL – Очистить все страницы

PUTSTRING=”Stroka”,X
Задача 1 записывает данные в очередь в которой информация о номере страницы по нажатию кнопки. Задача 2 записывает данные в ту же очередь, что и задача 1, в которой информация о номере страницы по команде из гипертерминала. Задача 3 считывает данные из очереди о номере страницы и работает с ЖК индикатором на выбранной странице.

## Работа программы
![](video_2024-05-29_21-37-08.gif) 