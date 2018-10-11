/*
 * fonctions_zigbee.c
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

void txzigbee(char *chaine){
    int l = 0;

    char *pointeurchaine = chaine;

    l = tablength(pointeurchaine);//ne sert pas

    //manencode();

    TXuarttrame(pointeurchaine);
}

void rxzigbee(void){
}

