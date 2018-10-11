/*
 * fonctions_uart.c
 *
 *  Created on: 11 oct. 2018
 *      Author: theo-
 */

#include <msp430.h>
#include <msp430g2553.h>

#include "main.h"
#include "utils.h"
#include "fonctions_zigbee.h"
#include "fonctions_uart.h"


void InitUART(void)//initialisation de la com uart
{
    P1SEL |= (BIT1 | BIT2);                     // P1.1 = RXD, P1.2=TXD
        P1SEL2 |= (BIT1 | BIT2);                    // P1.1 = RXD, P1.2=TXD
        UCA0CTL1 = UCSWRST;                         // SOFTWARE RESET
        UCA0CTL1 |= UCSSEL_3;                       // SMCLK (2 - 3)

        UCA0CTL0 &= ~(UCPEN | UCMSB | UCDORM);
        UCA0CTL0 &= ~(UC7BIT | UCSPB | UCMODE_3 | UCSYNC); // dta:8 stop:1 usci_mode3uartmode
        UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**

        UCA0BR0 = 104;                              // 1MHz, OSC16, 9600 (8Mhz : 52) : 8/115k
        UCA0BR1 = 0;                                // 1MHz, OSC16, 9600
        UCA0MCTL = 10;

        /* Enable USCI_A0 RX interrupt */
        IE2 |= UCA0RXIE;


}

void TXuartdata( unsigned char c )//envoi d'un caractère via l'uart
{
    while (!(IFG2&UCA0TXIFG));  // USCI_A0 TX buffer ready?
    UCA0TXBUF = c;              // TX -> RXed character

    P1OUT ^= BIT6;
}

void TXuarttrame(char *chaine){
    char car = ' ';
    int cpt = 0;


    while(chaine[cpt] != '\0'){
        TXuartdata(chaine[cpt]);
        cpt++;
    }




}


#pragma vector=USCIAB0RX_VECTOR//reception d'une trame uart
__interrupt void USCI0RX_ISR(void)
{
    unsigned char c;
    c = UCA0RXBUF;//lecture du registre
}
