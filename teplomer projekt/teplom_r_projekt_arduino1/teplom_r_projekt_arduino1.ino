// ===================================
//
// Teploměr projekt pro Arduino
// Vypracoval: Tomáš K.
//
// ===================================

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

 // funkce na to defaultní UI
void defaultUI() {
    lcd.clear();
    lcd.setCursor(0, 0); // nastavi aby se text/symboly/proste cokoliv zaclo psat na souradnici 0x0
    lcd.print("Teplota:");  // napíše slovo Teplota
    lcd.setCursor(5, 1); // nastavi aby se text/symboly/proste cokoliv zaclo psat na souradnici 5x1
    lcd.write(byte(0)); // vypíše znak stupně. Pozor! Nelze použít lcd.print, jelikož daný symbol není definován jako ASCII symbol (písmeno, čísla, ...)Add commentMore actions
    
    // postranni menu (to kde se zobrazi C a F iyky)
    lcd.setCursor(13, 0); // nastavi aby se text/symboly/proste cokoliv zaclo psat na souradnici 13x0
    lcd.write(byte(1)); // vypíše znak svislé čáry. Pozor! Nelze použít lcd.print, jelikož daný symbol není definován jako ASCII symbol (písmeno, čísla, ...)
    lcd.print("C"); // vypíše vedle svislé čáry znak C
    lcd.setCursor(13, 1); // nastavi aby se text/symboly/proste cokoliv zaclo psat na souradnici 13x1
    lcd.write(byte(1)); // vypíše znak svislé čáry. Pozor! Nelze použít lcd.print, jelikož daný symbol není definován jako ASCII symbol (písmeno, čísla, ...)
    lcd.print("F"); // vypíše vedle svislé čáry znak F
    lcd.setCursor(0, 1); // nastavi aby se dalsi text (v tomto pripade jiz teplota) psala na souradnici 0x1
}

 // funkce na vypsání teploty
void prepsaniTeploty(float teplota ,int y){
    lcd.print(teplota); // zobrazí se poslední naměřená teplota
    lcd.setCursor(15, y); // nastavi aby se text/symboly/proste cokoliv zaclo psat na souradnici 15x dané y
    lcd.write(byte(2)); // vypíše znak šipky. Pozor! Nelze použít lcd.print, jelikož daný symbol není definován jako ASCII symbol (písmeno, čísla, ...)
}

void setup() {
  pinMode(Tlacitko1, INPUT_PULLUP); // nastaveni tlacitek, tlacitka jsou zapojena v pull-upu aby se nemusely zbytečně používat rezistory a vše šlo pekně přes 5V a GND
  pinMode(Tlacitko2, INPUT_PULLUP); // nastaveni tlacitek, tlacitka jsou zapojena v pull-upu aby se nemusely zbytečně používat rezistory a vše šlo pekně přes 5V a GND
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
bool teplotaNactena = false; // Proměnná na zkontrolování zda už někdy byla načetena teplota
bool potrebaPrekreslit = true; // Proměnná na to jestli se má překreslit displej nebo ne


void loop() {
  float teplota = dht.readTemperature(); // inicializuje promennou na cteni teploty ze senzoru

  if (isnan(teplota)) { // pokud teplota není žádné číslo, zobrazí se další error. The isnan() function tests whether x is NaN (not a number).
    // Zobrazí error, když je chyba senzoru
    lcd.clear();
    lcd.print("Error 621/gurt");//maly easter egg pro ty co to zapojí špatně xdd
    lcd.setCursor(0,1);
    lcd.print("Chyba senzoru!"); // chyba kdyz neni senzor nalezen nebo kdyz je jeho hardware poškozen, stručně řečeno pokud arduino nedostane danou teplotu od senzoru, tak to napíše tuto chybu
    teplotaNactena = false; // nenacte se teplota
    delay(1000); // to je zde, aby se potom dokázalo načíst to co je na normální obrazovce
    return;
  }
  else{
    // Pokud se stikne Tlacitko2, ulozi se nova teplota
  if (digitalRead(Tlacitko2) == LOW) { // tlacitka jsou zapojena jako pull-up, takze LOW znamena stisknuti
    posledniTeplota = teplota; // do proměnné posledniTeplota se načte teplota která se pošle ze senzoru
    teplotaNactena = true; // začne se načítat teplota
    potrebaPrekreslit = true;
    delay(500); // Debounce tlačítka
  }

  // Přepínání jednotek zůstává na Tlacitko1
  if (digitalRead(Tlacitko1) == LOW) { // tlacitka jsou zapojena jako pull-up, takze LOW znamena stisknuti
    Normalnijednotky = !Normalnijednotky; //negace hodnoty normalni jednotky - prepne mezi mezi C a F
    potrebaPrekreslit = true; // Prekresli se UI
    delay(500);
  }

  // Překreslí displej když je potřeba - když to zde není, tak se nic nezobrazí
  if (potrebaPrekreslit) {
      defaultUI(); 

    if (teplotaNactena) { 
      if (Normalnijednotky) { // normalni jednotky - stupne Celsia, nenormalni jednotky pro ameicany - frenheit
        prepsaniTeploty(posledniTeplota, 0);
      } else {
        prepsaniTeploty(posledniTeplota * 9 / 5 + 32, 1);
      }
    } else {
      lcd.print("----"); // pokud není naměřená žádná hodnota, ukáže se na obrazovce místo teploty toto
    }
    potrebaPrekreslit = false; // když tadyto tu není, tak se pořád bude v loopu opakovat vyvolávání UI. Proč? Protože je proměnná setlá na true.
  }

  }

  delay(50); // Krátká prodleva pro lepší odezvu
}