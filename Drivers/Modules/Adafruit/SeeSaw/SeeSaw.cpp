#include "SeeSaw.h"
#include "../../../Peripherals/TWI.h"
#include "../../../Utilities/driverexcept.h"

namespace SeeSaw
{
    SeeSaw::SeeSaw(ISerialIO &_twi, uint8_t _address, bool reset) : 
        twi_address(_address),
        twi(_twi)
    {
        uint8_t temp[2] = {0x7f, 0xff};
        twi.write(temp, 2);
        twi.set_final();
        twi.start(twi_address);
        while(!twi.write_finished());
            
        if (reset)
        {
            SWReset();
        }

        uint8_t hw_id = getHWID();
        if (hw_id != HW_ID_REF)
        {
            throw DriverError("Wrong HW_ID detected.");
        }
    }

    void SeeSaw::setTWIAddr(uint8_t addr)
    {
        twi_address = addr;
    }

    uint8_t SeeSaw::getTWIAddr()
    {
        return twi_address;
    }

    SeeSaw::Options SeeSaw::getOptions()
    {
        volatile uint32_t options = 0;
        read(0, StatusRegisters::OPTIONS, options);
        return *(Options *)&options;
    }

    SeeSaw::Version SeeSaw::getVersion()
    {
        volatile uint32_t version;
        read(0, StatusRegisters::VERSION, version);  
        return *(Version *)&version;
    }

    uint8_t SeeSaw::getHWID()
    {
        volatile uint8_t hwid;
        read(0, StatusRegisters::HW_ID, hwid);
        return hwid;
    }

    void SeeSaw::SWReset()
    {
        write(0, StatusRegisters::SWRST, &SWRST_TRIGGER, 1);
        delay_ms(1000);
    }
    
    float SeeSaw::readTemp()
    {
        int32_t temp;
        read(0, StatusRegisters::TEMP, temp, 1000);
        return (1.0/(1UL << 16)) * temp;
    }
} // namespace SeeSaw