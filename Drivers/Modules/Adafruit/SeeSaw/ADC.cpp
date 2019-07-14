#include "ADC.h"
#include "../../../Utilities/driverexcept.h"

namespace SeeSaw
{
    ADC::ADC(SeeSaw & _host) : SubModule(_host)
    {
        if (!host.getOptions().ADC)
        {
            throw DriverError("SeeSAW: ADC option not implemented");
        }
    }
	
    const uint8_t ADC::base_address(void)
    { 
        return 0x09; 
    }    
    
    uint16_t ADC::read(ADC::Channel channel)
    {
        uint16_t buf;
        uint8_t address = (uint8_t)Registers::CHANNEL_OFFSET + (uint8_t)channel;
        SubModule::read(address, buf, 500);
        return buf;
    }
}
