#define encoderPinA 2 // Pin für Phase A
#define encoderPinB 3 // Pin für Phase B
volatile long encoderCount = 0;
// Anpassung des mm-Werts: Der Wert mmPerPulse muss basierend auf der Mechanik (z.B. Übersetzung, Radgröße) angepasst werden
float mmPerPulse = 0.000104; //bei 2400 Umdrehungen

void setup() 
{
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPinA), readEncoder, CHANGE);
  Serial.begin(9600);

}

void loop() 
{
  Serial.print("Position: ");
  Serial.print(encoderPos * mmPerPulse);
  Serial.println(" mm");
  delay(500);


}


void readEncoder()
{
  bool signalA = digitalRead(ENCODER_A);
  bool signalB = digitalRead(ENCODER_B);
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
