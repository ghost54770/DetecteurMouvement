#include <Arduino.h>
#include <Wire.h>
#include <string.h>
#include <stdio.h>
#include <SPI.h>
#include <../.pio/libdeps/uno/SD/src/SD.h>
#include "../.pio/libdeps/uno/LiquidCrystal_I2C/LiquidCrystal_I2C.h"
#include "MR_DS1307.h"
#include <time.h>

//*********************************************************************************************************************************************************
//----------------------------------------------DECLARATIONS------------------------------------------------------------------------------------------------------
//**********************************************************************************************************************************************************
#define SPI_CS_PIN 10 //SPI Slave PIN, for write on SD CARD


//Déclaration des variables____________________________________________________________________
const char version[4] = "1.3"; //Version du soft
int compteurMouvement = 0; // Incrementation a chaque passage
char debutDate[11];        // Tableau utilisé pour le changement de jour
int volatile short flag_interruptMouvement = 0;   // Obligé que cette variable soi volatile sinon la condition de fonctionne pas
int presenceSD = 0;                               // 0 = pas de carte SD     1 = presence carte SD
LiquidCrystal_I2C ecran(ADRESSE_PCF85741, 16, 2); // Instance de l'ecran LCD
DateTime_t dateActuelMouvement;                   //Structure contenant les informations du passage actuel
//____________________________________________________________________Déclaration des variables

//Déclaration des prototypes___________________________________________________________________
int i2c(void);
void pulse(void);
void envoieTrameLCD(int);
byte bcd_to_decimal(byte);
byte decimal_to_bcd(byte);
void MouvementDetecte(void);
void InterruptionMouvement(void);
//___________________________________________________________________Déclaration des prototypes

//*********************************************************************************************************************************************************
//----------------------------------------------SETUP------------------------------------------------------------------------------------------------------
//**********************************************************************************************************************************************************
void setup()
{
  //Configuration des pins___________________________________________________________________
  pinMode(LED_DEBUG, OUTPUT);
  pinMode(CLK, INPUT);
  pinMode(SDA, INPUT);
  pinMode(CAPTEUR_SORTIE, INPUT);
  pinMode(SPI_CS_PIN, OUTPUT);
  pinMode(INIT_DS1307, INPUT);
  //___________________________________________________________________Configuration des pins

  //Configuration de port SERIE______________________________________________________________
  Serial.begin(9600, SERIAL_8N1);
  while (!Serial)
    //______________________________________________________________Configuration de port SERIE

    //Initialisation I2C_______________________________________________________________________
    Wire.setClock(100000);
  Wire.begin();
  //_______________________________________________________________________Initialisation I2C

  Serial.println("");
  Serial.println("");
  Serial.println(F("////////////////////////////////////////////////////////////////////"));
  Serial.println(F("----------------------DEBUT DU PROGRAMME----------------------------"));
  Serial.println(F("////////////////////////////////////////////////////////////////////"));

  char MessageSerial[30];
  sprintf(MessageSerial, "Adresse PCF85741 : %d", ADRESSE_PCF85741);
  Serial.println(MessageSerial);

  //Initialiastion de l'ecran LCD via le PCF85741____________________________________________
  ecran.init();
  ecran.begin(16, 1, 0);
  ecran.backlight();
  ecran.display();
  //____________________________________________Initialiastion de l'ecran LCD via le PCF85741

  _delay_ms(500);

  /* Initialise le DS1307________________________________________________________________________________________________________________________________________________
  *   Initialise le DS1307 avec la date d'aujourd'huie (en utilisant la date de compilation lors du transfert du programme vers l'arduino UNO).
  *   Il est necessaire de mettre le pin INIT_DS1307 a 1 pendant la transfert du programme vers l'arduino, puis de remettre le pin a 0 avant un redemarrage de l'arduino*/
  if (digitalRead(INIT_DS1307) == 1)
  {
    ProgrammeDateCompilation();
  }
  //_______________________________________________________________________________________________________________________________________________Fin Initialise le DS1307

  attachInterrupt(digitalPinToInterrupt(3), InterruptionMouvement, RISING); //attribution de l'interruption au capteur de mouvement (déclancehement quand front montant)
}

