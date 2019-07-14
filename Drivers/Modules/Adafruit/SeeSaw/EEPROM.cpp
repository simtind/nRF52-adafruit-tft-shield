#include "EEPROM.h"
#include "../../../Utilities/driverexcept.h"

namespace SeeSaw
{
    
    EEPROM::EEPROM(SeeSaw & _host) : SubModule(_host)
    {        
        if (!host.getOptions().EEPROM)
        {
            throw DriverError("SeeSAW: EEPROM option not implemented");
        }
    }
    
    const uint8_t EEPROM::base_address(void) 
    {
        return 0x0D; 
    }
}