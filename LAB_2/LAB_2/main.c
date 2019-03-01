//
//  Created by Anjey Nov on 2/28/19.
//  Copyright � 2019 Anjey Nov. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

short C[8][8], A[8][8], B[8][8];


void andC(void);												// ���������� � ��� ���� ������ �� ����� ��

void andASM(void);												// ���������� � ��� ���� ������ �� ����� ���������
																// ��� ������������� ������ ������ MMX

void andMMX(void);												// ���������� � ��� ��� ������ �� ����� ���������
																// � �������������� ������ ������ MMX

void outMatrix(void);											// ����� ����������

int main(int argc, char *argv[]) {
	
	// ������������� ���� ������
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			A[i][j] = (i + j) * 2;
			B[i][j] = (i*j) + 2;
		}
	}

	// ����������
	andC();
	andASM();
	andMMX();

	// �������� ������� ��� ������������ ������
	_getch();
	return 0;
}

void andC(void)
{
	double t;
	LARGE_INTEGER timerFrequency, timerStart, timerStop;
	QueryPerformanceFrequency(&timerFrequency);					
	QueryPerformanceCounter(&timerStart);

	// ��������� ���������� � ��� ��������������� ��������� � ������� ��������� � ������� �
	for (double k = 0; k < 10000000; k++) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				C[i][j] = A[i][j] & B[i][j];
			}
		}
	}

	QueryPerformanceCounter(&timerStop);

	// ������ � ����� ������������ �������
	t = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf("\nTime C is %lf seconds.\n", t);
	
	// ����� ����������
	outMatrix();					
	return;
}

void andASM(void)
{
	double t;
	LARGE_INTEGER timerFrequency, timerStart, timerStop;
	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&timerStart);

	_asm {
		pusha									// ��������� � ���� ��� ��������

		xor ECX,ECX								// EXC = 0
		xor EAX, EAX							// EAX = 0

		mov ECX, 10000000						// ECX = 10000000
		loop0:									// for(; ECX > 0; ECX--){
			push ECX							//		��������� ������� ECX � ����	
			mov ECX, 08h						//		ECX = 8
			xor ESI, ESI						//		EXI = 0
			loop1:								//		for(; ECX > 0; ECX--){
				push ECX						//			��������� ������� ECX � ����
				mov ECX, 08h					//			ECX = 8
				xor EDI, EDI					//			EDI = 0
				loop2:							//			for(; ECX > 0; ECX--){
					mov AX, A[ESI][EDI]			//				AX = A[ESI][EDI]
					and AX, B[ESI][EDI]			//				AX = AX & B[ESI][EDI] 
					mov C[ESI][EDI], AX			//				C[ESI][EDI] = AX
					inc EDI						//				EDI++
					inc EDI						//				EDI++
				loop loop2						//			}
				inc ESI							//			ESI++
				inc ESI							//			ESI++
				pop ECX							//			��������� ECX �� �����
			loop loop1							//		}
		pop ECX									// ��������� ECX �� �����
		loop loop0								// }
		popa									// ��������� ��� �������� �� �����
	}

	QueryPerformanceCounter(&timerStop);

	t = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf("\nTime .asm is %lf seconds.\n", t);

	outMatrix();
	return;
}

void andMMX(void)
{
	//double t;
	//clock_t start, end;
	//LARGE_INTEGER timerFrequency, timerStart, timerStop;
	//QueryPerformanceFrequency(&timerFrequency);
	//QueryPerformanceCounter(&timerStart);
	//start = clock();
	_asm {
		pusha									// ��������� � ���� ��� ��������

		xor ECX, ECX							// ECX = 0

		pxor MM0, MM0							// MM0 = {0,0,0,0}			

		mov ECX, 10000000						// ECX = 10000000
		loop0 :									// for(; ECX > 0; ECX--){
			push ECX							//		��������� ECX � ����
			mov ECX, 08h						//		ECX = 8
			xor ESI, ESI						//		ESI = 0
			loop1 :								//		for(; ECX > 0; ECX--){
				push ECX						//			��������� ECX � ����
				mov ECX, 08h					//			ECX = 8
				xor EDI, EDI					//			EDI = 0
				loop2 :							//			for(; ECX > 0; ECX--){
					movd MM0, A[ESI][EDI]		//				MM0 = { A[ESI][EDI], A[ESI][EDI+2], A[ESI][EDI+4], A[ESI][EDI+6] }
					pand MM0, B[ESI][EDI]		//				MM0 & { B[ESI][EDI], B[ESI][EDI+2], B[ESI][EDI+4], B[ESI][EDI+6] }
					movd C[ESI][EDI], MM0		//				{ C[ESI][EDI], C[ESI][EDI+2], C[ESI][EDI+4], C[ESI][EDI+6] } = MM0
					add EDI, 08h				//				EDI +=8
				loop loop2						//			}
				inc ESI							//			ESI++
				inc ESI							//			ESI++
				pop ECX							//			��������� ECX �� �����
			loop loop1							//		}
			pop ECX								//		��������� ECX �� �����
		loop loop0								// }
		popa									// ��������� ��� �������� �� �����
	}

	//end = clock();
	//QueryPerformanceCounter(&timerStop);

	//t = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	//printf("\nTime .asm with MMX is %lf seconds.\n", t);
	outMatrix();								// ����� ����������
	return;
}

void outMatrix(void)
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			printf("%2hd ", C[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
