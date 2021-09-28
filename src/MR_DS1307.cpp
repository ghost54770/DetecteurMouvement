
#ifndef DS1307_H
#define DSL1307_h

#include <Arduino.h>
#include <Wire.h>
#include <time.h>
//#include <string.h>
#include "MR_DS1307.h"

/** Fonction de conversion BCD -> decimal */
byte bcd_to_decimal(byte bcd)
{
   return (bcd / 16 * 10) + (bcd % 16);
}

/** Fonction de conversion decimal -> BCD */
byte decimal_to_bcd(byte decimal)
{
   return (decimal / 10 * 16) + (decimal % 10);
}

/** 
 * Fonction récupérant l'heure et la date courante à partir du module RTC.
 * Place les valeurs lues dans la structure passée en argument (par pointeur).
 * N.B. Retourne 1 si le module RTC est arrêté (plus de batterie, horloge arrêtée manuellement, etc.), 0 le reste du temps.
 */
DateTime_t read_current()
{
   /*Debut de la transmission I2C*/
   Wire.beginTransmission(DS1307_ADDRESS);
   Wire.write((byte)0);    // Lecture mémoire à l'adresse 0x00
   Wire.endTransmission(); // Fin de la transmission i2c

   /* Lecture des 7 octets depuis la mémoire du DS1307 */
   Wire.requestFrom(DS1307_ADDRESS, (byte)7);
   DateTime_t dateActuelle;
   dateActuelle.seconds = bcd_to_decimal(Wire.read());     // bcd_to_decimal(Wire.read());
   dateActuelle.minutes = bcd_to_decimal(Wire.read());     // bcd_to_decimal(Wire.read());
   dateActuelle.hours = bcd_to_decimal(Wire.read());       // bcd_to_decimal(Wire.read());
   dateActuelle.day_of_week = bcd_to_decimal(Wire.read()); // bcd_to_decimal(Wire.read());
   dateActuelle.days = bcd_to_decimal(Wire.read());        // bcd_to_decimal(Wire.read());
   dateActuelle.months = bcd_to_decimal(Wire.read());      // bcd_to_decimal(Wire.read());
   dateActuelle.year = bcd_to_decimal(Wire.read());        // bcd_to_decimal(Wire.read());

   Serial.begin(9600, SERIAL_8N1);
   Serial.print("");
   Serial.println(F("--------mouvement detectee------"));
   Serial.print(F("seconde : "));
   Serial.println(dateActuelle.seconds);
   Serial.print(F("minutes : "));
   Serial.println(dateActuelle.minutes);
   Serial.print(F("heures : "));
   Serial.println(dateActuelle.hours);
   Serial.print(F("jour semaine : "));
   Serial.println(dateActuelle.day_of_week);
   Serial.print(F("jour : "));
   Serial.println(dateActuelle.days);
   Serial.print(F("mois : "));
   Serial.println(dateActuelle.months);
   Serial.print(F("annee : "));
   Serial.println(dateActuelle.year);

   //Serial.end();
   return dateActuelle;
}

void ResetTime(void)
{
   Serial.print(F("Adresse DS1307 : "));
   Serial.println(DS1307_ADDRESS);

   Wire.beginTransmission(DS1307_ADDRESS);
   Wire.write(0);                 // Ce positionne a l'adresse 0
   Wire.write(0);                 // secondes (0 à 59)
   Wire.write(decimal_to_bcd(0)); // minutes (0 à 59)
   Wire.write(decimal_to_bcd(0)); // heures     10101
   Wire.write(decimal_to_bcd(0)); // numero du jours de la semaines
   Wire.write(decimal_to_bcd(0)); // numero du jours
   Wire.write(decimal_to_bcd(0)); // numero du mois
   Wire.write(decimal_to_bcd(0)); // numero de l'année
   Wire.endTransmission();
}

void ProgrammeDateCompilation(void)
{
   Serial.begin(9600, SERIAL_8N1);

   Serial.print(F("Adresse DS1307 : "));
   _delay_ms(10);
   Serial.println(DS1307_ADDRESS);
   _delay_ms(10);
   Serial.print(F("prepros date : "));
   _delay_ms(10);
   Serial.println(__DATE__);
   _delay_ms(10);
   Serial.print(F("prepros time : "));
   _delay_ms(10);

   Serial.println(__TIME__);

   DateTimeCompilation_t DateTime;
   DateTime = GetTimeAndDateCompilation();

   char valeur[20] = "";

   Wire.beginTransmission(DS1307_ADDRESS);
   // Ce positionne a l'adresse 0
   Wire.write(0);
   // secondes (0 à 59)
   Wire.write(decimal_to_bcd(atoi(DateTime.seconds)));
   // minutes (0 à 59)
   Wire.write(decimal_to_bcd(atoi(DateTime.minutes)));
   // heures     10101
   Wire.write(decimal_to_bcd(atoi(DateTime.hours)));
   // numero du jours de la semaines
   Wire.write(decimal_to_bcd(0));
   // numero du jours
   Wire.write(decimal_to_bcd(atoi(DateTime.days)));
   // numero du mois
   Wire.write(decimal_to_bcd(atoi(DateTime.months)));
   // numero de l'année
   Wire.write(decimal_to_bcd(atoi(DateTime.year)));

   Wire.endTransmission();


   _delay_ms(10);
   Serial.print(F("1 secondes int apres : "));
   _delay_ms(10);
   Serial.println(DateTime.seconds);
   _delay_ms(10);
   Serial.print(F("2 minutes int apres : "));
   _delay_ms(10);
   Serial.println(DateTime.minutes);
   _delay_ms(10);
   Serial.print(F("3 heure int apres : "));
   _delay_ms(10);
   Serial.println(DateTime.hours);
   _delay_ms(10);
   Serial.print(F("4 jour int apres : "));
   _delay_ms(10);
   Serial.println(DateTime.day_of_week);
   _delay_ms(10);
   Serial.print(F("5 jour int apres : "));
   _delay_ms(10);
   Serial.println(DateTime.days);
   _delay_ms(10);
   Serial.print(F("6 mois int apres : "));
   _delay_ms(10);
   Serial.println(DateTime.months);
   _delay_ms(10);
   Serial.print(F("7 annee int apres : "));
   _delay_ms(10);
   Serial.println(DateTime.year);
   _delay_ms(10);

   Serial.end();
}

