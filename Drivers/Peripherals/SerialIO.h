#pragma once

#include <nrf.h>
#include <cstdint>

class ISerialIO
{
public:    

    virtual void start(uint32_t slave) = 0;
    virtual void stop (uint32_t slave) = 0;
    
    virtual void start(void) = 0;
    virtual void stop (void) = 0;
    
    virtual void set_final(void) = 0;
    
    template<class Y> void read(const volatile Y & data)
    { 
        return this->read((volatile uint8_t *)&data, sizeof(data)); 
    }
    virtual void read(volatile uint8_t * data, uint32_t length) = 0;
    virtual bool read_finished(void) = 0;
    
    template<class Y> void write(const Y & data)
    { 
        return this->write((uint8_t *)&data, sizeof(data)); 
    }
    virtual void write(const uint8_t * data, uint32_t length) = 0;
    virtual bool write_finished(void) = 0;
    
};

template <class T> class SerialIO : public ISerialIO
{
public:
    T * instance;
    NRF_GPIO_Type * pio;
    
    SerialIO(T * _instance, NRF_GPIO_Type * _pio) : instance(_instance), pio(_pio) {}     
};