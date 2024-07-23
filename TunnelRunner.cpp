/*  T U N N E L  R U N N E R (c) Jeremias Kosonen 2007

  Testattu toimivaksi Borland C++ 3.1 kääntäjällä.

Ohjelma kirjoitettu valmiiksi 20:05 21.5. 2007
Highscore taulukot jäivät puuttumaan ajanpuutteesta ja siitä,
että ne tehdään Borlandin käyttäjässä kovin erilailla.

Lähdekoodi sisältää lukuisia funktioita joiden toiminta on
kommentoitu funktioiden alkuun. Lisäksi on kaksi uutta luokkaa,
hirvio ja viestiruutu. Ohjelma on kauttaaltaan melko hyvin kommentoitu,
mutta koodi on paikoin sekavaa. Jos tekisin oikeaa peliä, voitaisiin
sanoa että pelin ensimmäinen versio on valmis, painaa deleteä ja
aloittaa uusiksi. On selvää että 1500 riviä voisi tiivistää ainakin
tuhanteen, ehkä viiteen sataan, ja samalla keveyttää koodia. Ajan-
puuttessa en tätä tee mutta hioin kyllä kaikki kohdat ja pyrin käyttämään
mahdollisimman kevyitä ja nopeita ratkaisuja ohjelmassani.
	Toisinaan tuli kohtia jotka olivat minulle täysin uusia, mm. hirviöiden
tekoäly osio, ja se näkyy koodissa. :D Ohjelma kuitenkin toimii kauttaaltaan
enkä ole törmännyt minkäänlaisiin bugeihin jälkitestauksessa.
	Muutamia sinulle ehkä outoja komentoja olen joutunut käyttämään,
mm. gotoxy jonka toiminnan selvitin jo ja strcpy jonka toiminnan kommentoin
myöhemmin.
	Lopuksi tein pelistä myös värillisen version koska sitä on
mukavampi pelata. En kuitenkaan laittanut "lopulliseen" versioon
komentoja jotka käsittelee värejä koska ne sotkevat koodia ja joudun
vaihtamaan siinä syssyssä mm. cout >> komennon cprintf() komentoon,
tein sen ihan vaan huvikseen. Muuten kummatkin .exe:t ovat täysin samankaltaisia.
	Eli laitoin värillisen version mukaan myös, jos kiinnostaa, on sitä mukavampi
pelaa ja testailla.

*/


#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define MAXX 40 // Luolan maksimikoko määritykset
#define MAXY 24


#define NAKYVYYS 3 // Arvoksi kelpaa kahdella jaollinen luku (2,4,6) ja ykkönen.

#define MARGINAALI 1


// Karttamerkit
#define TYHJA '¨'
#define SEINA '#'
#define REUNA '#'
#define LATTIA '.'
#define SISAANK '_'
#define ULOSK '_'

// Esineet
#define AARRE '$'
#define POMMI '%'
#define KARTTA '~'

// Hahmot
#define PELAAJA '@'
#define HIRVIO '&'

// Muut määritykset
#define POMMEJAKENTASSA 2
#define AARTEITAKENTASSA 3
#define KARTTOJAKENTASSA 1
#define MAXHIRVIOITA 20
#define MAXVUOROJA 100

#define AARTEENARVO 500

int suddendeath = 0; // Ns. Äkkikuolema, tila joka lähtee käyntiin kun vuorot ylittävät maxvuoroja

void TeeKentta();      // Funktioiden alustukset
void PiirraKentta();
int LuoTunnelit();
void VoikoRakentaa(int);
void RakennaReitti(int);
void RakennaHaara();

void PiirraPalkit();
void PiirraHahmot();
void PiirraYmparys();

void PiirraRuutu();
void KaytaPommi();

int SuuntaVapaana(int, int, int);

void PelaaPeli();
void SijoitaKartalle(int);
void OtaKomento();
void TeeJaTarkista();


void Ohje();


void NaytaViesti(char[45], char[45], int);


int ArvoSuunta(int);

int pljx, pljy; // pelaajan x, pelaajan y
int kxy[MAXX][MAXY]; // Kartta objekteille
int luku = 1;
int level;

int hirvioita;
int pommeja;

long int pisteet;
int vuoroja;

int haaroja = 0;
int suoria = 0;


int x, y;
int kmaxX, kmaxY;
int vkulmax, vkulmay;
int kohtax, kohtay;


int maxpituus;
int exkohtax, exkohtay;

int lopeta = 0;
char komento;

// Hirviön class olio alustus
class hirvio {
public:

int vuorojajaljella;
int x, y;
int onkohuomannut;
int nakokentassa;

void Tapa();
int VoikoLiikkua(int);
void Liikuta();
void Piirra();
void ArvoPaikka();
};

// alaoikeassa pelissä näkyvän viestiruudun luokan alustus
class viestiruutu {
public:

char rivi1[25];
char rivi2[25];
char rivi3[25];
char rivi4[25];
char rivi5[25];

void LisaaRivi(char[25]);
void NaytaTeksti();
};

viestiruutu Viestit;
hirvio luuranko[MAXHIRVIOITA];

int main(void)
{
randomize();

char komento;
int lopeta = 0;

do {
luku = 0;
// Tulosta alkuruutu
clrscr();
gotoxy(1,4);
cout << "       -====~~~= // // /|  //  /|| // ==== / /" << endl;
cout << "            //  // // /|| //  //||// /--- / /________" << endl;
cout << "           //  -__// //||//  // ||/ /=== /__________/;;" << endl;
cout << "              ___   __   __  __   _  __  _  ______ ______" << endl;
cout << "             / # / / / /  / / /| // /  |// / ____// //  /::::..." << endl;
cout << "            /---/ / |_/  / / /||// / / |/ / ____ / __  /:::... " << endl;
cout << "           / /|_| |_____/ / / | / /_/| / /_____//_/ |_|:::..______>>" << endl;
cout << endl;
cout << "           _" << endl;
cout << "          ( ) " << endl;
cout << "   ____  _/|_/                A)loita Peli" << endl;
cout << "    ___    |_                                 " << endl;
cout << "   ____  \\/  \\                O)hje" << endl;
cout << "" << endl;
cout << "                              L)opeta";
cout << endl << endl << endl << endl;
cout << "(C) Copyright Jeremias Kosonen 2007                 Valitse - (a,o,l) ";

do {
komento = getch();

if(komento == 'a' || komento == 'o' || komento == 'l')
	luku = 1;

} while(luku == 0);

switch(komento) {
	case 'a':
                PelaaPeli();
		break;
	case 'o':
		Ohje();
		break;
	case 'l':
        	NaytaViesti("              Peli lopetetaan", "     Kiitos että pelasit Tunnel Runneria!",0);
		lopeta = 1;
                break;

}
} while(lopeta == 0);


	return 0;

}



