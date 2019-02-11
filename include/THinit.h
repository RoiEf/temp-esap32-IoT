#ifndef __THinit__
#define __THinit__


#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <EEPROM.h>         // include library to read and write from flash memory

#define EEPROM_SIZE 125       // define the number of bytes you want to access
#define eepromStart 0
#define eepromPadding 3
#define ssidLength 32
#define wifiPassLength 63
#define iotIdLength 1

class THinit {
    char wifi_ssid[ssidLength] = "";
    char wifi_password[wifiPassLength] = "";
    float  longitude = 0;
    float  latitude = 0;
    int iotId = 0;

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

    int setSSID(char *, int);
    int setWifiPassword(char *, int);
    int setLongitude(float);
    int setLatitude(float);
    int setIotId(int);

};

#endif