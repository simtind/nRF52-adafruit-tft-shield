#pragma once
#include "SeeSaw.h"
#include "SubModule.h"

namespace SeeSaw
{
	class Encoder : public SubModule
	{
	  private: 
		const uint8_t base_address(void);
	  public:
		
		enum Registers
		{
			STATUS = 0x00,
			INTENSET = 0x02,
			INTENCLR = 0x03,
			POSITION = 0x04,
			DELTA = 0x05,
		};
	  
		Encoder(SeeSaw & _host);	

        int32_t getPosition();
        int32_t getDelta();
        void enableInterrupt();
        void disableInterrupt();
        void setPosition(int32_t pos);
	};
}