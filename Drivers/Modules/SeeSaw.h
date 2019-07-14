#pragma once
#include "../Drivers/TWI.h"

class SeeSaw
{
  public:
    
    /** Module Base Addreses
     *  The module base addresses for different seesaw modules.
     */
    enum
    {
        SEESAW_STATUS_BASE = 0x00,
        SEESAW_GPIO_BASE = 0x01,
        SEESAW_SERCOM0_BASE = 0x02,

        SEESAW_TIMER_BASE = 0x08,
        SEESAW_ADC_BASE = 0x09,
        SEESAW_DAC_BASE = 0x0A,
        SEESAW_INTERRUPT_BASE = 0x0B,
        SEESAW_DAP_BASE = 0x0C,
        SEESAW_EEPROM_BASE = 0x0D,
        SEESAW_NEOPIXEL_BASE = 0x0E,
        SEESAW_TOUCH_BASE = 0x0F,
        SEESAW_KEYPAD_BASE = 0x10,
        SEESAW_ENCODER_BASE = 0x11,
    };
  private:
    template <class T> 
    struct __PACKED register_write
    {
        uint8_t address;
        T value;
    };
    
    const uint8_t TWI_ADDR = 0x49;
    const uint8_t REGISTERS_ADDR = 0x00;
    const uint8_t SENSORS_ADDR = 0x01;
    const uint8_t OFFSET_ADDR = 0x09;
    const uint8_t CTRL_REG_ADDR = 0x10;
    TWI & twi;
  public:
    SeeSaw(TWI & _twi) : twi(_twi)
    {
        
    }
};