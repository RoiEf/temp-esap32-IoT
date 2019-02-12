#include <THinit.h>

int THinit::init(void){
    EEPROM.begin(EEPROM_SIZE);
    readEepromString(eepromStart, (ssidLength + eepromPadding), wifi_ssid);
    readEepromString((wifiPassStart), (wifiPassLength + eepromPadding), wifi_password);
    longitude = EEPROM.readFloat(longitudeStart);
    latitude = EEPROM.readFloat(latitudeStart);
    iotId = EEPROM.readInt(iotIdStart);
    readEepromString((nameStart), (nameLength + eepromPadding), iotName);
    readEepromString((descriptionStart), (descriptionLength + eepromPadding), iotDescription);

    return 0;
}

int THinit::resetEEPROM(void){
    for(int i = 0; i < EEPROM_SIZE; i++)
    {
        EEPROM.write(i, '\0');
    }
    EEPROM.commit();

    return 0;
}

int THinit::readEepromString(int start, int maxSize, char * arr){
    for(int i = 0; i < maxSize; i++)
    {
        arr[i] = EEPROM.read(start + i);
    }

    return 0;
}

int THinit::writeEepromString(int start, int datSize, char * arr){
    for(int i = 0; i < datSize; i++)
    {
        EEPROM.write(start + i, arr[i]);
        // EEPROM.put();
    }
    EEPROM.commit();

    return 0;
}

int THinit::getSSID(char * ssid){
    strCpy(ssid, wifi_ssid, ssidLength);

    return 0;
}

int THinit::getWifiPassword(char * wifiPass){
    strCpy(wifiPass, wifi_password, wifiPassLength);

    return 0;
}

int THinit::getLongitude(float * ret){
    *ret = longitude;
    
    return 0;
}

int THinit::getLatitude(float * ret){
    *ret = latitude;

    return 0;
}

int THinit::getIotId(int * ret){
    *ret = iotId;

    return 0;
}

int THinit::getName(char * iotname){
    strCpy(iotname, iotName, nameLength);

    return 0;
}

int THinit::getDescription(char * iotdescription){
    strCpy(iotdescription, iotDescription, descriptionLength);

    return 0;
}

int THinit::setSSID(char * ssid, int length){
    THinit::writeEepromString(eepromStart, length, ssid);
    strCpy(wifi_ssid,ssid,length);

    return 0;
}

int THinit::setWifiPassword(char* wifiPass, int length){
    THinit::writeEepromString((eepromStart + ssidLength + eepromPadding + 1), length, wifiPass);
    strCpy(wifi_password,wifiPass,length);

    return 0;
}

int THinit::setLongitude(float longitud){
    EEPROM.writeFloat((eepromStart + ssidLength + eepromPadding + wifiPassLength + eepromPadding + 1), longitud);
    EEPROM.commit();
    longitude = longitud;

    return 0;
}

int THinit::setLatitude(float latitud){
    EEPROM.writeFloat(eepromStart + ssidLength + eepromPadding + wifiPassLength + eepromPadding + 4 + 1, latitud);
    EEPROM.commit();
    latitude = latitud;

    return 0;
}

int THinit::setIotId(int iotid){
    EEPROM.writeInt(eepromStart + ssidLength + eepromPadding + wifiPassLength + eepromPadding + 4 + 1 + 4 + 1, iotid);
    EEPROM.commit();
    iotId = iotid;

    return 0;
}

int THinit::setName(char * iotid, int length){
    THinit::writeEepromString(nameStart, length, iotid);
    strCpy(iotName,iotid,length);

    return 0;
}

int THinit::setDescription(char * description, int length){
    THinit::writeEepromString(descriptionStart, length, description);
    strCpy(iotDescription,description,length);

    return 0;
}

void THinit::strCpy(char* str1, char* str2, int length){
    for(int i = 0; i < length; i++)
    {
        str1[i] = str2[i];
        if (str1[i] == '\0') {
            break;
        }
    }
}