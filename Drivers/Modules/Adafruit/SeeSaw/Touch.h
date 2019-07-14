#pragma once
#include "SeeSaw.h"
#include "SubModule.h"

namespace SeeSaw
{
	class Touch : public SubModule
	{
	  private: 
		const uint8_t base_address(void);
	  public:
		
		enum Registers
		{
			CHANNEL_OFFSET = 0x10,
		};
	  
		Touch(SeeSaw & _host);
		
		uint16_t read(uint8_t pin);
	};
}