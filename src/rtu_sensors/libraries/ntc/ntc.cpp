#include "ntc.h"


const float NTC::A = 1.11492089e-3;
const float NTC::B = 2.372075385e-4;
const float NTC::C = 6.954079529e-8;
const float NTC::K = 2.5;


NTC::NTC( uint8_t pin, float rc, float vcc )
	: _pin( pin ),
    _rc( rc ),
    _vcc ( vcc )
{
}


float NTC::read_temperature( )
{
  float raw = analogRead(_pin);
  float V =  raw / 1024 * _vcc;

  float R = (_rc * V ) / (_vcc - V);
  
  float logR  = log(R);
  float R_th = 1.0 / (A + B * logR + C * logR * logR * logR );

  float kelvin = R_th - V * V / (K * R) * 1000;
  float celsius = kelvin - 273.15;
  
  return( celsius );
}