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

short A[64], B[64], C[64];

void andC(void);												// AND for two matrix on C
void andASM(void);												// AND for two matrix on .asm
void andMMX(void);												// AND for two matrix on .asm with MMX
															
void outMatrix(short *C);										// out result

int main(int argc, char *argv[]) {
	
	for (int i = 0; i < 64; i++) {
		A[i] = (rand()%60000+1)/2;
		B[i] = (rand()%30000+1) * 2;
	}

	andC();
	andASM();
	andMMX();

	_getch();
	return 0;
}

void andC(void)
{
	double t;
	LARGE_INTEGER timerFrequency, timerStart, timerStop;
	QueryPerformanceFrequency(&timerFrequency);					
	QueryPerformanceCounter(&timerStart);

	for (double k = 0; k < 10000000; k++) {
		for (int i = 0; i < 64; i++) {
			C[i] = A[i] & B[i];
		}
	}

	QueryPerformanceCounter(&timerStop);

	t = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf("\nTime C is %lf seconds.\n", t);
	
	outMatrix(C);					
	return;
}

void andASM(void)
{
	double t;
	LARGE_INTEGER timerFrequency, timerStart, timerStop;
	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&timerStart);

	_asm {
		pusha									// save all reg in stack

		xor ECX,ECX								// EXC = 0
		xor EAX, EAX							// EAX = 0

		mov ECX, 10000000						// ECX = 10000000
		loop0:									// for(; ECX > 0; ECX--){
			push ECX							//		save ECX in stack	
			mov ECX, 64							//		ECX = 8
			xor EDI, EDI
			loop1:								//		for(; ECX > 0; ECX--){
				mov AX, A[EDI]					//				AX = A[EDI]
				and AX, B[EDI]					//				AX = AX & B[EDI] 
				mov C[EDI], AX					//				C[EDI] = AX
				inc EDI							//				EDI++
				inc EDI							//				EDI++
			loop loop1							//		}
		pop ECX									// get ECX form stack
		loop loop0								// }
		popa									// get all reg form stack
	}

	QueryPerformanceCounter(&timerStop);

	t = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf("\nTime .asm is %lf seconds.\n", t);

	outMatrix(C);
	return;
}

void andMMX(void)
{
	double t;
	LARGE_INTEGER timerFrequency, timerStart, timerStop;
	QueryPerformanceFrequency(&timerFrequency);
	QueryPerformanceCounter(&timerStart);

	_asm {
		pusha									// сохранить в стэк все регистры

		xor ECX, ECX							// ECX = 0

		pxor MM0, MM0							// MM0 = {0,0,0,0}			

		mov ECX, 10000000						// ECX = 10000000
		loop0 :									// for(; ECX > 0; ECX--){
			push ECX							//		загрузить ECX в стек
			mov ECX, 16 						//		ECX = 8
			xor EDI, EDI						//			EDI = 0
			loop1:								//		for(; ECX > 0; ECX--){
				movq MM0, A[EDI]				//				MM0 = { A[EDI], A[EDI+2], A[EDI+4], A[EDI+6] }
				pand MM0, B[EDI]				//				MM0 & { B[EDI], B[EDI+2], B[EDI+4], B[EDI+6] }
				movq C[EDI], MM0				//				{ C[EDI], C[EDI+2], C[EDI+4], C[EDI+6] } = MM0
				add EDI, 08h					//				EDI +=8
			loop loop1							//		}
			pop ECX								//		выгрузить ECX из стека
		loop loop0								// }
		popa									// выгрузить все регистры из стека
		EMMS
	}

	QueryPerformanceCounter(&timerStop);

	t = (double)(timerStop.QuadPart - timerStart.QuadPart) / (double)timerFrequency.QuadPart;
	printf("\nTime .asm with MMX is %lf seconds.\n", t);
	outMatrix(C);								// вывод результата
	return;
}

void outMatrix(short *C)
{
	for (int i = 0; i < 64; i++) {
		if (i % 8 == 0) {
			printf("\n");
		}
		printf("%5.1hd ", C[i]);
		C[i] = 0;
	}
	printf("\n");
}