void PelaaPeli()
{
// void PelaaPeli()
// Alustaa kartan pelaamista varten ja käynnistää silmukan pelin suorittamiseen
// Tyhjentää muuttujat ja varastoi niihin tarpeelliset tiedot

NaytaViesti("Huom! Mikäli käytät Windows käyttöjärjestelmää","on suositeltavaa että pelaat peliä kokoruudun",0);
NaytaViesti("tilassa. Paina alt+enter mennäksesi kokoruudun","tilaan. Ikkunassa peli toimii hitaasti.",0);

Viestit.LisaaRivi("");
Viestit.LisaaRivi("");
Viestit.LisaaRivi("");
Viestit.LisaaRivi("Tervetuloa pelaamaan");
Viestit.LisaaRivi("Tunnel Runneria!");
	  pommeja = 0;
	  pisteet = 0;
	  level = 1;  // Huom. max level = 10 ja sen ylipuskeminen aiheuttaa ohjelman toimimattomuuden.
	  clrscr();
do {

lopeta = 0;

suddendeath = 0;
vuoroja = 0;

hirvioita = level / 2;

kmaxX = 19 + level * 2; // HUOM näiden on oltava vähintaan 1 pienempi kuin MAXX ja MAXY
kmaxY = 12 + level;     // JA HUOM kmaxX aina PARITON ja kmaxY AINA PARILLINEN!!!
	                // maximi kmaxX = 39 kmaxY = 22
luku = kmaxY % 2;       
if (luku == 1)          // Koska Y on oltava parillinen, tämä koodi tarkistaa että
	kmaxY--;        // jos se on pariton se poistaa siitä yhden tehden siitä parillisen.


TeeKentta();
SijoitaKartalle(1);
NaytaViesti("         :::    T   A   S   O   ::: ", "                      ", 1);
PiirraRuutu();

	do {
	OtaKomento();
	TeeJaTarkista();
	PiirraRuutu();

	} while(lopeta == 0);

} while (lopeta == 2);

if(lopeta == 3) {
	NaytaViesti("          You are dead...","           ...Rest in Peace...",0);
	NaytaViesti("   Lopullinen pistemäärä: ","          Pääsit tasolle: ",4); }

if(lopeta == 4) {
		level = 10;
        NaytaViesti("         !!! ONNEKSI OLKOON !!!", " Olet selviytyjä! Rikas elämä odottaa!",0);
	NaytaViesti("   Lopullinen pistemäärä: ","          Läpäisit tason ",4); }


// lopeta = 1 :: Lopeta peli kokonaan
// lopeta = 2 :: Pelaaja on päässyt uudelle tasolle,
// lopeta = 3 :: Pelaaja on kuollut
// lopeta = 4 :: Pelaaja selvisi elossa loppuun saakka
// Palaa valikkoon...
}

void OtaKomento()                  
{
int voiko;
komento = ' ';

// Ota käyttäjältä komento komento muuttujaan ja toimi sen mukaan
komento = getch();

switch(komento) {
// Älä päästä hahmoa kulkemaan seinissä, reunoissa, "tyhjässä"
// Jos pelaaja liikkui / yritti liikkua, vähennä vuoroja
case 'w': 
        voiko = SuuntaVapaana(4, pljx, pljy);
        if (voiko == 1)
	    pljy--;
	vuoroja++;
	break;

case 's':
	voiko = SuuntaVapaana(1, pljx, pljy);
        if (voiko == 1)
	  pljy++;
	 vuoroja++;
         break;

case 'a':
	voiko = SuuntaVapaana(3, pljx, pljy);
        if (voiko == 1)
	pljx--;
	 vuoroja++;
	 break;

case 'd': 
	voiko = SuuntaVapaana(2,pljx,pljy);
        if (voiko == 1)
	pljx++;
	 vuoroja++;
	 break;
case 'r':
	KaytaPommi();
	break;
case '?':
	Ohje();
	break;
case 'Q':
	NaytaViesti("    Peli lopetetaan; palataan valikkoon...","",0);
	lopeta = 1;
	break;

}


}


void TeeJaTarkista()
{

// void TeeJaTarkista()
// Liikuttaa hirviöitä, jos pelaaja osuu esineeseen aktivoi esineen tehtävän
// (lisää dynamiitin mukaan, lisää pisteita jos aarre, näyttää kartan jos tutkalaite)
// Jos pelaajan vieree osuu hirviö, poistaa pelaajalta vuoroja ja poistaa hirviön
// Jos pelaaja on maalipisteessä siirtää pelaajan seuraavalle tasolle
// Lopuksi vähentää pelaajalta yhden vuoron.

switch(kxy[pljx + vkulmax][pljy + vkulmay]) {  
	case 5: // Pelaaja on saapunut radan uloskäynnille (siirtyy seuraavalle tasolle)

		
		if(level <= 10){
		NaytaViesti("               Onneksi olkoon!", "    Selvisit elossa ja ajoissa tasosta ", 1);
		NaytaViesti("               ", "              Vuoroja kului:", 2);
                NaytaViesti("Jäljelle jääneitä vuoroja: ", "        Tasobonus:",3);
                pisteet = pisteet + ((MAXVUOROJA - vuoroja) * 10);
                pisteet = pisteet + (level * 1000);
		level++;
		lopeta = 2;

		if(level == 11)
        	lopeta = 4;

		}
		break;
	case 6: // Pelaaja löysi pommin
        	Viestit.LisaaRivi("Löysit pommin!");
		kxy[pljx + vkulmax][pljy + vkulmay] = 3;
        	pommeja++;
        	break;
	case 7: // Pelaaja löysi aarteen
		Viestit.LisaaRivi("Löysit aarteen! +500 pt");
        	kxy[pljx + vkulmax][pljy + vkulmay] = 3;
            pisteet = pisteet + AARTEENARVO;
        	break;
        case 8: // Pelaaja löysi kartan
		Viestit.LisaaRivi("Löysit kartan!");
		Viestit.LisaaRivi(" Paina näppäintä.");
		Viestit.LisaaRivi("");
		PiirraRuutu();
		getch();
                kxy[pljx + vkulmax][pljy + vkulmay] = 3;
        	PiirraKentta();
		Viestit.LisaaRivi("Kartta paloi poroksi");
        	break;
        }

// Hirviöt: jos vuoroja on jäljellä (se on olemassa) liikuta hirviötä
// jos hirviö on huomattu, vähennä siltä yksi vuoro.
// jos hirviön vuorot laskevat nollaan, hirviö ei enää ole olemassa
// joten siltä on turha laskea vuoroja enää.
for(luku = 0; luku < hirvioita; luku++) {  
	if(luuranko[luku].vuorojajaljella > 0)
		luuranko[luku].Liikuta();
	if(luuranko[luku].x == pljx && luuranko[luku].y == pljy && suddendeath == 1) {
        	NaytaViesti("    Luurankosotilaat saivat sinut kiinni","              ...Game over...",0);
		lopeta = 3; }
	if(luuranko[luku].x == pljx && luuranko[luku].y == pljy && suddendeath == 0) {
        	NaytaViesti("    Luurankosotilas pahoinpiteli sinut!!","   Joudut keräämään itsesi (-20 vuoroa)",0);
		vuoroja = vuoroja + 20;
		if(vuoroja > MAXVUOROJA) // Pahoinpitely voi johtaa vain maksimivuoroihin
			vuoroja = MAXVUOROJA; // ettei sudden death viesti mene ohi ja peli sekavaksi.
		luuranko[luku].Tapa(); }
	if(luuranko[luku].onkohuomannut == 1) {
	   luuranko[luku].vuorojajaljella--;
	   if(luuranko[luku].vuorojajaljella == 0)
        luuranko[luku].Tapa();
	}
}

// Jos vuoroja ylittävät MAXVUOROJA, aktivoi sudden death
if(vuoroja > MAXVUOROJA) // SUDDEN DEATH!! Luo level * 2 luurankoa karttaan jotka osuessaan TAPPAVAT
                         // pelaajan.
{
if(suddendeath == 0) {
suddendeath = 1;

hirvioita = hirvioita + level * 2;
SijoitaKartalle(2);

NaytaViesti("Liian myöhäistä! Aurinko laskee taivaanrantaan", "    ja luurangot nousevat haudoistaan!",0);
NaytaViesti("      Yritä vielä päästä ulospääsyyn,"," tällä kertaa luurangot eivät tyydy vähään..",0);
}

}
}

