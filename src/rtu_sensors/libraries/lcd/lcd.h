#ifndef _LCD_H
#define _LCD_H

#include <Arduino.h>


class LCD
{
  public:
    LCD(uint8_t rs_pin, uint8_t rw_pin, uint8_t e_pin, uint8_t* d7_0_pin);
    uint8_t send_command(uint8_t rs_bit, uint8_t rw_bit, uint8_t d7_0_bit);
    void initialize(bool data_bits, bool display_lines, bool style, bool cursor_direction, bool shift);
    void write_character(uint8_t d7_0_bit);
    void screen_clear();
    void cursor_return();
    void input_set(bool cursor_direction, bool shift);
    void display_switch(bool turn_on, bool cursor_on, bool blink_on);
    void shift(bool shift_type, bool shift_direction);
    void function_set(bool data_bits, bool display_lines, bool style);
  private:
    uint8_t _send_command(uint8_t rs_bit, uint8_t rw_bit, uint8_t d7_0_bit, bool wait_busy);
    const uint8_t _rs_pin, _rw_pin, _e_pin;
    const uint8_t* _d7_0_pin;
};

#endif
