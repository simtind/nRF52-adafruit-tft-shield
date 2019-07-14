#pragma once
#include "SeeSaw.h"
#include "SubModule.h"

namespace SeeSaw
{
	class ADC : public SubModule
	{
	  private: 
		const uint8_t base_address(void);
	  public:
		
		enum Registers
		{
			STATUS = 0x00,
			INTEN = 0x02,
			INTENCLR = 0x03,
			WINMODE = 0x04,
			WINTHRESH = 0x05,
			CHANNEL_OFFSET = 0x07,
		};

        enum Channel
        {
            Channel0 = 0,
            Channel1 = 1,
            Channel2 = 2,
            Channel3 = 3
        };
	  
		ADC(SeeSaw & _host);
		
        uint16_t read(Channel channel);
	};
}