void KaytaPommi()
{
char suunta; int voiko;
// void KaytaPommi()
// Jos pelaajalla on pommi, käyttää sen. Pelaajan valittua suunta
// ohjelma tarkistaa että valitussa kohdassa on seinä(3) tms ja jos on
// muuttaa sen lattiaksi (1) sekä vähentää pelaajalta pommin.
// Välttääkseen koodin liiallista kasvamista, ohjelma käyttää
// funktiota onkovapaana ja mikäli funktio palautta arvon ei voi liikkua
// muuttaa sen lattiaksi. Tämä aiheuttaa ongelman siinä että kenttärajoja
// ja "tyhjää" voi räjäyttää mutta säästää koodin pituutta.

if(pommeja > 0) {
	NaytaViesti("        Käytit pommin!", "          Valitse suunta (w,a,s,d)",0);
        PiirraRuutu();  // Refresh (parantaa tuntumaa)
	suunta = getch();

	switch(suunta) {
case 'w':
        voiko = SuuntaVapaana(4, pljx, pljy);
        if (voiko == 0) {
	    kxy[pljx + vkulmax][pljy + vkulmay - 1] = 3;
            pommeja--;}
	break;

case 's':
	voiko = SuuntaVapaana(1, pljx, pljy);
	if (voiko == 0) {
	    kxy[pljx + vkulmax][pljy + vkulmay + 1] = 3;
            pommeja--;}
	 break;

case 'a':
	voiko = SuuntaVapaana(3, pljx, pljy);
	if (voiko == 0) {
	    kxy[pljx + vkulmax - 1][pljy + vkulmay] = 3;
            pommeja--;}
	 break;

case 'd': 
	voiko = SuuntaVapaana(2,pljx,pljy);
	if (voiko == 0) {
	    kxy[pljx + vkulmax + 1][pljy + vkulmay] = 3;
            pommeja--;}
	 break;

}

if(voiko == 0) {  // Jos räjäytys onnistui, näytä viesti
	    Viestit.LisaaRivi("Räjäytit seinän");
	    Viestit.LisaaRivi(" kappaleiksi!");}
	else Viestit.LisaaRivi("Tähtää seinään.");
}

        
}


void SijoitaKartalle(int vaihe)
{
// void SijoitaKartalle()
// Sijoita kartalle luurankosotilaat, esineet ja pelaaja.
// (Alusta kartta ja peli vaiheeseen 1)

// Itemit voi tallentaa kxy[][] muuttujaan sillä aina itemin nostaessa = sen poistuessa
// tilalle voi laittaa lattian ilman kummia kyselemettä. 
// funktiota muutettu niin että vaihe määrää funktion tehtävää; aloitetaanko peli 0
// tilanteesta vai spawnataako sata luurankosotilasta


if(vaihe == 1) {
pljx = 0; pljy = 1; // Sijoita pelaaja sisäänkäyntiin

// Alusta ja sijoita hirviöt sen mukaan paljon niitä halutaan karttaan (hirvioita)
for(luku = 0; luku < hirvioita; luku++) {
luuranko[luku].ArvoPaikka();
luuranko[luku].vuorojajaljella = 15;
}
// Tähän tulee sitten itemien sijoittaminen.
for(luku = 0; luku < POMMEJAKENTASSA; luku++) {
do {
x = rand()%kmaxX;
y = rand()%kmaxY;
} while(kxy[x+vkulmax][y+vkulmay] != 3);
kxy[x+vkulmax][y+vkulmay] = 6; }

for(luku = 0; luku < AARTEITAKENTASSA; luku++) {
do {
x = rand()%kmaxX;
y = rand()%kmaxY;
} while(kxy[x+vkulmax][y+vkulmay] != 3);
kxy[x+vkulmax][y+vkulmay] = 7; }

for(luku = 0; luku < KARTTOJAKENTASSA; luku++) {
do {
x = rand()%kmaxX;
y = rand()%kmaxY;
} while(kxy[x+vkulmax][y+vkulmay] != 3);
kxy[x+vkulmax][y+vkulmay] = 8; }




}

if(vaihe == 2)  // Sudden Death -moodi.
{
for(luku = 0; luku < hirvioita; luku++) {
luuranko[luku].ArvoPaikka();
luuranko[luku].vuorojajaljella = 15;
 }
}

}
void PiirraRuutu()
{
// void PiirraRuutu().
// Piirtää ruudulle hahmon, NAKYVYYS verran ruutuja kartasta sen ympärille,
// oikeaan kulmaan tietopalkit ja yläreunaan pelin nimen
clrscr();
PiirraYmparys();
PiirraHahmot();
PiirraPalkit();

}

void PiirraHahmot()
{
// Piirrä ukkeli
gotoxy(vkulmax + pljx + MARGINAALI, vkulmay + pljy + MARGINAALI);
cout << PELAAJA;

// Jos hirviöitä näkökentässä, piirrä
for(luku = 0; luku < hirvioita; luku++) {
if(luuranko[luku].nakokentassa == 1)
	luuranko[luku].Piirra();

}


}

