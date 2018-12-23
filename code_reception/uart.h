/*
 * uart.h
 *
 *  Created on: 3 mars 2018
 *      Author: theo-
 */

#include <msp430g2553.h>
#include <msp430.h>

#ifndef UART_H_
#define UART_H_



extern volatile char Trame_uart_rx[8];
extern volatile char Trame_uart_tx[8];
extern volatile int unsigned index_uart_rx_buffer;
extern volatile int flag_trame_uart_recu;

extern char tab_temp[4];//tableau temporraire pour faire la conversion int en char*


void InitUART(void);
void TXdata( unsigned char c );
void TXframe( char *texte );
void ack(void);
void nak(void);
void itoad(long unsigned int value, char *result, int base);
void reset_uart(void);
void interpreteur_uart(void);
void ajout_trame_uart(char type_de_mesure, char *tab_temp, char *Trame_uart_tx);
int temp();

#endif /* UART_H_ */
