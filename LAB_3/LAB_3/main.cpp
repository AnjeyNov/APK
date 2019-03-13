#include <stdio.h>
#include <stdlib.h>
#include<dos.h>

char color = 0x00;

struct VIDEO
{
	unsigned char symb;
	unsigned char attr;
};

void get_reg();
void print(int, int, int);
void printstr(int, int, int);

// указатели на старые обработчики прерываний на MASTER
void interrupt(*int8) (...);
void interrupt(*int9) (...);
void interrupt(*intA) (...);
void interrupt(*intB) (...);
void interrupt(*intC) (...);
void interrupt(*intD) (...);
void interrupt(*intE) (...);
void interrupt(*intF) (...);


// указатели на старые обработчики прерываний на SLAVE
void interrupt(*int88) (...);
void interrupt(*int89) (...);
void interrupt(*int8A) (...);
void interrupt(*int8B) (...);
void interrupt(*int8C) (...);
void interrupt(*int8D) (...);
void interrupt(*int8E) (...);
void interrupt(*int8F) (...);


// новые обработчики прерываний на MASTER
void interrupt  new8(...) { get_reg(); int8(); }
void interrupt  new9(...) { color++; get_reg(); int9(); }		// обработчик клавиатуры
void interrupt  newA(...) { get_reg(); intA(); }
void interrupt  newB(...) { get_reg(); intB(); }
void interrupt  newC(...) { get_reg(); intC(); }
void interrupt  newD(...) { get_reg(); intD(); }
void interrupt  newE(...) { get_reg(); intE(); }
void interrupt  newF(...) { get_reg(); intF(); }

// новыие обработчики прерываний на SLAVE
void interrupt  new88(...) { get_reg(); int88(); }
void interrupt  new89(...) { get_reg(); int89(); }
void interrupt  new8A(...) { get_reg(); int8A(); }
void interrupt  new8B(...) { get_reg(); int8B(); }
void interrupt  new8C(...) { color++; get_reg(); int8C(); }		// обработчик мыши
void interrupt  new8D(...) { get_reg(); int8D(); }
void interrupt  new8E(...) { get_reg(); int8E(); }
void interrupt  new8F(...) { get_reg(); int8F(); }


void print(int val, int y, int x)
{
	char temp;
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);  // получаем адрес 0 символа 
	screen += (y * 80 + x);								// двигаемс€ по консоли

	int i;
	for (i = 7; i >= 0; i--) {
		temp = val & 1;
		val /= 2;
		screen->symb = temp + '0';						// заносим символ в €чейку
		color &= 15;
		screen->attr = color;							// заносим цвет в €чейку
		screen++;										// переходим на след символ
	}
}

void printstr(int num, int y, int x)
{
	char str1[] = { 'M', 'A', 'S', 'T', 'E', 'R', ' ', ' ', ' ', 'I', 'R', 'R', '\0' };
	char str2[] = { 'I', 'S', 'R', '\0' };
	char str3[] = { 'I', 'M', 'R', '\0' };
	char str4[] = { 'S', 'L', 'A', 'V', 'E', ' ', ' ', ' ', ' ', 'I', 'R', 'R', '\0' };
	int i;
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);	// получаем адрес 0 символа
	screen += (y * 80 + x);								// двигаемс€ по консоли
	switch (num)
	{
	case 1:
	{
		for (i = 0; str1[i] != 0; i++)
		{
			screen->symb = str1[i];
			screen->attr = 0x04;
			screen++;
		}
		break;
	}
	case 2:
	{
		for (i = 0; str2[i] != 0; i++)
		{
			screen->symb = str2[i];
			screen->attr = 0x04;
			screen++;
		}
		break;
	}
	case 3:
	{
		for (i = 0; str3[i] != 0; i++)
		{
			screen->symb = str3[i];
			screen->attr = 0x04;
			screen++;
		}
		break;
	}
	case 4:
	{
		for (i = 0; str4[i] != 0; i++)
		{
			screen->symb = str4[i];
			screen->attr = 0x04;
			screen++;
		}
		break;
	}
	}
}

