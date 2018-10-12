/*
 * fonctions_uart.h
 *
 *  Created on: 11 oct. 2018
 *      Author: theo-
 */

#ifndef FONCTIONS_UART_H_
#define FONCTIONS_UART_H_


extern void InitUART(void);
extern void TXuartdata( unsigned char c );
extern void TXuarttrame(char *chaine);

extern char uarttramerx[16];

#endif /* FONCTIONS_UART_H_ */