void PiirraPalkit()
{
gotoxy(2,1);
cout << "_*____,.-'¨'-.,_,.- T U N N e L   R U N N e R -.,_,.-'¨'-.,_____*_";

for(y=3; y < 23; y++)
{ gotoxy(MAXX + 4, y);
  cout << ":"; }

gotoxy(MAXX + 15, 4);
cout << "TASO " << level;

gotoxy(MAXX + 5, 6);
cout << "-------------------------";

gotoxy(MAXX + 6, 7);
cout << "Pommeja (" << pommeja << ")";
gotoxy(MAXX + 6, 9);
cout << "Vuoroja (" << vuoroja << "/"<< MAXVUOROJA << ")";
gotoxy(MAXX + 6, 11);
cout << "Pisteet (" << pisteet << ")";
gotoxy(MAXX + 13, 14);
cout << "'?' - Näytä ohje";
gotoxy(MAXX + 5, 15);
cout << "-------------------------";

Viestit.NaytaTeksti();
}


void PiirraYmparys()
{
int x2 = 0, y2 = 0;

x = 0; y = 0;

// Ohjelma aloittaa piirtämisen -2, -2 koordinaateista suhteessa
// pelaan koordinaatteihin. Tai jos näkyvyyttä muutetaan,
// numero muuttuu kuinka kaukaa pelaajaa aloitetaan piirtäminen..

for(y = 0 -(NAKYVYYS); y < (NAKYVYYS + 1); y++)
	{
	for(x = 0 -(NAKYVYYS ); x < (NAKYVYYS + 1); x++)
	{
        //Sijoita kursori kohtaan mistä tarkistetaan
	gotoxy(pljx + x + vkulmax + MARGINAALI, pljy + y + vkulmay + MARGINAALI);

		

		x2 = pljx + vkulmax + x; // Laske koordinaatit valmiiksi mistä etsiä kxy- muuttujasta
                y2 = pljy + vkulmay + y;

		

                if(kxy[x2][y2] == 0)
			cout << TYHJA;
		else if (kxy[x2][y2] == 1)
			cout << REUNA;
		else if (kxy[x2][y2] == 2)
			cout << SEINA;
		else if (kxy[x2][y2] == 3)
                	cout << LATTIA;
		else if (kxy[x2][y2] == 4)
			cout << SISAANK;
		else if (kxy[x2][y2] == 5)
			cout << ULOSK;
		else if (kxy[x2][y2] == 6)
			cout << POMMI;
		else if (kxy[x2][y2] == 7)
			cout << AARRE;
		else if (kxy[x2][y2] == 8)
			cout << KARTTA;

		else cout << "";

		// Tarkasta onko näkökentässä hirviötä
                for(luku = 0; luku < hirvioita; luku++) {  
		if(pljx + x == luuranko[luku].x && pljy + y == luuranko[luku].y
		   && luuranko[luku].vuorojajaljella > 0) {
		   if(luuranko[luku].onkohuomannut == 0) {
			Viestit.LisaaRivi("Luurankosotilas!");
			Viestit.LisaaRivi(" Juokse karkuun!");
                        }
		   luuranko[luku].nakokentassa = 1;}
		
		}
                

	}
	}

}

int SuuntaVapaana(int suunta, int x, int y)
{
// Funktio SuuntaVapaana
// Pelaajan liikuttamista varten, tarkistaa onko annettu suunta vapaana
// Jos on, palauttaa onko = 1
int onko = 0;

switch(suunta) {
// Älä päästä hahmoa kulkemaan seinissä, reunoissa, "tyhjässä"

case 1:   // Alas
	if (kxy[x + vkulmax][y + vkulmay + 1] != 0
	 && kxy[x + vkulmax][y + vkulmay + 1] != 1
	 && kxy[x + vkulmax][y + vkulmay + 1] != 2)
		onko = 1; 
         break;

case 2:   // Oikee
	if (kxy[x + vkulmax + 1][y + vkulmay] != 0
	 && kxy[x + vkulmax + 1][y + vkulmay] != 1
	 && kxy[x + vkulmax + 1][y + vkulmay] != 2)
		onko = 1;
         break;

case 3:   // Vasen
	if (kxy[x + vkulmax - 1][y + vkulmay] != 0
	 && kxy[x + vkulmax - 1][y + vkulmay] != 1
	 && kxy[x + vkulmax - 1][y + vkulmay] != 2)
		onko = 1;
	 break;

case 4:  // Ylös
	if (kxy[x + vkulmax][y + vkulmay - 1] != 0
	 && kxy[x + vkulmax][y + vkulmay - 1] != 1
	 && kxy[x + vkulmax][y + vkulmay - 1] != 2)
		onko = 1;
	break;

}
return onko;
}


/* class Hirvio KAIKKI FUNKTIOT
hirvio::ArvoPaikka() Arpoo hirviölle paikan (hirviön alustamisfunktio)
hirvio::VoikoLiikkua(suunta) Tarkistaa voiko hirviö liikkua haluttuun/arvottuun suuntaan
hirvio::Liikuta() Keinoäly osuus, laskee parhaimman suunnan hirviölle ja yrittää liikuttaa hirviötä
hirvio::Piirra() Piirtää hirviön kentälle
hirvio::Tapa() Poistaa hirviön kentältä
*/

void hirvio::ArvoPaikka()
{
// Arpoo hirviölle paikan kartalla
do {
hirvio::x = rand()%kmaxX;
hirvio::y = rand()%kmaxY;
} while(kxy[hirvio::x + vkulmax][hirvio::y + vkulmay] != 3);
}


int hirvio::VoikoLiikkua(int suunta)
{
// Hirviön hieman erilailla toimiva voiko tähän liikkua funktio..
int voiko;

switch(suunta) {
case 1:	voiko = SuuntaVapaana(1,hirvio::x,hirvio::y);
	if (voiko == 1)
		hirvio::y++;
	break;
case 2:	voiko = SuuntaVapaana(2,hirvio::x,hirvio::y);
        if (voiko == 1)
		hirvio::x++;
         break;
case 3:	voiko = SuuntaVapaana(3,hirvio::x,hirvio::y);
        if (voiko == 1)
		hirvio::x--;
	 break;
case 4:	voiko = SuuntaVapaana(4,hirvio::x,hirvio::y);
        if (voiko == 1)
		hirvio::y--;
         break;
}
	 return voiko;
}

