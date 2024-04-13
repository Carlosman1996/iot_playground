#ifndef _NTC_H
#define _NTC_H

#include <Arduino.h>

class NTC
{
  public:
    NTC( uint8_t pin, float rc, float vcc );
    float read_temperature( );

  private:
    const uint8_t _pin;
    const float _rc;
    const float _vcc;

    static const float A;
    static const float B;
    static const float C;
    static const float K;
};

#endif