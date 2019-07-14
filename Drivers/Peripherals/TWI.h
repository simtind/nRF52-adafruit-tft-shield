#pragma once
#include <nrf.h>
#include "SerialIO.h"

class TWI : public SerialIO<NRF_TWI_Type>
{ 
  public:

    enum frequency_en {
        K100 = TWI_FREQUENCY_FREQUENCY_K100,
        K250 = TWI_FREQUENCY_FREQUENCY_K250,
        K400 = TWI_FREQUENCY_FREQUENCY_K400,
    };
    
    enum twi_mode_en {
        READ,
        WRITE
    };
    
  private:
    
    twi_mode_en mode;
    
    volatile uint8_t * rx_buf;
    volatile uint8_t * tx_buf;
    uint32_t rx_buf_len;
    uint32_t tx_buf_len;
    
    uint32_t iterator;
    bool busy;
    
    void set_read_buf_ptr(volatile uint8_t * buf);
    void set_write_buf_ptr(volatile uint8_t * buf);   
    
    void set_read_buf_len(uint32_t len);
    void set_write_buf_len(uint32_t len);   
    
    void rx_interrupt(volatile uint32_t * event);
    void tx_interrupt(volatile uint32_t * event);
    void stop_interrupt(volatile uint32_t * event);
    
    friend void twi_tx_interrupt_handler(void * instance, volatile uint32_t * event);
    friend void twi_rx_interrupt_handler(void * instance, volatile uint32_t * event);
    friend void twi_stop_interrupt_handler(void * instance, volatile uint32_t * event);
    
  public:    
  
    TWI(
        NRF_TWI_Type * _instance,
        NRF_GPIO_Type * _pio,
        uint32_t scl_pin,
        uint32_t sda_pin,
        frequency_en frequency = K250);
            
    ~TWI();
    
    void set_address(uint32_t address) { instance->ADDRESS = address; }    
    void set_mode(twi_mode_en _mode) { mode = _mode; }
    
    void start(uint32_t address) { set_address(address); start(); }
    void stop (uint32_t address) { stop();  }
    
    void start(void);
    void stop (void);
    
    void set_final(void) {}
    
    void enable(void)  { instance->ENABLE |= TWI_ENABLE_ENABLE_Enabled << TWI_ENABLE_ENABLE_Pos; }
    void disable(void) { instance->ENABLE &= ~(TWI_ENABLE_ENABLE_Enabled << TWI_ENABLE_ENABLE_Pos); }
    
    using SerialIO<NRF_TWI_Type>::read;
    void read(volatile uint8_t * data, uint32_t length);
    bool read_finished(void);
    
    using SerialIO<NRF_TWI_Type>::write;
    void write(volatile uint8_t * data, uint32_t length);
    bool write_finished(void);
};