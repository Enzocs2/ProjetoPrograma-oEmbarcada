# 1 "timer.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Program Files (x86)/Microchip/MPLABX/v5.35/packs/Microchip/PIC18Fxxxx_DFP/1.2.26/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "timer.c" 2
# 20 "timer.c"
# 1 "./pic18f4520.h" 1
# 21 "timer.c" 2
# 1 "./timer.h" 1
# 23 "./timer.h"
 char timerEnded(void);
 void timerWait(void);

 void timerReset(unsigned int tempo);
 void timerInit(void);
# 22 "timer.c" 2


char timerEnded(void)
{
 return (((*(volatile __near unsigned char*)0xFF2)) & (1<<2));
}

void timerWait(void)
{
 while(!(((*(volatile __near unsigned char*)0xFF2)) & (1<<2)));
}


void timerReset(unsigned int tempo)
{

 unsigned ciclos = tempo * 2;

 ciclos = 65535 - ciclos;

 ciclos -= 14;
 (*(volatile __near unsigned char*)0xFD7) = (ciclos >> 8);
 (*(volatile __near unsigned char*)0xFD6) = (ciclos & 0x00FF);

 (((*(volatile __near unsigned char*)0xFF2)) &= ~(1<<2));
}

void timerInit(void)
{
 (*(volatile __near unsigned char*)0xFD5) = 0b00001000;
 (((*(volatile __near unsigned char*)0xFD5)) |= (1<<7));
}
