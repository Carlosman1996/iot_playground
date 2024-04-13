#ifndef _LDR_H
#define _LDR_H

#include <Arduino.h>

class LDR
{
  public:
    LDR( uint8_t pin, float rc );
    float read_ilumination( );

  private:
    const uint8_t _pin;
    const float _rc;

    static const float A;
    static const float B;
};

#endif