void get_reg()
{
	//получаем данные дл€ регистра запросов ¬≈ƒ”ў≈√ќ и печатаем
	printstr(1, 13, 25);
	outp(0x20, 0x0A);							// помещаем в порт 0x20 значение регистра запросов
	print(inp(0x20), 14, 34);					// считываем из порта значение регистра запросов

	//получаем данные дл€ регистра обслуживани€ ¬≈ƒ”ў≈√ќ
	printstr(2, 15, 34);
	outp(0x20, 0x0B);							// помещаем в порт 0x20 значение регистра обслуживани€
	print(inp(0x20), 16, 34);					// считываем из порта значение регистра обслуживани€

	//р-р масок ¬≈ƒ”ў≈√ќ
	printstr(3, 17, 34);						
	print(inp(0x21), 18, 34);					// считываем из порта 0x21 значение регистра масок

	//все тоже дл€ ведомого
	printstr(4, 19, 25);
	outp(0xA0, 0x0A);
	print(inp(0xA0), 20, 34);
	printstr(2, 21, 34);
	outp(0xA0, 0x0B);
	print(inp(0xA0), 22, 34);
	printstr(3, 23, 34);
	print(inp(0xA1), 24, 34);
}

void init() {

	// получаем вектора прерываний MASTER
	int8 = getvect(0x08);
	int9 = getvect(0x09);
	intA = getvect(0x0A);
	intB = getvect(0x0B);
	intC = getvect(0x0C);
	intD = getvect(0x0D);
	intE = getvect(0x0E);
	intF = getvect(0x0F);

	// получаем вектора прерываний SLAVE
	int88 = getvect(0x70);
	int89 = getvect(0x71);
	int8A = getvect(0x72);
	int8B = getvect(0x73);
	int8C = getvect(0x74);
	int8D = getvect(0x75);
	int8E = getvect(0x76);
	int8F = getvect(0x77);

	// устанавливаем вектора прерываний MASTER
	setvect(0x08, new8);
	setvect(0x09, new9);
	setvect(0x0A, newA);
	setvect(0x0B, newB);
	setvect(0x0C, newC);
	setvect(0x0D, newD);
	setvect(0x0E, newE);
	setvect(0x0F, newF);

	// устанавливаем вектора прерываний SLAVE
	setvect(0x88, new88);
	setvect(0x89, new89);
	setvect(0x8A, new8A);
	setvect(0x8B, new8B);
	setvect(0x8C, new8C);
	setvect(0x8D, new8D);
	setvect(0x8E, new8E);
	setvect(0x8F, new8F);

	_disable();				// запрещаем прерывани€

	// master
	outp(0x20, 0x11);		// 00010001 ICW1 http://perscom.ru/2012-01-20-09-29-00/29-i8259/90-icw1
	outp(0x21, 0x08);		// ICW2 http://perscom.ru/2012-01-20-09-29-00/29-i8259/91-icw2
	outp(0x21, 0x04);		// ICW3 http://perscom.ru/2012-01-20-09-29-00/29-i8259/92-icw3
	outp(0x21, 0x05);		// ICW4 http://perscom.ru/2012-01-20-09-29-00/29-i8259/93-icw4

	
	// slave
	outp(0xA0, 0x11);		// 0001001 ICW1
	outp(0xA1, 0x88);		// ICW2
	outp(0xA1, 0x02);		// ICW3
	outp(0xA1, 0x01);		// ICW4

	_enable();				// разрешаем прерывани€

}

int main()
{
	unsigned far *fp;
	init();

	FP_SEG(fp) = _psp;		// получаем сегмент
	FP_OFF(fp) = 0x2c;		// и смещение сегмента данных

	_dos_freemem(*fp);		// освобождаем пам€ть 

	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);	// оставл€ем резидентной
	return 0;
}