void hirvio::Liikuta()
{
int suunta, voiko = 2; int voiko2 = 0;
int suunta2; // Väliaikaismuuttuja reitin suunnan päättämiseen
int luku1, luku2; // ^---- sama
int x2, y2;
// Liikuttaa hirviöitä joko satunnaisesti kartalla tai pelaajan suuntaan
// mikäli pelaaja ja hirviö ovat huomanneet toisensa.
// Liikuta satunnaisesti

if(hirvio::onkohuomannut == 0) {
do {
	suunta = ArvoSuunta(2);
 // Arvo liikkumisen suunta (funktio alunperin tunnelin suunnalle
voiko = hirvio::VoikoLiikkua(suunta);
} while(voiko == 0);
}

// Jos hirvio ja pelaaja on kohdannut, se lähtee kulkemaan kohti pelaajaa "suorinta tietä"
// Mikäli jokin suunta on tukittu, se yrittää toista tietä mikä vie hirviötä lähemmäksi.
// (seuraa pitkä pätkä tekstiä) jea keinoälyn riemuvoitto :D
if(hirvio::onkohuomannut == 1) { 
// Valitse suunta koordinaattien perusteella
x2 = hirvio::x - pljx; // Hirviön ja pelaajan koordinaattien erotus
y2 = hirvio::y - pljy; // (kertoo hirviön paikan suhteessa pelaajaan)
luku1 = 0; luku2 = 0; // Väliaikais muuttujia
suunta = 0;
suunta2 = 0;

if(x2 < 0){ // suuntana oikea (2)
   luku1 = luku1 - x2;
   suunta2 = suunta2 + 2; }
if(x2 > 0){ // suuntana vasen (3)
   luku1 = x2;
   suunta2 = suunta2 + 3; }
if(y2 < 0){ // suuntana alas (1)
   luku2 = luku2 - y2;
   suunta2 = suunta2 + 1; }
if(y2 > 0){ // suuntana ylös (4)
   luku2 = y2;
   suunta2 = suunta2 + 4; }
if(x2 == 0 || y2 == 0) { // Jos jompikumpi luvuista on nolla, hirvio haluaa kulkea suoraan
	suunta = suunta2; // ei siis tarvetta laskea toista haluttua suuntaa ja siksi case 123 = abort
	suunta2 = 123; }

// Vertaa luku1 ja luku2 saadaksesi tietää kumpi kahdesta paremmasta suunnasta
// kannattaa ottaa (pidempi, lyhyentämällä tasaisesti kumpaakin on "nopein" reitti)
// (ei ota huomioon esteitä... mutta luurankosotilaathan on tunnetusti tyhmiä ;))
// Siinä tilanteesa että ne ovat yhtäpitkiä, luuranko yrittää kumpaakin, mikäli kumpikaan
// ei toimi, arpoo hän itsestään jonkun muun.
switch(suunta2) {
	case 123: // Tapaus jossa matka on suora (haluaa vain yhteen suuntaan)
        	break;
    	case 3: // oikee(2) + alas(1)
	       if(luku1 > luku2)
               		suunta = 2;
	       else if(luku2 > luku1)
			suunta = 1;
	       else suunta = ArvoSuunta(2);
		break;
	case 6: // oikea(2) + ylös(4)
	       if(luku1 > luku2)
               		suunta = 2;
	       else if(luku2 > luku1)
			suunta = 4;
	       else suunta = ArvoSuunta(2); 
               	break;
	case 4: // vasen(3) + alas(1)
		if(luku1 > luku2)
               		suunta = 3;
	       else if(luku2 > luku1)
			suunta = 1;
	       else suunta = ArvoSuunta(2);
                break;
	case 7: // vasen(3) + ylös(4)
		if(luku1 > luku2)
               		suunta = 3;
	       else if(luku2 > luku1)
			suunta = 4;
	       else suunta = ArvoSuunta(2); 
                break;
	}
if(suunta > 4)
	suunta = ArvoSuunta(2);

voiko = hirvio::VoikoLiikkua(suunta);

if(voiko == 0) // Jos ei voinut liikkua ensiksi haluttuun suuntaan, yritä vielä toiseen
 {
 switch(suunta2) {
	
	case 3: // oikee(2) + alas(1)
	       cout << suunta;
	       if(suunta == 2)
               		suunta = 1;
	       else if(suunta == 1)
               		suunta = 2;
	       else suunta = ArvoSuunta(2);
		break;
	case 6: // oikea(2) + ylös(4)
	       cout << suunta;
	       if(suunta == 2)
               		suunta = 4;
	       else if(suunta == 4)
               		suunta = 2;
	       else suunta = ArvoSuunta(2); 
		break;
	case 4: // vasen(3) + alas(1)
		cout << suunta;
		if(suunta == 3)
               		suunta = 1;
	        else if(suunta == 1)
               		suunta = 3;
	        else suunta = ArvoSuunta(2); 
		break;
	case 7: // vasen(3) + ylös(4)
	       cout << suunta;
	       if(suunta == 3)
               		suunta = 4;
	       else if(suunta == 4)
               		suunta = 3;
	       else suunta = ArvoSuunta(2);
		break;
	}
voiko2 = hirvio::VoikoLiikkua(suunta);
voiko = 0; voiko2 = 0;
}}}



void hirvio::Piirra()
{                                 
// Tarkastaa onko hirviö pelaajan näkökentässä ja piirtää jos on
// Erillinen funktio käytännössä tarpeeton mutta olemassa sitä varten
// että ohjelmoinnin kannalta monipuolisempi ohjelma. (olio-ohjelmointia)


gotoxy(hirvio::x + vkulmax + MARGINAALI, hirvio::y + vkulmay + MARGINAALI);
cout << HIRVIO;

hirvio::onkohuomannut = 1;
hirvio::nakokentassa = 0; // Tämä nollataan koska jostain syystä funktiossa joka tarkastaa
			  // pelaajan näkökentän (PiirraYmparys();) if - else rakenne ei toimi
			  // niin että if nakokentassa, = 1, else = 0. O_o käy se näinki yhtä hyvi.
                          // (jos lukua ei nollattaisi hirviö näkyisi ruudulla loputtomiin)
}

void hirvio::Tapa()
{
Viestit.LisaaRivi("Luurankosotilas");
Viestit.LisaaRivi(" lähti pois.");

hirvio::x = 0; hirvio::y = 0;
hirvio::onkohuomannut = 0;
hirvio::vuorojajaljella = 0;
}


/* ============================================================
 K A R T A N   L U O M I S E N   F U N K T I O T   J A
 D E B U G I T   T Ä S T Ä   E T E E N P Ä I N
 ==============================================================
 */

