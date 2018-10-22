#include <msp430f169.h>
#include <time.h>
#include <stdio.h>
#define CPU_F ((double)80000000)
#define delayus(x)  __delay_cycles((long)CPU_F*(double)x/1000000.0)
#define delayms(x)  __delay_cycles((long)CPU_F*(double)x/1000.0)
/**
 * main.c
 */
typedef unsigned char u8;
typedef unsigned int u16;

void port_init(){
       P6DIR =0xff;
       P6OUT =0xff;
}
void delay(u16 i){
       while(i--)
       while(i--);

}

void main(){
    int i;
    WDTCTL = WDTPW | WDTHOLD;  // stop watchdog timer
    u8 light = 0x01;
    port_init();
    while(1){
        light = 0x01;
        for(i=0;i<8;i++){
            P6OUT =~light;
            light=light<<1;
            delayms(5);
        }
    }
}
