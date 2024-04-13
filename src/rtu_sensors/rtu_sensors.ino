#include <math.h>

#include <ntc.h>
#include <ldr.h>



// Temperature sensor (NTC):
static const int INTERVAL_TEMPERATURE_MS = 2500;
static const int NTC_PIN = A0;
static const float NTC_RC = 10000;
static const float NTC_VCC = 5;

unsigned long millis_temperature = INTERVAL_TEMPERATURE_MS;

NTC ntc_sensor( NTC_PIN, NTC_RC, NTC_VCC );

void readTemperature() {
  float celsius = ntc_sensor.read_temperature();

  Serial.print("Temperature = ");
  Serial.print(celsius, 2);
  Serial.println(" ºC");
}

// Ilumination sensor (LDR):
static const int INTERVAL_ILUMINATION_MS = 20000;
static const int LDR_PIN = A1;
static const float LDR_RC = 10000;

unsigned long millis_ilumination = INTERVAL_ILUMINATION_MS;

LDR ldr_sensor( LDR_PIN, LDR_RC );

void readIlumination() {
  float ilumination = ldr_sensor.read_ilumination();
  
  Serial.print("Ilumination = ");
  Serial.println(ilumination);  
}

// Control LCD:

static const int LCD_E_PIN = 3;
static const int LCD_D0_PIN = 4;
static const int LCD_D1_PIN = 5;
static const int LCD_D2_PIN = 6;
static const int LCD_D3_PIN = 7;
static const int LCD_D4_PIN = 8;
static const int LCD_D5_PIN = 9;
static const int LCD_D6_PIN = 10;
static const int LCD_D7_PIN = 11;
static const int LCD_RS_PIN = 12;

const DDRD = B11111111;

#define LCD_INITIALIZATION 0
#define LCD_INIT_SET_1 1
#define LCD_INIT_SET_2 2
#define LCD_INIT_SET_3 3
#define LCD_INIT_CONF_1 4
#define LCD_INIT_CONF_2 5
#define LCD_INIT_CONF_3 6
#define LCD_INIT_CONF_4 7
#define LCD_INIT_CONF_5 8
#define LCD_INIT_CONF_6 9
#define LCD_INIT_CONF_7 10
#define LCD_INIT_CONF_8 11
#define LCD_INIT_CONF_9 12
#define LCD_WRITE 13
#define LCD_IDLE 13

int lcd_state = LCD_INITIALIZATION;

unsigned long millis_lcd = 0;

void writeLcd() {
  switch (lcd_state) {

    case LCD_INITIALIZATION:
      if (millis() - millis_lcd > 15000) {
        millis_lcd = millis();
        lcd_state = LCD_INIT_SET_1;
      }
      break;

    case LCD_INIT_SET_1:
      if (millis() - millis_lcd > 4100) {
        millis_lcd = millis();
        lcd_state = LCD_INIT_SET_2;
      }
      break;

    case LCD_INIT_SET_2:
      if (millis() - millis_lcd > 100) {
        millis_lcd = millis();
        lcd_state = LCD_INIT_SET_3;
      }
      break;

    case LCD_INIT_SET_2:
      if (millis() - millis_lcd > 100) {
        millis_lcd = millis();
        lcd_state = LCD_INIT_SET_3;
      }
      break;

    default:
      break;
  }
}


// General:

void setup()
{
  Serial.begin(9600);
}

void loop() 
{
  // Temperature stage:
  if (millis() - millis_temperature >= INTERVAL_TEMPERATURE_MS) {
    millis_temperature = millis();
    readTemperature();
  }

  // Ilumination stage:
  if (millis() - millis_ilumination >= INTERVAL_ILUMINATION_MS) {
    millis_ilumination = millis();
    readIlumination();
  }
}