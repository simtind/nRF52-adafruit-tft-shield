#pragma once
#include "SeeSaw.h"
#include "SubModule.h"

namespace SeeSaw
{
	class KeyPad : public SubModule
	{
	  private: 
		const uint8_t base_address(void);
	  public:
		
		enum Registers
		{
			STATUS = 0x00,
			EVENT = 0x01,
			INTENSET = 0x02,
			INTENCLR = 0x03,
			COUNT = 0x04,
			FIFO = 0x10,
		};
		
		enum EdgeMode
		{
			HIGH = 0,
			LOW,
			FALLING,
			RISING,
		};
		
		/** raw key event stucture for keypad module */
		union keyEventRaw {
			struct {
				uint8_t EDGE: 2; ///< the edge that was triggered
				uint8_t NUM: 6; ///< the event number
			} bit; ///< bitfield format
			uint8_t reg; ///< register format
		};


		/** extended key event stucture for keypad module */
		union keyEvent {
			struct {
				uint8_t EDGE: 2; ///< the edge that was triggered
				uint16_t NUM: 14; ///< the event number
			} bit; ///< bitfield format
			uint16_t reg; ///< register format
		};

		/** key state struct that will be written to seesaw chip keypad module */
        struct keyState
        {
            uint8_t key;
            union {
                struct {
                    uint8_t STATE: 1; ///< the current state of the key
                    uint8_t ACTIVE: 4; ///< the registered events for that key
                } bit; ///< bitfield format
                uint8_t reg; ///< register format
		    } state; 
        };
				
		KeyPad(SeeSaw & _host);

        void setEvent(uint8_t key, uint8_t edge, bool enable=true);
        void enableInterrupt();
        void disableInterrupt();
        uint8_t getCount();
        void read(keyEventRaw & buf);
	};
}