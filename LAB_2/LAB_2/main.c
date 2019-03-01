//
//  Created by Anjey Nov on 2/28/19.
//  Copyright © 2019 Anjey Nov. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

short C[8][8], A[8][8], B[8][8];
short C1[64], A1[64], C1[64];
LARGE_INTEGER timerFrequency, timerStart, timerStop;

void andC(void);
void andASM(void);
void andMMX(void);

int main(int argc, char *argv[]) {
	

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			A[i][j] = (i + j) * 2;
			B[i][j] = (i*j) + 2;
		}
	}
	andC();
	andASM();
	_getch();
	return 0;
}

void andC(void)
{
	double t;

	QueryPerformanceFrequency(&timerFrequency);					
	QueryPerformanceCounter(&timerStart);

	for (double k = 0; k < 1000000; k++) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				C[i][j] = A[i][j] & B[i][j];
			}
		}
	}

	QueryPerformanceCounter(&timerStop);

	t = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf("\nTime C is %lf seconds.\n", t);
	
	return;
}

void andASM(void)
{
	double t;

	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&timerStart);

	_asm {
		pusha

		xor ESI,ESI
		xor ECX,ECX
		xor EAX, EAX

		mov ECX, 0F4240h
		loop0:
		push ECX
			mov ECX, 08h
			loop1:
				push ECX
				mov ECX, 08h
				xor EDI, EDI
				loop2:
					mov AX, A[ESI][EDI]
					and AX, B[ESI][EDI]
					mov C[ESI][EDI], AX
					inc EDI
					inc EDI
				loop loop2
				inc ESI
				inc ESI
				pop ECX
			loop loop1
		pop ECX
		loop loop0
		popa
	}

	QueryPerformanceCounter(&timerStop);

	t = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf("\nTime .asm is %lf seconds.\n", t);
	return;
}

void andMMX(void)
{
	double t;

	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&timerStart);

	_asm {
		pusha

		xor ESI, ESI
		xor ECX, ECX
		
		pxor MM0, MM0
		pxor MM1, MM1

		mov ECX, 0F4240h
		loop0 :
			push ECX
			mov ECX, 08h
			loop1:
				push ECX
				mov ECX, 08h
				xor EDI, EDI
				loop2:
					movd ÌÌ0, A[ESI][EDI]
					pand MM0, B[ESI][EDI]
					movd C[ESI][EDI], MM0
					add EDI, 08h
				loop loop2
				inc ESI
				inc ESI
				pop ECX
			loop loop1
			pop ECX
		loop loop0

		popa
	}


	QueryPerformanceCounter(&timerStop);

	t = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf("\nTime .asm is %lf seconds.\n", t);
	return;
}