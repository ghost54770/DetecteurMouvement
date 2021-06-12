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
//#include "../.pio\libdeps\uno\LiquidCrystal_I2C\LiquidCrystal_I2C.h"

//#include "C:\Users\Mathieu\Documents\Projet Arduino\Afficheur I2C\include\math.h"
#define CLK 19
#define SDA 18
#define CAPTEUR_SORTIE 2
#define SPI_CS_PIN 10

int adressePFC85741;
int trame;
int compteurMouvement = 0;
int byteEntrant = 0;
int compteur = 0;

int i2c(void);
void pulse(void);
void envoieTrameLCD(int);

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
  //Initialisation
  adressePFC85741 = i2c();
}

void loop()
{
  Serial.println(adressePFC85741);
  Wire.beginTransmission(adressePFC85741);
  LiquidCrystal_I2C ecran(adressePFC85741, 16, 2);
  ecran.init();
  ecran.begin(16, 1, 0);
  ecran.backlight();
  ecran.clear();
  ecran.blink();
  ecran.display();

  while (1)
  {

    if (digitalRead(CAPTEUR_SORTIE) == 1)
    {
      compteurMouvement++;
      //Affichage ecran
      char contenuTexte[16];
      sprintf(contenuTexte, "Compteur : %d", compteurMouvement);
      ecran.clear();
      ecran.printstr(contenuTexte);
      Wire.endTransmission();
      _delay_ms(1300); //Durée de l'impulsion de detection

      //Ecriture sur la carte SD
      if (SD.begin(SPI_CS_PIN) == true)
      {
        SDLib::File fichier;
        fichier = SD.open("log.txt", FILE_WRITE);
        fichier.println("salut");
        fichier.close();
      }
      else
      {
        Serial.print("Erreur de la fonction SD.begin");
      }
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

void envoieTrameLCD(int trameEntre)
{
  //ConvertionTrame(trameEntre);
  //Serial.println(bit7Mod + bit6Mod + bit5Mod + bit4Mod + bit3Mod + bit2Mod + bit1Mod + bit0Mod,BIN);

  //byte trameSortie[8];
  //trameSortie[0] = trameEntre[7]; //E
  //trameSortie[1] = trameEntre[6]; //RS
  //trameSortie[2] = trameEntre[5]; //RW
  //trameSortie[3] = trameEntre[4]; //D7
  //trameSortie[4] = trameEntre[3]; //D6
  //trameSortie[5] = trameEntre[2]; //D5
  //trameSortie[6] = trameEntre[1];  //D4
  //trameSortie[7] = trameEntre[0]; //D0

  /*                4  (P0)	-->  4 (RS)
                   5  (P1)	-->  5 (RW)
                   6  (P2)	-->  6 (E)
                   7  (P3)	-->  NC
                   9  (P4)	-->  11 (D4)
                   10 (P5)	-->  12 (D5)
                   11 (p6)	-->  13 (D6)
                   12 (p7)	-->  14 (D7)*/
}