void TeeKentta()
{  // Funktio: TeeKentta(), luo arvotusti tunneliverkoston x ja y 2d taulukkopohjalle
   //
   // Käytetyjen muuttujien selitykset:
   // vkulmax, vkulmay: vasen yläkulma x, vasen yläkulma y. kartan kohittamista
   //    ja kohtien laskemista (tämän ansiosta itse taso voi olla keskellä karttaa..)
   // suoria, haaroja: kun ohjelma luo suoria ja haaroja se laskee ne ylös näihin muuttujiin
   //    ja lopuksi katsoo että kun kartta on muuten valmis ja toimiva, se on tarpeeksi
   //    täynnä suoria näiden avulla.

int onnistuiko = 0;

do {
x = 0; y = 0;
kohtax = 0; kohtay = 0;
suoria = 0;
haaroja = 0;

//Täytä kartta tyhjällä jossa ei voi kulkea
	for(y = 0; y < MAXY; y++)
   	{
		for(x = 0; x < MAXX; x++)
		{
			kxy[x][y] = 0;
		}
	}

// Luo mahd. keskelle karttaa reunukset kartalle
// annettujen kartan tietojen mukaan (kmaxX, kmaxY)
x = 0; y = 0; vkulmax = 0; vkulmay = 0;  // tyhjennä muuttujat

// vasen ylä reunan koordinaatit x = (MAXX / 2) - (kmaxX / 2), y = (MAXY / 2) - (kmaxY / 2) jne.
// Näitä tarvitaan paljon! (kartan koon ja sijainnin suhteutukset koordinaatistoon)
vkulmax = MAXX / 2 - kmaxX / 2;
vkulmay = MAXY / 2 - kmaxY / 2;

for(x = 0; x < kmaxX; x++) // YLÄ JA ALAREUNA
{
	kxy[x + vkulmax][vkulmay] = 1;
	kxy[x + vkulmax][kmaxY + vkulmay] = 1;
}
for(y = 0; y < kmaxY; y++) // VASEN JA OIKEAREUNA
{
	kxy[vkulmax][y + vkulmay] = 1;
	kxy[vkulmax + kmaxX - 1][y + vkulmay] = 1;
}


// Täytä reunojen sisällä oleva tila seinällä
for(y =1; y < kmaxY; y++) {
	for(x = 1; x < kmaxX - 1; x++)
	  kxy[x + vkulmax][y + vkulmay] = 2;
        }

 // Luo sisään- ja uloskäynti luolastoon
 kxy[vkulmax][vkulmay + 1] = 4;
 kxy[kmaxX + vkulmax - 1][kmaxY + vkulmay - 1] = 5;


 // Luo kartta uusiksi niin monta kertaa
 // Jos LuoTunnelit palauttaa 1, kartta on valmis ja toimii.
 // että kartta "onnistuu" (reitti yltää ulospääsyyn tukkiutumatta
 // ja kartassa on tarpeeksi tunnelia ollakseen haastava.

onnistuiko = LuoTunnelit();  
} while(onnistuiko == 0 || onnistuiko == 2);    

}



int LuoTunnelit()
{
// Funktio: LuoTunnelit(), luo karttapohjaan randomisti tunnelit
// yksi kerrallaa ja luo haaroja jotka hämäävät pelaajaa vaikeuttaen
// loppuun pääsyä.

kohtax = 1; kohtay = 1;

int suunta;
int arpa;
int onnistuiko = 0;
int maxyrityksia = level * 80;
int yrityksia = 0;

/* ::::::::: ITSE RANDOM TUNNELIEN LUOJA :::::::
Toiminta-idea selkokielellä on että ohjelma rakentaa
tunneleita pääasiallisesti ylä vasemmalta alaoikeaan suuntaan.
Jokaisen suoran luotuaan ohjelma yrittää 3 kertaa satunnaisesti luoda haaran
suoralle tai jo onnistuneelle haaralle. Tämän jälkeen se palaa suoran
rakentamiseen. Mikäli tunnelien rakentamiseen kuluu liian kauan
(kartta on aivan täynnä pikku tunneleita tai se on rakentanut itsensä
umpeen) se palaa ylempään funktioon ja aloittaa alusta kartan rakentamisen.
*/


 x = kohtax; y = kohtay; // x- ja y- ovat väliaikaisia muuttujia, kohtax ja kohtay
                         // merkkaa kohtaa jossa rakennus tapahtuu
 do {
 maxpituus = 0;
 suunta = ArvoSuunta(1);  // Arvo suunta, 1 alas 2 oikea 3 vasem 4 ylös


// Tarkasta oliko reitti mahdollissa rakentaa
VoikoRakentaa(suunta);

// Jos oli, rakenna reitti ja yritä rakentaa haaroja.
if(maxpituus > 0){
   RakennaReitti(suunta);
   RakennaHaara();
   suoria++;
   }




//Jos reitti yltää loppuun, lopeta kartan teko sillä se on valmis.
//Varmista myös että haaroja ja reittejä on tarpeeksi jotta kartta on haastava.
//(vaatimukset kasvaa levelin kasvaessa.)

if (kohtax == kmaxX - 2 && kohtay == kmaxY - 1) {  
   if(suoria + haaroja > level * 4 + (kmaxX + kmaxY) / 2)
	onnistuiko = 1;
	kxy[kohtax + vkulmax][kohtay + vkulmay] = 3;

	 }

if(yrityksia >= maxyrityksia) {    // Jos reittiä ollaan yritetty rakentaa jo liian kauan (yrityksia)
	                           // ohjelma luovuttaa ja alkaa rakentaa uutta, koska tämä tarkoittaa
	onnistuiko = 2;		   // että kartta on liian täynnä jo tai se on rakentanut itsensä umpeen.
	}

yrityksia++;
} while (onnistuiko == 0 ); // Jatka kunnes reitit ovat kaikki sulkeutuneet. (käytännössä kun ohjelma itse
                            // käskee tietyin ehtoin sulkea ne..)
return onnistuiko;  // Palauta ylemmälle funktiolle tieto, onnistuiko rakentaminen.

}

int ArvoSuunta(int tapa) {
 int suunta;
 int arpa;


 // Arvo numero joka määrää suunnan
 // 1-3 = alas (1), 4-6 = oikealle (2), 7-8 vasem (3), 9-10 ylös (4)

 if (tapa == 1) {
 arpa = rand()%10;

 if(arpa <= 3)
	suunta = 1;
 else if(arpa >= 4 && arpa <= 6)
	suunta = 2;
 else if(arpa >= 7 && arpa <= 8)
	suunta = 3;
 else if(arpa >= 9 && arpa <= 10)
	suunta = 4;
 else cout << "Nyt homma pissii!";
 }

 // Haaroilla ei ole painotusta mihinkään suuntaan, siksi oma arvontarutiininsa
 // tapa == 2. 1-2 alas (1) 3-4 oikea (2) 5-6 vasem (3) 7-8 ylös (4)

 if (tapa == 2) {
 arpa = rand()%8;

 if(arpa <= 2)
	suunta = 1;
 else if(arpa >= 3 && arpa <= 4)
	suunta = 2;
 else if(arpa >= 5 && arpa <= 6)
	suunta = 3;
 else if(arpa >= 7 && arpa <= 8)
	suunta = 4;
 else cout << "Nyt homma pissii!";
 }

 return suunta; // Palauta ylemmälle funktiolle arvottu suunta

}


