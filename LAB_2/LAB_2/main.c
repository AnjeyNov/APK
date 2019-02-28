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

		mov CX, 08h
		loop1:
			push CX
			mov CX, 08h
			xor EDI, EDI
			loop2:
				mov AX, A[ESI][EDI]
				and AX, B[ESI][EDI]
				mov C[ESI][EDI], AX
				inc EDI
			loop loop2
			inc ESI
			pop CX
		loop loop1
		popa
	}

	QueryPerformanceCounter(&timerStop);

	t = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf("\nTime C is %lf seconds.\n", t);
	return;
}

void andMMX(void)
{
}
