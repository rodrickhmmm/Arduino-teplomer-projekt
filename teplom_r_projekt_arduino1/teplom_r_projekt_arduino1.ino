#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include "DHT11.h"

LiquidCrystal_I2C lcd(0x20, 16, 2);

// CALLBACKY PRO ARDUINO - uprav si to pro jiny mikrokontrolery
void myPinMode(int pin, int mode) { pinMode(pin, mode); }
void myDigitalWrite(int pin, int value) { digitalWrite(pin, value); }
int myDigitalRead(int pin) { return digitalRead(pin); }
void myDelay(unsigned long ms) { delay(ms); }

DHT11 dht11(4, myPinMode, myDigitalWrite, myDigitalRead, myDelay);

const int Tlacitko1 = 8;
const int Tlacitko2 = 7;
int buttonState = HIGH;
bool Normalnijednotky = true;

void setup() {
  pinMode(Tlacitko1, INPUT_PULLUP);
  pinMode(Tlacitko2, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Teplomer");
  delay(1000);
  lcd.clear();
}

void loop() {
  int teplota = dht11.readTemperature();

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 1; j++) {
      lcd.setCursor(0, 0);
      lcd.print("Teplota:         ");
      lcd.setCursor(9, 0);
      if (Normalnijednotky) {
        lcd.print(teplota);
        lcd.print(" C ");
      } else {
        lcd.print(teplota * 9 / 5 + 32);
        lcd.print(" F ");
      }
      delay(500);
    }
  }

  if (digitalRead(Tlacitko1) == LOW) {
    Normalnijednotky = !Normalnijednotky;
    lcd.clear();
    delay(300);
  }
}
