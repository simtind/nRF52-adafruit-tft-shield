#include <nrf.h>
#include "Timer.h"

extern "C"
{
    void TIMER0_IRQHandler(void);
    void TIMER1_IRQHandler(void);
    void TIMER2_IRQHandler(void);
}

void TIMER0_IRQHandler()
{
    Interrupt::get_interrupt(TIMER0_IRQn).Handler();
}

void TIMER1_IRQHandler()
{
    Interrupt::get_interrupt(TIMER0_IRQn).Handler();
}

void TIMER2_IRQHandler()
{
    Interrupt::get_interrupt(TIMER0_IRQn).Handler();
}