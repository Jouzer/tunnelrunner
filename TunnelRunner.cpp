/*  T U N N E L  R U N N E R (c) Jeremias Kosonen 2007

  Testattu toimivaksi Borland C++ 3.1 k��nt�j�ll�.

Ohjelma kirjoitettu valmiiksi 20:05 21.5. 2007
Highscore taulukot j�iv�t puuttumaan ajanpuutteesta ja siit�,
ett� ne tehd��n Borlandin k�ytt�j�ss� kovin erilailla.

L�hdekoodi sis�lt�� lukuisia funktioita joiden toiminta on
kommentoitu funktioiden alkuun. Lis�ksi on kaksi uutta luokkaa,
hirvio ja viestiruutu. Ohjelma on kauttaaltaan melko hyvin kommentoitu,
mutta koodi on paikoin sekavaa. Jos tekisin oikeaa peli�, voitaisiin
sanoa ett� pelin ensimm�inen versio on valmis, painaa delete� ja
aloittaa uusiksi. On selv�� ett� 1500 rivi� voisi tiivist�� ainakin
tuhanteen, ehk� viiteen sataan, ja samalla keveytt�� koodia. Ajan-
puuttessa en t�t� tee mutta hioin kyll� kaikki kohdat ja pyrin k�ytt�m��n
mahdollisimman kevyit� ja nopeita ratkaisuja ohjelmassani.
	Toisinaan tuli kohtia jotka olivat minulle t�ysin uusia, mm. hirvi�iden
teko�ly osio, ja se n�kyy koodissa. :D Ohjelma kuitenkin toimii kauttaaltaan
enk� ole t�rm�nnyt mink��nlaisiin bugeihin j�lkitestauksessa.
	Muutamia sinulle ehk� outoja komentoja olen joutunut k�ytt�m��n,
mm. gotoxy jonka toiminnan selvitin jo ja strcpy jonka toiminnan kommentoin
my�hemmin.
	Lopuksi tein pelist� my�s v�rillisen version koska sit� on
mukavampi pelata. En kuitenkaan laittanut "lopulliseen" versioon
komentoja jotka k�sittelee v�rej� koska ne sotkevat koodia ja joudun
vaihtamaan siin� syssyss� mm. cout >> komennon cprintf() komentoon,
tein sen ihan vaan huvikseen. Muuten kummatkin .exe:t ovat t�ysin samankaltaisia.
	Eli laitoin v�rillisen version mukaan my�s, jos kiinnostaa, on sit� mukavampi
pelaa ja testailla.

*/


#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define MAXX 40 // Luolan maksimikoko m��ritykset
#define MAXY 24


#define NAKYVYYS 3 // Arvoksi kelpaa kahdella jaollinen luku (2,4,6) ja ykk�nen.

#define MARGINAALI 1


// Karttamerkit
#define TYHJA '�'
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

// Muut m��ritykset
#define POMMEJAKENTASSA 2
#define AARTEITAKENTASSA 3
#define KARTTOJAKENTASSA 1
#define MAXHIRVIOITA 20
#define MAXVUOROJA 100

#define AARTEENARVO 500

int suddendeath = 0; // Ns. �kkikuolema, tila joka l�htee k�yntiin kun vuorot ylitt�v�t maxvuoroja

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

// Hirvi�n class olio alustus
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

// alaoikeassa peliss� n�kyv�n viestiruudun luokan alustus
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
        	NaytaViesti("              Peli lopetetaan", "     Kiitos ett� pelasit Tunnel Runneria!",0);
		lopeta = 1;
                break;

}
} while(lopeta == 0);


	return 0;

}



void PelaaPeli()
{
// void PelaaPeli()
// Alustaa kartan pelaamista varten ja k�ynnist�� silmukan pelin suorittamiseen
// Tyhjent�� muuttujat ja varastoi niihin tarpeelliset tiedot

NaytaViesti("Huom! Mik�li k�yt�t Windows k�ytt�j�rjestelm��","on suositeltavaa ett� pelaat peli� kokoruudun",0);
NaytaViesti("tilassa. Paina alt+enter menn�ksesi kokoruudun","tilaan. Ikkunassa peli toimii hitaasti.",0);

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

kmaxX = 19 + level * 2; // HUOM n�iden on oltava v�hintaan 1 pienempi kuin MAXX ja MAXY
kmaxY = 12 + level;     // JA HUOM kmaxX aina PARITON ja kmaxY AINA PARILLINEN!!!
	                // maximi kmaxX = 39 kmaxY = 22
luku = kmaxY % 2;       
if (luku == 1)          // Koska Y on oltava parillinen, t�m� koodi tarkistaa ett�
	kmaxY--;        // jos se on pariton se poistaa siit� yhden tehden siit� parillisen.


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
	NaytaViesti("   Lopullinen pistem��r�: ","          P��sit tasolle: ",4); }

