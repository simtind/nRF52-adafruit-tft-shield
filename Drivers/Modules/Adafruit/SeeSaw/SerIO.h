#pragma once
#include "SeeSaw.h"
#include "SubModule.h"

namespace SeeSaw
{
	class SerIO : public SubModule
	{
	  private: 
		const uint8_t base_address(void);
        uint8_t channel;
	  public:
		
		enum Registers
		{
			STATUS = 0x00,
			INTEN = 0x02,
			INTENCLR = 0x03,
			BAUD = 0x04,
			DATA = 0x05,
		};
	  
		SerIO(SeeSaw & _host, uint8_t channel=0);

        void enableInterrupt(void);
        void disableInterrupt(void);

        char read(void);

        void setBaud(uint32_t baud);
	};
}