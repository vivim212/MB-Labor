#include <HX711.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);// I²C-LCD (Adresse 0x27), 16 Zeichen × 2 Zeilen

const int DOUT_PIN = 2;  // DT
const int SCK_PIN  = 3;  // SCK

const float CALIBRATION_FACTOR = -17893.53;;  

HX711 scale;

int zaehler = 0;//Zähler für die serielle Eingabe
double Max = 1.0;//Speichert Maximalwert der Kraft

void setup() {
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







void loop() {
if(zaehler == 0){
lcd.setCursor(0, 0);
lcd.print("Druecke t zum");
lcd.setCursor(0, 1);
lcd.print("starten.");
}

if(zaehler == 3){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fmax:");
  lcd.setCursor(6, 0);
  lcd.print(Max);
  lcd.setCursor(14, 0);
  lcd.print("N");
  delay(10000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Neustart");
  zaehler == 0;
  delay(3000);
  




}
if (Serial.available() > 0) {
    char eingabe = Serial.read();  // Zeichen lesen

    if (eingabe == 't') 
      zaehler++;
}

if(zaehler == 1){//Programm startet
  lcd.clear();
  scale.tare();
  lcd.setCursor(0, 0);
  lcd.print("Start");
  delay(3000);
  lcd.clear();
  zaehler = 2;
  lcd.setCursor(0, 0);
  lcd.print("Kraft:");
  lcd.setCursor(0, 1);
  lcd.print("Dehnung:");
  lcd.setCursor(14, 0);
  lcd.print("N");
  lcd.setCursor(14, 1);
  lcd.print("Pr");



}
while(zaehler == 2){

  float weight = scale.get_units(5);
  float F = weight * 9.81;
  lcd.setCursor(7, 0);
  lcd.print(F);
  double Dehnung = Wegkorrektur(F);
  lcd.setCursor(9, 1);
  lcd.print(Dehnung);
  Serial.print(F);//Ausgabe Messwerte Serieller Monitor
  Serial.print("  ");
  Serial.println(Dehnung);


  if(F>Max){
    Max = F;
  }

    if (Serial.available() > 0) {
    char eingabe = Serial.read();  // Zeichen lesen

    if (eingabe == 't') 
      zaehler++;
  }

}












delay(500);
}
