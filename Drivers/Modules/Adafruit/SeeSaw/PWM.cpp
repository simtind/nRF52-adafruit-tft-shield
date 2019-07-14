#include "PWM.h"

namespace SeeSaw
{
    PWM::PWM(SeeSaw & _host) : Timer(_host)
    {
        
    }

    void PWM::setFrequency(Channel channel, uint16_t frequency)
    {        
        uint8_t packet[3] = {channel, (uint8_t)(frequency >> 8), (uint8_t)frequency};

        SubModule::write(Timer::Registers::FREQ, packet, 3);
    }

    void PWM::write(Channel channel, uint8_t data)
    {
        uint8_t packet[2] = {channel, data};

        SubModule::write(Timer::Registers::PWM, packet, 2);
    }

    void PWM::write(Channel channel, uint16_t data)
    {
        uint8_t packet[3] = {channel, (uint8_t)(data >> 8), (uint8_t)data};
    
        SubModule::write(Timer::Registers::PWM, packet, 3);
    }
} // namespace SeeSaw