double Wegkorrektur(double Kraft){ //Funktion gibt Längenänderung an, die durch die Dehnung der Spindel erzeugt wurde.

int EModul = 210000; //Hier E Modul der Spindel angeben in Mpa
double Laenge = 100.0; //Hier Länge der Spindel in mm eingeben
double Kerndurchmesser = 8.47; //Kerndurchmesser der Spindel in mm
double Spannung =  4.0 * Kraft / (3.1415926 * Kerndurchmesser * Kerndurchmesser);

double Laengenaenderung = Spannung * Laenge / EModul;
return Laengenaenderung;
}
//Aufruf im Loop über z.B. double Laengenänderung = Wegkorrektur(Kraft);

