#pragma once
#include <cstdio>
#include <cstring>
#include "../../../Utilities/driverexcept.h"
#include "../../../Utilities/delay.h"
#include "../../../Peripherals/TWIM.h"

namespace SeeSaw
{
	class SeeSaw 
	{
	  private:
        

		/** status module function addres registers
		 */
		enum StatusRegisters
		{
			HW_ID = 0x01,
			VERSION = 0x02,
			OPTIONS = 0x03,
			TEMP = 0x04,
			SWRST = 0x7F,
		};

        struct Version
        {
            uint16_t date;
            uint16_t version;
        };

        struct Options
        {            
            uint32_t STATUS : 0x1;
            uint32_t GPIO   : 0x1;
            uint32_t SERIO  : 0x1;

            uint32_t RESERVED : 0x5;
            
            uint32_t TIMER : 0x1;
            uint32_t ADC : 0x1;
            uint32_t DAC : 0x1;
            uint32_t INTERRUPT : 0x1;
            uint32_t DAP : 0x1;
            uint32_t EEPROM : 0x1;
            uint32_t NEOPIXEL : 0x1;
            uint32_t TOUCH : 0x1;
            uint32_t KEYPAD : 0x1;
            uint32_t ENCODER : 0x1;
        };

		const uint8_t HW_ID_REF  = 0x55;
        const uint8_t SWRST_TRIGGER = 0xFF;
        
        struct __PACKED Address
        {
            uint8_t function;
            uint8_t address;
        };
        
		uint8_t twi_address;
		ISerialIO & twi;
	  public:
		SeeSaw(ISerialIO & _twi, uint8_t _address = 0x49, bool reset=true);	
        
        void send_preamble(uint8_t function, uint8_t address, bool final)
        {
            
            uint8_t address_packet[2] = {function, address};
            
            printf(" $%02x%02x : ", address_packet[0], address_packet[1]);
            
            twi.write(address_packet, 2);
            if (final) {twi.set_final();}
            twi.start(twi_address);
            while (!twi.write_finished());
        }

        void write(uint8_t function, uint8_t address, const uint8_t * data, uint32_t len)
        {
            
            uint8_t write_buf[10] = {0};
            write_buf[0] = function;
            write_buf[1] = address;
            
            if (len > 8)
            {
                throw DriverError("Too long message!");
            }
            
            printf("I2C write $%02x%02x : ", write_buf[0], write_buf[1]);
            
            memcpy(write_buf + 2, data, len);
            
            twi.write(write_buf, len + 2);
            twi.set_final();
            twi.start(twi_address);
            while (!twi.write_finished());
            delay_ms(1);
            twi.stop();
                         
            
            for (int i=0; i<len; i++) {
                printf("0x%02x,", data[i]);
            }
            printf("\n");
        }

        template <class T> void read(uint8_t function, uint8_t address, volatile T & data, uint16_t delay=500)
        {
            
            uint8_t temp[sizeof(data)];
                         
            printf("I2C read ");

            send_preamble(function, address, true);

            delay_us(delay);

            twi.read(temp, sizeof(data));
            twi.set_final();
            twi.start(twi_address);
            while (!twi.read_finished());
            twi.stop();       
            
            for (uint32_t i = 0; i < sizeof(data); i++)
            {
                ((uint8_t *)&data)[i] = temp[sizeof(data) - i - 1];
            }

            
            for (int i=0; i<sizeof(data); i++) {
                printf("0x%02x,", temp[i]);
            }
            printf("\n");
        }

        void setTWIAddr(uint8_t addr);
        uint8_t getTWIAddr();

        Options getOptions();
        Version getVersion();	
        uint8_t getHWID();
		
		void SWReset();
        
        float readTemp();
	};
}