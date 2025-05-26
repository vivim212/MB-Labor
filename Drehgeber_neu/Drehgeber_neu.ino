#define encoderPinA 2
#define encoderPinB 3

volatile long encoderTicks = 0;

void setup() {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoderPinA), handleEncoderA, RISING);

  Serial.begin(9600);
}

<<<<<<< HEAD
void loop() {
  static long lastTicks = 0;
  if (encoderTicks != lastTicks) {
    Serial.print("Encoder-Position: ");
    Serial.println(encoderTicks);
    lastTicks = encoderTicks;
  }
}

void handleEncoderA() {
  if (digitalRead(encoderPinB) == LOW) {
    encoderTicks++;
  } else {
    encoderTicks--;
=======
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
>>>>>>> b91b26d7eafe8a01b79cf770f08c5975dbfc1f9f
  }
}
