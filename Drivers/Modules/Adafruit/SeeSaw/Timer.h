#pragma once
#include "SeeSaw.h"
#include "SubModule.h"

namespace SeeSaw
{
	class Timer : public SubModule
	{
	  private: 
		const uint8_t base_address(void);
	  public:
	  
		enum Registers
		{
			STATUS = 0x00,
			PWM = 0x01,
			FREQ = 0x02,
		};
		
		Timer(SeeSaw & _host);
	};
}