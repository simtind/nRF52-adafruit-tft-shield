#pragma  once

#include <nrf.h>
#include "Interrupt.h"


class Timer
{
  private:
    NRF_TIMER_Type * instance;
    const uint8_t NUM_COMPARE_CHANNELS = 4;
    const uint8_t ROLLAROUND_RESERVED_CHANNEL = 0;
    
  public:
    enum Bits
    {
        Count32Bits = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos,
        Count24Bits = TIMER_BITMODE_BITMODE_24Bit << TIMER_BITMODE_BITMODE_Pos,
        Count16Bits = TIMER_BITMODE_BITMODE_16Bit << TIMER_BITMODE_BITMODE_Pos,
        Count8Bits  = TIMER_BITMODE_BITMODE_08Bit << TIMER_BITMODE_BITMODE_Pos
    };
    
    enum Prescaler
    {
        DivBy1   = 0,
        DivBy2   = 1,
        DivBy4   = 2,
        DivBy8   = 3,
        DivBy16  = 4,
        DivBy32  = 5,
        DivBy64  = 6,
        DivBy128 = 7,
        DivBy256 = 8,
        DivBy512 = 9
    };
    
    void set_rollaround_point(Prescaler prescaler_en, uint32_t frequency_hz)
    {
        uint32_t prescaler = (uint32_t)(1 << prescaler_en);
        
        float base_frequency = 16000000.0 / prescaler;
        
        uint32_t counts_per_rollover = (uint32_t) (base_frequency / frequency_hz);
        
        float actual_frequency = base_frequency / counts_per_rollover;
        
        instance->CC[ROLLAROUND_RESERVED_CHANNEL] = counts_per_rollover;
        instance->SHORTS |= TIMER_SHORTS_COMPARE0_CLEAR_Enabled << ROLLAROUND_RESERVED_CHANNEL;
    }
    
    Timer(NRF_TIMER_Type * _instance, Prescaler prescaler=DivBy1, uint32_t frequency_hz=0, Bits num_bits=Count32Bits) : instance(_instance) 
    { 
        instance->MODE = TIMER_MODE_MODE_Timer << TIMER_MODE_MODE_Pos;
        instance->BITMODE = num_bits;        
        instance->PRESCALER = prescaler;
        set_rollaround_point(prescaler, frequency_hz);                
    }
    
    void set_urgency(Interrupt::Urgency urgency=Interrupt::Urgency::Lowest)
    {
        Interrupt::get_interrupt(instance).set_urgency(urgency);
    }
    
    void setup_compare(float compare_point, uint32_t channel)
    {
        uint32_t compare_value = (uint32_t)(instance->CC[ROLLAROUND_RESERVED_CHANNEL] * compare_point);
        
        instance->CC[channel] = compare_value;
    }
    
    void register_callback(event_callback callback, uint32_t channel)
    {
        instance->INTENSET |= TIMER_INTENSET_COMPARE0_Enabled << (TIMER_INTENSET_COMPARE0_Pos + channel);
        Interrupt::get_interrupt(instance).register_callback(instance, &NRF_TIMER0->EVENTS_COMPARE[channel], callback);
    }
    
    void start(void)
    {
        instance->TASKS_START = 1;
    }
    
    void stop(void)
    {
        instance->TASKS_STOP = 1;
    }
};


class Counter
{
  private:
    NRF_TIMER_Type * instance;
    const uint8_t NUM_COMPARE_CHANNELS = 4;
    const uint8_t ROLLAROUND_RESERVED_CHANNEL = 0;
    
  public:
    enum Bits
    {
        Count32Bits = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos,
        Count24Bits = TIMER_BITMODE_BITMODE_24Bit << TIMER_BITMODE_BITMODE_Pos,
        Count16Bits = TIMER_BITMODE_BITMODE_16Bit << TIMER_BITMODE_BITMODE_Pos,
        Count8Bits  = TIMER_BITMODE_BITMODE_08Bit << TIMER_BITMODE_BITMODE_Pos
    };
    
    
    void set_rollaround_point(uint32_t max_count)
    {        
        setup_compare(max_count, ROLLAROUND_RESERVED_CHANNEL);
        instance->SHORTS |= TIMER_SHORTS_COMPARE0_CLEAR_Enabled << ROLLAROUND_RESERVED_CHANNEL;
    }
    
    Counter(NRF_TIMER_Type * _instance, Bits num_bits=Count32Bits) : instance(_instance) 
    { 
        instance->MODE = TIMER_MODE_MODE_Counter << TIMER_MODE_MODE_Pos;
        instance->BITMODE = num_bits;
    }
    
    void set_urgency(Interrupt::Urgency urgency=Interrupt::Urgency::Lowest)
    {
        Interrupt::get_interrupt(instance).set_urgency(urgency);        
    }
    
    void setup_compare(uint32_t count, uint8_t channel)
    {
        instance->CC[channel] = count;
    }
    
    void register_callback(event_callback callback, uint8_t channel)
    {
        instance->INTENSET |= 1 << channel;
        Interrupt::get_interrupt(instance).register_callback(instance, &NRF_TIMER0->EVENTS_COMPARE[channel], callback);
    }
    
    void start(void)
    {
        instance->TASKS_START = 1;
    }
    
    void stop(void)
    {
        instance->TASKS_STOP = 1;
    }
};