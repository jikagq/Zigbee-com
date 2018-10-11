#include <msp430.h> 


/**
 * main.c
 */

volatile int mode=0;

void iniboard(void){
    WDTCTL = WDTPW + WDTHOLD;   // Stop WDT


        if(CALBC1_1MHZ==0xFF || CALDCO_1MHZ==0xFF)
        {
            __bis_SR_register(LPM4_bits); // Low Power Mode #trap on Error
        }
        else
        {
            // Factory parameters
            DCOCTL = 0;
            BCSCTL1 = CALBC1_1MHZ;
            DCOCTL = (0 | CALDCO_1MHZ);
        }

        //BOUTON DE LANCEMENT
            P1SEL &= ~(0x09); // met à 0 les bits 0 et 3 du registre
            P1SEL2 &= ~(0x09);

            P1DIR |= 0x01; // met ligne P1.0 en sortie
            P1DIR &= ~(0x08); // met ligne P1.3 en entrée

            P1REN |= 0x08;
            P1OUT |= 0x08;
            P1OUT |= BIT3;

            P1IE |= BIT3;
            P1IES |= BIT3;
            P1IFG &= ~(BIT3);

            P1OUT &= ~(BIT0);


            __bis_SR_register(GIE); // interrupts enabled
}

void delay (unsigned int ms)
{
    volatile unsigned int i, z;
    i=100;
    while (i--) {
        z=ms;
        while(z--);
    }
}

void txzigbee(void){

}


int main(void)
{
    iniboard();
    //InitUART();


    while(1){
            P1OUT &= ~(BIT0);

            if(mode==1){
                P1OUT |= BIT0;
                ///code ici
                txzigbee();
                delay(5000);
                mode=0;
            }else{
                P1OUT &= ~(BIT0);
            }

        }

	return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR( void){

    if((P1IN & BIT3)==0){
        if(mode==0){
            mode=1;
        }else{
            mode=0;
        }
     }
   delay(200);

    P1IFG &= ~(BIT3);
}
