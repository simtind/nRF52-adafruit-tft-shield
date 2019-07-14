#ifndef _TFT_SHIELD_18_H
#define _TFT_SHIELD_18_H

#include "SeeSaw/SeeSaw.h"
#include "SeeSaw/PWM.h"
#include "SeeSaw/GPIO.h"
#include "../../Utilities/delay.h"

/* Adapted from https://github.com/adafruit/Adafruit_Seesaw/blob/master/Adafruit_TFTShield18.h */

/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for interacting with 1.8" tft shield variant of seesaw helper IC
*/
/**************************************************************************/
class Adafruit_TFTShield18 {
    
private:
        
public:
    SeeSaw::SeeSaw host;
    SeeSaw::PWM pwm;
    SeeSaw::GPIO gpio;


    static const uint8_t ADDR = 0x2E;

    static const SeeSaw::PWM::Channel BACKLIGHT_CHANNEL = SeeSaw::PWM::Channel0;
    static const uint16_t BACKLIGHT_ON  = 0xFFFF;
    static const uint16_t BACKLIGHT_OFF = 0x0000;

    static const uint8_t  OUT_RESET_PIN = 3;
    static const uint32_t OUT_RESET = 1UL << OUT_RESET_PIN;

    static const uint8_t  BUTTON_UP_PIN = 5;
    static const uint32_t BUTTON_UP = 1UL << BUTTON_UP_PIN;

    static const uint8_t  BUTTON_DOWN_PIN = 8;
    static const uint32_t BUTTON_DOWN = (1UL << BUTTON_DOWN_PIN);

    static const uint8_t  BUTTON_LEFT_PIN = 6;
    static const uint32_t BUTTON_LEFT = (1UL << BUTTON_LEFT_PIN);

    static const uint8_t  BUTTON_RIGHT_PIN = 9;
    static const uint32_t BUTTON_RIGHT = (1UL << BUTTON_RIGHT_PIN);

    static const uint8_t  BUTTON_IN_PIN = 7;
    static const uint32_t BUTTON_IN = (1UL << BUTTON_IN_PIN);

    static const uint8_t  BUTTON_1_PIN = 10;
    static const uint32_t BUTTON_1 = (1UL << BUTTON_1_PIN);

    static const uint8_t  BUTTON_2_PIN = 11;
    static const uint32_t BUTTON_2 = (1UL << BUTTON_2_PIN);

    static const uint8_t  BUTTON_3_PIN = 14;
    static const uint32_t BUTTON_3 = (1UL << BUTTON_3_PIN);

    static const uint32_t BUTTON_ALL = 
                                (  BUTTON_UP  
                                | BUTTON_DOWN  
                                | BUTTON_LEFT 
                                | BUTTON_RIGHT 
                                | BUTTON_IN 
                                | BUTTON_1 
                                | BUTTON_2 
                                | BUTTON_3);

	Adafruit_TFTShield18(ISerialIO & twi, bool reset=false, uint8_t addr = ADDR) 
        : host(twi, addr)
        , pwm(host)
        , gpio(host) 
    {
        //printf("Setup RESET pin\n");    
        //gpio.mode(OUT_RESET, SeeSaw::GPIO::OUTPUT);
        printf("Setup buttons pin\n");
        gpio.mode(BUTTON_ALL, SeeSaw::GPIO::INPUT, SeeSaw::GPIO::PULLUP);
        
        if (reset)
        {
            printf("RESET\n");
            gpio.write(OUT_RESET, false);
            delay_ms(20);
            gpio.write(OUT_RESET, true);
        }
    };
	~Adafruit_TFTShield18() {};

	void setBacklight(uint16_t value);
	void setBacklightFreq(uint16_t freq);
    void tftReset(bool rst = true);
    uint32_t readButtons();
};

#endif