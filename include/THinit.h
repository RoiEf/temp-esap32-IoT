#ifndef __THinit__
#define __THinit__


#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <EEPROM.h>         // include library to read and write from flash memory

#define EEPROM_SIZE 256       // define the number of bytes you want to access
#define eepromStart 0
#define eepromPadding 3
#define ssidLength 32
#define wifiPassLength 63
#define longitudeLength 4
#define latitudeLength 4
#define iotIdLength 4
#define nameLength 15
#define descriptionLength 50

const int wifiPassStart = eepromStart + ssidLength + eepromPadding + 1;
const int longitudeStart = wifiPassStart + wifiPassLength + eepromPadding;
const int latitudeStart = longitudeStart + 4;
const int iotIdStart = latitudeStart + 5;
const int nameStart = iotIdStart + 5;
const int descriptionStart = nameStart + nameLength + eepromPadding;

class THinit {
    char wifi_ssid[ssidLength] = "";
    char wifi_password[wifiPassLength] = "";
    float  longitude = 0;
    float  latitude = 0;
    int iotId = 0;
    char iotName[nameLength] = "";
    char iotDescription[descriptionLength] = "";

    int readEepromString(int, int, char *);
    int writeEepromString(int, int, char *);
    void strCpy(char*, char*, int);

public:
    int init(void);
    int resetEEPROM(void);

    int getSSID(char *);
    int getWifiPassword(char *);
    int getLongitude(float *);
    int getLatitude(float *);
    int getIotId(int *);
    int getName(char *);
    int getDescription(char *);

    int setSSID(char *, int);
    int setWifiPassword(char *, int);
    int setLongitude(float);
    int setLatitude(float);
    int setIotId(int);
    int setName(char *, int);
    int setDescription(char *, int);

};

#endif