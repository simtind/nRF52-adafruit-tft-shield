#pragma once
#include "SeeSaw.h"
#include "Timer.h"

namespace SeeSaw
{
	class PWM : public Timer
	{
	  public:
        enum Channel
        {
            Channel0 = 0,
            Channel1 = 1,
            Channel2 = 2,
            Channel3 = 3
        };
	  
		PWM(SeeSaw & _host);

        void setFrequency(Channel channel, uint16_t freq);
        void write(Channel channel, uint8_t data);
        void write(Channel channel, uint16_t data);
	};
}