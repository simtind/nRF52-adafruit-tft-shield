/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*            (c) 2014 - 2019 SEGGER Microcontroller GmbH             *
*                                                                    *
*           www.segger.com     Support: support@segger.com           *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* Redistribution and use in source and binary forms, with or         *
* without modification, are permitted provided that the following    *
* conditions are met:                                                *
*                                                                    *
* - Redistributions of source code must retain the above copyright   *
*   notice, this list of conditions and the following disclaimer.    *
*                                                                    *
* - Neither the name of SEGGER Microcontroller GmbH                  *
*   nor the names of its contributors may be used to endorse or      *
*   promote products derived from this software without specific     *
*   prior written permission.                                        *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED.                                                        *
* IN NO EVENT SHALL SEGGER Microcontroller GmbH BE LIABLE FOR        *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  *
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;    *
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF      *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE  *
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH   *
* DAMAGE.                                                            *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stdio.h>
#include <stdlib.h>
#include <nrf.h>
#include "Drivers/Utilities/driverexcept.h"
#include "Drivers/Utilities/delay.h"

#include "Drivers/Peripherals/SPI.h"
#include "Drivers/Peripherals/Timer.h"
#include "Drivers/Peripherals/Interrupt.h"
#include "Drivers/Modules/MAG3110.h"
#include "Drivers/Modules/Adafruit/Adafruit_TFTShield18.h"

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/

#define MOSI    (23) //MISO
#define MISO    (24) //MOSI
#define SCK     (25) //SCK
#define CS_TFT  (22) //SCK
#define CS_SD   (15) //SCK

#define SCL     (27)
#define SDA     (26)

TWIM twi(NRF_TWIM1, NRF_GPIO, SCL, SDA, TWIM::K100);
//MAG3110 accelerometer(twi);

void timed_read_accel_data(void * instance, volatile uint32_t * event)
{
    MAG3110::registers data;
    
    //accelerometer.read(data);
    
    printf("X = %6.3f, Y = %6.3f, Z = %6.3f\n", data.data_x/(float)INT16_MAX, data.data_y/(float)INT16_MAX, data.data_z/(float)INT16_MAX);
}

int main(void) {
        
    try
    {    
        SPI spi(NRF_SPI0, NRF_GPIO, MISO, MOSI, SCK);

        uint8_t write_buf[20] = {0};
        volatile uint8_t read_buf[20] = {0};

        for (uint32_t i = 0; i < 100; i++) {
            sprintf((char *)write_buf, "Hello World %d!\n", i);

            spi.write(write_buf, sizeof(write_buf));
            spi.read(read_buf, sizeof(read_buf));

            spi.start(3);

            while(!spi.write_finished());    
            while(!spi.read_finished());

            spi.stop(3);

            printf((char *)read_buf);
        }
    
        /*Timer timer(NRF_TIMER0, Timer::DivBy512, 10);
    
        timer.register_callback(timed_read_accel_data, 0);
        timer.set_urgency(Interrupt::Lowest);*/
        //timer.start();
    
        Adafruit_TFTShield18 tftshield(twi);
        auto version = tftshield.host.getVersion();
        printf("Options: %04x\n", tftshield.host.getOptions());
        printf("HWID: %02x\n", tftshield.host.getHWID());
        printf("Date: %04x Version: %04x\n", version.date, version.version);
                
        tftshield.setBacklightFreq(1000);
        tftshield.setBacklight(tftshield.BACKLIGHT_ON);
        
        uint32_t led_mask = (1 << 17) | (1 << 18) | (1 << 19) | (1 << 20);
        NRF_GPIO->DIRSET = led_mask;
        NRF_GPIO->OUTCLR = led_mask;
        
        uint32_t i = 17;   
        
        while (true)
        {            
            i = (i + 1) > 20 ? 17 : i + 1;
            NRF_GPIO->OUT = (NRF_GPIO->OUT & ~led_mask) | (1 << i);
            uint32_t buttons = tftshield.readButtons();
            const char * UP = buttons & tftshield.BUTTON_UP     ? "1" : "0";
            const char * DN = buttons & tftshield.BUTTON_DOWN   ? "1" : "0";
            const char * RT = buttons & tftshield.BUTTON_RIGHT  ? "1" : "0";
            const char * LT = buttons & tftshield.BUTTON_LEFT   ? "1" : "0";
            const char * BI = buttons & tftshield.BUTTON_IN     ? "1" : "0";
            const char * BA = buttons & tftshield.BUTTON_1      ? "1" : "0";
            const char * BB = buttons & tftshield.BUTTON_2      ? "1" : "0";
            const char * BC = buttons & tftshield.BUTTON_3      ? "1" : "0";
            printf("Buttons = %08x\n", buttons);
            printf("UP: %s DN: %s RT: %s LT: %s\n", UP, DN, RT, LT);
            printf("BI: %s BA: %s BB: %s BC: %s\n", BI, BA, BB, BC);
            
            if (buttons & tftshield.BUTTON_UP)
                NRF_GPIO->OUTSET |= 1 << 17;
            else
                NRF_GPIO->OUTCLR |= 1 << 17;
                
            if (buttons & tftshield.BUTTON_DOWN)
                NRF_GPIO->OUTSET |= 1 << 18;
            else
                NRF_GPIO->OUTCLR |= 1 << 18;

            if (buttons & tftshield.BUTTON_RIGHT)
                NRF_GPIO->OUTSET |= 1 << 19;
            else
                NRF_GPIO->OUTCLR |= 1 << 19;

            if (buttons & tftshield.BUTTON_LEFT)
                NRF_GPIO->OUTSET |= 1 << 20;
            else
                NRF_GPIO->OUTCLR |= 1 << 20;
                
            delay_ms(100);
        }
    }
    catch(DriverError e)
    {
        printf(e.what());
    }

}

/*************************** End of file ****************************/
