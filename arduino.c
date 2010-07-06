//#include <intrinsics.h>
#include "msp430.h"
#include "arduino.h"

// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

// Timer A0 interrupt service routine
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
//void SIGNAL(int TIMER0_OVF_vect)
{        // copy these to local variables so they can be stored in registers
        // (volatile variables must be read from memory on every access)
        unsigned long m = timer0_millis;
        unsigned char f = timer0_fract;

//  P1OUT ^= 0x01;                            // Toggle P1.0

        m += MILLIS_INC;
        f += FRACT_INC;
        if (f >= FRACT_MAX) {
                f -= FRACT_MAX;
                m += 1;
        }

        timer0_fract = f;
        timer0_millis = m;
        timer0_overflow_count++;
}

unsigned long millis()
{
        unsigned long m;
        istate_t state = __get_interrupt_state();
        __disable_interrupt();
//        uint8_t oldSREG = SREG;
        // disable interrupts while we read timer0_millis or we might get an
        // inconsistent value (e.g. in the middle of a write to timer0_millis)
//        cli();

         m = timer0_millis;
         
         __set_interrupt_state(state);
//        SREG = oldSREG;
        return m;
}
#if 0
unsigned long micros() {
        unsigned long m;
//        uint8_t oldSREG = SREG, t;
        uint8_t t = 0;
        istate_t state = __get_interrupt_state();
        __disable_interrupt();
        
//        cli();
        m = timer0_overflow_count;
//        t = TCNT0;
#warning what is this tcnt0 doing?
 
#ifdef TIFR0
        if ((TIFR0 & _BV(TOV0)) && (t < 255))
                m++;
#else
//        if ((TIFR & _BV(TOV0)) && (t < 255))
//                m++;
#endif

//        SREG = oldSREG;
         __set_interrupt_state(state);
       
        return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}
#endif
void delay(unsigned long ms)
{
  /*old*/
  unsigned long start = millis();
  
  while (millis() - start <= ms)
    ;
  /* new
        uint16_t start = (uint16_t)micros();

        while (ms > 0) {
                if (((uint16_t)micros() - start) >= 1000) {
                        ms--;
                        start += 1000;
                }
        }
  */
}


void init()
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT 
  
  BCSCTL1 = CALBC1_16MHZ; /*use precalibrated values to set 16mhz clock*/
  DCOCTL  = CALDCO_16MHZ; 
  _BIS_SR(OSCOFF);        /*we are not using an external oscillator*/
  
  BCSCTL2 = DIVS_3;       /*set smclk prescaler to 8*/
  /*NOTE: there is no reason for this other than to stick timer to a prescale
    of 64 after the next 8 divide like in the arduino libary*/
  
  
  TACCTL0 = CCIE;         // timer0 CCR0 interrupt enabled
  TACCR0 = 0xFF;
  TACTL = TASSEL_2 + ID_3 + MC_1;     /* SMCLK, /8, upmode*/
  
  __enable_interrupt();
   
  return;
#if 0
        // this needs to be called before setup() or some functions won't
        // work there
        sei();
       
        // on the ATmega168, timer 0 is also used for fast hardware pwm
        // (using phase-correct PWM would mean that timer 0 overflowed half as often
        // resulting in different millis() behavior on the ATmega8 and ATmega168)
#if !defined(__AVR_ATmega8__)
        sbi(TCCR0A, WGM01);
        sbi(TCCR0A, WGM00);
#endif  
        // set timer 0 prescale factor to 64
#if defined(__AVR_ATmega8__)
        sbi(TCCR0, CS01);
        sbi(TCCR0, CS00);
#else
        sbi(TCCR0B, CS01);
        sbi(TCCR0B, CS00);
#endif
        // enable timer 0 overflow interrupt
#if defined(__AVR_ATmega8__)
        sbi(TIMSK, TOIE0);
#else
        sbi(TIMSK0, TOIE0);
#endif

        // timers 1 and 2 are used for phase-correct hardware pwm
        // this is better for motors as it ensures an even waveform
        // note, however, that fast pwm mode can achieve a frequency of up
        // 8 MHz (with a 16 MHz clock) at 50% duty cycle

        // set timer 1 prescale factor to 64
        sbi(TCCR1B, CS11);
        sbi(TCCR1B, CS10);
        // put timer 1 in 8-bit phase correct pwm mode
        sbi(TCCR1A, WGM10);

        // set timer 2 prescale factor to 64
#if defined(__AVR_ATmega8__)
        sbi(TCCR2, CS22);
#else
        sbi(TCCR2B, CS22);
#endif
        // configure timer 2 for phase correct pwm (8-bit)
#if defined(__AVR_ATmega8__)
        sbi(TCCR2, WGM20);
#else
        sbi(TCCR2A, WGM20);
#endif


#if defined(__AVR_ATmega1280__)
        // set timer 3, 4, 5 prescale factor to 64
        sbi(TCCR3B, CS31);      sbi(TCCR3B, CS30);
        sbi(TCCR4B, CS41);      sbi(TCCR4B, CS40);
        sbi(TCCR5B, CS51);      sbi(TCCR5B, CS50);
        // put timer 3, 4, 5 in 8-bit phase correct pwm mode
        sbi(TCCR3A, WGM30);
        sbi(TCCR4A, WGM40);
        sbi(TCCR5A, WGM50);
#endif

        // set a2d prescale factor to 128
        // 16 MHz / 128 = 125 KHz, inside the desired 50-200 KHz range.
        // XXX: this will not work properly for other clock speeds, and
        // this code should use F_CPU to determine the prescale factor.
        sbi(ADCSRA, ADPS2);
        sbi(ADCSRA, ADPS1);
        sbi(ADCSRA, ADPS0);

        // enable a2d conversions
        sbi(ADCSRA, ADEN);

        // the bootloader connects pins 0 and 1 to the USART; disconnect them
        // here so they can be used as normal digital i/o; they will be
        // reconnected in Serial.begin()
#if defined(__AVR_ATmega8__)
        UCSRB = 0;
#else
        UCSR0B = 0;
#endif

#endif //if zero
}