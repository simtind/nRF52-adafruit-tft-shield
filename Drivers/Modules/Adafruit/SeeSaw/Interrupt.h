#pragma once
#include "SeeSaw.h"
#include "../../../Utilities/driverexcept.h"

namespace SeeSaw
{
	class Interrupt : public SubModule
	{
	  private: 
		const uint8_t base_address(void) { return 0x0B; }  
	  public:
		Interrupt(SeeSaw & _host) : SubModule(_host)
		{
            if (!getOptions().INTERRUPT)
            {
                throw DriverError("SeeSAW: Interrupt option not implemented");
            }
		}
	};
}