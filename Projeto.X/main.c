#include "pic18f4520.h"
#include "bits.h"
#include "keypad.h"
#include "ssd.h"
#include "lcd.h"
#include "funcoes.h"
#include "pwm.h"
#include "timer.h"

void main(void) {
    TRISC=0b00000000;
    TRISE=0x00;
    ssdInit();
    pwmInit();
    standBy();
    menu();
}