void VoikoRakentaa(int suunta) {
                            
// Funktio VoikoRakentaa(int suunta)
// Tarkistaa voiko arvottuun suuntaan rakentaa tunnelia
// (onko seinää; toisen tyhjän tunnelin päälle tai rajan yli
// ei rakenneta...)
// Sijoittaa tuloksen maxpituus muuttujaan.
// HUOM! Tämä funktio ei rakenna tunnelia vaan tutkii maxpituuden
// siksi kohtax ja kohtay ei muutu vaan käytetään väliaikaisia x ja y.

int eipysty = 0;
x = kohtax; y = kohtay;

 if (kxy[vkulmax + x][vkulmay + y] == 2 || kxy[vkulmax + x][vkulmay + y] == 3)  // Varmista että kohdassa käytettävää seinää
     { // Tarkista kuinka pitkälle seinää voi jatkaa

     maxpituus = 0;
     do {
		switch(suunta) {  // Suunnan mukaan, tarkista kuinka kauan suuntaan voi jatkaa
                                  // ennen kuin seinätila loppuu. (vastassa lattia, raja tai exit)
		case 1: {         // HUOM ohjelma tarkistaa joka toisen ruudun koska pelin tulee rakentaa vain yhden levyisiä käytäviä
			if(kxy[vkulmax+ x][vkulmay + y + 2] == 2) {
				y = y + 2;
				maxpituus = maxpituus + 2; }
			else(eipysty = 1);
                        break;
                        }
		case 2: {
			if(kxy[vkulmax+ x + 2][vkulmay + y] == 2) {
				x = x + 2;
				maxpituus = maxpituus + 2; }
			else(eipysty = 1);
                        break;
			}
		case 3: {
			if(kxy[vkulmax+ x - 2][vkulmay + y] == 2) {
				x = x - 2;
				maxpituus = maxpituus + 2; }
			else(eipysty = 1);
                        break;
		        }
		case 4: {
			if(kxy[vkulmax+ x][vkulmay + y - 2] == 2) {
				y = y - 2;
				maxpituus = maxpituus + 2; }
			else(eipysty = 1);
                        break;
		}
               
 		} } while (eipysty == 0);

}}

void RakennaReitti(int suunta) {
int arpa;
int pituus = 0;

// Reitin pituuden määrittämiseksi ohjelma arpoo luvun jonka max = maxpituus / 2
// Tämä jaetaan kahdella ja siihen lisätään 1. Reitin minimipituus on siis 2
// ja maximipituus on reitin (teoreettinen maximipituus / 2) / 3 + 2


arpa = rand()%maxpituus / 2;
pituus =  arpa / 2 + 1;
if(pituus < maxpituus)
	pituus++;

luku = pituus % 2;
if(luku == 1)
       pituus--;	

		x = 0; y = 0;  // väliaikaismuuttujia tarvitaan taas lausekkeen lopettamiseen
			       // kohtay:tä ei voi käyttää koska se voi olla jo suurempi kuin pituus
			       // alunperin. x ja y muuttujat hämäävät hiukan muttei ole järkeä
                               // tehdä uusia muuttujia tätä varten (vie muistia ja homma on selvä muutenkin)

		switch(suunta) {   // RAKENNA REITTI SUUNNAN PERUSTEELLA
		
		case 1: {
			for(kohtay = kohtay; y < pituus; kohtay++) { 
			kxy[kohtax + vkulmax][kohtay + vkulmay] = 3; y++;
			}
                        break;       
			}
		case 2: {
			for(kohtax = kohtax; x < pituus; kohtax++) {
			kxy[kohtax + vkulmax][kohtay + vkulmay] = 3; x++;
			} 			
                        break;
			}
		case 3: {
			for(kohtax = kohtax; x < pituus; kohtax--) {
			kxy[kohtax + vkulmax][kohtay + vkulmay] = 3; x++;
			} 			
                        break;
			}
		case 4: {
			for(kohtay = kohtay; y < pituus; kohtay--) {
			kxy[kohtax + vkulmax][kohtay + vkulmay] = 3; y++;
			} 			
                        break;
			}
        	}

}

void RakennaHaara() {
// Funktio: RakennaHaara().
// Kun tunnelisuora on rakennettu, ohjelma pyrkii rakentamaan kolme kertaa
// haaraa suoralle. Tämän jälkeen se jatkaa suoran tekemistä.
// exkohtax & exkohtay = talletuspaikka pääsuoran viimeisimmille koordinateille

int suunta;
int toisto = 0;

suunta = ArvoSuunta(2);

exkohtax = kohtax; exkohtay = kohtay;
VoikoRakentaa(suunta);

for(toisto = 0; toisto < 3; toisto++) {

suunta = ArvoSuunta(2);
VoikoRakentaa(suunta);

if(maxpituus > 0) {
   RakennaReitti(suunta);
   haaroja++;}

}

// Haarojen rakentamisen jälkeen palauta kohde koordinaatit takaisin
// pääreitin viimeisimmille koordinaateille ja jatka reitin tekemistä.
kohtax = exkohtax;
kohtay = exkohtay;   
  


}

/* class viestiruutu FUNKTIOT
viestiruutu::LisaaRivi(char);  Lisää uuden rivin boxiin, poistaa ylimmän
viestiruutu::NaytaTeksti();  Näyttää ruudulla boxin kaikki 5 riviä tekstiä
*/

void viestiruutu::LisaaRivi(char rivi[25])
{ // Lisää viestiruutuun uuden rivin, poistaa vanhimman
// NOTE: Tämä komento oli minullekin uusi. Borlandin C++
// (ainakaan se versio mitä itse käytän) ei sisällä string muuttujatyyppiä
// itsessään, tai en ainakaan löytänyt sitä.
// Sen sijaan string.h sisältää tapoja käsitellä char muuttujia
// (esim. rivi2 = rivi3 ei toimi, täytyy käyttää strcpy (string copy)
// strcpy(rivi1, rivi2) tarkoittaa siis rivi1 = rivi2.

strcpy(rivi1, rivi2);
strcpy(rivi2, rivi3);
strcpy(rivi3, rivi4);
strcpy(rivi4, rivi5);
strcpy(rivi5, rivi);
}
void viestiruutu::NaytaTeksti()
{ // Tulostaa alaoikeaan reunaan ruutua kaikki 5 riviä viestiä
gotoxy(MAXX + 6, 17);
cout << rivi1;
gotoxy(MAXX + 6, 18);
cout << rivi2;
gotoxy(MAXX + 6, 19);
cout << rivi3;
gotoxy(MAXX + 6, 20);
cout << rivi4;
gotoxy(MAXX + 6, 21);
cout << rivi5;

}

