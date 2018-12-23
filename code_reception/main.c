/**
 * main.c
 */

#include <msp430.h> 
#include "uart.h"

char tab_temp[4];


int main(void)
{
    int tmp;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR &= ~(BIT3|BIT5|BIT6|BIT7);//capteurs
    P1DIR |= (BIT0);
    P1OUT &= ~(BIT0);
    ADC_init();

    InitUART();// utilisation de l'uart via usb pour transmission
    __bis_SR_register(GIE); // interrupts enabled


    while(1){
        int i;
        if(flag_trame_uart_recu == 0){//check si une trame valide a été reçu
            //delay (1000);
           for(i=0;i<5;i++){ //5 mesures
               itoad(temp(), &tab_temp, 10);//mesure et conversion de l'entier en chaine
               ajout_trame_uart('t',&tab_temp,&Trame_uart_tx);//trame uart
               TXframe(&Trame_uart_tx);//transmission
               reset_uart();
               delay (1000);
           }
           IE2 |= UCA0RXIE;//reactivation des interruptions sur uca0rx
        }else{
            delay (1000);
        }
    }
    return 0;
}


/**delai du pauvre*/
void delay (unsigned int ms)
{
    volatile unsigned int i, z;
    i=100;
    while (i--) {
        z=ms;
        while(z--);
    }
}




