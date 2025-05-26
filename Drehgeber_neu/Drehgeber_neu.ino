#define encoderPinA 2 // Pin für Phase A
#define encoderPinB 3 // Pin für Phase B
volatile long encoderPos = 0;
// Anpassung des mm-Werts: Der Wert mmPerPulse muss basierend auf der Mechanik (z.B. Übersetzung, Radgröße) angepasst werden
float mmPerPulse = 0.000104; //bei 2400 Umdrehungen

void setup() 
{
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), readEncoder, CHANGE);
  Serial.begin(9600);

}

void loop() 
{
  readEncoder();
  Serial.print("Position:");
  Serial.print(encoderPos * mmPerPulse);
  Serial.println(" mm");
  delay(500);


}


void readEncoder()
{
  bool signalA = digitalRead(encoderPinA);
  bool signalB = digitalRead(encoderPinB);
  bool xorResult = signalA ^ signalB;  // XOR-Gatter zur Richtungserkennung

  if(xorResult)
  {
    encoderPos++;
  }
  else
  {
    encoderPos--;
  }


}