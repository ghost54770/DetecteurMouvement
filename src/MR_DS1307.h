
#ifndef DS1307_H
#define DSL1307_h

#include <Arduino.h>
#include <Wire.h>
#include <time.h>
#include <string.h>

/** Adresse I2C du module RTC DS1307 */
const uint8_t DS1307_ADDRESS = 104;

/** Adresse du registre de contrôle du module RTC DS1307 */
const uint8_t DS1307_CTRL_REGISTER = 0x07;

/** Adresse et taille de la NVRAM du module RTC DS1307 */
const uint8_t DS1307_NVRAM_BASE = 0x08;
const uint8_t DS1307_NVRAM_SIZE = 56;

/** Structure contenant les informations de date et heure en provenance ou à destination du module RTC */
typedef struct
{
    uint8_t seconds;     /**!< Secondes 00 - 59 */
    uint8_t minutes;     /**!< Minutes 00 - 59 */
    uint8_t hours;       /**!< Heures 00 - 23 (format 24h), 01 - 12 (format 12h) */
    uint8_t is_pm;       /**!< Vaut 1 si l'heure est en format 12h et qu'il est l'aprés midi, sinon 0 */
    uint8_t day_of_week; /**!< Jour de la semaine 01 - 07, 1 = lundi, 2 = mardi, etc.  */
    uint8_t days;        /**!< Jours 01 - 31 */
    uint8_t months;      /**!< Mois 01 - 12 */
    uint8_t year;        /**!< Année au format yy (exemple : 16 = 2016) */
} DateTime_t;

/** Structure contenant les informations de date et heure en provenance ou à destination du module RTC */
typedef struct
{
    String seconds;     /**!< Secondes 00 - 59 */
    String minutes;     /**!< Minutes 00 - 59 */
    String hours;       /**!< Heures 00 - 23 (format 24h), 01 - 12 (format 12h) */
    String is_pm;       /**!< Vaut 1 si l'heure est en format 12h et qu'il est l'aprés midi, sinon 0 */
    String day_of_week; /**!< Jour de la semaine 01 - 07, 1 = lundi, 2 = mardi, etc.  */
    String days;        /**!< Jours 01 - 31 */
    String months;      /**!< Mois 01 - 12 */
    String year;        /**!< Année au format yy (exemple : 16 = 2016) */
} DateTimeCompilation_t;

/** Mode de fonctionnement pour la broche SQW */
typedef enum
{
    SQW_1_HZ = 0, /**!< Signal à 1Hz sur la broche SQW */
    SQW_4096_HZ,  /**!< Signal à 4096Hz sur la broche SQW */
    SQW_8192_HZ,  /**!< Signal à 8192Hz sur la broche SQW */
    SQW_32768_HZ, /**!< Signal à 32768Hz sur la broche SQW */
    SQW_DC        /**!< Broche SQW toujours à LOW ou HIGH */
} DS1307_Mode_t;

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
    dateActuelle.seconds = bcd_to_decimal(Wire.read());
    dateActuelle.minutes = bcd_to_decimal(Wire.read());
    dateActuelle.hours = bcd_to_decimal(Wire.read());
    dateActuelle.day_of_week = bcd_to_decimal(Wire.read());
    dateActuelle.days = bcd_to_decimal(Wire.read());
    dateActuelle.months = bcd_to_decimal(Wire.read());
    dateActuelle.year = bcd_to_decimal(Wire.read());

    Serial.begin(9600);
    Serial.println("-------------------------------");

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
    Serial.print("année : ");
    Serial.println(dateActuelle.year);
    Serial.print("prepros date : ");
    Serial.println(__LINE__);
    Serial.print("prepros time : ");
    Serial.println(__TIME__);

    return dateActuelle;
}

void ResetTime()
{
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0);                          // Ce positionne a l'adresse 0
    Wire.write(decimal_to_bcd(0));          // secondes (0 à 59)
    Wire.write(decimal_to_bcd(0)); // minutes (0 à 59)
    Wire.write(decimal_to_bcd(0)); // heures     10101
    Wire.write(decimal_to_bcd(0)); // numero du jours de la semaines
    Wire.write(decimal_to_bcd(0)); // numero du jours
    Wire.write(decimal_to_bcd(0)); // numero du mois
    Wire.write(decimal_to_bcd(0)); // numero de l'année
    Wire.endTransmission();
}

void ResetTime()
{
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0);                          // Ce positionne a l'adresse 0
    Wire.write(decimal_to_bcd(0));          // secondes (0 à 59)
    Wire.write(decimal_to_bcd(0)); // minutes (0 à 59)
    Wire.write(decimal_to_bcd(0)); // heures     10101
    Wire.write(decimal_to_bcd(0)); // numero du jours de la semaines
    Wire.write(decimal_to_bcd(0)); // numero du jours
    Wire.write(decimal_to_bcd(0)); // numero du mois
    Wire.write(decimal_to_bcd(0)); // numero de l'année
    Wire.endTransmission();
}

DateTimeCompilation_t GetTimeAndDateCompilation()
{
    String heureCompilation = __TIME__;
    String dateCompilation = __DATE__;
    DateTimeCompilation_t dateHeureCompilation;

    String month = dateCompilation.substring(0, 3);
    if (month == "Jan")
        dateHeureCompilation.months = 1;
    else if (month == "Feb")
        dateHeureCompilation.months = 2;
    else if (month == "Mar")
        dateHeureCompilation.months = 3;
    else if (month == "Apr")
        dateHeureCompilation.months = 4;
    else if (month == "May")
        dateHeureCompilation.months = 5;
    else if (month == "Jun")
        dateHeureCompilation.months = 6;
    else if (month == "Jul")
        dateHeureCompilation.months = 7;
    else if (month == "Aug")
        dateHeureCompilation.months = 8;
    else if (month == "Sep")
        dateHeureCompilation.months = 9;
    else if (month == "Oct")
        dateHeureCompilation.months = 10;
    else if (month == "Nov")
        dateHeureCompilation.months = 11;
    else if (month == "Dec")
        dateHeureCompilation.months = 12;

    dateHeureCompilation.days       = dateCompilation.substring(4,6);
    dateHeureCompilation.year       = dateCompilation.substring(7,11);
    dateHeureCompilation.hours      = heureCompilation.substring(0,2);
    dateHeureCompilation.minutes    = heureCompilation.substring(3,5);
    dateHeureCompilation.seconds    = heureCompilation.substring(6,8);

}

#endif