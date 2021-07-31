#include "funcoes.h"
#include "lcd.h"
#include "keypad.h"
#include "bits.h"
#include "pwm.h"
#include "timer.h"
#include "ssd.h"
#include "pic18f4520.h"
#include "delay.h"

void standBy() {
    kpInit();
    lcdInit();
    unsigned int tecla = 0;
    lcdCommand(0x80 + 4);
    lcdString("Stand By");
    lcdCommand(0xC0);
    lcdString("Aperte a tecla 9");
    lcdCommand(0x90 + 2);
    lcdString("para iniciar");
    for (;;) {
        ssdInit();
        ssdDigit(0,0);
        ssdDigit(0,1);
        ssdDigit(0,2);
        ssdDigit(0,3);
        ssdUpdate();
        kpDebounce();
        if (tecla != kpRead()) {
            tecla = kpRead();
            if (bitTst(tecla, 9)) {
                lcdCommand(0x01);
                lcdCommand(0x40);
                menuLigando();
                break;
            }
        }
    }
}

void menu() {
    for (;;) {
        ssdInit();
        ssdDigit(0,0);
        ssdDigit(0,1);
        ssdDigit(0,2);
        ssdDigit(0,3);
        ssdUpdate();
        unsigned int tecla = 0;
        lcdCommand(0x80);
        lcdString("1-Potencia");
        lcdCommand(0xC0);
        lcdString("2-Timer");
        lcdCommand(0x90);
        lcdString("3-Desligar");
        kpDebounce();
        if (tecla != kpRead()) {
            tecla = kpRead();
            if (bitTst(tecla, 3)) {
                potencia();
            }
            if (bitTst(tecla, 7)) {
                timer();
                standBy();
            }
            if (bitTst(tecla, 11)) {
                pwmSet1(0);
                standBy();
            }
        }

    }
}

void menuLigando() {
    char logo[48] = {
        0x01, 0x03, 0x03, 0x0E, 0x1C, 0x18, 0x08, 0x08, //0,0
        0x11, 0x1F, 0x00, 0x01, 0x1F, 0x12, 0x14, 0x1F, //0,1
        0x10, 0x18, 0x18, 0x0E, 0x07, 0x03, 0x02, 0x02, //0,2
        0x08, 0x18, 0x1C, 0x0E, 0x03, 0x03, 0x01, 0x00, //1,0
        0x12, 0x14, 0x1F, 0x08, 0x00, 0x1F, 0x11, 0x00, //1,1
        0x02, 0x03, 0x07, 0x0E, 0x18, 0x18, 0x10, 0x00 //1,2
    };
    int i;
    float j;
    lcdInit();
    lcdCommand(0x40);
    for (i = 0; i < 48; i++) {
        lcdData(logo[i]);
    }
    
    PORTC=0x01;
    atraso_ms(1000);
    PORTC=0x00;
    PORTE=0x01;
    atraso_ms(1000);
    PORTE=0x00;
    
    
    for (i = 0; i < 22; i++) {
        lcdCommand(0x01);
        lcdCommand(0x80 + i);
        lcdData(0);
        lcdData(1);
        lcdData(2);
        lcdCommand(0xC0 + i);
        lcdData(3);
        lcdData(4);
        lcdData(5);
        
        lcdString(" Ligando");

        for (j = 0; j < 700; j++);
    }
    lcdCommand(0x01);
}

