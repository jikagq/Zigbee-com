/*
 * utils.c
 *
 *  Created on: 11 oct. 2018
 *      Author: theo-
 */


void delay (unsigned int ms)
{
    volatile unsigned int i, z;
    i=100;
    while (i--) {
        z=ms;
        while(z--);
    }
}

int tablength(char *tab){
    int longueur=0;

    while(tab[longueur] != '\0'){
        longueur++;
    }


    return longueur; //+1 pour '\0'
}
