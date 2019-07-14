#include "KeyPad.h"
#include "../../../Utilities/driverexcept.h"

namespace SeeSaw
{
    
    KeyPad::KeyPad(SeeSaw & _host) : SubModule(_host)
    {
        if (!host.getOptions().KEYPAD)
        {
            throw DriverError("SeeSAW: KeyPad option not implemented");
        }
    }
    
    const uint8_t KeyPad::base_address(void) 
    {
        return 0x10; 
    }

    void KeyPad::setEvent(uint8_t key, uint8_t edge, bool enable)
    {
        keyState state = 
        {
            .key = key,
            .state = 
            {
                .bit = 
                {
                    .STATE = enable,
                    .ACTIVE = (uint8_t)(1 << edge)
                }
            }
        };
        
        SubModule::write(Registers::STATUS, (uint8_t *)&state, 2);
    }

    void KeyPad::enableInterrupt()
    {
        uint8_t one = 1;
        SubModule::write(Registers::INTENSET, one);
    }

    void KeyPad::disableInterrupt()
    {
        uint8_t one = 1;
        SubModule::write(Registers::INTENCLR, one);
    }

    uint8_t KeyPad::getCount()
    {
        uint8_t count;
        SubModule::read(Registers::COUNT, count, 500);
        return count;
    }

    void KeyPad::read(keyEventRaw & buf)
    {
        SubModule::read(Registers::FIFO, buf, 1000);
    }
}