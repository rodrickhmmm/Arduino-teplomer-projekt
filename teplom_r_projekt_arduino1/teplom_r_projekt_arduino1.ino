#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include "DHT.h"

const int Tlacitko1 = 8;
const int Tlacitko2 = 7;
int buttonState = HIGH;
bool Normalnijednotky = true;

// Vyber JEDEN řádek podle svého displeje:
//LiquidCrystal_I2C lcd(0x27, 20, 4); // pro 20x4 displej
LiquidCrystal_I2C lcd(0x20, 16, 2); // pro 16x2 displej 

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(Tlacitko1, INPUT_PULLUP);
  pinMode(Tlacitko2, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Teplomer");
  delay(1000);
  lcd.clear();
  dht.begin();
}

void loop() {
  float teplota = dht.readTemperature();
  float vlhkost = dht.readHumidity();

  if (isnan(teplota) || isnan(vlhkost)) {
    lcd.print("Chyba senzoru");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Teplota:");
    lcd.setCursor(8, 0);
    if (Normalnijednotky) {
      lcd.print(teplota);
      lcd.print("C");
    } else {
      lcd.print(teplota * 9 / 5 + 32);
      lcd.print("F");
    }
    lcd.setCursor(0, 1);
    lcd.print("Vlhkost:");
    lcd.setCursor(8, 1);
    lcd.print(vlhkost);
    lcd.print("%");
  }

  if (digitalRead(Tlacitko1) == LOW) {
    Normalnijednotky = !Normalnijednotky;
    lcd.clear();
    delay(500);
  }
}
