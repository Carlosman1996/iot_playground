#include "ldr.h"


const float LDR::A = 1000;
const float LDR::B = 15;


LDR::LDR( uint8_t pin, float rc )
	: _pin( pin ),
    _rc( rc )
{ 
}


float LDR::read_ilumination( )
{
  float raw = analogRead(_pin);
  float ilum = ((1024 - raw) * A * 10) / (B * (_rc / 1000) * raw);
  
  return( ilum );
}
