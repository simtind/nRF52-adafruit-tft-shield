#include "TWIM.h"
#include "Interrupt.h"
#include "../Utilities/driverexcept.h"
#include <stdio.h>
#include <stdlib.h>

#include <nrf.h>
    
void twim_error_interrupt_handler(void * instance, volatile uint32_t * event)
{
    char msg[50] = {0};
    uint32_t error = static_cast<TWIM *>(instance)->instance->ERRORSRC;    
    sprintf(msg, "TWI error occured! Got error %i.\n", error);
    throw DriverError(msg);
}
  
TWIM::TWIM(
        NRF_TWIM_Type * _instance,
        NRF_GPIO_Type * _pio,
        uint32_t scl_pin,
        uint32_t sda_pin,
        frequency_en frequency) :
        SerialIO(_instance, _pio),
        final(false)
{    
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    
    pio->PIN_CNF[scl_pin] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                            (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                            (GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos);
                              
    pio->PIN_CNF[sda_pin] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                            (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                            (GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos);
    
    instance->PSEL.SCL = scl_pin;
    instance->PSEL.SDA = sda_pin;
    
    instance->FREQUENCY = (uint32_t)frequency;
    
    Interrupt::get_interrupt(instance).register_callback(this, &instance->EVENTS_ERROR, twim_error_interrupt_handler);
    
    instance->INTENSET |= TWIM_INTENSET_ERROR_Enabled << TWI_INTENSET_ERROR_Pos;
    
    enable();
}
        

TWIM::~TWIM()
{
    stop();
    disable();
}
    
void TWIM::set_read_buf_ptr(volatile uint8_t * buf)
{
    instance->RXD.PTR = (uint32_t)buf;
}

void TWIM::set_write_buf_ptr(const uint8_t * buf)
{
    instance->TXD.PTR = (uint32_t)buf;    
}

void TWIM::set_read_buf_len(uint32_t len)
{
    instance->RXD.MAXCNT = len;
}

void TWIM::set_write_buf_len(uint32_t len)
{
    instance->TXD.MAXCNT = len;
}

void TWIM::read(volatile uint8_t * data, uint32_t length)
{
    set_read_buf_ptr(data);
    set_read_buf_len(length);
    set_mode(TWIM::twi_mode_en::READ);
}    
    
bool TWIM::read_finished(void)
{
    if (mode != TWIM::twi_mode_en::READ )
    {
        return true;
    }
    
    if (final)
    {
        return instance->EVENTS_STOPPED;
    }
    
    return instance->EVENTS_LASTRX;
}

void TWIM::write(const uint8_t * data, uint32_t length)
{
    set_write_buf_ptr(data);
    set_write_buf_len(length);
    set_mode(TWIM::twi_mode_en::WRITE);
}
    
bool TWIM::write_finished(void)
{
    if (mode != TWIM::twi_mode_en::WRITE )
    {
        return true;
    }
    
    if (final)
    {
        return instance->EVENTS_STOPPED;
    }
    
    return instance->EVENTS_LASTTX;
}
   
void TWIM::start(void)
{    
    instance->EVENTS_STOPPED = 0;
    instance->EVENTS_RXSTARTED = 0;
    instance->EVENTS_TXSTARTED = 0;
    instance->EVENTS_LASTRX = 0;
    instance->EVENTS_LASTTX = 0;
    instance->EVENTS_ERROR = 0;
            
    switch (mode)
    {
        case TWIM::twi_mode_en::WRITE:
        {
            if (final)
            {
                instance->SHORTS = TWIM_SHORTS_LASTTX_STOP_Enabled << TWIM_SHORTS_LASTTX_STOP_Pos;
            }
            else
            {
                instance->SHORTS = TWIM_SHORTS_LASTTX_SUSPEND_Enabled << TWIM_SHORTS_LASTTX_SUSPEND_Pos;
            }
            instance->TASKS_STARTTX = 1;
            break;
        }
        
        case TWIM::twi_mode_en::READ:
        {
            if (final)
            {
                instance->SHORTS = TWIM_SHORTS_LASTRX_STOP_Enabled << TWIM_SHORTS_LASTRX_STOP_Pos;
            }
            instance->TASKS_STARTRX = 1;
            break;

        }        
    }
    
    if (instance->EVENTS_SUSPENDED)
    {
        instance->EVENTS_SUSPENDED = 0;
        instance->TASKS_RESUME = 1;
    }
}

void TWIM::stop (void)
{    
    if (instance->EVENTS_SUSPENDED)
    {
        instance->EVENTS_SUSPENDED = 0;
        instance->TASKS_RESUME = 1;
    }
    
    final = false;
    instance->TASKS_STOP = 1;
    instance->SHORTS = 0;
}


void TWIM::set_final(void)
{
    final = true;
}