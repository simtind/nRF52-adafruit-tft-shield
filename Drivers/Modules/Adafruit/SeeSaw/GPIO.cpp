#include "GPIO.h"
#include "../../../Utilities/driverexcept.h"

namespace SeeSaw
{
    
    GPIO::GPIO(SeeSaw & _host) : SubModule(_host)
    {
        if (!host.getOptions().GPIO)
        {
            throw DriverError("SeeSAW: GPIO option not implemented");
        }
    }
    
    const uint8_t GPIO::base_address(void) 
    {
        return 0x01; 
    }    
        
    void GPIO::mode(uint8_t pin, GPIO::Direction direction, GPIO::Pull pull)
    {
        mode((uint32_t)(1 << pin), direction, pull);
    }

    void GPIO::mode(uint32_t pins, GPIO::Direction direction, GPIO::Pull pull)
    {       
        uint8_t pin_select[4] = {0};
        
        for (uint32_t i = 0; i < 4; i++)
        {
            pin_select[i] = ((uint8_t *)&pins)[3-i];
        } 
        
        switch(direction)
        {
            case INPUT:
                SubModule::write(Registers::DIRCLR_BULK, pin_select, 4);
                break;
            case OUTPUT:
                SubModule::write(Registers::DIRSET_BULK, pin_select, 4);
                break;
        }

        switch(pull)
        {
            case PULLUP:
                SubModule::write(Registers::PULLENSET, pin_select, 4);
                SubModule::write(Registers::BULK_SET, pin_select, 4);
                break;
            case PULLDOWN:
                SubModule::write(Registers::PULLENSET, pin_select, 4);
                SubModule::write(Registers::BULK_CLR, pin_select, 4);
                break;
            case NONE:
                SubModule::write(Registers::PULLENCLR, pin_select, 4);
                break;
        }
    }

    void GPIO::mode(uint32_t pinsa, uint32_t pinsb, GPIO::Direction direction, GPIO::Pull pull)
    {
        uint8_t pin_select[8] = {0};
        
        for (uint32_t i = 0; i < 4; i++)
        {
            pin_select[i] = ((uint8_t *)&pinsa)[3-i];
            pin_select[i + 4] = ((uint8_t *)&pinsb)[3-i];
        }

        switch(direction)
        {
            case INPUT:
                SubModule::write(Registers::DIRSET_BULK, pin_select, 8);
                break;
            case OUTPUT:
                SubModule::write(Registers::DIRCLR_BULK, pin_select, 8);
                break;
        }

        switch(pull)
        {
            case PULLUP:
                SubModule::write(Registers::PULLENSET, pin_select, 8);
                SubModule::write(Registers::BULK_SET, pin_select, 8);
                break;
            case PULLDOWN:
                SubModule::write(Registers::PULLENSET, pin_select, 8);
                SubModule::write(Registers::BULK_CLR, pin_select, 8);
                break;
            case NONE:
                SubModule::write(Registers::PULLENCLR, pin_select, 8);
                break;
        }
    }

    void GPIO::write(uint8_t pin, bool value)
    {
        write((uint32_t)(1 << pin), value);
    }

    void GPIO::write(uint32_t pins, bool value)
    {     
        uint8_t pin_select[4] = {0};
        
        for (uint32_t i = 0; i < 4; i++)
        {
            pin_select[i] = ((uint8_t *)&pins)[3-i];
        } 
        
        if (value)
        {
            SubModule::write(Registers::BULK_SET, pin_select, 4);
        }
        else
        {
            SubModule::write(Registers::BULK_CLR, pin_select, 4);
        }
    }

    void GPIO::write(uint32_t pinsa, uint32_t pinsb, bool value)
    {
        uint8_t pin_select[8] = {0};
        
        for (uint32_t i = 0; i < 4; i++)
        {
            pin_select[i] = ((uint8_t *)&pinsa)[3-i];
            pin_select[i + 4] = ((uint8_t *)&pinsb)[3-i];
        } 

        if (value)
        {
            SubModule::write(Registers::BULK_SET, pin_select, 8);
        }
        else
        {
            SubModule::write(Registers::BULK_CLR, pin_select, 8);
        }
    }

    bool GPIO::read(uint8_t pin)
    {
        return read((uint32_t)(1 << pin)) != 0;
    }

    uint32_t GPIO::read(uint32_t pins)
    {
        uint32_t data;
        SubModule::read(Registers::BULK, data, 1000);
        return data & pins;
    }

    void GPIO::enableInterrupts(uint32_t pins)
    {
        SubModule::write(Registers::INTENSET, pins);
    }

    void GPIO::disableInterrupts(uint32_t pins)
    {
        SubModule::write(Registers::INTENCLR, pins);
    }
}