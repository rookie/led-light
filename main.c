
//#include "io430.h"
#include "msp430.h"
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

void sw_delay(unsigned int i);

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR |= 0x01;                        // Set P1.0 to output direction

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