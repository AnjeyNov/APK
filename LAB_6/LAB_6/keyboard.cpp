#include <dos.h>
#include <conio.h>
#include <stdio.h>

void interrupt newInt9(...); 		 // Функция обработки прерывания
void interrupt(*oldInt9)(...); 		 // Указатель на обработчик прерывания
void indicator(unsigned char mask);	 // Функция управления индикаторами
void blinking(void);				 // Функция мигания индикаторами


int quitFlag = 0; 		 // Флаг выхода из программы
int blinkingON = 0;		 // Флаг мигания индикаторами


void main() {
	oldInt9 = getvect(0x09);	 // Сохраняем указатель на старый обработчик
	setvect(0x09, newInt9); 	 // Меняем его на новый

	while (!quitFlag) {			 // Пока не установлен флаг выхода
		if (blinkingON) 		 // Если установлен флаг мигания индикаторов
			blinking(); 		 // мигаем индикаторами
	}

	setvect(0x09, oldInt9); // Восстанавливаем старый обработчик прерывания
	return;

}

// Функция обработки прерывания

void interrupt newInt9(...) {

	unsigned char value = 0;
	oldInt9();
	value = inp(0x60); 				// Получаем значение из порта 60h

	if (value == 0x01) quitFlag = 1; 		// Устанавливаем флаг выхода(нажата Esc)

	if (value == 0x26 && blinkingON == 0) // Поставить
		blinkingON = 1; 				 // или снять флаг мигания,

	//else if (value == 0x26 && blinkingON == 1) // если нажата клавиша L
	//	blinkingON = 0;

	printf("\t%x", value);
	outp(0x20, 0x20); 				// EOI to MASTER
}

// Функция управления индикаторами

void indicator(unsigned char mask) {


	int flg = 0;			
	// проверяем незанятость входного регистра контроллера клавиатуры
	while ((inp(0x64) & 0x02) != 0x00);
	outp(0x60, 0xED);

	for (int i = 9; i > 0; i--) {
		if ((inp(0x60) == 0xFA)) {
			flg = 1;
			break;
		}
		delay(200);
	}

	if (flg) {
		while ((inp(0x64) & 0x02) != 0x00);
		outp(0x60, mask);
	}

	delay(50);

}


// Функция мигания индикаторами

void blinking() {


	indicator(0x02); // вкл. индикатор Num Lock
	delay(150);

	indicator(0x00); // выкл. все индикаторы
	delay(50);

	indicator(0x04); // вкл. индикатор Caps Lock
	delay(150);

	indicator(0x00); // выкл. все индикаторы
	delay(50);

	indicator(0x6); // вкл. индикаторы Num Lock и Caps Lock
	delay(200);

	indicator(0x00); // выкл. все индикаторы
	delay(50);

}