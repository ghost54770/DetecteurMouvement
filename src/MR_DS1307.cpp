
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
    dateActuelle.seconds = Wire.read();     // bcd_to_decimal(Wire.read());
    dateActuelle.minutes = Wire.read();     // bcd_to_decimal(Wire.read());
    dateActuelle.hours = Wire.read();       // bcd_to_decimal(Wire.read());
    dateActuelle.day_of_week = Wire.read(); // bcd_to_decimal(Wire.read());
    dateActuelle.days = Wire.read();        // bcd_to_decimal(Wire.read());
    dateActuelle.months = Wire.read();      // bcd_to_decimal(Wire.read());
    dateActuelle.year = Wire.read();        // bcd_to_decimal(Wire.read());

    Serial.begin(9600, SERIAL_8N1);
              Serial.print("");
          Serial.println("--------mouvement detectee------");
    Serial.println("--------read_current------");
    Serial.print("seconde : ");
    Serial.println(dateActuelle.seconds);
    Serial.print("minutes : ");
    Serial.println(dateActuelle.minutes);
    Serial.print("heures : ");
    Serial.println(dateActuelle.hours);
    Serial.print("jour semaine : ");
    Serial.println(dateActuelle.day_of_week);
    Serial.print("jour : ");
    Serial.println(dateActuelle.days);
    Serial.print("mois : ");
    Serial.println(dateActuelle.months);
    Serial.print("annee : ");
    Serial.println(dateActuelle.year);
    Serial.print("prepros date : ");
    Serial.println(__DATE__);
    Serial.print("prepros time : ");
    Serial.println(__TIME__);

    return dateActuelle;
}

void ResetTime()
{
    Serial.print("Adresse DS1307 : ");
    Serial.println(DS1307_ADDRESS);

    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0);                 // Ce positionne a l'adresse 0
    Wire.write(0b1000000);         // secondes (0 à 59)
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
    DateTimeCompilation_t DateTime;
    DateTime = GetTimeAndDateCompilation();

    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0);                                        // Ce positionne a l'adresse 0
    Wire.write(decimal_to_bcd(DateTime.seconds.toInt())); // secondes (0 à 59)
    Wire.write(0b01001111);                               //decimal_to_bcd(DateTime.minutes.toInt())      // minutes (0 à 59)
    Wire.write(decimal_to_bcd(DateTime.hours.toInt()));   // heures     10101
    Wire.write(decimal_to_bcd(0));                        // numero du jours de la semaines
    Wire.write(decimal_to_bcd(DateTime.days.toInt()));    // numero du jours
    Wire.write(decimal_to_bcd(DateTime.months.toInt()));  // numero du mois
    Wire.write(decimal_to_bcd(DateTime.year.toInt()));    // numero de l'année
    Wire.endTransmission();

   Serial.println("");
   Serial.print("1 secondes int apres : ");
   Serial.println(DateTime.seconds);
   Serial.print("2 minutes int apres : ");
   Serial.println(DateTime.minutes);
   Serial.print("3 heure int apres : ");
   Serial.println(DateTime.hours);
   Serial.print("4 n jour int apres : ");
   Serial.println(DateTime.day_of_week);
   Serial.print("5 jour int apres : ");
   Serial.println(DateTime.days);
   Serial.print("6 mois int apres : ");
   Serial.println(DateTime.months);
   Serial.print("7 annee int apres : ");
   Serial.println(DateTime.year);
    /*  Serial.print("minutes int : ");
    Serial.println(DateTime.minutes);
    Serial.print("heures int : ");
    Serial.println(DateTime.hours);   */
}

DateTimeCompilation_t GetTimeAndDateCompilation()
{
    String heureCompilation = __TIME__;
    char dateCompilation[] = __DATE__;
    Serial.print("DATE DE LA COMPILATION : ");
    Serial.println(dateCompilation);
    DateTimeCompilation_t dateHeureCompilation;
    /*
    String month = dateCompilation.substring(0, 3);
    if (month == "Jan")
        dateHeureCompilation.months = "01";
    else if (month == "Feb")
        dateHeureCompilation.months = "02";
    else if (month == "Mar")
        dateHeureCompilation.months = "03";
    else if (month == "Apr")
        dateHeureCompilation.months = "04";
    else if (month == "May")
        dateHeureCompilation.months = "05";
    else if (month == "Jun")
        dateHeureCompilation.months = "06";
    else if (month == "Jul")
        dateHeureCompilation.months = "07";
    else if (month == "Aug")
        dateHeureCompilation.months = "08";
    else if (month == "Sep")
        dateHeureCompilation.months = "09";
    else if (month == "Oct")
        dateHeureCompilation.months = "10";
    else if (month == "Nov")
        dateHeureCompilation.months = "11";
    else if (month == "Dec")
        dateHeureCompilation.months = "12";

*/

    //Serial.println(dateHeureCompilation.months);

    //  const char *str2 = "test1";

    //Serial.print("*****");
    // Serial.print(str1);
    //Serial.println("*****");

    // const char *str1 = dateCompilation.substring(4, 6).c_str();
    char secondes[3];
    char minutes[3];
    char heures[3];
    char jours[3];
    char mois[3];
    char annee[5];

    SousChaine(annee, dateCompilation, 7, 2);
    /*
    strcpy(secondes, dateCompilation.substring(6, 8).c_str());
    strcpy(minutes, dateCompilation.substring(3, 5).c_str());
    strcpy(heures, dateCompilation.substring(0, 2).c_str());
    strcpy(jours, dateCompilation.substring(4, 6).c_str());
    strcpy(mois, dateCompilation.substring(0, 2).c_str());
    strcpy(annee, dateCompilation.substring(0, 2).c_str());

    dateHeureCompilation.seconds = heureCompilation.substring(6, 8);
    dateHeureCompilation.minutes = heureCompilation.substring(3, 5);
    dateHeureCompilation.hours = heureCompilation.substring(0, 2);
    dateHeureCompilation.days = dateCompilation.substring(4, 6);
    dateHeureCompilation.months = dateCompilation.substring(0, 2);
    dateHeureCompilation.year = dateCompilation.substring(7, 11);
*/

    //       char arr[dateCompilation.length() + 1];
    //   String test = "salut";
    //   strcpy(arr, test.c_str());

  //  Serial.print("secondes int avant : ");
  //  Serial.println(annee);

    return dateHeureCompilation;
}

void SousChaine(char chaineCible[], char chaineSource[], int posDebut, int longueur)
{
    int y = 0;
    Serial.print("longueur egale a : ");
    int size = strlen(chaineSource);
    Serial.println(size);

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

#endif