//*********************************************************************************************************************************************************
//----------------------------------------------LOOP------------------------------------------------------------------------------------------------------
//**********************************************************************************************************************************************************
void loop()
{
  //1er ligne
  char introduction[16];
  ecran.printstr("Detect mouvement");
  // 2eme ligne
  ecran.setCursor(0, 1);
  sprintf(introduction, "ghost54770 %s", version);
  
  ecran.printstr(introduction);
  _delay_ms(3000);
  while (1)
  {
    if (flag_interruptMouvement == 1)
    {
  Serial.println("1");

      MouvementDetecte();
      attachInterrupt(digitalPinToInterrupt(3), InterruptionMouvement, RISING);
    }
  }
}
//*********************************************************************************************************************************************************
//----------------------------------------------FONCTIONS------------------------------------------------------------------------------------------------------
//**********************************************************************************************************************************************************

void InterruptionMouvement()
{
  detachInterrupt(digitalPinToInterrupt(3));
  flag_interruptMouvement = 1;
}

void MouvementDetecte()
{
  _delay_ms(10);
  dateActuelMouvement = read_current();
  _delay_ms(10);

  char debutDate2[11] = "";
  char jour[3] = "";
  char mois[3] = "";

  //---------Formattage du texte, ajoute 0 avant le jour ou le mois si ceux si sont inferieur à 10----------------------//
  if (dateActuelMouvement.days < 10 && dateActuelMouvement.months < 10)                                                 //
  {                                                                                                                     //
    sprintf(debutDate2, "0%d/0%d/%d ", dateActuelMouvement.days, dateActuelMouvement.months, dateActuelMouvement.year); //
  }                                                                                                                     //
  else if (dateActuelMouvement.days < 10 && dateActuelMouvement.months > 9)                                             //
  {                                                                                                                     //
    sprintf(debutDate2, "0%d/%d/%d ", dateActuelMouvement.days, dateActuelMouvement.months, dateActuelMouvement.year);  //
  }                                                                                                                     //
  else if (dateActuelMouvement.days > 9 && dateActuelMouvement.months < 10)                                             //
  {                                                                                                                     //
    sprintf(debutDate2, "%d/0%d/%d ", dateActuelMouvement.days, dateActuelMouvement.months, dateActuelMouvement.year);  //
  }                                                                                                                     //
  else if (dateActuelMouvement.days > 9 && dateActuelMouvement.months > 9)                                              //
  {                                                                                                                     //
    sprintf(debutDate2, "%d/%d/%d ", dateActuelMouvement.days, dateActuelMouvement.months, dateActuelMouvement.year);   //
  }                                                                                                                     //
  //--------------------------------------------------------------------------------------------------------------------//

  if (SD.begin(SPI_CS_PIN) == true)
  {
    presenceSD = 1;
    SDLib::File fichier;
    fichier = SD.open("log.txt", FILE_WRITE);
    //--------nouveau jour si necessaire---------------//
    if (strcmp(debutDate, debutDate2) != 0)       //
    {                                             //
      fichier.print("=====================  ");   //
      fichier.print(debutDate2);                  //
      fichier.println("  ====================="); //
      strcpy(debutDate, debutDate2);              //
      compteurMouvement = 0;                      //
    }                                             //
    //-------------------------------------------------//

    compteurMouvement++;
    fichier.print(dateActuelMouvement.hours);
    fichier.print("h ");
    fichier.print(dateActuelMouvement.minutes);
    fichier.print("m ");
    fichier.print(dateActuelMouvement.seconds);
    fichier.print("s -> ");
    fichier.println(compteurMouvement);
    fichier.close();
  }
  else
  {
    presenceSD = 0;
    compteurMouvement++;
    _delay_ms(20);
    Serial.println("Erreur de la fonction SD.begin");
    _delay_ms(100);
  }

  //************************************************************

  //************Affichage ecran****************************
  char contenuTexte[16];
  ecran.clear();
  // 1er ligne
  ecran.setCursor(0, 0);
  if (presenceSD == 0)
  {
    sprintf(contenuTexte, "%d/%d/20%d", dateActuelMouvement.days, dateActuelMouvement.months, dateActuelMouvement.year);
  }
  else
  {
    sprintf(contenuTexte, "%d/%d/20%d   SD", dateActuelMouvement.days, dateActuelMouvement.months, dateActuelMouvement.year);
  }
  ecran.printstr(contenuTexte);
  // 2eme ligne
  ecran.setCursor(0, 1);
  sprintf(contenuTexte, "Passage : %d", compteurMouvement);
  ecran.printstr(contenuTexte);

  _delay_ms(1300); //Durée de l'impulsion de detection du capteur de mouvement
  //********************************************************
  flag_interruptMouvement = 0;
}