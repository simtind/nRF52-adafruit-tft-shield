#pragma once
#include "SeeSaw.h"
#include "SubModule.h"

namespace SeeSaw
{
	class EEPROM : public SubModule
	{
	  private: 
		const uint8_t base_address(void);
		
	  public:
		
		enum
		{
			I2C_ADDRESS = 0x3F,
		} Registers;

		EEPROM(SeeSaw & _host);

        template<class T> void write(uint8_t addr, T & data);
        template<class T> void read8(uint8_t addr, T & data);
	};
}