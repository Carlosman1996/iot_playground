#include "lcd.h"


#define LCD_INITIALIZATION 0


LCD::LCD(uint8_t rs_pin, uint8_t rw_pin, uint8_t e_pin, uint8_t* d7_0_pin)
	: _rs_pin(rs_pin), 
    _rw_pin(rw_pin), 
    _e_pin(e_pin),
    _d7_0_pin(d7_0_pin)
{
  // lcd_state = LCD_INITIALIZATION;

  pinMode(_rs_pin, OUTPUT);
  pinMode(_rw_pin, OUTPUT);
  pinMode(_e_pin, OUTPUT);
}


uint8_t LCD::_sendCommand(
  uint8_t rs_bit,
  uint8_t rw_bit,
  uint8_t d7_0_bit,
  bool wait_busy
)
{
  // Wait while LCD is busy:
  int timeout = 60;
  uint8_t data_bf = B00000000;
  while (wait_busy && timeout > 0) {
    data_bf = this->_sendCommand(0, 1, B10000000, false);
    wait_busy = (data_bf >> 7) & 1;
    timeout--;
  }

  for (int i = 0; i <= 7; i++) {
    pinMode(_d7_0_pin[7 - i], !rw_bit);
    if (rw_bit == 0) {
      digitalWrite(_d7_0_pin[7 - i], (d7_0_bit >> i) & 1);
    }
  }

  digitalWrite(_rs_pin, rs_bit);
  digitalWrite(_rw_pin, rw_bit);

  digitalWrite(_e_pin, 1);

  uint8_t data_read = B00000000;
  for (int i = 0; i <= 7; i++) {
    if (rw_bit == 1) {
      data_read = (data_read | digitalRead(_d7_0_pin[i])) << 1;
    }
  }

  digitalWrite(_e_pin, 0);

  return data_read;
}


void LCD::functionSet(bool data_bits, bool display_lines, bool style)
{
  uint8_t data_command = B00100000;
  data_command |= (data_bits << 4);
  data_command |= (display_lines << 3);
  data_command |= (style << 2);
  this->_sendCommand(0, 0, data_command, true);
}


void LCD::shift(bool shift_type, bool shift_direction)
{
  uint8_t data_command = B00010000;
  data_command |= (shift_type << 3);
  data_command |= (shift_direction << 2);
  this->_sendCommand(0, 0, data_command, true);
}


void LCD::displaySwitch(bool turn_on, bool cursor_on, bool blink_on)
{
  uint8_t data_command = B00001000;
  data_command |= (turn_on << 2);
  data_command |= (cursor_on << 1);
  data_command |= blink_on;
  this->_sendCommand(0, 0, data_command, true);
}


void LCD::inputSet(bool cursor_direction, bool shift)
{
  uint8_t data_command = B00000100;
  data_command |= (cursor_direction << 1);
  data_command |= shift;
  this->_sendCommand(0, 0, data_command, true);
}


void LCD::cursorReturn()
{
  this->_sendCommand(0, 0, B00000010, true);
}


void LCD::setCursor(uint8_t row, uint8_t column)
{
  uint8_t address = B10000000 | (row * 16 + column);
  this->_sendCommand(0, 0, address, true);
}


void LCD::screenClear()
{
  this->_sendCommand(0, 0, B00000001, true);
}


void LCD::writeCharacter(uint8_t d7_0_bit)
{
  this->_sendCommand(1, 0, d7_0_bit, true);
}


void LCD::writeString(char *str)
{
  while (*str != '\0') {
      this->writeCharacter(*str);
      str++;
  }
}


uint8_t LCD::sendCommand(uint8_t rs_bit, uint8_t rw_bit, uint8_t d7_0_bit)
{
  return this->_sendCommand(rs_bit, rw_bit, d7_0_bit, true);
}


void LCD::initialize(
  bool data_bits,
  bool display_lines,
  bool style,
  bool cursor_direction,
  bool shift
)
{
  // Init LCD:

  delayMicroseconds(15000);
  this->_sendCommand(0, 0, B00110000, false);
  delayMicroseconds(4100);
  this->_sendCommand(0, 0, B00110000, false);
  delayMicroseconds(100);
  this->_sendCommand(0, 0, B00110000, false);

  // Set bits interface:
  this->functionSet(data_bits, false, false);
  // Function set (display lines and character font):
  this->functionSet(data_bits, display_lines, style);
  // Display off:
  this->displaySwitch(false, false, false);
  // Display clear:
  this->screenClear();
  delayMicroseconds(1640);
  // Mode set:
  this->inputSet(cursor_direction, shift);
}

