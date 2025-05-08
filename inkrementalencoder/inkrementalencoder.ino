#define encoderA 2  // Signal A (Interrupt)
#define encoderB 3  // Signal B
volatile int position = 0;  // Position des Encoders

void setup() {
    Serial.begin(9600);
    pinMode(encoderA, INPUT);
    pinMode(encoderB, INPUT);
    attachInterrupt(digitalPinToInterrupt(encoderA), updateEncoder, CHANGE); // Interrupt für Signal A
}

void loop() {
    Serial.print("Position: ");
    Serial.println(position);  // Ausgabe der Position
    delay(500);
}

void updateEncoder() {
    if (digitalRead(encoderA) == digitalRead(encoderB)) {
        position++;  // Vorwärtsbewegung
    } else {
        position--;  // Rückwärtsbewegung
    }
}

float aufloesung = 10.0;  // Beispiel: 10 Pulse pro mm
float position_mm = position / aufloesung;  // Position in mm berechnen
Serial.print("Position in mm: ");
Serial.println(position_mm);
