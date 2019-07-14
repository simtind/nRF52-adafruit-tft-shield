#include "SeeSaw.h"
#include "Timer.h"
#include "../../../Utilities/delay.h"
#include "../../../Utilities/driverexcept.h"

namespace SeeSaw
{
    Timer::Timer(SeeSaw & _host) : SubModule(_host)
    {
        auto options = host.getOptions();
        if (!options.TIMER)
        {
            throw DriverError("SeeSAW: Timer option not implemented");
        }
    }

    const uint8_t Timer::base_address(void)
    {
        return 0x08;
    }
} // namespace SeeSaw