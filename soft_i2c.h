#ifndef SOFT_I2C_H
#define SOFT_I2C_H

#include <stdint.h>

typedef struct{
  uint8_t scl;
  uint8_t sda;
  uint8_t *ddr;
  uint8_t *port;
  uint8_t *pin;
} i2c_pin_config;

//Function prototypes
void i2c_init(uint8_t scl, uint8_t sda, uint8_t *ddr, uint8_t *port, uint8_t *pin);
void i2c_begin(void);
void i2c_end(void);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read(uint8_t ack);

static void _set_pin(uint8_t pin, uint8_t value);
static uint8_t _read_pin(uint8_t pin);

#endif