

#include "msp430.h"
#include "arduino.h"
/*
defined (__MSP430F2013__)
#include "MSP430F2013.h"
*/

/*
slau144e.pdf page 409
Register                     Short Form Register Type Address Initial State
Timer_A control                   TACTL     Read/write 0160h  Reset with POR
Timer_A counter                   TAR       Read/write 0170h  Reset with POR
Timer_A capture/compare control 0 TACCTL0   Read/write 0162h  Reset with POR
Timer_A capture/compare 0         TACCR0    Read/write 0172h  Reset with POR
Timer_A capture/compare control 1 TACCTL1   Read/write 0164h  Reset with POR
Timer_A capture/compare 1         TACCR1    Read/write 0174h  Reset with POR
Timer_A interrupt vector          TAIV      Read only  012Eh  Reset with POR
*/







//#include <msp430x20x3.h>

void setup(){
  P1DIR |= 0x01;                            // P1.0 output
}

void loop(){
  static unsigned long matt;
  P1OUT ^= 0x01;                            // Toggle P1.0
  matt = millis();
  delay(10000);

}

int main(void)
{
  init();
  setup();

  for (;;){
    loop();
  }
  //return 0;
}


void main2(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ; 
  _BIS_SR(OSCOFF);
  
  BCSCTL2 = DIVS_3;
  
/*
(2) If you set BIT4 of both of P1SEL and P1DIR to 1, you
should be able to see SMCLK at P1.4 with an oscilloscope.
  */
  
  P1SEL |= BIT4;
  P1DIR |= BIT4;
  _EINT();
  
//#ifdef TEST_CLOCKS
  P1DIR |= 0x01;                            // P1.0 output
  
  TACCTL0 = CCIE;                             // CCR0 interrupt enabled
  //TACCR0 = 0xFF;
  TACTL = TASSEL_2 + ID_3 + MC_2;                  // SMCLK, /8, contmode
  //TACTL = TASSEL_2 + ID_0 + MC_1;             // SMCLK, upmode
  //TACTL = TASSEL_1 + ID_3 + MC_1;             // ACLK, /8 upmode
  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupts

  //#endif

}

/*
// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  P1OUT ^= 0x01;                            // Toggle P1.0
}

*/





