void potencia() {
    int i;
    char porcentagem[32] = {
        0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00, 0x00,
        0x00, 0x00, 0x01, 0x03, 0x06, 0x0C, 0x18, 0x10,
        0x02, 0x06, 0x0C, 0x18, 0x10, 0x00, 0x00, 0x00,
        0x00, 0x0E, 0x11, 0x11, 0x11, 0x0E, 0x00, 0x00
    };
    lcdCommand(0x01);

    int ptc = 0;
    for (;;) {
        unsigned int tecla;
        lcdCommand(0x80);
        lcdString("Potencia:");
        lcdCommand(0xC0);
        lcdCommand(0xD0);
        lcdString("Voltar:(*)");
        kpDebounce();
        if (tecla != kpRead()) {
            tecla = kpRead();
            if (bitTst(tecla, 5)) {
                ptc += 10;
                if (ptc >= 100) {
                    ptc = 99;
                }
                pwmSet1(ptc);
            }
            if (bitTst(tecla, 4)) {
                ptc -= 10;
                if (ptc <= 0) {
                    ptc = 0;
                }
                if (ptc == 89) {
                    ptc = 90;
                }
                pwmSet1(ptc);
            }
            if (bitTst(tecla, 0)) {
                lcdCommand(0x01);
                return;
            }
        }

        lcdCommand(0xDB);
        lcdNumber(ptc);
        lcdCommand(0x40);
        for (i = 0; i < 32; i++) {
            lcdData(porcentagem[i]);
        }
        lcdCommand(0x9E);
        lcdData(0);
        lcdData(1);
        lcdCommand(0xDE);
        lcdData(2);
        lcdData(3);
    }
}

void contagem(int dMin, int min, int dSeg, int seg) {
    long int cont = dMin * 600 + min * 60 + dSeg * 10 + seg;
    timerInit();
    float i;
    for (;;) {
        for(i=0; i<2500; i++){
            if((int)i%7==0){
                ssdUpdate();
            }
        }
        cont--;
        ssdDigit((cont/600)%10,0);
        ssdDigit((cont/60)%10,1);
        ssdDigit((cont/10)%6,2);
        ssdDigit((cont)%10,3);
        if(cont==0){
            //Cooler
            pwmSet1(0);
            //Buzzer
            bitSet(TRISC, 1);
            atraso_ms(600);
            bitClr(TRISC, 1);

            return;
        }
    }
}  

void timer() {
    lcdCommand(0x01);
    ssdInit();
    kpInit();
    float j;
    unsigned int tecla = 0x0000;
    int dMin = 0, min = 0, dSeg = 0, seg = 0;
    for (;;) {
        lcdCommand(0x80);
        lcdString("Timer:");
        lcdCommand(0xC0);
        lcdString("Coloque o tempo");
        lcdCommand(0x90);
        lcdString("");
        lcdCommand(0xD0);
        lcdString("Confirmar-6");
        kpDebounce();
        if (tecla != kpRead()) {
            tecla = kpRead();
            if (bitTst(tecla, 3)) {
                dMin++;
                if(dMin>=9){
                    dMin=9;
                }
            }
            if (bitTst(tecla, 2)) {
                min++;
                if(min>=9){
                    min=9;
                }
            }
            if (bitTst(tecla, 1)) {
                dSeg++;
                if(dSeg>=6){
                    dSeg=6;
                }
            }
            if (bitTst(tecla, 0)) {
                seg++; 
                if(seg>=9){
                    seg=9;
                }
                if(seg>=0 && dSeg==6){
                    seg=0;
                }
            }
            if (bitTst(tecla, 7)) {
                dMin--;
                if(dMin<=0){
                    dMin=0;
                }
            }
            if (bitTst(tecla, 6)) {
                min--;
                if(min<=0){
                    min=0;
                }
            }
            if (bitTst(tecla, 5)) {
                dSeg--;
                if(dSeg<=0){
                    dSeg=0;
                }
            }
            if (bitTst(tecla, 4)) {
                seg--;
                if(seg<=0){
                    seg=0;
                }
            }
            if (bitTst(tecla, 10)) {
                contagem(dMin, min, dSeg, seg);
                return;
            }
        }
        ssdDigit(dMin % 10, 0);
        ssdDigit(min % 10, 1);
        ssdDigit(dSeg % 10, 2);
        ssdDigit(seg % 10, 3);
        for (j = 0; j < 5; j++);
        ssdUpdate();
    }
}

  