#include "DAC.h"
#include "../../../Utilities/driverexcept.h"

namespace SeeSaw
{
    DAC::DAC(SeeSaw & _host) : PWM(_host)
    {
        if (!host.getOptions().DAC)
        {
            throw DriverError("SeeSAW: DAC option not implemented");
        }
    }
}
