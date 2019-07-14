#include "Encoder.h"
#include "../../../Utilities/driverexcept.h"

namespace SeeSaw
{
    Encoder::Encoder(SeeSaw & _host) : SubModule(_host)
    {
        if (!host.getOptions().ENCODER)
        {
            throw DriverError("SeeSAW: Encoder option not implemented");
        }
    }
    
    const uint8_t Encoder::base_address(void) 
    {
        return 0x11; 
    }

    

    int32_t Encoder::getPosition()
    {
        int32_t position;
        read(Registers::POSITION, position);

        return position;
    }

    int32_t Encoder::getDelta()
    {
        int32_t position;
        read(Registers::DELTA, position);

        return position;
    }

    void    Encoder::enableInterrupt()
    {
        const uint8_t one = 1;
	    write(Registers::INTENSET, &one, 1ul);
    }

    void    Encoder::disableInterrupt()
    {
        const uint8_t one = 1;
	    write(Registers::INTENCLR, &one, 1ul);
    }

    void    Encoder::setPosition(int32_t position)
    {
        write(Registers::POSITION, position);
    }
}