DateTimeCompilation_t GetTimeAndDateCompilation(void)
{
   char mois[4]     = "";
   char *numeroMois = "";

   char heureCompilation[9] = __TIME__;
   char dateCompilation[12] = __DATE__;

   DateTimeCompilation_t dateHeureCompilation;

   SousChaine(mois, dateCompilation, 0, 3);

   if (strcmp(mois, "Jun") == 0)
      numeroMois = "01";
   else if (strcmp(mois, "Feb") == 0)
      numeroMois = "02";
   else if (strcmp(mois, "Mar") == 0)
      numeroMois = "03";
   else if (strcmp(mois, "Apr") == 0)
      numeroMois = "04";
   else if (strcmp(mois, "May") == 0)
      numeroMois = "05";
   else if (strcmp(mois, "Jun") == 0)
      numeroMois = "06";
   else if (strcmp(mois, "Jul") == 0)
      numeroMois = "07";
   else if (strcmp(mois, "Aug") == 0)
      numeroMois = "08";
   else if (strcmp(mois, "Sep") == 0)
      numeroMois = "09";
   else if (strcmp(mois, "Oct") == 0)
      numeroMois = "10";
   else if (strcmp(mois, "Nov") == 0)
      numeroMois = "11";
   else if (strcmp(mois, "Dec") == 0)
      numeroMois = "12";

   SousChaine(dateHeureCompilation.seconds, heureCompilation, 6, 2);
   SousChaine(dateHeureCompilation.minutes, heureCompilation, 3, 2);
   SousChaine(dateHeureCompilation.hours  , heureCompilation, 0, 2);
   SousChaine(dateHeureCompilation.days   , dateCompilation , 4, 2);
   strcpy(dateHeureCompilation.months, numeroMois);
   SousChaine(dateHeureCompilation.year   , dateCompilation , 9, 2);

   return dateHeureCompilation;
}

void SousChaine(char chaineCible[], char chaineSource[], int posDebut, int longueur)
{
   int y = 0;

   for (int i = posDebut; i < longueur + posDebut; i++)
   {
      if (chaineSource[i] != '\0')
      {
         chaineCible[y] = chaineSource[i];
         y++;
      }
      else
      {
         break;
      }
   }
   chaineCible[y + 1] = '\0';
}

void DebugLed(void)
{
   _delay_ms(10);
   digitalWrite(LED_DEBUG, HIGH);
   _delay_ms(1000);
   digitalWrite(LED_DEBUG, LOW);
   _delay_ms(10);
}

void DecimalVersHexa(char chaineCible[], int decimal)
{
   char resultat[20] = "";
   int  reste        = 0;
   int  quotient     = 0;
   int  i            = 0;

   Serial.begin(9600, SERIAL_8N1);

   while (1)
   {
      if (decimal != 0)
      {
         quotient = decimal / 16;
         reste    = decimal % 16;
         decimal  = quotient;

         if (reste == 10)
            resultat[i] = 'A';
         else if (reste == 11)
            resultat[i] = 'B';
         else if (reste == 12)
            resultat[i] = 'C';
         else if (reste == 13)
            resultat[i] = 'D';
         else if (reste == 14)
            resultat[i] = 'E';
         else if (reste == 15)
            resultat[i] = 'F';
         else
            resultat[i] = reste + '0';

         _delay_ms(10);
         Serial.println(F("valeur : "));
         _delay_ms(10);
         Serial.println(resultat[i]);
         _delay_ms(10);
         i++;
      }
      else
      {
         break;
      }
   }

   //inverson des valeurs du tableau de char
   char resultatFinal[20];
   for (int y = 0; y < i; y++)
   {
      resultatFinal[y] = resultat[i - y - 1];
   }

   resultatFinal[strlen(resultatFinal) + 1] = '\0'; //ajout du caractere de fin au tableau resultatFinal
   strcpy(chaineCible, resultatFinal);
}

#endif