#pragma once

#include <nrf.h>
#include "../Peripherals/TWI.h"

class MAG3110
{
    private:        

        template <class T> 
        struct __PACKED register_write
        {
            uint8_t address;
            T value;
        };
        
        const uint8_t TWI_ADDR = 0x0e;
        const uint8_t REGISTERS_ADDR = 0x00;
        const uint8_t SENSORS_ADDR = 0x01;
        const uint8_t OFFSET_ADDR = 0x09;
        const uint8_t CTRL_REG_ADDR = 0x10;
        TWI & twi;
    
    public:
        
        typedef struct __PACKED
        {
            uint8_t ready;
            int16_t data_x;
            int16_t data_y;
            int16_t data_z;
            uint8_t devid;
            uint8_t mode;
            int16_t offset_x;
            int16_t offset_y;
            int16_t offset_z;
            int8_t temp;
            uint8_t ctrl_1;
            uint8_t ctrl_2;
        } registers;
        
        typedef struct __PACKED
        {
            int16_t data_x;
            int16_t data_y;
            int16_t data_z;
        } sensors;
    
        MAG3110(TWI & _twi) : twi(_twi)
        {
            // Set active mode
            // Set maximum over sampling
            // Set maximum data rate    
            uint8_t ctrl_reg_1_value = (1 << 0) | (3 << 3)  | (0 << 5);
        
            write(CTRL_REG_ADDR, ctrl_reg_1_value);   
            
            uint8_t ctrl_reg_2_value = (1 << 0) | (3 << 3)  | (0 << 5);
        
            write(CTRL_REG_ADDR, ctrl_reg_2_value);   
        }
        
        template <class T> void read(const uint8_t & address, const T & data)
        {
            twi.enable();
            twi.write(address);

            twi.start(TWI_ADDR);
            while(!twi.write_finished());
      
            twi.read(data);  
            twi.start(TWI_ADDR);
            while(!twi.read_finished());  
            twi.stop();
            twi.disable();
        }
        
        void read(const registers & data)
        {
            read(REGISTERS_ADDR, data);
        }
        
        void read(const sensors & data)
        {
            read(SENSORS_ADDR, data);
        }
        
        template<class T> void write(uint8_t address, T value)
        {
            register_write<T> packet = 
            {
                .address = address,
                .value = value
            };
            
            twi.enable();
            twi.write(packet);
            twi.start(TWI_ADDR);
            while(!twi.write_finished());
            twi.stop();
            twi.disable();
        }
};