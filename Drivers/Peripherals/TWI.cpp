#include "TWI.h"
#include "Interrupt.h"

#include <nrf.h>

void twi_tx_interrupt_handler(void * instance, volatile uint32_t * event)
{
    static_cast<TWI *>(instance)->tx_interrupt(event);
}

void twi_rx_interrupt_handler(void * instance, volatile uint32_t * event)
{
    static_cast<TWI *>(instance)->rx_interrupt(event);
}

void twi_stop_interrupt_handler(void * instance, volatile uint32_t * event)
{
    static_cast<TWI *>(instance)->stop_interrupt(event);    
}
  
TWI::TWI(
        NRF_TWI_Type * _instance,
        NRF_GPIO_Type * _pio,
        uint32_t scl_pin,
        uint32_t sda_pin,
        frequency_en frequency) :
        SerialIO(_instance, _pio),
        rx_buf(nullptr),
        tx_buf(nullptr),
        rx_buf_len(0),
        tx_buf_len(0),
        iterator(0),
        busy(false)
{    
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    
    pio->PIN_CNF[scl_pin] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                            (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                            (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) |
                            (GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos);
                              
    pio->PIN_CNF[sda_pin] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                            (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                            (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) |
                            (GPIO_PIN_CNF_DRIVE_S0D1 << GPIO_PIN_CNF_DRIVE_Pos);
    
    instance->PSELSCL = scl_pin;
    instance->PSELSDA = sda_pin;
    
    instance->FREQUENCY = (uint32_t)frequency;
    
    Interrupt & interrupt = Interrupt::get_interrupt(instance);
    
    interrupt.register_callback(this, &instance->EVENTS_RXDREADY, twi_rx_interrupt_handler);    
    instance->INTENSET |= TWI_INTENSET_RXDREADY_Enabled << TWI_INTENSET_RXDREADY_Pos;
    
    interrupt.register_callback(this, &instance->EVENTS_TXDSENT, twi_tx_interrupt_handler);    
    instance->INTENSET |= TWI_INTENSET_TXDSENT_Enabled << TWI_INTENSET_TXDSENT_Pos;  
    
    interrupt.register_callback(this, &instance->EVENTS_STOPPED, twi_stop_interrupt_handler);    
    instance->INTENSET |= TWI_INTENSET_STOPPED_Enabled << TWI_INTENSET_STOPPED_Pos;   
    
    instance->ENABLE |= TWI_ENABLE_ENABLE_Enabled << TWI_ENABLE_ENABLE_Pos;
}
        

TWI::~TWI()
{
    stop();
    instance->ENABLE &= ~(TWI_ENABLE_ENABLE_Enabled << TWI_ENABLE_ENABLE_Pos);
    
    Interrupt & interrupt = Interrupt::get_interrupt(instance);
    interrupt.remove_callbacks(&instance->EVENTS_RXDREADY);
    interrupt.remove_callbacks(&instance->EVENTS_TXDSENT);
    interrupt.remove_callbacks(&instance->EVENTS_STOPPED);
}
    
void TWI::set_read_buf_ptr(volatile uint8_t * buf)
{
    rx_buf = buf;
}

void TWI::set_write_buf_ptr(volatile uint8_t * buf)
{
    tx_buf = buf;    
}

void TWI::set_read_buf_len(uint32_t len)
{
    rx_buf_len = len;
}

void TWI::set_write_buf_len(uint32_t len)
{
    tx_buf_len = len;
}

void TWI::read(volatile uint8_t * data, uint32_t length)
{
    set_read_buf_ptr(data);
    set_read_buf_len(length);
    set_mode(TWI::twi_mode_en::READ);
}    
    
bool TWI::read_finished(void)
{
    return mode != TWI::twi_mode_en::READ || !busy;
}

void TWI::write(volatile uint8_t * data, uint32_t length)
{
    set_write_buf_ptr(data);
    set_write_buf_len(length);
    set_mode(TWI::twi_mode_en::WRITE);
}
    
bool TWI::write_finished(void)
{
    return mode != TWI::twi_mode_en::WRITE || !busy;
}
   
void TWI::start(void)
{    
    instance->EVENTS_STOPPED = 0;
    instance->EVENTS_RXDREADY = 0;
    instance->EVENTS_TXDSENT = 0;
    instance->EVENTS_BB = 0;
    instance->EVENTS_ERROR = 0;
    instance->EVENTS_SUSPENDED = 0;
    
    iterator = 0;
    busy = true;
    
    switch (mode)
    {
        case TWI::twi_mode_en::WRITE:
        {
            instance->SHORTS = 0;
            instance->TASKS_STARTTX = 1;
            instance->TXD = tx_buf[iterator++];
            break;
        }
        
        case TWI::twi_mode_en::READ:
        {
            if (rx_buf_len == 1)
            {
                instance->SHORTS = TWI_SHORTS_BB_STOP_Enabled << TWI_SHORTS_BB_STOP_Pos;
            }
            else
            {
                instance->SHORTS = TWI_SHORTS_BB_SUSPEND_Enabled << TWI_SHORTS_BB_SUSPEND_Pos;                
            }    
            
            instance->TASKS_STARTRX = 1;
            
            if (rx_buf_len == 1)
            {
                
            }
            break;
        }
    }
}

void TWI::stop (void)
{
    if (busy)
    {
        instance->TASKS_STOP = 1;
    }
    
    if (mode == twi_mode_en::READ && iterator < rx_buf_len)
    {        
        rx_buf[iterator] = instance->RXD;
        iterator++;
    }
}

void TWI::tx_interrupt(volatile uint32_t * event)
{
    if (!busy)
    {
        return;
    }
    if (iterator >= tx_buf_len)
    {
       busy = false;
       return;
    }
    
    instance->TXD = tx_buf[iterator];
    iterator++;
}

void TWI::rx_interrupt(volatile uint32_t * event)
{
    if (iterator > rx_buf_len - 2)
    {
       busy = false;
       instance->SHORTS = TWI_SHORTS_BB_STOP_Enabled << TWI_SHORTS_BB_STOP_Pos;
    }
    
    rx_buf[iterator] = instance->RXD;
    iterator++;
    
    instance->EVENTS_SUSPENDED = 0;
    instance->TASKS_RESUME = 1;
}

void TWI::stop_interrupt(volatile uint32_t * event)
{
    busy = false;
    
    if (mode == TWI::READ)
    {
        rx_buf[iterator] = instance->RXD;
        iterator++;
    }
}
