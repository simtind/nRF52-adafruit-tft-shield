#pragma once
#include "SeeSaw.h"
#include "SubModule.h"

namespace SeeSaw
{
	class GPIO : public SubModule
	{
	  private: 
		const uint8_t base_address(void);        

        struct __PACKED PinSelect
        {
            uint32_t pinsa;
            uint32_t pinsb;
        };
	  public:
		
		enum Registers
		{
			DIRSET_BULK = 0x02,
			DIRCLR_BULK = 0x03,
			BULK = 0x04,
			BULK_SET = 0x05,
			BULK_CLR = 0x06,
			BULK_TOGGLE = 0x07,
			INTENSET = 0x08,
			INTENCLR = 0x09,
			INTFLAG = 0x0A,
			PULLENSET = 0x0B,
			PULLENCLR = 0x0C,
		};

        enum Direction
        {
            OUTPUT,
            INPUT,
        };

        enum Pull
        {
            NONE,
            PULLUP,
            PULLDOWN,
        };
	  
		GPIO(SeeSaw & _host);
		
        void mode(uint8_t pin, Direction direction, Pull pull=NONE);
        void mode(uint32_t pins, Direction direction, Pull pull=NONE);
        void mode(uint32_t pinsa, uint32_t pinsb, Direction direction, Pull pull=NONE);
		
        void write(uint8_t pin, bool value);
        void write(uint32_t pins, bool value);
        void write(uint32_t pinsa, uint32_t pinsb, bool value);

        bool read(uint8_t pin);
        uint32_t read(uint32_t pins);

        void enableInterrupts(uint32_t pins);
        void disableInterrupts(uint32_t pins);
	};
}