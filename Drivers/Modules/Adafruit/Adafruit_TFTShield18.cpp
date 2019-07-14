#include "Adafruit_TFTShield18.h"

/* Adapted from https://github.com/adafruit/Adafruit_Seesaw/blob/master/Adafruit_TFTShield18.cpp */

/**************************************************************************/
/*! 
    @brief  set the intensity of the backlight
    @param  value to set the backlight to, 
			0x0000 = 0% 	(TFTSHIELD_BACKLIGHT_OFF)
			  to
			0xFFFF = 100%	(TFTSHIELD_BACKLIGHT_ON)

*/
/**************************************************************************/
void Adafruit_TFTShield18::setBacklight(uint16_t value){
    pwm.write(BACKLIGHT_CHANNEL, value);
}

/**************************************************************************/
/*! 
    @brief  set the PWM frequency for the backlight
    @param  freq the frequency to set the backlight to
*/
/**************************************************************************/
void Adafruit_TFTShield18::setBacklightFreq(uint16_t freq){
    pwm.setFrequency(BACKLIGHT_CHANNEL, freq);
}

/**************************************************************************/
/*! 
    @brief  reset the TFT screen by setting the value of the reset pin
    @param  rst the value to set the reset pin to
*/
/**************************************************************************/
void Adafruit_TFTShield18::tftReset(bool rst){
    gpio.write(OUT_RESET, rst);
}

/**************************************************************************/
/*! 
    @brief  read all buttons on the wing and return as a 32 bit integer
    @returns the value of the buttons
*/
/**************************************************************************/
uint32_t Adafruit_TFTShield18::readButtons(){
    return gpio.read(BUTTON_ALL);
}