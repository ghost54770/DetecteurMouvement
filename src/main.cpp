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
#include <time.h>
//#include "../.pio\libdeps\uno\LiquidCrystal_I2C\LiquidCrystal_I2C.h"
//#include "C:\Users\Mathieu\Documents\Projet Arduino\Afficheur I2C\include\math.h"
#define CLK 19
#define SDA 18
#define CAPTEUR_SORTIE 3
#define SPI_CS_PIN 10
#define ADRESSE_PCF85741 33

int compteurMouvement = 0;
int byteEntrant = 0;
int compteur = 0;
char debutDate[11];

int i2c(void);
void pulse(void);
void envoieTrameLCD(int);
byte bcd_to_decimal(byte);
byte decimal_to_bcd(byte);

void MouvementDetecte(LiquidCrystal_I2C ecran, DateTime_t date);


void setup()
{
  /**Configuration des PIN*/
  pinMode(CLK, INPUT);
  pinMode(SDA, INPUT);
  pinMode(CAPTEUR_SORTIE, INPUT);
  pinMode(SPI_CS_PIN, OUTPUT);

  /**Configuration de port SERIE*/
  Serial.begin(9600, SERIAL_8N1);
  while (!Serial)

  /**Initialisation I2C*/
  Wire.setClock(100000);
  Wire.begin();

  /**Attribution des interruptions*/
  attachInterrupt(1,MouvementDetecte,RISING);

}

void loop()
{
  Serial.println("");
  Serial.println("");
  Serial.println("////////////////////////////////////////////////////////////////////");
  Serial.println("----------------------DEBUT DU PROGRAMME----------------------------");
  Serial.println("////////////////////////////////////////////////////////////////////");

  char MessageSerial[30];
  sprintf(MessageSerial, "Adresse PCF85741 : %d", ADRESSE_PCF85741);
 // Serial.println("");
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
  //--------Test DS1307---------------------------------
  _delay_ms(1000);

  DateTime_t date;
 // ProgrammeDateCompilation();
  ResetTime();
  //---------------------------------------------------

  while (1)
  {
        //MouvementDetecte(ecran,date);
  //  if (digitalRead(CAPTEUR_SORTIE) == 1)
  //  {

  //  }
  }
}

void MouvementDetecte(LiquidCrystal_I2C ecran, DateTime_t date){
    _delay_ms(100);

      Serial.println("--------mouvement detecte-----------");

      compteurMouvement++;
      date = read_current();

      //--------Ecriture sur la carte SD-------
      if (SD.begin(SPI_CS_PIN) == true)
      {
        SDLib::File fichier;
        fichier = SD.open("log.txt", FILE_WRITE);

        char debutDate2[11] = "";
        sprintf(debutDate2, "%s/%s/%s ", date.days, date.months, date.year);
        Serial.print("debutDate :");
        Serial.println(debutDate);
        Serial.print("debutDate2 :");
        Serial.println(debutDate2);

        if (debutDate != debutDate2)
        {
          fichier.print("=====================  ");
          fichier.print(date.days);
          fichier.print("/");
          fichier.print(date.months);
          fichier.print("/");
          fichier.print(date.year);
          fichier.println("  =====================");
          strcpy(debutDate, debutDate2);
        }

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
        Serial.println("Erreur de la fonction SD.begin");
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


