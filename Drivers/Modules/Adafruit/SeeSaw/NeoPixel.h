#pragma once
#include "SeeSaw.h"
#include "SubModule.h"

namespace SeeSaw
{
	class NeoPixel : public SubModule
	{
	  private: 
		const uint8_t base_address(void);
	  public:
		
		enum
		{
			STATUS = 0x00,
			PIN = 0x01,
			SPEED = 0x02,
			BUF_LENGTH = 0x03,
			BUF = 0x04,
			SHOW = 0x05,
		} Registers;
		
		NeoPixel(SeeSaw & _host);
	};
}