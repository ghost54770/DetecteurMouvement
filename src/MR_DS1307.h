
#ifndef DS1307_H
#define DSL1307_h

#include <Arduino.h>
#include <Wire.h>

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

    return dateActuelle;
}

#endif