#include "SPI.h"

#include <nrf.h>
  
SPIM::SPIM(
    NRF_SPIM_Type * _instance,
    NRF_GPIO_Type * _pio,
    uint32_t sck_pin,
    uint32_t mosi_pin,
    uint32_t miso_pin,
    frequency_en frequency,
    clock_mode_en clk_mode,
    bool msb_first) :
        instance(_instance),
        pio(_pio)
{    
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    
    pio->PIN_CNF[miso_pin] = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                             (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                             (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos);
                              
    pio->PIN_CNF[mosi_pin] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
                             (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) |
                             (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos);
                              
    pio->PIN_CNF[sck_pin] = (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos) |
                            (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos) |
                            (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos);
            
    instance->PSEL.SCK = sck_pin;
    instance->PSEL.MOSI = mosi_pin;
    instance->PSEL.MISO = miso_pin;
    
    instance->FREQUENCY = (uint32_t)frequency;
     
    if (msb_first)
    {
        instance->CONFIG = SPIM_CONFIG_ORDER_MsbFirst << SPIM_CONFIG_ORDER_Pos;         
    }
    else
    {
        instance->CONFIG = SPIM_CONFIG_ORDER_LsbFirst << SPIM_CONFIG_ORDER_Pos;
    }    
    
    switch(clk_mode)
    {
        case MODE0:
            instance->CONFIG |= SPIM_CONFIG_CPOL_ActiveHigh << SPIM_CONFIG_CPOL_Pos;
            instance->CONFIG |= SPIM_CONFIG_CPHA_Leading << SPIM_CONFIG_CPHA_Pos;
        break;
        case MODE1:
            instance->CONFIG |= SPIM_CONFIG_CPOL_ActiveHigh << SPIM_CONFIG_CPOL_Pos;
            instance->CONFIG |= SPIM_CONFIG_CPHA_Trailing << SPIM_CONFIG_CPHA_Pos;
        break;
        case MODE2:
            instance->CONFIG |= SPIM_CONFIG_CPOL_ActiveLow << SPIM_CONFIG_CPOL_Pos;
            instance->CONFIG |= SPIM_CONFIG_CPHA_Leading << SPIM_CONFIG_CPHA_Pos;
        break;
        case MODE3:
            instance->CONFIG |= SPIM_CONFIG_CPOL_ActiveLow << SPIM_CONFIG_CPOL_Pos;
            instance->CONFIG |= SPIM_CONFIG_CPHA_Trailing << SPIM_CONFIG_CPHA_Pos;
        break;
    }
    
    instance->ENABLE |= SPIM_ENABLE_ENABLE_Enabled << SPIM_ENABLE_ENABLE_Pos;
        
}
        

SPIM::~SPIM()
{
    instance->TASKS_STOP = 1;
    instance->ENABLE &= ~(SPIM_ENABLE_ENABLE_Enabled << SPIM_ENABLE_ENABLE_Pos);
}

void SPIM::read(uint8_t & data)
{
    return this->read(&data, 1);
}

void SPIM::read(uint8_t * data, uint32_t length)
{    
    instance->RXD.AMOUNT = length;
    instance->RXD.MAXCNT = (uint32_t)data;
}    
    
bool SPIM::read_finished(void)
{
    if (instance->EVENTS_ENDRX)
    {
        instance->EVENTS_ENDRX = 0;
        return true;
    }
    else
    {
        return false;
    }
}
    
    
void SPIM::write(uint8_t & data)
{
    return this->write(&data, 1);
}

void SPIM::write(uint8_t * data, uint32_t length)
{
    instance->TXD.AMOUNT = length;
    instance->TXD.MAXCNT = (uint32_t)data;
}
    
    
bool SPIM::write_finished(void)
{
    if (instance->EVENTS_ENDTX)
    {
        instance->EVENTS_ENDTX = 0;
        return true;
    }
    else
    {
        return false;
    }
}
