#include "SPI.h"
#include "interrupt.h"

#include <nrf.h>

void spi_interrupt_handler(void * instance, volatile uint32_t * event)
{
    static_cast<SPI *>(instance)->interrupt(event);
}
  
SPI::SPI(
    NRF_SPI_Type * _instance,
    NRF_GPIO_Type * _pio,
    uint32_t sck_pin,
    uint32_t mosi_pin,
    uint32_t miso_pin,
    frequency_en frequency,
    clock_mode_en clk_mode,
    bool msb_first) :
        SerialIO(_instance, _pio),
        rx_buf(nullptr),
        tx_buf(nullptr),
        rx_buf_len(0),
        tx_buf_len(0),
        rx_iterator(0),
        tx_iterator(0),
        busy(false)
{        
    instance->PSELSCK = sck_pin;
    instance->PSELMOSI = mosi_pin;
    instance->PSELMISO = miso_pin;
    
    instance->FREQUENCY = (uint32_t)frequency;
     
    if (msb_first)
    {
        instance->CONFIG = SPI_CONFIG_ORDER_MsbFirst << SPI_CONFIG_ORDER_Pos;         
    }
    else
    {
        instance->CONFIG = SPI_CONFIG_ORDER_LsbFirst << SPI_CONFIG_ORDER_Pos;
    }    
    
    switch(clk_mode)
    {
        case MODE0:
            instance->CONFIG |= SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos;
            instance->CONFIG |= SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos;
        break;
        case MODE1:
            instance->CONFIG |= SPI_CONFIG_CPOL_ActiveHigh << SPI_CONFIG_CPOL_Pos;
            instance->CONFIG |= SPI_CONFIG_CPHA_Trailing << SPI_CONFIG_CPHA_Pos;
        break;
        case MODE2:
            instance->CONFIG |= SPI_CONFIG_CPOL_ActiveLow << SPI_CONFIG_CPOL_Pos;
            instance->CONFIG |= SPI_CONFIG_CPHA_Leading << SPI_CONFIG_CPHA_Pos;
        break;
        case MODE3:
            instance->CONFIG |= SPI_CONFIG_CPOL_ActiveLow << SPI_CONFIG_CPOL_Pos;
            instance->CONFIG |= SPI_CONFIG_CPHA_Trailing << SPI_CONFIG_CPHA_Pos;
        break;
    }
    
    Interrupt & interrupt = Interrupt::get_interrupt(instance);    
    interrupt.register_callback(this, &instance->EVENTS_READY, spi_interrupt_handler);
        
    instance->INTENSET |= SPI_INTENSET_READY_Set << SPI_INTENSET_READY_Pos;   
    
    instance->ENABLE |= SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos; 
}
        

SPI::~SPI()
{
    stop();
    instance->ENABLE &= ~(SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);
}
    
void SPI::set_read_buf_ptr(volatile uint8_t * buf)
{
    rx_buf = buf;
}

void SPI::set_write_buf_ptr(const uint8_t * buf)
{
    tx_buf = buf;    
}

void SPI::set_read_buf_len(uint32_t len)
{
    rx_buf_len = len;
}

void SPI::set_write_buf_len(uint32_t len)
{
    tx_buf_len = len;
}

void SPI::read(volatile uint8_t * data, uint32_t length)
{
    set_read_buf_ptr(data);
    set_read_buf_len(length);
}    
    
bool SPI::read_finished(void)
{
    return !busy;
}

void SPI::write(const uint8_t * data, uint32_t length)
{
    set_write_buf_ptr(data);
    set_write_buf_len(length);
}
    
    
bool SPI::write_finished(void)
{
    return !busy;
}
   
void SPI::start(void)
{
    if (busy)
    {
        return;
    }
    
    busy = true;
    rx_iterator = 0;
    tx_iterator = 1;
    instance->TXD = tx_buf[0];
}

void SPI::stop (void)
{
    busy = false;
}

void SPI::interrupt(volatile uint32_t * event)
{      
    if (!busy)
    {
        return;
    }
    
    if (tx_iterator < tx_buf_len)
    {
        instance->TXD = tx_buf[tx_iterator];
        tx_iterator++;
    }
        
    if (rx_iterator < rx_buf_len)
    {            
        rx_buf[rx_iterator] = instance->RXD;
        rx_iterator++;
    }
    else
    {
        uint32_t val = instance->RXD;
    }
    
    if (rx_iterator >= rx_buf_len && tx_iterator >= tx_buf_len)
    {
        stop();
    }
}
