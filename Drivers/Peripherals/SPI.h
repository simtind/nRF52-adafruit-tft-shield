#pragma once
#include <nrf.h>
#include "SerialIO.h"

class SPI : public SerialIO<NRF_SPI_Type>
{
  private:
    volatile uint8_t * rx_buf;
    const uint8_t * tx_buf;
    uint32_t rx_buf_len;
    uint32_t tx_buf_len;
    
    uint32_t rx_iterator;
    uint32_t tx_iterator;
    bool busy;
    
    void set_read_buf_ptr(volatile uint8_t * buf);
    void set_write_buf_ptr(const uint8_t * buf);   
    
    void set_read_buf_len(uint32_t len);
    void set_write_buf_len(uint32_t len);       
    
    void interrupt(volatile uint32_t * event);
    
    friend void spi_interrupt_handler(void * instance, volatile uint32_t * event);
  public:

    enum frequency_en {
        K125 = 0x02000000,
        K250 = 0x04000000,
        K500 = 0x08000000,
        M1   = 0x10000000,
        M2   = 0x20000000,
        M4   = 0x40000000,
        M8   = 0x80000000,
    };
    
    enum clock_mode_en {
        MODE0,
        MODE1,
        MODE2,
        MODE3
    };
  
    SPI(
        NRF_SPI_Type * _instance,
        NRF_GPIO_Type * _pio,
        uint32_t sck_pin,
        uint32_t mosi_pin,
        uint32_t miso_pin,
        frequency_en frequency = M2,
        clock_mode_en clk_mode = MODE0,
        bool msb_first = true);
            
    ~SPI();
    
    void start(uint32_t cs_pin) { pio->OUTCLR = 1 << cs_pin; start(); }
    void stop (uint32_t cs_pin) { pio->OUTSET = 1 << cs_pin; stop();  }
    
    void start(void);
    void stop (void);
    
    void set_final(void) {}
    
    void read(volatile uint8_t * data, uint32_t length);
    bool read_finished(void);
    //bool when_read_finished(std::function<SPIM &> callback);
    
    void write(const uint8_t * data, uint32_t length);
    bool write_finished(void);
    //bool when_read_finished(std::function<SPIM &> callback);
};