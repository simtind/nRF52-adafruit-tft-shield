#pragma once
#include "SeeSaw.h"

namespace SeeSaw
{
    class SubModule
    {
      private:
        
		virtual const uint8_t base_address(void) = 0;
    
      protected:    
		SeeSaw & host;

      public:
        SubModule(SeeSaw & _host) : host(_host)
        {
        
        }
        
        template<class T> void read (uint8_t address, volatile T & data, uint16_t delay=125)
        {
            host.read(base_address(), address, data, delay);
        }

        void write (uint8_t address, const uint8_t * data, uint32_t len)
        {
            host.write(base_address(), address, data, len);        
        }
        void write (uint8_t address, uint32_t & data)
        {
            host.write(base_address(), address, (const uint8_t *)&data, sizeof(uint32_t));        
        }
        void write (uint8_t address, uint16_t & data)
        {
            host.write(base_address(), address, (const uint8_t *)&data, sizeof(uint16_t));        
        }
        void write (uint8_t address, uint8_t & data)
        {
            host.write(base_address(), address, (const uint8_t *)&data, sizeof(uint8_t));        
        }
        void write (uint8_t address, int32_t & data)
        {
            host.write(base_address(), address, (const uint8_t *)&data, sizeof(int32_t));        
        }
        void write (uint8_t address, int16_t & data)
        {
            host.write(base_address(), address, (const uint8_t *)&data, sizeof(int16_t));        
        }
        void write (uint8_t address, int8_t & data)
        {
            host.write(base_address(), address, (const uint8_t *)&data, sizeof(int8_t));        
        }
    };
}