/*
 * uart.c
 *
 *  Created on: 3 mars 2018
 *      Author: theo-
 */

#include <msp430g2553.h>
#include <msp430.h>


#include "uart.h"


volatile char Trame_uart_rx[8];
volatile char Trame_uart_tx[8];
volatile int unsigned index_uart_rx_buffer=0;//buffer rx uart
volatile int flag_trame_uart_recu=0;
volatile unsigned char car_recu;

char tab_temp[4];//tableau temporraire pour faire la conversion int en char*


/*ini de l'uart*/
void InitUART(void)
{


    if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
      {
        while(1);                               // do not load, trap CPU!!
      }
      DCOCTL = 0;                               // Select lowest DCOx and MODx settings
      BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
      DCOCTL = CALDCO_1MHZ;
      P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
      P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
      UCA0CTL1 |= UCSSEL_2;                     // SMCLK
      UCA0BR0 = 104;                            // 1MHz 9600
      UCA0BR1 = 0;                              // 1MHz 9600
      UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
      UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
      IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
}

/**mesure de la température interne**/
int temp(){
    int a;
    int b;

    ADC_Demarrer_conversion(10);
     a=ADC_Lire_resultat();
     b = (a-0.986)/0.00355;
     return b/1023;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR (void)
{
       car_recu = UCA0RXBUF;//lecture du registre
       if(car_recu == 'A'){ //si c'est 'A' c'est que le tag à été reconu
           P1OUT |= (BIT0);
           flag_trame_uart_recu=1;//au retour dans le main() traitement
           IE2 &= ~UCA0RXIE;//desactivation des interruption sur RX pour eviter les conflits sur l'uart
       }else{
           P1OUT &= ~(BIT0);
           flag_trame_uart_recu=0;
       }
}
/*envoi d'un char*/
void TXdata( unsigned char c )
{
    while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
    UCA0TXBUF = c;                    // TX -> RXed character
    delay (100);
}

/*envoi d'un tableau*/
void TXframe( char *texte )
{
    int i=0;
    while(texte[i] != '\0'){
        TXdata(texte[i]);
        i++;
    }
    TXdata(';');//fin
}


/*Trame à envoyer*/
void ajout_trame_uart(char type_de_mesure, char *tab_temp, char *Trame_uart_tx){
    int i=0;

    Trame_uart_tx[0] = type_de_mesure;
    Trame_uart_tx[1] = ':';

   for(i=2; i<8;i++){
        Trame_uart_tx[i] = tab_temp [i-2];
    }
}

/*remise à 0 des tableaux de transmission et reception*/
void reset_uart(void){
    int i;
    index_uart_rx_buffer=0;
    flag_trame_uart_recu =0;
    for(i =0;i<8;i++){
        Trame_uart_rx[i] = '\0' ;
        Trame_uart_tx[i] = '\0';
    }
    for(i =0;i<4;i++){
        tab_temp[i] = '\0' ;

    }
}


void itoad(long unsigned int value, char *result, int base){//conversion entier en char *
      // check that the base if valid
      if (base < 2 || base > 36){
          *result = '\0';
      }

      char *ptr = result, *ptr1 = result, tmp_char;
      int tmp_value;

      do{
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
      }while(value);

      // Apply negative sign
      if (tmp_value < 0) *ptr++ = '-';
      *ptr-- = '\0';
      while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
      }

}


/*******************************legacy code*************************/
void interpreteur_uart(void){
    char c = '\0';
    c = Trame_uart_rx[0]; //1er caractere reçu indique l'action à effectuer

    switch(c){
          case '0' :{//mode test test
              TXframe("Frame detected");
              TXframe("Ground telemetry ok");
              TXframe("Test ok :)");
              break;
          }

           default :{
               nak();
               break;
               }
          }
}

void ack(void){
    unsigned int i;
    char ack[] = "ack";
    for(i=0;i<4;i++){
        TXdata(ack[i]);
    }
    TXdata('\0');//fin
    TXdata('\n');//fin
    TXdata('\r');//fin
}
void nak(void){
    unsigned int i;
    char nak[] = "nak";
    for(i=0;i<4;i++){
        TXdata(nak[i]);
    }
    TXdata('\0');//fin
    TXdata('\n');//fin
    TXdata('\r');//fin
}

