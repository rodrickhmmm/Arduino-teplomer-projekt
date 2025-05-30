#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);  // nejakej brdel, sirka, vyska

const int Tlacitko1 = 8; //pin d8
const int Tlacitko2 = 7; //pin d7

int buttonState = LOW;

void setup() {
  // nastaveni nejakych chujovin bez kterych to nejede 
  pinMode(Tlacitko1, INPUT_PULLUP);
  pinMode(Tlacitko2, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Teplomer");
}

void loop() {
  buttonState = digitalRead(Tlacitko1);
  
  if (digitalRead(Tlacitko1) == LOW) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Teplomer! ");
  }
  
  else if (digitalRead(Tlacitko2) == LOW) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Teplomer! 2");
  } 

}
