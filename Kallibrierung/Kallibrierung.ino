#include <HX711.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);// I²C-LCD (Adresse 0x27), 16 Zeichen × 2 Zeilen

const int DOUT_PIN = 4;  // DT auf anderen freien Pin gelegt
const int SCK_PIN  = 3;  // SCK

const float CALIBRATION_FACTOR = -17893.53;;  

HX711 scale;

float weight = 1.0;
float F = 1.0;
double Max = 1.0;//Speichert Maximalwert der Kraft

const int tasterPin = 2;                // Taster an Pin 2
unsigned long debounceDelay = 50;       // Entprellzeit in ms

bool lastState = HIGH;                  // Letzter Tasterzustand
unsigned long lastPressTime = 0;        // Letzter gültiger Tastendruck

int zaehler = 0;                        // Zählvariable für Tastendrücke

void setup() {
  pinMode(tasterPin, INPUT_PULLUP);
  Serial.begin(9600);
  while (!Serial);

  // LCD initialisieren,der Anschluss A4 für SDA und A5 für SCL ist über die Bibliotek definiert
  lcd.init();
  lcd.backlight();
  
  // HX711 initialisieren
  scale.begin(DOUT_PIN, SCK_PIN);
  scale.set_scale(CALIBRATION_FACTOR);

  delay(500);
}


double Wegkorrektur(double Kraft){ //Funktion gibt Längenänderung an, die durch die Dehnung der Spindel erzeugt wurde.

int EModul = 210000; //Hier E Modul der Spindel angeben in Mpa
double Laenge = 100.0; //Hier Länge der Spindel in mm eingeben
double Kerndurchmesser = 8.47; //Kerndurchmesser der Spindel in mm
double Spannung =  4.0 * Kraft / (3.1415926 * Kerndurchmesser * Kerndurchmesser);

double Dehnung = Spannung *1000.0 / EModul;//Dehnung in Promille
return Dehnung;
}

bool tasterGedrueckt() {//Funktion zur Erkennung der Tasterbetätigung
  bool currentState = digitalRead(tasterPin);
  if (lastState == HIGH && currentState == LOW) {
    if (millis() - lastPressTime > debounceDelay) {
      lastPressTime = millis();
      lastState = currentState;
      return true;
    }
  }
  lastState = currentState;
  return false;
}


void loop() {
if(zaehler == 0){//Startbildschirm
lcd.setCursor(0, 0);
lcd.print("zum Starten");
lcd.setCursor(0, 1);
lcd.print("Taster druecken.");
}

if(zaehler == 3){//Programm wurde vom Nutzer beendet, zurückkehren in die Ausgangslage durch erneutzes Drücken des Tasters
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fmax:");
  lcd.setCursor(6, 0);
  lcd.print(Max);
  lcd.setCursor(14, 0);
  lcd.print("N");
  while(zaehler == 3){
    if (tasterGedrueckt()){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Neustart");
    zaehler = 0; 
    Max = 1.0;
    delay(2000);
    }
  }
}

if (tasterGedrueckt()){//Abfragen, ob Taster gedrückt wurde, bei betätigen start des Programms
  zaehler = 1;
}

if(zaehler == 1){//Programm startet durch drücken des Tasters, geht danach über in die Mess-Schleife
  lcd.clear();
  scale.tare();
  lcd.setCursor(0, 0);
  lcd.print("Start");
  delay(2000);
  lcd.clear();
  zaehler = 2;
  lcd.setCursor(0, 0);
  lcd.print("Kraft:");
  lcd.setCursor(0, 1);
  lcd.print("Dehnung:");
  lcd.setCursor(14, 0);
  lcd.print("kg");
  lcd.setCursor(14, 1);
  lcd.print("Pr");
}

while(zaehler == 2){//Mess-Schleife, wird verlassen bei betätigen des Tasters

  weight = scale.get_units(5);//Hier kann auch Mittelwert aus mehreren Werten gebildet werden, aber nach möglichkeit lange Funktionen vermeiden,um ein verpassen des Taster-Drucks zu verhindern 
  //F = weight * 9.81;
  lcd.setCursor(7, 0);
  lcd.print(weight, 3); // Bessere Anzeige: 2 Nachkommastellen
  double Dehnung = Wegkorrektur(F);
  lcd.setCursor(9, 1);
  lcd.print(Dehnung, 3); // Bessere Anzeige: 3 Nachkommastellen
  Serial.print(weight);//Ausgabe Messwerte Serieller Monitor
  Serial.print("  ");
  Serial.println(Dehnung);

  if(F>Max){//Speichern des maximalen Wertes der Kraftmessung
    Max = F;
  }

  if (tasterGedrueckt()){
    zaehler = 3;
    delay(500);
  }
}//gibt sehr schnell Werte aus, über serielle Schnittstelle, ev. verzögern durch Mittelwert.
//erhöhen des Delay nach Tasterdruck, dass vermieden wird, gleich weiter zu springen im Programm, weil Taster zu lange gedrückt
}
