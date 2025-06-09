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

2. **Postup zapojení:**
   - Vše připojte podle screenshotu z Tinkercadu:
   <img src="schema/screenshot.png" width="588" height="340">

   - Nebo podle schéma obvodu:
   <img src="schema/schema.png" width="511" height="396">


   - Vodič, který vede z portu SDA na displeji zapojte do A4 portu na arduinu
   - Vodič, který vede z portu SCL na displeji zapojte do A5 portu na arduinu
   - Senzor DHT11 zapojíme zadní stranou (tou, kde není ten světle modrý senzor), aby nesnímal teplotu u kabelů, ale okolí. To je proč jsou vodiče zapojené na DHT11 na breadboardu v opačném směru.
   - 

