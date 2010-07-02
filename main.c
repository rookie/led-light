
//#include "io430.h"
//#include "msp430.h"
/*

defined (__MSP430F2013__)
#include "io430x20x3.h"

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
Timer_A capture/compare control 2 TACCTL2†  Read/write 0166h  Reset with POR
Timer_A capture/compare 2         TACCR2†   Read/write 0176h  Reset with POR
Timer_A interrupt vector          TAIV      Read only  012Eh  Reset with POR
*/







#include <msp430x20x3.h>

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= 0x01;                            // P1.0 output

  CCTL0 = CCIE;                             // CCR0 interrupt enabled
  CCR0 = 50000;
  TACTL = TASSEL_2 + MC_2;                  // SMCLK, contmode

  _BIS_SR(LPM0_bits + GIE);                 // Enter LPM0 w/ interrupt
  for(;;);
}

// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  P1OUT ^= 0x01;                            // Toggle P1.0
  CCR0 += 50000;                            // Add Offset to CCR0
}
























#if 0
void sw_delay(unsigned int i);

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR |= 0x01;                        // Set P1.0 to output direction

  SetupP4
  bis.b #00Eh,&P4SEL ; Select TB1, TB2, TB3 instead of
  bis.b #00Eh,&P4DIR ; P4.x, and set as outputs
  P4SEL |= 0x0E;
  P4DIR |= 0x0E;
    
  /*firgure out how to get these set*/
  TACTL = TASSEL1 | TASSEL0 |
          ID1 | ID0 | 
          MC1 | MC0 |
          TACLR | TAIE | TAIFG;
  TACCTL0 = CM1 | CM0 |
            CCIS1 | CCIS0 |
            SCS | SCCI | CAP |
            OUTMOD2 | OUTMOD1 | OUTMOD0 |
            CCIE | CCI | OUT | COV | CCIFG;
  
  for (;;)
  {
    P1OUT ^= 0x01;                      // Toggle P1.0 using exclusive-OR
    sw_delay(20000);                          // SW Delay
  }
}

void sw_delay(volatile unsigned int i){
  while(i-- != 0);
}
#endif