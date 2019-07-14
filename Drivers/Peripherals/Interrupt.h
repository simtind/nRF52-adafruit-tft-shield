#pragma once

#include <nrf.h>
#include <cstdint>

typedef void (* event_callback)(void * object, volatile uint32_t * event);
static const uint32_t NUM_EVENTS_PER_LINE = 32;

class Interrupt
{    
private:    
    typedef struct
    {
        bool valid;
        void * object;
        volatile uint32_t * event;
        event_callback callback;
    } event_listing;
   
    IRQn_Type interrupt_number; 
    static const uint8_t lowest_urgency = (1 << __NVIC_PRIO_BITS) -1;
public:
    event_listing events[NUM_EVENTS_PER_LINE];
    uint32_t num_events;   
    
    enum Urgency
    {
        Lowest = lowest_urgency,
        Highest = 0
    };
    
    Interrupt(IRQn_Type int_number) : 
        events{0},
        num_events(0),
        interrupt_number(int_number)
        {}    

    void register_callback(void * object, volatile uint32_t * event, event_callback callback);    
    void remove_callbacks(volatile uint32_t * event);
    void set_urgency(Urgency urgency) { NVIC_SetPriority(interrupt_number, (uint32_t)urgency); }
    
    void Handler(void);
    
    static Interrupt & get_interrupt(IRQn_Type interrupt_number);
    static Interrupt & get_interrupt(void * instance);
};