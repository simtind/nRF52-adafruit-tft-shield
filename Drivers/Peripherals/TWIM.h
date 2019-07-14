#pragma once
#include <nrf.h>
#include "SerialIO.h"


class TWIM : public SerialIO<NRF_TWIM_Type>
{ 
  public:

    enum frequency_en {
        K100 = TWIM_FREQUENCY_FREQUENCY_K100,
        K250 = TWIM_FREQUENCY_FREQUENCY_K250,
        K400 = TWIM_FREQUENCY_FREQUENCY_K400,
    };
    
    enum twi_mode_en {
        READ,
        WRITE
    };
    
  private:
    
    twi_mode_en mode;
    bool final;
    
    void set_read_buf_ptr(volatile uint8_t * buf);
    void set_write_buf_ptr(const uint8_t * buf);   
    
    void set_read_buf_len(uint32_t len);
    void set_write_buf_len(uint32_t len);   
    
  public:    
  
    TWIM(
        NRF_TWIM_Type * _instance,
        NRF_GPIO_Type * _pio,
        uint32_t scl_pin,
        uint32_t sda_pin,
        frequency_en frequency = K250);
            
    ~TWIM();
    
    void set_address(uint32_t address) { instance->ADDRESS = address; }    
    void set_mode(twi_mode_en _mode) { mode = _mode; }
    
    void start(uint32_t address) { set_address(address); start(); }
    void stop (uint32_t address) { stop();  }
    
    void start(void);
    void stop (void);
    
    void set_final(void);
    
    void enable(void)  { instance->ENABLE |= TWIM_ENABLE_ENABLE_Enabled << TWIM_ENABLE_ENABLE_Pos; }
    void disable(void) { instance->ENABLE &= ~(TWIM_ENABLE_ENABLE_Enabled << TWIM_ENABLE_ENABLE_Pos); }
    
    using SerialIO<NRF_TWIM_Type>::read;
    void read(volatile uint8_t * data, uint32_t length);
    bool read_finished(void);
    
    using SerialIO<NRF_TWIM_Type>::write;
    void write(const uint8_t * data, uint32_t length);
    bool write_finished(void);
};