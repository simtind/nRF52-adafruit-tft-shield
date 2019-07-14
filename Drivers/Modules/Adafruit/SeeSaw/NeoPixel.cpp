#include "NeoPixel.h"
#include "../../../Utilities/driverexcept.h"

namespace SeeSaw
{
    NeoPixel::NeoPixel(SeeSaw & _host) : SubModule(_host)
    {
        if (!host.getOptions().NEOPIXEL)
        {
            throw DriverError("SeeSAW: NeoPixel option not implemented");
        }
    }
    
    const uint8_t NeoPixel::base_address(void) 
    {
        return 0x0E; 
    }
}