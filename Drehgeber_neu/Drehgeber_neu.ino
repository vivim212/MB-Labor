#define encoderPinA 2
#define encoderPinB 3

volatile long encoderTicks = 0;

void setup() {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoderPinA), handleEncoderA, RISING);

  Serial.begin(9600);
}

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
  }
}
