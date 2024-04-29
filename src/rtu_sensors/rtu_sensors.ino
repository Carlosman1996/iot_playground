#include <math.h>

#include <ntc.h>
#include <ldr.h>
#include <lcd.h>



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

static const uint8_t LCD_D7_0_PIN[8] = {9, 8, 7, 6, 5, 4, 3, 2};
static const uint8_t LCD_E_PIN = 10;
static const uint8_t LCD_RW_PIN = 12;
static const uint8_t LCD_RS_PIN = 11;

// int lcd_state = LCD_INITIALIZATION;

unsigned long millis_lcd = 0;

LCD lcd_display(LCD_RS_PIN, LCD_RW_PIN, LCD_E_PIN, LCD_D7_0_PIN);

void writeLcd() {
  lcd_display.initialize(true, true, false, true, false);

  // Display on:
  lcd_display.display_switch(true, false, false);

  // LUCIA
  lcd_display.write_character(B01001100);
  lcd_display.write_character(B01010101);
  lcd_display.write_character(B01000011);
  lcd_display.write_character(B01001001);
  lcd_display.write_character(B01000001);
}


// General:

void setup()
{
  Serial.begin(9600);
}

void loop() 
{
  // LCD stage:
  if (millis_lcd == 0) {
    millis_lcd = millis();
    writeLcd();
  }

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
