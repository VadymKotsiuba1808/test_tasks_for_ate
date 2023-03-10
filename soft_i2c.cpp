#include "soft_i2c.h"
#include <util/delay.h>

i2c_pin_config i2c_pins;

void i2c_init(uint8_t scl, uint8_t sda, uint8_t *ddr, uint8_t *port, uint8_t *pin)
{
  i2c_pins.scl = scl;
  i2c_pins.sda = sda;
  i2c_pins.ddr = ddr;
  i2c_pins.port = port;
  i2c_pins.pin = pin;

  *i2c_pins.ddr |= (1 << i2c_pins.scl) | (1 << i2c_pins.sda);
   _set_pin(i2c_pins.scl, 1);
   _set_pin(i2c_pins.sda, 1);
}

void i2c_begin(void)
{
  _delay_us(10);
  _set_pin(i2c_pins.sda, 0);
  _delay_us(10);
  _set_pin(i2c_pins.scl, 0);
}

void i2c_end(void)
{
  _set_pin(i2c_pins.sda, 0);
  _delay_us(10);
  _set_pin(i2c_pins.scl, 1);
  _delay_us(10);
  _set_pin(i2c_pins.sda, 1);
  _delay_us(10);
}

uint8_t i2c_write(uint8_t data)
{
  uint8_t ack;
  for (int8_t i = 7; i >= 0; i--) {
    if (data & (1<<i)) {
      _set_pin(i2c_pins.sda, 1);
    } else {
      _set_pin(i2c_pins.sda, 0);
    }
    _delay_us(10);
    _set_pin(i2c_pins.scl, 1);
    _delay_us(10);
    _set_pin(i2c_pins.scl, 0);
  }
  *i2c_pins.ddr &= ~(1 << i2c_pins.sda);
  _delay_us(10);
  _set_pin(i2c_pins.scl, 1);
  _delay_us(10);
  ack = _read_pin(i2c_pins.sda);
  *i2c_pins.ddr |= (1 << i2c_pins.sda);
  _set_pin(i2c_pins.scl, 0);
  return ack;
}

uint8_t i2c_read(uint8_t ack)
{
  uint8_t data = 0;
  *i2c_pins.ddr &= ~(1 << i2c_pins.sda);

  for (int8_t i = 7; i >= 0; i--) {
    _set_pin(i2c_pins.scl, 1);
    _delay_us(10);
    if(_read_pin(i2c_pins.sda)) data |= (1<<i);
    _set_pin(i2c_pins.scl, 0);
    _delay_us(10);
  }
  *i2c_pins.ddr |= (1 << i2c_pins.sda);

  if (ack)
  {
    _set_pin(i2c_pins.sda, 0);
  }
  else
  {
    _set_pin(i2c_pins.sda, 1);
  }

  _set_pin(i2c_pins.scl, 1);
  _delay_us(10);
  _set_pin(i2c_pins.scl, 0);
  _delay_us(10);
  _set_pin(i2c_pins.sda, 1);
  
  return data;
}

static void _set_pin(uint8_t pin, uint8_t value)
{
  if(!value)
  {
    *i2c_pins.port &= ~(1<<pin);
  }
  else
  {
    *i2c_pins.port |= (1<<pin);
  }
}

static uint8_t _read_pin(uint8_t pin)
{
  return (*i2c_pins.pin & (1<<pin));
}
