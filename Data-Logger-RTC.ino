#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;

const int pinNTC = A0;
const int pinPot1 = A1;
const int pinPot2 = A2;
const int pinPot3 = A3;
const int pinButton = 2;

const float BETA = 3950;
int displayMode = 0;

int eepromAddress = 0;

struct LogRecord {
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  float voltage1;
  float voltage2;
  float voltage3;
};

void setup() {
  Serial.begin(9600);
  pinMode(pinButton, INPUT_PULLUP);
  
  lcd.init();
  lcd.backlight();
  
  if (!rtc.begin()) {
    while (1);
  }
  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  lcd.clear();
}

void loop() {
  // Reading and calculating signals to voltage
  float v1 = (analogRead(pinPot1) / 1023.0) * 5.0;
  float v2 = (analogRead(pinPot2) / 1023.0) * 5.0;
  float v3 = (analogRead(pinPot3) / 1023.0) * 5.0;
  
  int rawNTC = analogRead(pinNTC);
  float tempC = 25.0;
  if (rawNTC > 0 && rawNTC < 1023) {
    tempC = 1.0 / (log(1.0 / (1023.0 / rawNTC - 1.0)) / BETA + 1.0 / 298.15) - 273.15;
  }

  // Transimissing Data to Serial Plotter 
  Serial.print(v1, 2);
  Serial.print(" ");
  Serial.print(v2, 2);
  Serial.print(" ");
  Serial.print(v3, 2);
  Serial.print(" ");
  Serial.println(tempC, 1);

  if (digitalRead(pinButton) == LOW) {
    delay(20);
    
    if (digitalRead(pinButton) == LOW) {
      displayMode = (displayMode + 1) % 5;
      
      lcd.clear();
      
      Serial.flush(); 
      
      saveMeasurementsToEEPROM(v1, v2, v3);

      while (digitalRead(pinButton) == LOW) {
        delay(10); 
      }
    }
  }

  updateDisplay(v1, v2, v3, tempC);
  
  delay(40);
}

void updateDisplay(float v1, float v2, float v3, float temp) {
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(temp, 1);
  lcd.print("\xDF""C        ");

  lcd.setCursor(0, 0);
  switch (displayMode) {
    case 0:
      lcd.print("A1: "); lcd.print(v1, 2); lcd.print("V        ");
      break;
    case 1:
      lcd.print("A2: "); lcd.print(v2, 2); lcd.print("V        ");
      break;
    case 2:
      lcd.print("A3: "); lcd.print(v3, 2); lcd.print("V        ");
      break;
    case 3:
      lcd.print("A1:"); lcd.print(v1, 1); lcd.print("V A2:"); lcd.print(v2, 1); lcd.print("V");
      break;
    case 4:
      lcd.print("1:"); lcd.print(v1, 1);
      lcd.print(" 2:"); lcd.print(v2, 1);
      lcd.print(" 3:"); lcd.print(v3, 1);
      break;
  }
}

void saveMeasurementsToEEPROM(float v1, float v2, float v3) {
  DateTime now = rtc.now();
  
  LogRecord currentLog = {
    now.hour(),
    now.minute(),
    now.second(),
    v1,
    v2,
    v3
  };
  
  EEPROM.put(eepromAddress, currentLog);
  eepromAddress += sizeof(LogRecord);
  
  if (eepromAddress >= (EEPROM.length() - sizeof(LogRecord))) {
    eepromAddress = 0; 
  }
}
