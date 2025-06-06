#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include "DHT.h"

//Nastaveni
const int Tlacitko1 = 8; // pin praveho tlacitka
const int Tlacitko2 = 7; //pin leveho tlacitka
#define DHTPIN 4 // pin senzoru
#define DHTTYPE DHT11 // typ senzoru

// Vyber JEDEN řádek podle svého displeje:
//LiquidCrystal_I2C lcd(0x27, 20, 4); // pro 20x4 displej
LiquidCrystal_I2C lcd(0x27, 16, 2); // pro 16x2 displej, pokud displej nefunguje tak můžete předělat adresu na 0x20 nebo 0x3F

// inicializace promenych
// Tohle je zde jenom aby IDE nevyhazovalo chyby
int buttonState = HIGH;
bool Normalnijednotky = true;

// ZACATEK KODU, NEHRABAT SE TU!
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
  0b00100
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
  pinMode(Tlacitko2, INPUT_PULLUP); // nastaveni tlacitek
  lcd.init(); //nastaveni displeje
  lcd.backlight();
  lcd.setCursor(0, 0); // nastavi aby se text/symboly/proste cokoliv zaclo psat na souradnici 0x0
  lcd.print("Teplomer"); // splash screen
  delay(1000);
  lcd.setCursor(0, 1); 
  lcd.print("Vytvoril:TomasK."); // splash screen
  delay(2000);
  lcd.createChar(0, stupenChar); //vytvori symbol pro znak stupně s číslem 0
  lcd.createChar(1, caraChar); //vytvori symbol pro svislou čáru s číslem 1
  lcd.createChar(2, sipkaChar); //vytvori symbol pro znak šipky s číslem 2
  lcd.clear(); // vse na obrazovce se vymaze
  dht.begin(); //aktivace senzoru
}

float posledniTeplota = 0; // Globální proměnná pro poslední teplotu
bool teplotaNactena = false; // Zda už byla teplota někdy načtena
bool potrebaPrekreslit = true; // Překreslit displej?

void loop() {
  float teplota = dht.readTemperature(); // inicializuje promennou na cteni teploty ze senzoru

  if (isnan(teplota)) {
    // VŽDY zobraz error, když je chyba senzoru
    lcd.clear();
    lcd.print("Error 621/gurt");//maly easter egg pro ty co to zapojí špatně xdd
    lcd.setCursor(0,1);
    lcd.print("Chyba senzoru!"); // chyba kdyz neni senzor nalezen nebo kdyz je jeho hardware poškozen, stručně řečeno pokud arduino nedostane danou teplotu od senzoru, tak to napíše tuto chybu
    teplotaNactena = false;
    delay(1000);
    return;
  }
  else{
    // Pokud je stisknuto Tlacitko2, ulož novou teplotu
  if (digitalRead(Tlacitko2) == LOW) {
    posledniTeplota = teplota;
    teplotaNactena = true;
    potrebaPrekreslit = true;
    delay(500); // Debounce tlačítka
  }

  // Přepínání jednotek zůstává na Tlacitko1
  if (digitalRead(Tlacitko1) == LOW) {
    Normalnijednotky = !Normalnijednotky;
    potrebaPrekreslit = true;
    delay(500);
  }

  // Překresli displej jen pokud je potřeba
  if (potrebaPrekreslit) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Teplota:");
    lcd.setCursor(5, 1);
    lcd.write(byte(0));
    
    lcd.setCursor(13, 0);
    lcd.write(byte(1));
    lcd.print("C");
    lcd.setCursor(13, 1);
    lcd.write(byte(1));
    lcd.print("F");
    lcd.setCursor(0, 1);

    if (teplotaNactena) {
      if (Normalnijednotky) {
        lcd.print(posledniTeplota);
        lcd.setCursor(15, 0);
        lcd.write(byte(2));
      } else {
        lcd.print(posledniTeplota * 9 / 5 + 32);
        lcd.setCursor(15, 1);
        lcd.write(byte(2));
      }
    } else {
      lcd.print("----");
    }
    potrebaPrekreslit = false;
  }

  }

  delay(50); // Krátká prodleva pro lepší odezvu
}