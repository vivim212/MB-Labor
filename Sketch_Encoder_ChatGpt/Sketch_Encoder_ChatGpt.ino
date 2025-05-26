#define encoderPinA 2 // Phase A (Interrupt)
#define encoderPinB 3 // Phase B

volatile long encoderPos = 0;

// Anpassen: mm pro Schritt (z. B. 20 mm Umfang / 2400 Schritte = 0.008333 mm)
const float mmPerPulse = 0.008333;

void setup() {
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoderPinA), readEncoder, CHANGE);

  Serial.begin(9600);
  Serial.println("Encoder gestartet...");
}

void loop() {
  // Lokale Kopie für konsistente Ausgabe
  static long lastPos = 0;
  noInterrupts(); // Zugriff sichern
  long pos = encoderPos;
  interrupts();

  if (pos != lastPos) {
    float mm = pos * mmPerPulse;
    Serial.print("Position: ");
    Serial.print(mm, 3); // 3 Nachkommastellen
    Serial.print(" mm  |  Rohwert: ");
    Serial.println(pos);
    lastPos = pos;
  }

  delay(100); // Abfrageintervall
}

void readEncoder() {
  bool a = digitalRead(encoderPinA);
  bool b = digitalRead(encoderPinB);
  
  // XOR zur Richtungsbestimmung
  encoderPos += (a ^ b) ? 1 : -1;
}
