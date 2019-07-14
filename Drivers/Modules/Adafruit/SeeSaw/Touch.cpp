#include "Touch.h"
#include "../../../Utilities/driverexcept.h"
	
namespace SeeSaw
{
    
    Touch::Touch(SeeSaw & _host) : SubModule(_host)
    {
        if (!host.getOptions().TOUCH)
        {
            throw DriverError("SeeSAW: Touch option not implemented");
        }
    }
    
    const uint8_t Touch::base_address(void) 
    {
        return 0x0F; 
    }

	uint16_t Touch::read(uint8_t pin)
    {
        uint16_t data;
        SubModule::read(Registers::CHANNEL_OFFSET + pin, data, 1000);

        return data;
    }
}