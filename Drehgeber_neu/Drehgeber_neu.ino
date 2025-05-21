#define encoderPinA 2 // Pin für Phase A
#define encoderPinB 3 // Pin für Phase B
volatile long encoderCount = 0;
// Anpassung des mm-Werts: Der Wert mmPerPulse muss basierend auf der Mechanik (z.B. Übersetzung, Radgröße) angepasst werden
float mmPerPulse = 0.1; // Beispielwert: 0.1 mm pro Impuls (Anpassen!)

void setup() 
{
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), countEncoder, CHANGE);
  Serial.begin(9600);

}

void loop() 
{
  float positionMM = encoderCount * mmPerPulse;
  Serial.print("Position: ");
  Serial.print(positionMM);
  Serial.println(" mm");
  delay(500);

}

// Erkennung der Bewegungsrichtung
void countEncoder()
{
  if (digitalRead(encoderPinA) == digitalRead(encoderPinB))
  {
    encoderCount++;
  }
  else
  {
    encoderCount--;
  }
}
