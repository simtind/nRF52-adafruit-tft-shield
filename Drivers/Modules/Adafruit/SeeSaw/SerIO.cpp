#include "SerIO.h"
#include "../../../Utilities/driverexcept.h"

namespace SeeSaw
{
    
    SerIO::SerIO(SeeSaw & _host, uint8_t _channel) :
        SubModule(_host),
        channel(_channel)
    {
        if (!host.getOptions().SERIO)
        {
            throw DriverError("SeeSAW: Serial IO option not implemented");
        }
    }
    
    const uint8_t SerIO::base_address(void) 
    {
        return 0x02 + channel; 
    }

    void SerIO::enableInterrupt(void)
    {
	    uint8_t one = 1;
        SubModule::write(Registers::INTEN, one);
    }

    void SerIO::disableInterrupt(void)
    {
	    uint8_t zero = 0;
        SubModule::write(Registers::INTEN, zero);
    }

    char SerIO::read(void)
    {
        char data;
        SubModule::read(Registers::DATA, data);
        return data;
    }

    void SerIO::setBaud(uint32_t baud)
    {
        SubModule::write(Registers::BAUD, baud);
    }
}