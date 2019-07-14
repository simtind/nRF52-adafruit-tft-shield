#include "Interrupt.h"
#include <nrf.h>
#include <cstdint>

#define NUM_INTERRUPTS (128)

static Interrupt * interrupts[NUM_INTERRUPTS] = {nullptr};

Interrupt & Interrupt::get_interrupt(IRQn_Type interrupt_number)
{
    if (interrupts[interrupt_number] == nullptr)
    {
        interrupts[interrupt_number] = new Interrupt(interrupt_number);
    }
    
    return *interrupts[interrupt_number];
}


Interrupt & Interrupt::get_interrupt(void * instance)
{
    IRQn_Type interrupt_number = (IRQn_Type)((((uint32_t) instance & 0x000FF000) >> 12) & 0x000000FF);
    
    return Interrupt::get_interrupt(interrupt_number);
}

void Interrupt::register_callback(void * object, volatile uint32_t * event, event_callback callback)
{
    uint32_t callback_index = num_events;
    for (uint32_t i = 0; i < num_events; i++)
    {
        if (events[i].event == event)
        {
            callback_index = i;
            break;
        }
    }
    
    if (callback_index >= NUM_EVENTS_PER_LINE)
    {
        return;
    }
    
    events[callback_index].valid = true;
    events[callback_index].event = event;
    events[callback_index].callback = callback;
    events[callback_index].object = object;
    
    NVIC_ClearPendingIRQ(interrupt_number);
    NVIC_EnableIRQ(interrupt_number);
    
    if (callback_index == num_events)
    {
        num_events++;
    }
}

void Interrupt::remove_callbacks(volatile uint32_t * event)
{
    bool interrupts = false;
    for (uint32_t i = 0; i < num_events; i++)
    {
        if (events[i].event == event)
        {
            events[i].valid = false;
        }
        interrupts |= events[i].valid;
    }
    
    if (!interrupts)
    {
        NVIC_DisableIRQ(interrupt_number);
    }
}

void Interrupt::Handler(void)
{
    for (uint32_t i = 0; i < num_events; i++)
    {
        event_listing event = events[i];
        if (event.valid && *event.event != 0)
        {
            *event.event = 0;
            event.callback(event.object, event.event);
        }
    }
}