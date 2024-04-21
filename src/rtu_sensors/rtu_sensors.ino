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

static const uint8_t LCD_D7_0_PIN[8] = {2, 3, 4, 5, 6, 7, 8, 9};
static const uint8_t LCD_E_PIN = 10;
static const uint8_t LCD_RW_PIN = 12;
static const uint8_t LCD_RS_PIN = 11;

// int lcd_state = LCD_INITIALIZATION;

unsigned long millis_lcd = 0;

uint8_t sendCommand(uint8_t bit_rs, uint8_t bit_rw, uint8_t dataWrite, bool check_bf) {
  int timeout = 60;
  uint8_t data_bf = B00000000;
  while (check_bf && timeout > 0) {
    Serial.println("Boolean BF: ");
    data_bf = sendCommand(0, 1, B10000000, false);
    Serial.print(data_bf, BIN);
    check_bf = (data_bf >> 7) & 1;
    Serial.print(check_bf, DEC);
    timeout--;
  }

  for (int i = 0; i <= 7; i++) {
    pinMode(LCD_D7_0_PIN[7 - i], !bit_rw);
    if (bit_rw == 0) {
      digitalWrite(LCD_D7_0_PIN[7 - i], (dataWrite >> i) & 1);
    }
  }

  digitalWrite(LCD_RS_PIN, bit_rs);
  digitalWrite(LCD_RW_PIN, bit_rw);

  digitalWrite(LCD_E_PIN, 1);

  uint8_t dataRead = B00000000;
  for (int i = 0; i <= 7; i++) {
    if (bit_rw == 1) {
      dataRead = (dataRead | digitalRead(LCD_D7_0_PIN[i])) << 1;
    }
  }

  delayMicroseconds(1);
  digitalWrite(LCD_E_PIN, 0);

  delayMicroseconds(40);
  return dataRead;
}

void writeLcd() {
  Serial.println("Init LCD");

  pinMode(LCD_E_PIN, OUTPUT);
  pinMode(LCD_RS_PIN, OUTPUT);
  pinMode(LCD_RW_PIN, OUTPUT);

  Serial.println("Start init secuence");
  delayMicroseconds(15000);
  sendCommand(0, 0, B00110000, false);
  delayMicroseconds(4100);
  sendCommand(0, 0, B00110000, false);
  delayMicroseconds(100);
  sendCommand(0, 0, B00110000, false);

  Serial.println("Set 8 bit interface");
  sendCommand(0, 0, B00110000, true);
  Serial.println("Function set (display lines and character font)");
  sendCommand(0, 0, B00100000, true);
  Serial.println("Display off");
  sendCommand(0, 0, B00001000, true);
  Serial.println("Display clear");
  sendCommand(0, 0, B00000001, true);
  delayMicroseconds(1640);
  Serial.println("Mode set:");
  sendCommand(0, 0, B00000111, true);
  
  Serial.println("LCD initialized");

  // Display on:
  delay(100);
  sendCommand(0, 0, B00001111, true);

  delay(100);
  sendCommand(1, 0, B0011000, true);

  //switch (lcd_state) {

  //  case LCD_INITIALIZATION:
  //    if (millis() - millis_lcd > 15000) {
  //      millis_lcd = millis();
  //      lcd_state = LCD_INIT_SET_1;
  //    }
  //    break;/

  //  case LCD_INIT_SET_1:
  //    if (millis() - millis_lcd > 4100) {
  //      millis_lcd = millis();
  //      lcd_state = LCD_INIT_SET_2;
  //    }
  //    break;/

  //  case LCD_INIT_SET_2:
  //    if (millis() - millis_lcd > 100) {
  //      millis_lcd = millis();
  //      lcd_state = LCD_INIT_SET_3;
  //    }
  //    break;/

  //  case LCD_INIT_SET_2:
  //    if (millis() - millis_lcd > 100) {
  //      millis_lcd = millis();
  //      lcd_state = LCD_INIT_SET_3;
  //    }
  //    break;/

  //  default:
  //    break;
  //}
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