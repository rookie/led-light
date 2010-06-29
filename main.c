
#include "io430.h"
//#include "msp430.h"

void sw_delay(unsigned int i);

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR |= 0x01;                        // Set P1.0 to output direction

  for (;;)
  {
    P1OUT ^= 0x01;                      // Toggle P1.0 using exclusive-OR
    sw_delay(20000);                          // SW Delay
  }
}

void sw_delay(volatile unsigned int i){
  while(i-- != 0);
}