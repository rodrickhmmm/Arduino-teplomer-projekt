# Arduino teploměr

Tento projekt se zaměřuje na jednoduchý teploměr postavený na Arduinu. Změří teplotu okolí a zobrazí ji na displeji. Dá se přepínat mezi stupni Celsia a Fahrenheitu.

---

## 1. Jak zařízení zapojit

1. **Na sestrojení budete potřebovat:**
   - Arduino Uno
   - Nepájivé pole (breadboard)
   - Senzor DHT11 na měření teploty
   - LCD 1602 Displej (nebo 20x4, funguje na obojím)
   - 9 Samec/Samec propojovacích vodičů 
   - 4 Samec/Samice propojovací vodiče
   - Nepájivé pole (breadboard)

2. **Jak zařízení zapojit:**
   - Vše připojte podle screenshotu z Tinkercadu:
   <img src="schema/screenshot.png" width="588" height="340">

   - Nebo podle schéma obvodu:
   <img src="schema/schema.png" width="511" height="396">


   - Vodič, který vede z portu SDA na displeji zapojte do A4 portu na arduinu
   - Vodič, který vede z portu SCL na displeji zapojte do A5 portu na arduinu
   - Senzor DHT11 zapojíme zadní stranou (tou, kde není ten světle modrý senzor), aby nesnímal teplotu u kabelů, ale okolí. To je proč jsou vodiče zapojené na DHT11 na breadboardu v opačném směru.

---

## 2. Co lze v kódu upravit/změnit bez hrabání se ve stežějních částech kódu
   kód (nebo nastavení zařízení) mužete změnit mezi řádkem 14 až 23.

   - Přenastavení pinu tlačítek (řádek 15 a 16):
   ```c++
      const int TLACITKO1 = 8; // pin praveho tlacitka
      const int TLACITKO2 = 7; //pin leveho tlacitka
   ```
   číslo 8 a 7 mužete změnit za jiné, na kterém máte tlačítka připojena do arduina.

   - Inicializace displeje (řádek 20 až 22):
   ```c++
      // Vyber JEDEN řádek podle svého displeje:
      //LiquidCrystal_I2C lcd(0x27, 20, 4); // pro 20x4 displej
      //LiquidCrystal_I2C lcd(0x27, 16, 2);
   ```
   Odstraňte na začátku jednoho z řádku dvě lomítka, podle toho, jestli používáte displej s rozlišením 20x4 nebo 16x2.

---

## 3. Jak zařízení ovládat
- Arduino připojte k počítači a nahrajte do něj kód.
- Po spuštění se zapne automaticky displej, kde po zmáčknutí pravého tlačítka (pokud je vše zapojeno správně) by se měla ukázat teplota.
- Pomocí levého tlačítka se přepíná mezi stupnicí Celsia a Fahrenheitu. To, na jaké stupnici je zařízení nastaveno, se ukáže šipka buďto vedle C nebo F.
