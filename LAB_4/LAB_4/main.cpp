#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define TimerFrequensy  1193180

int frequencyArray[9] = { 293,392,293,392,523,659,880,784,698 };
int delayArray[9] = { 200,400,200,400,200,200,400,200,200 };

void generateSound(int frequency, int d);
void print(int state);
void strcan();
void getRandomValue(long val);
void kdcan();

int main()
{
	int maximum;

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 9; j++) {
			generateSound(frequencyArray[j], delayArray[j]);
		}
		delay(100);
	}

	strcan();

	printf("\n");

	kdcan();

	printf("Get max val: ");
	scanf("%d", &maximum);
	getRandomValue(maximum);

	return 0;
}


void generateSound(int frequency, int d)
{
	int frequencyDivider;	

	outp(0x43, 0xB6);		// 10 11 011 0
							// write to control register of the system timer 
							// (set square wave generation in port 2)

	frequencyDivider = TimerFrequensy / frequency;
	outp(0x42, frequencyDivider % 256);			// younger half to port 3
	frequencyDivider /= 256;
	outp(0x42, frequencyDivider);				// senior half to port 3

	outp(0x61, inp(0x61) | 3);					// get value of speaker port, set 0 and 1 bit
												// in 1 and set valuse in speaker port
												// (on speaker)
	delay(d);
	outp(0x61, inp(0x61) & 0xFC);				// of speaker
}

void print(int state)
{
	int i;
	char temp[8];
	for (i = 7; i >= 0; i--)
	{
		temp[i] = state % 2 + '0';
		state /= 2;
	}
	for (i = 0; i < 8; i++)
	{
		printf("%c", temp[i]);
	}
	printf("\n");
}

void strcan()
{
	int state;

	outp(0x43, 0xE2); //11 1 0 001 0	//read (11),not fix counter(1),get status(0),
										//0,0,set 1 channel(1), 0
	state = inp(0x40);					//read from 1 channel
	printf("Channel 1: ");
	print(state);

	outp(0x43, 0xE4); //11 1 0 010 0	//2 channel
	state = inp(0x41);					//read from 2 channel
	printf("Channel 2: ");
	print(state);

	outp(0x43, 0xE8); //11 1 0 100 0	//3 channel
	state = inp(0x42);					//read from 3 channel
	printf("Channel 3: ");
	print(state);
}

void getRandomValue(long val)
{
	long Low, High, Full, maxVal;
	outp(0x43, 0xB4); // 10 11 010 0    set 2 channel, write younger half and senior half
									  //pulse generation, 16-bit mode
	
	maxVal = TimerFrequensy / val;
	outp(0x42, maxVal % 256);
	maxVal /= 256;
	outp(0x42, maxVal);
	outp(0x61, inp(0x61) | 0x01);	// activ system timer
	int i = 0;
	while (i < 10)
	{
		i++;
		delay(i);
		outp(0x43, 0x86); // 1000 0110
		Low = inp(0x42);
		High = inp(0x42);
		Full = High * 256 + Low;
		printf("%ld\n", Full);
	}
	outp(0x61, inp(0x61) & 0xFC);	// disactiv system timer
}

void kdcan()
{
	long kd, kdHigh, kdLow, kdMax;

	kdMax = 0;

	for (long i = 0; i < 65535; i++)
	{
		outp(0x43, 0x00); //0000 0000
		kdLow = inp(0x40);
		kdHigh = inp(0x40);
		kd = kdHigh * 256 + kdLow;
		if (kd > kdMax) kdMax = kd;
	}
	printf("%hx\n", kdMax);
	kdMax = 0;
	for (i = 0; i < 65535; i++)
	{
		outp(0x43, 0x40); // 0100 0000
		kdLow = inp(0x41);
		kdHigh = inp(0x41);
		kd = kdHigh * 256 + kdLow;
		if (kd > kdMax) kdMax = kd;
	}
	printf("%hx\n", kdMax);
	kdMax = 0;
	for (i = 0; i < 65535; i++)
	{
		outp(0x43, 0x80); //1000 0000
		kdLow = inp(0x42);
		kdHigh = inp(0x42);
		kd = kdHigh * 256 + kdLow;
		if (kd > kdMax) kdMax = kd;
	}
	printf("%hx\n", kdMax);

}
