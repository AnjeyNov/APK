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

// ��������� �� ������ ����������� ���������� �� MASTER
void interrupt(*int8) (...);
void interrupt(*int9) (...);
void interrupt(*intA) (...);
void interrupt(*intB) (...);
void interrupt(*intC) (...);
void interrupt(*intD) (...);
void interrupt(*intE) (...);
void interrupt(*intF) (...);


// ��������� �� ������ ����������� ���������� �� SLAVE
void interrupt(*int70) (...);
void interrupt(*int71) (...);
void interrupt(*int72) (...);
void interrupt(*int73) (...);
void interrupt(*int74) (...);
void interrupt(*int75) (...);
void interrupt(*int76) (...);
void interrupt(*int77) (...);


// ����� ����������� ���������� �� MASTER
void interrupt  new8(...) { get_reg(); int8(); }
void interrupt  new9(...) { color++; get_reg(); int9(); }		// ���������� ����������
void interrupt  newA(...) { get_reg(); intA(); }
void interrupt  newB(...) { get_reg(); intB(); }
void interrupt  newC(...) { get_reg(); intC(); }
void interrupt  newD(...) { get_reg(); intD(); }
void interrupt  newE(...) { get_reg(); intE(); }
void interrupt  newF(...) { get_reg(); intF(); }

// ������ ����������� ���������� �� SLAVE
void interrupt  newB8(...) { get_reg(); int70(); }
void interrupt  newB9(...) { get_reg(); int71(); }
void interrupt  newBA(...) { get_reg(); int72(); }
void interrupt  newBB(...) { get_reg(); int73(); }
void interrupt  newBC(...) { color++; get_reg(); int74(); }		// ���������� ����
void interrupt  newBD(...) { get_reg(); int75(); }
void interrupt  newBE(...) { get_reg(); int76(); }
void interrupt  newBF(...) { get_reg(); int77(); }


void print(int val, int y, int x)
{
	char temp;
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);  // �������� ����� 0 ������� 
	screen += (y * 80 + x);								// ��������� �� �������

	int i;
	for (i = 7; i >= 0; i--) {
		temp = val & 1;
		val /= 2;
		screen->symb = temp + '0';						// ������� ������ � ������
		color &= 15;
		screen->attr = color;							// ������� ���� � ������
		screen++;										// ��������� �� ���� ������
	}
}

void printstr(int num, int y, int x)
{
	char str1[] = { 'M', 'A', 'S', 'T', 'E', 'R', ' ', ' ', ' ', 'I', 'R', 'R', '\0' };
	char str2[] = { 'I', 'S', 'R', '\0' };
	char str3[] = { 'I', 'M', 'R', '\0' };
	char str4[] = { 'S', 'L', 'A', 'V', 'E', ' ', ' ', ' ', ' ', 'I', 'R', 'R', '\0' };
	int i;
	VIDEO far* screen = (VIDEO far *)MK_FP(0xB800, 0);	// �������� ����� 0 �������
	screen += (y * 80 + x);								// ��������� �� �������
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
	//�������� ������ ��� �������� �������� �������� � ��������
	printstr(1, 13, 25);
	outp(0x20, 0x0A);							// �������� � ���� 0x20 �������� �������� ��������
	print(inp(0x20), 14, 34);					// ��������� �� ����� �������� �������� ��������

	//�������� ������ ��� �������� ������������ ��������
	printstr(2, 15, 34);
	outp(0x20, 0x0B);							// �������� � ���� 0x20 �������� �������� ������������
	print(inp(0x20), 16, 34);					// ��������� �� ����� �������� �������� ������������

	//�-� ����� ��������
	printstr(3, 17, 34);						
	print(inp(0x21), 18, 34);					// ��������� �� ����� 0x21 �������� �������� �����

	//��� ���� ��� ��������
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

	// �������� ������� ���������� MASTER
	int8 = getvect(0x08);
	int9 = getvect(0x09);
	intA = getvect(0x0A);
	intB = getvect(0x0B);
	intC = getvect(0x0C);
	intD = getvect(0x0D);
	intE = getvect(0x0E);
	intF = getvect(0x0F);

	// �������� ������� ���������� SLAVE
	int70 = getvect(0x70);
	int71 = getvect(0x71);
	int72 = getvect(0x72);
	int73 = getvect(0x73);
	int74 = getvect(0x74);
	int75 = getvect(0x75);
	int76 = getvect(0x76);
	int77 = getvect(0x77);

	// ������������� ������� ���������� MASTER
	setvect(0x08, new8);
	setvect(0x09, new9);
	setvect(0x0A, newA);
	setvect(0x0B, newB);
	setvect(0x0C, newC);
	setvect(0x0D, newD);
	setvect(0x0E, newE);
	setvect(0x0F, newF);

	// ������������� ������� ���������� SLAVE
	setvect(0xB8, newB8);
	setvect(0xB9, newB9);
	setvect(0xBA, newBA);
	setvect(0xBB, newBB);
	setvect(0xBC, newBC);
	setvect(0xBD, newBD);
	setvect(0xBE, newBE);
	setvect(0xBF, newBF);

	_disable();				// ��������� ����������

	// master
	outp(0x20, 0x11);		// 00010001 ICW1 http://perscom.ru/2012-01-20-09-29-00/29-i8259/90-icw1
	outp(0x21, 0x08);		// ICW2 http://perscom.ru/2012-01-20-09-29-00/29-i8259/91-icw2
	outp(0x21, 0x04);		// ICW3 http://perscom.ru/2012-01-20-09-29-00/29-i8259/92-icw3
	outp(0x21, 0x05);		// ICW4 http://perscom.ru/2012-01-20-09-29-00/29-i8259/93-icw4

	
	// slave
	outp(0xA0, 0x11);		// 0001001 ICW1
	outp(0xA1, 0xB8);		// ICW2
	outp(0xA1, 0x02);		// ICW3
	outp(0xA1, 0x01);		// ICW4

	_enable();				// ��������� ����������

}

int main()
{
	unsigned far *fp;
	init();

	FP_SEG(fp) = _psp;		// �������� �������
	FP_OFF(fp) = 0x2c;		// � �������� �������� ������

	_dos_freemem(*fp);		// ����������� ������ 

	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);	// ��������� �����������
	return 0;
}