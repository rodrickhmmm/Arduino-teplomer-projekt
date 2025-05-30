#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include "DHT.h"

//Nastaveni
const int Tlacitko1 = 8; // pin praveho tlacitka
const int Tlacitko2 = 7; //pin levehoi tlacitka
#define DHTPIN 4 // pin senzoru
#define DHTTYPE DHT11 // typ senzoru
// Vyber JEDEN řádek podle svého displeje:

//LiquidCrystal_I2C lcd(0x27, 20, 4); // pro 20x4 displej
//LiquidCrystal_I2C lcd(0x20, 16, 2); // pro 16x2 displej 

// inicializace promenych
//tohohle si nevsimej, je to jenom aby ide nevyhazovalo chyby
int buttonState = HIGH;
bool Normalnijednotky = true;

// ZACATEK KODU, NEHRAB SE TU!
// init senzoru
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(Tlacitko1, INPUT_PULLUP); // nastaveni tlacitek
  pinMode(Tlacitko2, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Teplomer"); // splash screen
  delay(1000);
  lcd.clear();
  dht.begin(); //aktivace senzoru
}

void loop() {
  float teplota = dht.readTemperature(); //cteni teploty
  if (isnan(teplota)) {
    lcd.print("Chyba senzoru"); // chyba kdyz se posere HARDWARE senzoru
  } else {
    //rederovani teploty
    lcd.setCursor(0, 0);
    lcd.print("Teplota:");
    lcd.setCursor(8, 0);
    if (Normalnijednotky) { // normalni jednotky - stupne Celsia, nenormalni jednotky pro ameicany - frenheit
      lcd.print(teplota);
      lcd.print("C");
    } else {
      lcd.print(teplota * 9 / 5 + 32); //nejakej vzorecek z googlu na prevod jednotek
      lcd.print("F");
      }
    // konec renderovani teploty
  if (digitalRead(Tlacitko1) == LOW) { //ty tlacitka jsou zapojeny jako pull-up, takze LOW znamena stisknuti
    Normalnijednotky = !Normalnijednotky; //negace hodnoty normalni jednotky - prepne mezi mezi C a F
    lcd.clear();
    delay(500); // delay aby se to tlacitko neregistrovalo vickrat
    }
  }
}