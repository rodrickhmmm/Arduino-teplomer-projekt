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
LiquidCrystal_I2C lcd(0x27, 16, 2); // pro 16x2 displej 

// inicializace promenych
//tohohle si nevsimej, je to jenom aby ide nevyhazovalo chyby
int buttonState = HIGH;
bool Normalnijednotky = true;

// ZACATEK KODU, NEHRAB SE TU!
// init senzoru
DHT dht(DHTPIN, DHTTYPE);

// vlastní symbol pro znak stupně (ano, taky se divim že tam není xd)
byte stupenChar[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

// vlastní symbol pro svislou čáru
byte caraChar[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00000
};

// vlastní symbol pro šipku
byte sipkaChar[8]{
  0b00000,
  0b00100,
  0b01100,
  0b11111,
  0b11111,
  0b01100,
  0b00100,
  0b00000
};

void setup() {
  pinMode(Tlacitko1, INPUT_PULLUP); // nastaveni tlacitek
  pinMode(Tlacitko2, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Teplomer"); // splash screen
  delay(1000);
  lcd.createChar(0, stupenChar);
  lcd.createChar(1, caraChar);
  lcd.createChar(2, sipkaChar);
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
    lcd.setCursor(5, 1);
    lcd.write(byte(0));
    lcd.setCursor(12, 0);
    lcd.write(byte(1));
    lcd.setCursor(14, 0);
    lcd.print("C");
    lcd.setCursor(12, 1);
    lcd.write(byte(1));
    lcd.setCursor(14, 1);
    lcd.print("F");
    lcd.setCursor(0, 1);
    if (Normalnijednotky) { // normalni jednotky - stupne Celsia, nenormalni jednotky pro ameicany - frenheit
      lcd.print(teplota);
      lcd.setCursor(15, 0);
    lcd.write(byte(2));
    } else {
      lcd.print(teplota * 9 / 5 + 32); //nejakej vzorecek z googlu na prevod jednotek
      lcd.setCursor(15, 1);
    lcd.write(byte(2));
      }
    // konec renderovani teploty
  if (digitalRead(Tlacitko1) == LOW) { //ty tlacitka jsou zapojeny jako pull-up, takze LOW znamena stisknuti
    Normalnijednotky = !Normalnijednotky; //negace hodnoty normalni jednotky - prepne mezi mezi C a F
    lcd.clear();
    delay(500); // delay aby se to tlacitko neregistrovalo vickrat
    }
  }
}