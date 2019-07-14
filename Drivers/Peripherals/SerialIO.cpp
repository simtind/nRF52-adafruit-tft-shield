#include "SerialIO.h"
#include "Interrupt.h"
#include <nrf.h>
#include <cstdint>

extern "C"
{
    void SPI0_TWI0_IRQHandler(void);
    void SPI1_TWI1_IRQHandler(void);
}

void SPI0_TWI0_IRQHandler()
{
    Interrupt::get_interrupt(SPI0_TWI0_IRQn).Handler();
}

void SPI1_TWI1_IRQHandler()
{
    Interrupt::get_interrupt(SPI1_TWI1_IRQn).Handler();
}