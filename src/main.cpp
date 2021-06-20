/*
Afficheur LCD avec microcontrolleur HD44780U
I2C -> parrallele : PCF85741
                   PCF85741      LCD
                   4  (P0)	-->  4 (RS)
                   5  (P1)	-->  5 (RW)
                   6  (P2)	-->  6 (E)
                   7  (P3)	-->  NC
                   9  (P4)	-->  11 (D4)
                   10 (P5)	-->  12 (D5)
                   11 (p6)	-->  13 (D6)
                   12 (p7)	-->  14 (D7)
Arduino uno
*/
#include <Arduino.h>
#include <Wire.h>
#include <string.h>
#include <stdio.h>
#include <SPI.h>
#include <../.pio/libdeps/uno/SD/src/SD.h>
#include "../.pio/libdeps/uno/LiquidCrystal_I2C/LiquidCrystal_I2C.h"
#include "MR_DS1307.h"
//#include "../.pio\libdeps\uno\LiquidCrystal_I2C\LiquidCrystal_I2C.h"
//#include "C:\Users\Mathieu\Documents\Projet Arduino\Afficheur I2C\include\math.h"
#define CLK 19
#define SDA 18
#define CAPTEUR_SORTIE 2
#define SPI_CS_PIN 10
#define ADRESSE_PCF85741 33

int trame;
int compteurMouvement = 0;
int byteEntrant = 0;
int compteur = 0;

int i2c(void);
void pulse(void);
void envoieTrameLCD(int);
byte bcd_to_decimal(byte);
byte decimal_to_bcd(byte);

void setup()
{
  //Configuration des PIN
  pinMode(CLK, INPUT);
  pinMode(SDA, INPUT);
  pinMode(CAPTEUR_SORTIE, INPUT);
  pinMode(SPI_CS_PIN, OUTPUT);
  //Configuration de l'I2C

  //Configuration de port SERIE
  Serial.begin(9600, SERIAL_8N1);
  while (!Serial)
  {
  }
  //Initialisation I2C
  Wire.setClock(100000);
  Wire.begin();
}

void loop()
{
  char MessageSerial[99];
  sprintf(MessageSerial, "Adresse PCF85741 : %d", ADRESSE_PCF85741);
  Serial.println(MessageSerial);

  //----Initialiastion de l'ecran LCD via le PCF85741----
  LiquidCrystal_I2C ecran(ADRESSE_PCF85741, 16, 2);
  ecran.init();
  ecran.begin(16, 1, 0);
  ecran.backlight();
  ecran.clear();
  ecran.blink();
  ecran.display();
  //----------------------------------------------------

  //--------Test DS1307-----------------------
  _delay_ms(1000);

  Wire.beginTransmission(DS1307_ADDRESS); //envoie de l'adresse de l'esclave
  // Wire.write(0);
  Wire.write(DS1307_CTRL_REGISTER); //Adresse du registre CONTROL
  Wire.write(0x80);
  Wire.write(0x11);
  Wire.endTransmission();

  DateTime_t date;

  //-----------------------------------------

  while (1)
  { 
    if (digitalRead(CAPTEUR_SORTIE) == 1)
    {
      
      compteurMouvement++;
      date = read_current();

      //--------Ecriture sur la carte SD-------
      if (SD.begin(SPI_CS_PIN) == true)
      {
        SDLib::File fichier;
        fichier = SD.open("log.txt", FILE_WRITE);
        fichier.print(date.days);
        fichier.print("/");
        fichier.print(date.months);
        fichier.print("/");
        fichier.print(date.year);
        fichier.print(" ");
        fichier.print(date.hours);
        fichier.print("h");
        fichier.print(date.minutes);
        fichier.print("m");
        fichier.print(date.seconds);
        fichier.print("---> ");
        fichier.println(compteurMouvement);
       
        fichier.close();
      }
      else
      {
        Serial.print("Erreur de la fonction SD.begin");
      }
      //--------------------------------------

      //--------Affichage ecran---------------
      char contenuTexte[16];
      sprintf(contenuTexte, "Compteur : %d", compteurMouvement);
      ecran.clear();
      ecran.printstr(contenuTexte);
      _delay_ms(1300); //Durée de l'impulsion de detection
      //--------------------------------------
    }
  }
}

int i2c(void)
{
  //Activation de l'I2C
  Wire.setClock(100000);
  Wire.begin();
  //Recherche de l'adresse du PCF85741
  int adressePFC85741 = 0;
  bool appareilTrouve = false;
  while (adressePFC85741 != 128)
  {
    byte codeErreur;
    Wire.beginTransmission(adressePFC85741);
    codeErreur = Wire.endTransmission();
    if (codeErreur == 0)
    {
      Serial.write("Appareil trouve a l'adresse : ");
      Serial.println(adressePFC85741);
      appareilTrouve = true;
      break;
    }
    adressePFC85741++;
  }
  if (appareilTrouve == false)
  {
    //Appareil non trouvé
    Serial.println("Appareil non trouvee");
  }
  return adressePFC85741;
  //Appareil trouvé
}

//*****************************************************************************************
//***********FONCTIONS***FONCTIONS***FONCTIONS***FONCTIONS***FONCTIONS*********************
//*****************************************************************************************