void Ohje()
{
// Void Ohje(), näyttää pikaohjeen peliin.

clrscr();
cout << "************************* T U N N E L   R U N N E R ***************************";
cout << endl;
cout << "Tervetuloa Tunnel Runneriin. Tämä pikaohje opastaa sinut peliin." << endl;
cout << "Pelissä on 10 tasoa josta jokaisesta sinun tulee selvitä läpi," << endl;
cout << "matkallasi kohtaat aarteita ja vaaroja." << endl << endl;
cout << "Peli on toteutettu merkkigrafiikalla joka tarkoittaa että jokainen" << endl;
cout << "merkki ruudulla vastaa tiettyä objektia. Merkkien selitykset:" << endl;
cout << "  " << PELAAJA << " - Pelaaja          " << SEINA << " - Seinämä       " << LATTIA << " - Lattia" << endl;
cout << "  " << HIRVIO << " - Luurankosotilas  " << POMMI << " - Pommi         " << AARRE << " - Aarre" << endl; 
cout << "  " << KARTTA << " - Kertakäyttökartta" << endl << endl;
cout << "Tasolla sinulla on kaksi päämäärää: Ehtiä ulosmenoon ajoissa ja kerätä aarteet." << endl;
cout << "Aarteen löytäminen antaa sinulle " << AARTEENARVO << " pistettä," << endl;
cout << "selvitettyäsi radan saat pisteitä ajasta ja tasosta millä olit." << endl;
cout << "  Matkallasi kohtaat luurankosotilaita. Nämä ovat laiskoja ja tyhmiä otuksia" << endl;
cout << "mutta mikäli he saavat sinut kiinni he varmastikin pätkivät sinut huonoon" << endl;
cout << "kuntoon. Tästä toipuminen vie päiväsaikaan sinulta vuoroja (20), " << endl;
cout << "yöaikaan (kun vuorosi ovat ylittäneet maksimi vuoro rajan) he tappavat" << endl;
cout << "sinut. TÄMÄ ON AINUT TAPA HÄVITÄ PELISSÄ ENNEN AIKOJASI." << endl;
cout << "  Sinua vastaan tulee myös kahdenlaisia erikoisesineitä:" << endl;
cout << "  " << POMMI << " - Pommi. Tämän avulla voit räjäyttää seinän pako tai oikoreitiksi" << endl;
cout << "  " << KARTTA << " - Kertakäyttökartta. Tämä kartta näyttää tason pohjapiirron mutta kestää" << endl;
cout << "                     vain yhden katsauksen, kunnes se palaa savuna ilmaan." << endl << endl;
cout << "*************************(paina jotain näppäintä)*****************************";
getch();
clrscr();
cout << "************************* T U N N E L   R U N N E R ***************************";
cout << endl;
cout << "NÄPPÄIMET" << endl << endl;
cout << "Hahmoasi ohjaat w,a,s,d näppäimillä ikään kuin ne olisivat nuolinäppäimet." << endl;
cout << "  w     w - ylös    d - oikealle" << endl;
cout << "a   d   a - alas    s - vasemmalle" << endl;
cout << "  s"   << endl << endl;
cout << "Muita käytössäsi olevia näppäimiä ovat:" << endl << endl;
cout << "r - Räjäytä. Käytä pommi räjäyttääksesi seinän vierestäsi." << endl;
cout << "? - Näyttää pelin aikana tämän ohjeen." << endl;
cout << "Q - (huom. caps) Quit, poistuu pelistä" << endl << endl;
cout << "Muista että välillä joudut pelissä harkitsemaan esineiden ja varman ulospääsyn" << endl;
cout << "välillä. Kun vuoroja on kulunut 100, aurinko pakenee taivaanrantaan ja yö" << endl;
cout << "ottaa vallan sokkelossa. Sinulle annetaan vielä viimeinen epätoivoinen" << endl;
cout << "yritys koettaa ehtiä ulospääsyyn luurankoarmeijan juostessa perässäsi." << endl;
cout << "Pidä siis aina kiirettä! ;>" << endl << endl << endl;
cout << "Mukavia hetkiä Tunnel Runnerin parissa :D" << endl << endl;
cout << "                                                   Jeremias 'Jouzer' Kosonen" << endl;
cout << "*************************(paina jotain näppäintä)*****************************";


getch();
}



void NaytaViesti(char rivi1[45], char rivi2[45], int special) {
// void NaytaViesti(rivi1, rivi2, rivi3)
// Tulostaa keskelle näyttöä max 2 rivisen viestin,
// ja odottaa että käyttäjä painaa jotain näppäintä

gotoxy(1,10);
cout << "           ***************************************************" << endl;
cout << "           *                                                 *" << endl;
cout << "           *                                                 *" << endl;
cout << "           *                                                 *" << endl;
cout << "           *                                                 *" << endl;
cout << "           **********(Paina mitä tahansa näppäintä)***********";

gotoxy(15,12); cout << rivi1;
gotoxy(15,13); cout << rivi2;
if(special == 1) {
	cout << level; }
if(special == 2) {
	cout << vuoroja; }
if(special == 3) {
gotoxy(42,12); cout << MAXVUOROJA - vuoroja << " x 10pt = +" << (MAXVUOROJA - vuoroja) * 10 << "pt!" ;
gotoxy(34,13); cout << level << " x 1000pt = +" << level * 1000 << "pt!";
}
if(special == 4) {
gotoxy(42,12); cout << pisteet << " pt";
gotoxy(42,13); cout << level;

}



getch();
}

void PiirraKentta() {
// void PiirraKentta()
// Lähinnä debug tarkoitukseeen luotu funktio joka piirtää ruudulle koko kentän.
// Sittemmin toiminta siirtynyt kartta esineen käyttöön. :P

clrscr();
x = 0, y = 0;

	for(y = 0; y < MAXY; y++)
	{
		for(x = 0; x < MAXX; x++)
		{
		gotoxy(x,y);
		if(kxy[x][y] == 0)
			cout << TYHJA;
		else if (kxy[x][y] == 1)
			cout << REUNA;
		else if (kxy[x][y] == 2)
			cout << SEINA;
		else if (kxy[x][y] == 3)
                	cout << LATTIA;
		else if (kxy[x][y] == 4)
			cout << SISAANK;
		else if (kxy[x][y] == 5)
			cout << ULOSK;
		else if (kxy[x][y] == 6)
			cout << LATTIA;  // Näissä kolmessa lattia ettei kartta esine paljastaisi
		else if (kxy[x][y] == 7) // muiden esineiden sijaintia.
			cout << LATTIA;
		else if (kxy[x][y] == 8)
			cout << LATTIA;

		else cout << "Nyt kävi näin! (outo karttakoodi numero)";
		}
        }
getch();
}

// EOF