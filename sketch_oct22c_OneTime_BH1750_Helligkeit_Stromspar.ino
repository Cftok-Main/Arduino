// Programm für die OneTime-BH1750 Lib von JVKran https://github.com/JVKran/OneTime-BH1750 
// Statt readLightLevel muss getLightIntensity verwendet werden
// Verbraucht weniger Strom und Speicher
// 4618 Byte Programmspeicher
// 282 Byte globale Variablen

#include <Wire.h>
#include <LiquidCrystal.h>
#include <BH1750.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // Pins anpassen, falls nötig
BH1750 lightMeter;

int rightPin = A0; // Pin für die RIGHT-Taste
int lastButtonState = 5; // Variable zum Lesen des letzten Tastenzustands
int currentBrightness = 0; // Variable für die aktuelle Helligkeit

// Neue Definitionen für Tasten
#define Tasterrechts 0
#define Tasteroben 1
#define Tasterunten 2
#define Tasterlinks 3
#define Tasterselect 4
#define KeinTaster 5

// Neuer Programmblock zur Überprüfung des Tasterstatus
int Tasterstatus()
{
  int Analogwert = analogRead(rightPin); // Auslesen der Taster am Analogen Pin A0.
  if (Analogwert > 1000) return KeinTaster;
  if (Analogwert < 50) return Tasterrechts;
  if (Analogwert < 195) return Tasteroben;
  if (Analogwert < 380) return Tasterunten;
  if (Analogwert < 555) return Tasterlinks;
  if (Analogwert < 790) return Tasterselect;
 
  return KeinTaster; // Ausgabe wenn kein Taster gedrückt wurde.
}

void setup() {
  lcd.begin(16, 2);                      // Initialisiere das LCD 
  lightMeter.begin();
}

void loop() {
  int buttonState = Tasterstatus(); // Lese den Zustand der Tasten

  if (buttonState == Tasterrechts) { 
    // Wenn die RIGHT-Taste gedrückt wird
    currentBrightness = lightMeter.getLightIntensity(); // Aktualisiere die aktuelle Helligkeit
    delay(200);  // Zeitabstand zwischen den Messungen, 200 wirkt fast flüssig, je niedriger umso mehr Strom durch häufigere Messungen
    lcd.setCursor(0,0);
    lcd.print("Licht: ");
    lcd.print(currentBrightness);
    lcd.print(" lux"); // Einheit "lx" für Lux
  }

  if (buttonState != Tasterrechts && lastButtonState == Tasterrechts) {
    // Wenn die RIGHT-Taste losgelassen wird und der vorherige Zustand RIGHT war
    lcd.clear(); // Lösche den Inhalt des Displays
    lcd.setCursor(0,0);
    lcd.print("Licht: ");
    lcd.print(currentBrightness); // Zeige den zuletzt gemessenen Helligkeitswert an
    lcd.print(" lux"); // Einheit "lx" für Lux
  }

  lastButtonState = buttonState; // Merke dir den aktuellen Zustand der Tasten
}