
#ifndef DS1307_H
#define DSL1307_h

#include <Arduino.h>
#include <Wire.h>
#include <time.h>
//#include <string.h>


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
    uint8_t seconds = 0;     /**!< Secondes 00 - 59 */
    uint8_t minutes = 0;     /**!< Minutes 00 - 59 */
    uint8_t hours = 0;       /**!< Heures 00 - 23 (format 24h), 01 - 12 (format 12h) */
    uint8_t is_pm = 0;       /**!< Vaut 1 si l'heure est en format 12h et qu'il est l'aprés midi, sinon 0 */
    uint8_t day_of_week = 0; /**!< Jour de la semaine 01 - 07, 1 = lundi, 2 = mardi, etc.  */
    uint8_t days = 0;        /**!< Jours 01 - 31 */
    uint8_t months = 0;      /**!< Mois 01 - 12 */
    uint8_t year = 0;        /**!< Année au format yy (exemple : 16 = 2016) */
} DateTime_t;

/** Structure contenant les informations de date et heure en provenance ou à destination du module RTC */
typedef struct
{
    String seconds = "";     /**!< Secondes 00 - 59 */
    String minutes = "";     /**!< Minutes 00 - 59 */
    String hours = "";       /**!< Heures 00 - 23 (format 24h), 01 - 12 (format 12h) */
    String is_pm = "";       /**!< Vaut 1 si l'heure est en format 12h et qu'il est l'aprés midi, sinon 0 */
    String day_of_week = ""; /**!< Jour de la semaine 01 - 07, 1 = lundi, 2 = mardi, etc.  */
    String days = "";        /**!< Jours 01 - 31 */
    String months = "";      /**!< Mois 01 - 12 */
    String year = "";        /**!< Année au format yy (exemple : 16 = 2016) */
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
byte bcd_to_decimal(byte bcd);


/** Fonction de conversion decimal -> BCD */
byte decimal_to_bcd(byte decimal);

/** 
 * Fonction récupérant l'heure et la date courante à partir du module RTC.
 * Place les valeurs lues dans la structure passée en argument (par pointeur).
 * N.B. Retourne 1 si le module RTC est arrêté (plus de batterie, horloge arrêtée manuellement, etc.), 0 le reste du temps.
 */
DateTime_t read_current(void);

/** remet la valeur des registres à 0 */
void ResetTime(void);

/** Parametre le DS1307 à la date de la compilation */
void ProgrammeDateCompilation(void);

/** obtien la date et l'heure de la compilation */
DateTimeCompilation_t GetTimeAndDateCompilation(void);

void SousChaine(char* chaineCible, char* chaineSource, int posDebut, int longueur);


#endif