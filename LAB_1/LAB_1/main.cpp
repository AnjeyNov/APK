// 22.	F(x) = sin (cos ( sqrt (x) + 1 )), x >= 0
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>

#include <windows.h>

void main(int argc, char * argv[])
{
	long double a = 0, b = 0, step = 0, x, f;
	int one = 1;
	double t;
	LARGE_INTEGER timerFrequency, timerStart, timerStop;
	while (true) {
		printf("Input 'a': ");						
		scanf("%lf", &a);										// ��������� ���������� � - ������ ���������
		printf("\nInput 'b': ");
		scanf("%lf", &b);										// ��������� ���������� b - ����� ���������
		printf("\nInput 'step': ");				
		scanf("%lf", &step);									// ��������� ���������� step - ��� ����������
		if (a >= b)	
			printf("\nYou are stupid! ");						// ���� ����� ��������� ������ ��� ������, �� ������������
																// ������ ������
		else
			break;
	}

	x = a;
	f = 0;
	QueryPerformanceFrequency(&timerFrequency);					// ��������� ������� �������� � ���������� timerFrequency
	QueryPerformanceCounter(&timerStart);						// ��������� �������� �������� �� ������ ������ � 
																// ���������� timerStart
	while(x <= b)
	{
		f += sin(cos(sqrt(x) + 1));
		x += step;
	};
	QueryPerformanceCounter(&timerStop);						// ��������� �������� �������� �� ������ ��������� � 
																// ���������� timerStop
	t = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
																// ���������� ���������� �������
	printf("\nTime C is %lf seconds.\n", t);
	printf("%lf\n", f);


	x = a;
	f = 0;				
	QueryPerformanceCounter(&timerStart);				// ��������� �������� �������� �� ������ ������ � 
														// ���������� timerStart
	_asm {
		finit											//������������� ��� ������������

		fld b											//ST(0) = b

		fld x											//ST(0) = x
														//ST(1) = b

		loop_start :
		fcom											// ���������� ST(0) � ST(1)  (x � b)
			fstsw ax									// ������� SW -> AX
			and ah, 01000101b							// if(x>b)[C3=1, C2 = 1, C1 =1] loop_end
			jz loop_end

			fld x										//ST(0) = x
														//ST(1) = x
														//ST(1) = b

			fsqrt										//ST(0) = sqrt(x)
														//ST(1) = x
														//ST(1) = b

			fiadd one									// ST(0) = sqtr(x)+1
														// ST(1) = x
														// ST(2) = b

			fcos										// ST(0) = cos(sqrt(x)+1)
														// ST(1) = x
														// ST(2) = b
	
			fsin										// ST(0) = sin(cos(sqrt(x)+1))
														// ST(1) = x
														// ST(2) = b

			fadd f										// ST(0) = sin(cos(sqrt(x)+1)) + f
														// ST(1) = x
														// ST(2) = b

			fstp f										// F = sin(cos(sqrt(x)+1)) + f
														// ST(0) = x
														// ST(1) = b

			fadd step									// ST(0) = x + step
														// ST(1) = b

			fst x										// X = ST(0)
														// ST(0) = x + step
														// ST(1) = b

			jmp loop_start								// goto loop_start
	loop_end :
		fwait											// ������� ���������� ������ FPU
	}
	QueryPerformanceCounter(&timerStop);				// ��������� �������� �������� �� ������ ������ � 
														// ���������� timerStop
	t = ((double)timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
														// ���������� ���������� �������
	printf("Time ASM is %lf seconds.\n", t);
	printf("%lf\n", f);


	_getch();
}