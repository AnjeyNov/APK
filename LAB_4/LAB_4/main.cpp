#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define TimerFrequensy  1193180

int frequencyArray[16] = { 440,261,293,293,
						  293,329,349,349,
						  349,392,329,329,
						  293,261,261,293};
int delayArray[16] = { 200,200,200,400,
					  200,200,200,400,
					  200,200,200,400,
					  200,200,200,400};

void generateSound(int frequency, int d);
void print(int state);
void strcan();
void getRandomValue(long val);
void kdcan();

int main()
{
	int maximum;

	
	for (int j = 0; j < 16; j++) {
		generateSound(frequencyArray[j], delayArray[j]);
	}
	delay(100);

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
							// (set square wave generation in channel 3)

	frequencyDivider = TimerFrequensy / frequency;
	outp(0x42, frequencyDivider % 256);			// younger half to channel 3
	frequencyDivider /= 256;
	outp(0x42, frequencyDivider);				// senior half to channel 3

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

	//outp(0x61, inp(0x61) | 1);
	outp(0x43, 0xE8); //11 1 0 100 0	//3 channel
	state = inp(0x42);					//read from 3 channel
	printf("Channel 3: ");
	print(state);
}

void getRandomValue(long val)
{
	long Low, High, Full;
	outp(0x43, 0xB4); // 10 11 010 0    set 3 channel, write younger half and senior half
									  //pulse generation, 16-bit mode
	
	outp(0x42, val % 256);
	val /= 256;
	outp(0x42, val);
	outp(0x61, inp(0x61) | 0x01);	// activ system timer
	int i = 0;
	while (i < 10)
	{
		i++;
		delay(i);
		outp(0x43, 0x86); // 10 00 0000 channel 3 get value
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
		outp(0x43, 0x00); //00 00 0000   channel 1 get value
		kdLow = inp(0x40);
		kdHigh = inp(0x40);
		kd = kdHigh * 256 + kdLow;
		if (kd > kdMax) kdMax = kd;
	}
	printf("%hx\n", kdMax);
	kdMax = 0;
	for (i = 0; i < 65535; i++)
	{
		outp(0x43, 0x40); // 01 00 0000		channel 2 get value
		kdLow = inp(0x41);
		kdHigh = inp(0x41);
		kd = kdHigh * 256 + kdLow;
		if (kd > kdMax) kdMax = kd;
	}
	printf("%hx\n", kdMax);
	kdMax = 0;
	for (i = 0; i < 65535; i++)
	{
		outp(0x43, 0x80); //10 00 0000		channel 3 get value
		kdLow = inp(0x42);
		kdHigh = inp(0x42);
		kd = kdHigh * 256 + kdLow;
		if (kd > kdMax) kdMax = kd;
	}
	printf("%hx\n", kdMax);

}
