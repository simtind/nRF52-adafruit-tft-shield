#pragma once
//#include <functional>
#include <nrf.h>

/*
class SPIM
{
  private:
    NRF_SPIM_Type * instance;
    NRF_GPIO_Type * pio;
    //std::function<SPIM &> read_callback;
    //std::function<SPIM &> write_callback;
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
  
    SPIM(
        NRF_SPIM_Type * _instance,
        NRF_GPIO_Type * _pio,
        uint32_t sck_pin,
        uint32_t mosi_pin,
        uint32_t miso_pin,
        frequency_en frequency = M2,
        clock_mode_en clk_mode = MODE0,
        bool msb_first = true);
            
    ~SPIM();
    
    void start(uint32_t cs_pin) { pio->OUTCLR = 1 << cs_pin; start(); }
    void stop (uint32_t cs_pin) { pio->OUTSET = 1 << cs_pin; stop();  }
    
    void start() { instance->TASKS_START = 1; }
    void stop () { instance->TASKS_STOP  = 1; }
    
    void read(uint8_t & data);
    void read(uint8_t * data, uint32_t length);
    bool read_finished(void);
    //bool when_read_finished(std::function<SPIM &> callback);
    
    void write(uint8_t & data);
    void write(uint8_t * data, uint32_t length);
    bool write_finished(void);
    //bool when_read_finished(std::function<SPIM &> callback);
};
*/

class SPI
{
  private:
    NRF_SPI_Type * instance;
    NRF_GPIO_Type * pio;
    //std::function<SPIM &> read_callback;
    //std::function<SPIM &> write_callback;
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
    
    void start() { instance->TASKS_START = 1; }
    void stop () { instance->TASKS_STOP  = 1; }
    
    void read(uint8_t & data);
    void read(uint8_t * data, uint32_t length);
    bool read_finished(void);
    //bool when_read_finished(std::function<SPIM &> callback);
    
    void write(uint8_t & data);
    void write(uint8_t * data, uint32_t length);
    bool write_finished(void);
    //bool when_read_finished(std::function<SPIM &> callback);
};