if(lopeta == 4) {
		level = 10;
        NaytaViesti("         !!! ONNEKSI OLKOON !!!", " Olet selviytyj�! Rikas el�m� odottaa!",0);
	NaytaViesti("   Lopullinen pistem��r�: ","          L�p�isit tason ",4); }


// lopeta = 1 :: Lopeta peli kokonaan
// lopeta = 2 :: Pelaaja on p��ssyt uudelle tasolle,
// lopeta = 3 :: Pelaaja on kuollut
// lopeta = 4 :: Pelaaja selvisi elossa loppuun saakka
// Palaa valikkoon...
}

void OtaKomento()                  
{
int voiko;
komento = ' ';

// Ota k�ytt�j�lt� komento komento muuttujaan ja toimi sen mukaan
komento = getch();

switch(komento) {
// �l� p��st� hahmoa kulkemaan seiniss�, reunoissa, "tyhj�ss�"
// Jos pelaaja liikkui / yritti liikkua, v�henn� vuoroja
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
// Liikuttaa hirvi�it�, jos pelaaja osuu esineeseen aktivoi esineen teht�v�n
// (lis�� dynamiitin mukaan, lis�� pisteita jos aarre, n�ytt�� kartan jos tutkalaite)
// Jos pelaajan vieree osuu hirvi�, poistaa pelaajalta vuoroja ja poistaa hirvi�n
// Jos pelaaja on maalipisteess� siirt�� pelaajan seuraavalle tasolle
// Lopuksi v�hent�� pelaajalta yhden vuoron.

switch(kxy[pljx + vkulmax][pljy + vkulmay]) {  
	case 5: // Pelaaja on saapunut radan ulosk�ynnille (siirtyy seuraavalle tasolle)

		
		if(level <= 10){
		NaytaViesti("               Onneksi olkoon!", "    Selvisit elossa ja ajoissa tasosta ", 1);
		NaytaViesti("               ", "              Vuoroja kului:", 2);
                NaytaViesti("J�ljelle j��neit� vuoroja: ", "        Tasobonus:",3);
                pisteet = pisteet + ((MAXVUOROJA - vuoroja) * 10);
                pisteet = pisteet + (level * 1000);
		level++;
		lopeta = 2;

		if(level == 11)
        	lopeta = 4;

		}
		break;
	case 6: // Pelaaja l�ysi pommin
        	Viestit.LisaaRivi("L�ysit pommin!");
		kxy[pljx + vkulmax][pljy + vkulmay] = 3;
        	pommeja++;
        	break;
	case 7: // Pelaaja l�ysi aarteen
		Viestit.LisaaRivi("L�ysit aarteen! +500 pt");
        	kxy[pljx + vkulmax][pljy + vkulmay] = 3;
            pisteet = pisteet + AARTEENARVO;
        	break;
        case 8: // Pelaaja l�ysi kartan
		Viestit.LisaaRivi("L�ysit kartan!");
		Viestit.LisaaRivi(" Paina n�pp�int�.");
		Viestit.LisaaRivi("");
		PiirraRuutu();
		getch();
                kxy[pljx + vkulmax][pljy + vkulmay] = 3;
        	PiirraKentta();
		Viestit.LisaaRivi("Kartta paloi poroksi");
        	break;
        }

// Hirvi�t: jos vuoroja on j�ljell� (se on olemassa) liikuta hirvi�t�
// jos hirvi� on huomattu, v�henn� silt� yksi vuoro.
// jos hirvi�n vuorot laskevat nollaan, hirvi� ei en�� ole olemassa
// joten silt� on turha laskea vuoroja en��.
for(luku = 0; luku < hirvioita; luku++) {  
	if(luuranko[luku].vuorojajaljella > 0)
		luuranko[luku].Liikuta();
	if(luuranko[luku].x == pljx && luuranko[luku].y == pljy && suddendeath == 1) {
        	NaytaViesti("    Luurankosotilaat saivat sinut kiinni","              ...Game over...",0);
		lopeta = 3; }
	if(luuranko[luku].x == pljx && luuranko[luku].y == pljy && suddendeath == 0) {
        	NaytaViesti("    Luurankosotilas pahoinpiteli sinut!!","   Joudut ker��m��n itsesi (-20 vuoroa)",0);
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

// Jos vuoroja ylitt�v�t MAXVUOROJA, aktivoi sudden death
if(vuoroja > MAXVUOROJA) // SUDDEN DEATH!! Luo level * 2 luurankoa karttaan jotka osuessaan TAPPAVAT
                         // pelaajan.
{
if(suddendeath == 0) {
suddendeath = 1;

hirvioita = hirvioita + level * 2;
SijoitaKartalle(2);

NaytaViesti("Liian my�h�ist�! Aurinko laskee taivaanrantaan", "    ja luurangot nousevat haudoistaan!",0);
NaytaViesti("      Yrit� viel� p��st� ulosp��syyn,"," t�ll� kertaa luurangot eiv�t tyydy v�h��n..",0);
}

}
}

void KaytaPommi()
{
char suunta; int voiko;
// void KaytaPommi()
// Jos pelaajalla on pommi, k�ytt�� sen. Pelaajan valittua suunta
// ohjelma tarkistaa ett� valitussa kohdassa on sein�(3) tms ja jos on
// muuttaa sen lattiaksi (1) sek� v�hent�� pelaajalta pommin.
// V�ltt��kseen koodin liiallista kasvamista, ohjelma k�ytt��
// funktiota onkovapaana ja mik�li funktio palautta arvon ei voi liikkua
// muuttaa sen lattiaksi. T�m� aiheuttaa ongelman siin� ett� kentt�rajoja
// ja "tyhj��" voi r�j�ytt�� mutta s��st�� koodin pituutta.

if(pommeja > 0) {
	NaytaViesti("        K�ytit pommin!", "          Valitse suunta (w,a,s,d)",0);
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

if(voiko == 0) {  // Jos r�j�ytys onnistui, n�yt� viesti
	    Viestit.LisaaRivi("R�j�ytit sein�n");
	    Viestit.LisaaRivi(" kappaleiksi!");}
	else Viestit.LisaaRivi("T�ht�� sein��n.");
}

        
}


void SijoitaKartalle(int vaihe)
{
// void SijoitaKartalle()
// Sijoita kartalle luurankosotilaat, esineet ja pelaaja.
// (Alusta kartta ja peli vaiheeseen 1)

// Itemit voi tallentaa kxy[][] muuttujaan sill� aina itemin nostaessa = sen poistuessa
// tilalle voi laittaa lattian ilman kummia kyselemett�. 
// funktiota muutettu niin ett� vaihe m��r�� funktion teht�v��; aloitetaanko peli 0
// tilanteesta vai spawnataako sata luurankosotilasta


if(vaihe == 1) {
pljx = 0; pljy = 1; // Sijoita pelaaja sis��nk�yntiin

// Alusta ja sijoita hirvi�t sen mukaan paljon niit� halutaan karttaan (hirvioita)
for(luku = 0; luku < hirvioita; luku++) {
luuranko[luku].ArvoPaikka();
luuranko[luku].vuorojajaljella = 15;
}
// T�h�n tulee sitten itemien sijoittaminen.
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
// Piirt�� ruudulle hahmon, NAKYVYYS verran ruutuja kartasta sen ymp�rille,
// oikeaan kulmaan tietopalkit ja yl�reunaan pelin nimen
clrscr();
PiirraYmparys();
PiirraHahmot();
PiirraPalkit();

}

void PiirraHahmot()
{
// Piirr� ukkeli
gotoxy(vkulmax + pljx + MARGINAALI, vkulmay + pljy + MARGINAALI);
cout << PELAAJA;

// Jos hirvi�it� n�k�kent�ss�, piirr�
for(luku = 0; luku < hirvioita; luku++) {
if(luuranko[luku].nakokentassa == 1)
	luuranko[luku].Piirra();

}


}

void PiirraPalkit()
{
gotoxy(2,1);
cout << "_*____,.-'�'-.,_,.- T U N N e L   R U N N e R -.,_,.-'�'-.,_____*_";

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
cout << "'?' - N�yt� ohje";
gotoxy(MAXX + 5, 15);
cout << "-------------------------";

Viestit.NaytaTeksti();
}


void PiirraYmparys()
{
int x2 = 0, y2 = 0;

x = 0; y = 0;

// Ohjelma aloittaa piirt�misen -2, -2 koordinaateista suhteessa
// pelaan koordinaatteihin. Tai jos n�kyvyytt� muutetaan,
// numero muuttuu kuinka kaukaa pelaajaa aloitetaan piirt�minen..

for(y = 0 -(NAKYVYYS); y < (NAKYVYYS + 1); y++)
	{
	for(x = 0 -(NAKYVYYS ); x < (NAKYVYYS + 1); x++)
	{
        //Sijoita kursori kohtaan mist� tarkistetaan
	gotoxy(pljx + x + vkulmax + MARGINAALI, pljy + y + vkulmay + MARGINAALI);

		

		x2 = pljx + vkulmax + x; // Laske koordinaatit valmiiksi mist� etsi� kxy- muuttujasta
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

		// Tarkasta onko n�k�kent�ss� hirvi�t�
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
// �l� p��st� hahmoa kulkemaan seiniss�, reunoissa, "tyhj�ss�"

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

case 4:  // Yl�s
	if (kxy[x + vkulmax][y + vkulmay - 1] != 0
	 && kxy[x + vkulmax][y + vkulmay - 1] != 1
	 && kxy[x + vkulmax][y + vkulmay - 1] != 2)
		onko = 1;
	break;

}
return onko;
}


/* class Hirvio KAIKKI FUNKTIOT
hirvio::ArvoPaikka() Arpoo hirvi�lle paikan (hirvi�n alustamisfunktio)
hirvio::VoikoLiikkua(suunta) Tarkistaa voiko hirvi� liikkua haluttuun/arvottuun suuntaan
hirvio::Liikuta() Keino�ly osuus, laskee parhaimman suunnan hirvi�lle ja yritt�� liikuttaa hirvi�t�
hirvio::Piirra() Piirt�� hirvi�n kent�lle
hirvio::Tapa() Poistaa hirvi�n kent�lt�
*/

void hirvio::ArvoPaikka()
{
// Arpoo hirvi�lle paikan kartalla
do {
hirvio::x = rand()%kmaxX;
hirvio::y = rand()%kmaxY;
} while(kxy[hirvio::x + vkulmax][hirvio::y + vkulmay] != 3);
}


int hirvio::VoikoLiikkua(int suunta)
{
// Hirvi�n hieman erilailla toimiva voiko t�h�n liikkua funktio..
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
int suunta2; // V�liaikaismuuttuja reitin suunnan p��tt�miseen
int luku1, luku2; // ^---- sama
int x2, y2;
// Liikuttaa hirvi�it� joko satunnaisesti kartalla tai pelaajan suuntaan
// mik�li pelaaja ja hirvi� ovat huomanneet toisensa.
// Liikuta satunnaisesti

if(hirvio::onkohuomannut == 0) {
do {
	suunta = ArvoSuunta(2);
 // Arvo liikkumisen suunta (funktio alunperin tunnelin suunnalle
voiko = hirvio::VoikoLiikkua(suunta);
} while(voiko == 0);
}

// Jos hirvio ja pelaaja on kohdannut, se l�htee kulkemaan kohti pelaajaa "suorinta tiet�"
// Mik�li jokin suunta on tukittu, se yritt�� toista tiet� mik� vie hirvi�t� l�hemm�ksi.
// (seuraa pitk� p�tk� teksti�) jea keino�lyn riemuvoitto :D
if(hirvio::onkohuomannut == 1) { 
// Valitse suunta koordinaattien perusteella
x2 = hirvio::x - pljx; // Hirvi�n ja pelaajan koordinaattien erotus
y2 = hirvio::y - pljy; // (kertoo hirvi�n paikan suhteessa pelaajaan)
luku1 = 0; luku2 = 0; // V�liaikais muuttujia
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
if(y2 > 0){ // suuntana yl�s (4)
   luku2 = y2;
   suunta2 = suunta2 + 4; }
if(x2 == 0 || y2 == 0) { // Jos jompikumpi luvuista on nolla, hirvio haluaa kulkea suoraan
	suunta = suunta2; // ei siis tarvetta laskea toista haluttua suuntaa ja siksi case 123 = abort
	suunta2 = 123; }

// Vertaa luku1 ja luku2 saadaksesi tiet�� kumpi kahdesta paremmasta suunnasta
// kannattaa ottaa (pidempi, lyhyent�m�ll� tasaisesti kumpaakin on "nopein" reitti)
// (ei ota huomioon esteit�... mutta luurankosotilaathan on tunnetusti tyhmi� ;))
// Siin� tilanteesa ett� ne ovat yht�pitki�, luuranko yritt�� kumpaakin, mik�li kumpikaan
// ei toimi, arpoo h�n itsest��n jonkun muun.
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
	case 6: // oikea(2) + yl�s(4)
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
	case 7: // vasen(3) + yl�s(4)
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

if(voiko == 0) // Jos ei voinut liikkua ensiksi haluttuun suuntaan, yrit� viel� toiseen
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
	case 6: // oikea(2) + yl�s(4)
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
	case 7: // vasen(3) + yl�s(4)
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
// Tarkastaa onko hirvi� pelaajan n�k�kent�ss� ja piirt�� jos on
// Erillinen funktio k�yt�nn�ss� tarpeeton mutta olemassa sit� varten
// ett� ohjelmoinnin kannalta monipuolisempi ohjelma. (olio-ohjelmointia)


gotoxy(hirvio::x + vkulmax + MARGINAALI, hirvio::y + vkulmay + MARGINAALI);
cout << HIRVIO;

hirvio::onkohuomannut = 1;
hirvio::nakokentassa = 0; // T�m� nollataan koska jostain syyst� funktiossa joka tarkastaa
			  // pelaajan n�k�kent�n (PiirraYmparys();) if - else rakenne ei toimi
			  // niin ett� if nakokentassa, = 1, else = 0. O_o k�y se n�inki yht� hyvi.
                          // (jos lukua ei nollattaisi hirvi� n�kyisi ruudulla loputtomiin)
}

void hirvio::Tapa()
{
Viestit.LisaaRivi("Luurankosotilas");
Viestit.LisaaRivi(" l�hti pois.");

hirvio::x = 0; hirvio::y = 0;
hirvio::onkohuomannut = 0;
hirvio::vuorojajaljella = 0;
}


/* ============================================================
 K A R T A N   L U O M I S E N   F U N K T I O T   J A
 D E B U G I T   T � S T �   E T E E N P � I N
 ==============================================================
 */

void TeeKentta()
{  // Funktio: TeeKentta(), luo arvotusti tunneliverkoston x ja y 2d taulukkopohjalle
   //
   // K�ytetyjen muuttujien selitykset:
   // vkulmax, vkulmay: vasen yl�kulma x, vasen yl�kulma y. kartan kohittamista
   //    ja kohtien laskemista (t�m�n ansiosta itse taso voi olla keskell� karttaa..)
   // suoria, haaroja: kun ohjelma luo suoria ja haaroja se laskee ne yl�s n�ihin muuttujiin
   //    ja lopuksi katsoo ett� kun kartta on muuten valmis ja toimiva, se on tarpeeksi
   //    t�ynn� suoria n�iden avulla.

int onnistuiko = 0;

do {
x = 0; y = 0;
kohtax = 0; kohtay = 0;
suoria = 0;
haaroja = 0;

//T�yt� kartta tyhj�ll� jossa ei voi kulkea
	for(y = 0; y < MAXY; y++)
   	{
		for(x = 0; x < MAXX; x++)
		{
			kxy[x][y] = 0;
		}
	}

// Luo mahd. keskelle karttaa reunukset kartalle
// annettujen kartan tietojen mukaan (kmaxX, kmaxY)
x = 0; y = 0; vkulmax = 0; vkulmay = 0;  // tyhjenn� muuttujat

// vasen yl� reunan koordinaatit x = (MAXX / 2) - (kmaxX / 2), y = (MAXY / 2) - (kmaxY / 2) jne.
// N�it� tarvitaan paljon! (kartan koon ja sijainnin suhteutukset koordinaatistoon)
vkulmax = MAXX / 2 - kmaxX / 2;
vkulmay = MAXY / 2 - kmaxY / 2;

for(x = 0; x < kmaxX; x++) // YL� JA ALAREUNA
{
	kxy[x + vkulmax][vkulmay] = 1;
	kxy[x + vkulmax][kmaxY + vkulmay] = 1;
}
for(y = 0; y < kmaxY; y++) // VASEN JA OIKEAREUNA
{
	kxy[vkulmax][y + vkulmay] = 1;
	kxy[vkulmax + kmaxX - 1][y + vkulmay] = 1;
}


// T�yt� reunojen sis�ll� oleva tila sein�ll�
for(y =1; y < kmaxY; y++) {
	for(x = 1; x < kmaxX - 1; x++)
	  kxy[x + vkulmax][y + vkulmay] = 2;
        }

 // Luo sis��n- ja ulosk�ynti luolastoon
 kxy[vkulmax][vkulmay + 1] = 4;
 kxy[kmaxX + vkulmax - 1][kmaxY + vkulmay - 1] = 5;


 // Luo kartta uusiksi niin monta kertaa
 // Jos LuoTunnelit palauttaa 1, kartta on valmis ja toimii.
 // ett� kartta "onnistuu" (reitti ylt�� ulosp��syyn tukkiutumatta
 // ja kartassa on tarpeeksi tunnelia ollakseen haastava.

onnistuiko = LuoTunnelit();  
} while(onnistuiko == 0 || onnistuiko == 2);    

}



int LuoTunnelit()
{
// Funktio: LuoTunnelit(), luo karttapohjaan randomisti tunnelit
// yksi kerrallaa ja luo haaroja jotka h�m��v�t pelaajaa vaikeuttaen
// loppuun p��sy�.

kohtax = 1; kohtay = 1;

int suunta;
int arpa;
int onnistuiko = 0;
int maxyrityksia = level * 80;
int yrityksia = 0;

/* ::::::::: ITSE RANDOM TUNNELIEN LUOJA :::::::
Toiminta-idea selkokielell� on ett� ohjelma rakentaa
tunneleita p��asiallisesti yl� vasemmalta alaoikeaan suuntaan.
Jokaisen suoran luotuaan ohjelma yritt�� 3 kertaa satunnaisesti luoda haaran
suoralle tai jo onnistuneelle haaralle. T�m�n j�lkeen se palaa suoran
rakentamiseen. Mik�li tunnelien rakentamiseen kuluu liian kauan
(kartta on aivan t�ynn� pikku tunneleita tai se on rakentanut itsens�
umpeen) se palaa ylemp��n funktioon ja aloittaa alusta kartan rakentamisen.
*/


 x = kohtax; y = kohtay; // x- ja y- ovat v�liaikaisia muuttujia, kohtax ja kohtay
                         // merkkaa kohtaa jossa rakennus tapahtuu
 do {
 maxpituus = 0;
 suunta = ArvoSuunta(1);  // Arvo suunta, 1 alas 2 oikea 3 vasem 4 yl�s


// Tarkasta oliko reitti mahdollissa rakentaa
VoikoRakentaa(suunta);

// Jos oli, rakenna reitti ja yrit� rakentaa haaroja.
if(maxpituus > 0){
   RakennaReitti(suunta);
   RakennaHaara();
   suoria++;
   }




//Jos reitti ylt�� loppuun, lopeta kartan teko sill� se on valmis.
//Varmista my�s ett� haaroja ja reittej� on tarpeeksi jotta kartta on haastava.
//(vaatimukset kasvaa levelin kasvaessa.)

if (kohtax == kmaxX - 2 && kohtay == kmaxY - 1) {  
   if(suoria + haaroja > level * 4 + (kmaxX + kmaxY) / 2)
	onnistuiko = 1;
	kxy[kohtax + vkulmax][kohtay + vkulmay] = 3;

	 }

if(yrityksia >= maxyrityksia) {    // Jos reitti� ollaan yritetty rakentaa jo liian kauan (yrityksia)
	                           // ohjelma luovuttaa ja alkaa rakentaa uutta, koska t�m� tarkoittaa
	onnistuiko = 2;		   // ett� kartta on liian t�ynn� jo tai se on rakentanut itsens� umpeen.
	}

yrityksia++;
} while (onnistuiko == 0 ); // Jatka kunnes reitit ovat kaikki sulkeutuneet. (k�yt�nn�ss� kun ohjelma itse
                            // k�skee tietyin ehtoin sulkea ne..)
return onnistuiko;  // Palauta ylemm�lle funktiolle tieto, onnistuiko rakentaminen.

}

int ArvoSuunta(int tapa) {
 int suunta;
 int arpa;


 // Arvo numero joka m��r�� suunnan
 // 1-3 = alas (1), 4-6 = oikealle (2), 7-8 vasem (3), 9-10 yl�s (4)

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

 // Haaroilla ei ole painotusta mihink��n suuntaan, siksi oma arvontarutiininsa
 // tapa == 2. 1-2 alas (1) 3-4 oikea (2) 5-6 vasem (3) 7-8 yl�s (4)

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

 return suunta; // Palauta ylemm�lle funktiolle arvottu suunta

}


void VoikoRakentaa(int suunta) {
                            
// Funktio VoikoRakentaa(int suunta)
// Tarkistaa voiko arvottuun suuntaan rakentaa tunnelia
// (onko sein��; toisen tyhj�n tunnelin p��lle tai rajan yli
// ei rakenneta...)
// Sijoittaa tuloksen maxpituus muuttujaan.
// HUOM! T�m� funktio ei rakenna tunnelia vaan tutkii maxpituuden
// siksi kohtax ja kohtay ei muutu vaan k�ytet��n v�liaikaisia x ja y.

int eipysty = 0;
x = kohtax; y = kohtay;

 if (kxy[vkulmax + x][vkulmay + y] == 2 || kxy[vkulmax + x][vkulmay + y] == 3)  // Varmista ett� kohdassa k�ytett�v�� sein��
     { // Tarkista kuinka pitk�lle sein�� voi jatkaa

     maxpituus = 0;
     do {
		switch(suunta) {  // Suunnan mukaan, tarkista kuinka kauan suuntaan voi jatkaa
                                  // ennen kuin sein�tila loppuu. (vastassa lattia, raja tai exit)
		case 1: {         // HUOM ohjelma tarkistaa joka toisen ruudun koska pelin tulee rakentaa vain yhden levyisi� k�yt�vi�
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

// Reitin pituuden m��ritt�miseksi ohjelma arpoo luvun jonka max = maxpituus / 2
// T�m� jaetaan kahdella ja siihen lis�t��n 1. Reitin minimipituus on siis 2
// ja maximipituus on reitin (teoreettinen maximipituus / 2) / 3 + 2


arpa = rand()%maxpituus / 2;
pituus =  arpa / 2 + 1;
if(pituus < maxpituus)
	pituus++;

luku = pituus % 2;
if(luku == 1)
       pituus--;	

		x = 0; y = 0;  // v�liaikaismuuttujia tarvitaan taas lausekkeen lopettamiseen
			       // kohtay:t� ei voi k�ytt�� koska se voi olla jo suurempi kuin pituus
			       // alunperin. x ja y muuttujat h�m��v�t hiukan muttei ole j�rke�
                               // tehd� uusia muuttujia t�t� varten (vie muistia ja homma on selv� muutenkin)

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
// haaraa suoralle. T�m�n j�lkeen se jatkaa suoran tekemist�.
// exkohtax & exkohtay = talletuspaikka p��suoran viimeisimmille koordinateille

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

// Haarojen rakentamisen j�lkeen palauta kohde koordinaatit takaisin
// p��reitin viimeisimmille koordinaateille ja jatka reitin tekemist�.
kohtax = exkohtax;
kohtay = exkohtay;   
  


}

/* class viestiruutu FUNKTIOT
viestiruutu::LisaaRivi(char);  Lis�� uuden rivin boxiin, poistaa ylimm�n
viestiruutu::NaytaTeksti();  N�ytt�� ruudulla boxin kaikki 5 rivi� teksti�
*/

void viestiruutu::LisaaRivi(char rivi[25])
{ // Lis�� viestiruutuun uuden rivin, poistaa vanhimman
// NOTE: T�m� komento oli minullekin uusi. Borlandin C++
// (ainakaan se versio mit� itse k�yt�n) ei sis�ll� string muuttujatyyppi�
// itsess��n, tai en ainakaan l�yt�nyt sit�.
// Sen sijaan string.h sis�lt�� tapoja k�sitell� char muuttujia
// (esim. rivi2 = rivi3 ei toimi, t�ytyy k�ytt�� strcpy (string copy)
// strcpy(rivi1, rivi2) tarkoittaa siis rivi1 = rivi2.

strcpy(rivi1, rivi2);
strcpy(rivi2, rivi3);
strcpy(rivi3, rivi4);
strcpy(rivi4, rivi5);
strcpy(rivi5, rivi);
}
void viestiruutu::NaytaTeksti()
{ // Tulostaa alaoikeaan reunaan ruutua kaikki 5 rivi� viesti�
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
// Void Ohje(), n�ytt�� pikaohjeen peliin.

clrscr();
cout << "************************* T U N N E L   R U N N E R ***************************";
cout << endl;
cout << "Tervetuloa Tunnel Runneriin. T�m� pikaohje opastaa sinut peliin." << endl;
cout << "Peliss� on 10 tasoa josta jokaisesta sinun tulee selvit� l�pi," << endl;
cout << "matkallasi kohtaat aarteita ja vaaroja." << endl << endl;
cout << "Peli on toteutettu merkkigrafiikalla joka tarkoittaa ett� jokainen" << endl;
cout << "merkki ruudulla vastaa tietty� objektia. Merkkien selitykset:" << endl;
cout << "  " << PELAAJA << " - Pelaaja          " << SEINA << " - Sein�m�       " << LATTIA << " - Lattia" << endl;
cout << "  " << HIRVIO << " - Luurankosotilas  " << POMMI << " - Pommi         " << AARRE << " - Aarre" << endl; 
cout << "  " << KARTTA << " - Kertak�ytt�kartta" << endl << endl;
cout << "Tasolla sinulla on kaksi p��m��r��: Ehti� ulosmenoon ajoissa ja ker�t� aarteet." << endl;
cout << "Aarteen l�yt�minen antaa sinulle " << AARTEENARVO << " pistett�," << endl;
cout << "selvitetty�si radan saat pisteit� ajasta ja tasosta mill� olit." << endl;
cout << "  Matkallasi kohtaat luurankosotilaita. N�m� ovat laiskoja ja tyhmi� otuksia" << endl;
cout << "mutta mik�li he saavat sinut kiinni he varmastikin p�tkiv�t sinut huonoon" << endl;
cout << "kuntoon. T�st� toipuminen vie p�iv�saikaan sinulta vuoroja (20), " << endl;
cout << "y�aikaan (kun vuorosi ovat ylitt�neet maksimi vuoro rajan) he tappavat" << endl;
cout << "sinut. T�M� ON AINUT TAPA H�VIT� PELISS� ENNEN AIKOJASI." << endl;
cout << "  Sinua vastaan tulee my�s kahdenlaisia erikoisesineit�:" << endl;
cout << "  " << POMMI << " - Pommi. T�m�n avulla voit r�j�ytt�� sein�n pako tai oikoreitiksi" << endl;
cout << "  " << KARTTA << " - Kertak�ytt�kartta. T�m� kartta n�ytt�� tason pohjapiirron mutta kest��" << endl;
cout << "                     vain yhden katsauksen, kunnes se palaa savuna ilmaan." << endl << endl;
cout << "*************************(paina jotain n�pp�int�)*****************************";
getch();
clrscr();
cout << "************************* T U N N E L   R U N N E R ***************************";
cout << endl;
cout << "N�PP�IMET" << endl << endl;
cout << "Hahmoasi ohjaat w,a,s,d n�pp�imill� ik��n kuin ne olisivat nuolin�pp�imet." << endl;
cout << "  w     w - yl�s    d - oikealle" << endl;
cout << "a   d   a - alas    s - vasemmalle" << endl;
cout << "  s"   << endl << endl;
cout << "Muita k�yt�ss�si olevia n�pp�imi� ovat:" << endl << endl;
cout << "r - R�j�yt�. K�yt� pommi r�j�ytt��ksesi sein�n vierest�si." << endl;
cout << "? - N�ytt�� pelin aikana t�m�n ohjeen." << endl;
cout << "Q - (huom. caps) Quit, poistuu pelist�" << endl << endl;
cout << "Muista ett� v�lill� joudut peliss� harkitsemaan esineiden ja varman ulosp��syn" << endl;
cout << "v�lill�. Kun vuoroja on kulunut 100, aurinko pakenee taivaanrantaan ja y�" << endl;
cout << "ottaa vallan sokkelossa. Sinulle annetaan viel� viimeinen ep�toivoinen" << endl;
cout << "yritys koettaa ehti� ulosp��syyn luurankoarmeijan juostessa per�ss�si." << endl;
cout << "Pid� siis aina kiirett�! ;>" << endl << endl << endl;
cout << "Mukavia hetki� Tunnel Runnerin parissa :D" << endl << endl;
cout << "                                                   Jeremias 'Jouzer' Kosonen" << endl;
cout << "*************************(paina jotain n�pp�int�)*****************************";


getch();
}



void NaytaViesti(char rivi1[45], char rivi2[45], int special) {
// void NaytaViesti(rivi1, rivi2, rivi3)
// Tulostaa keskelle n�ytt�� max 2 rivisen viestin,
// ja odottaa ett� k�ytt�j� painaa jotain n�pp�int�

gotoxy(1,10);
cout << "           ***************************************************" << endl;
cout << "           *                                                 *" << endl;
cout << "           *                                                 *" << endl;
cout << "           *                                                 *" << endl;
cout << "           *                                                 *" << endl;
cout << "           **********(Paina mit� tahansa n�pp�int�)***********";

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
// L�hinn� debug tarkoitukseeen luotu funktio joka piirt�� ruudulle koko kent�n.
// Sittemmin toiminta siirtynyt kartta esineen k�ytt��n. :P

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
			cout << LATTIA;  // N�iss� kolmessa lattia ettei kartta esine paljastaisi
		else if (kxy[x][y] == 7) // muiden esineiden sijaintia.
			cout << LATTIA;
		else if (kxy[x][y] == 8)
			cout << LATTIA;

		else cout << "Nyt k�vi n�in! (outo karttakoodi numero)";
		}
        }
getch();
}

// EOF