
#include "HX711.h"

// HX711 Pins
#define DT_PIN  3
#define SCK_PIN 2

HX711 scale;

float calibration_factor = 17893.53; // Raw pro kg – anpassen!
long tareOffset = 0;
float maxWeight = 0.0;

void setup() {
  Serial.begin(9600);
  scale.begin(DT_PIN, SCK_PIN);

  Serial.println("Wägezelle initialisieren...");
  delay(1000);

  if (scale.is_ready()) {
    Serial.println("Tarierung... Bitte nichts auflegen.");
    delay(2000);
    tareOffset = scale.read();
    Serial.print("Tarierung abgeschlossen. Offset = ");
    Serial.println(tareOffset);
  } else {
    Serial.println("HX711 nicht bereit. Bitte überprüfen.");
    while (1);
  }
}

void loop() {
  // Seriellen Befehl lesen (ganze Zeile)
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim(); // Leerzeichen und Zeilenumbrüche entfernen

    if (input.equalsIgnoreCase("t")) {
      Serial.print("Maximal gemessenes Gewicht: ");
      Serial.print(maxWeight, 3);
      Serial.println(" kg");

      Serial.println("Pause für 20 Sekunden...");
      delay(20000); // 20 Sekunden warten
    }
  }

  if (scale.is_ready()) {
    long rawValue = scale.read();
    long netValue = rawValue - tareOffset;
    float weight = (float)netValue / calibration_factor;
    weight = weight* -1;

    if (weight > maxWeight) {
      maxWeight = weight;
    }

    Serial.print("Gewicht: ");
    Serial.print(weight, 3);
    Serial.println(" kg");
  } else {
    Serial.println("HX711 nicht bereit.");
  }

  delay(500);
}
