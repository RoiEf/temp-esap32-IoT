#include <Arduino.h>
#include "BluetoothSerial.h"                              //Header File for Serial Bluetooth
#include <WiFi.h>
#include <DHT.h>
#include <HTTPClient.h>
#include <timer.h>
#include <esp_int_wdt.h>    // wachdog library. used for reset
#include <esp_task_wdt.h>   // wachdog library. used for reset
#include <THinit.h>

#define DHTPIN 15       // Digital pin connected to the DATA pin of the DHT22
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define halfSecond 500
#define second 1000
#define minuet 60000
#define btPin "12345"       // pin code for Blue tooth connection
#define ledPin 2

// Function defenitions
void hard_restart(void);
void replaceSubString(char *, char *, int);

char ssid[32] = "";
char password[63] = "";
int wifiConnected = 0;
THinit initObj;

BluetoothSerial ESP_BT;                                   //Object for Bluetooth

int setupStage = 0;

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

timer timer1(15,second);
int fl1 = 0;
timer btBlackOut(1,halfSecond);
timer timer2(60,second);
int fl2;

HTTPClient http;

// String json = "{\"longitude\":\"AAAAAAAAA\",\"latitude\":\"BBBBBBBBB\",\"temp\":\"CCCCC\",\"grade\":\"c\",\"humid\":\"DDDDD\",\"mark\": \"%\"}";
String json = "{\"iotId\":\"IIII\",\"temp\":\"CCCCC\",\"grade\":\"c\",\"humid\":\"DDDDD\",\"mark\": \"%\"}";
//    float longi;
//    float latit;

    int iotId;
    char name[15] = "";
    char description[50] = "";

void setup() {
 Serial.begin(9600);                                      //Start Serial monitor in 9600
    pinMode(ledPin,OUTPUT);
    digitalWrite(ledPin,LOW);
    fl2 = 0;

  ESP_BT.begin("Temp_IoT_Control");                      //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");

    dht.begin();                                         // starting communications with the DHT module.

    initObj.init();
    initObj.getSSID(ssid);
    initObj.getWifiPassword(password);
//    initObj.getLongitude(&longi);
//    initObj.getLatitude(&latit);
    initObj.getIotId(&iotId);

    Serial.print("ssid = ");
    Serial.println(ssid);
    Serial.print("wifi password = ");
    Serial.println(password);
 //   Serial.print("Longitude = ");
 //   Serial.println(longi,6);
 //   Serial.print("Latitude = ");
 //   Serial.println(latit,6);
    Serial.print("IoT ID = ");
    Serial.println(iotId);

    String tStr;
//    tStr = String(longi,6);
//    json.replace("AAAAAAAAA",tStr);
//    tStr = String(latit,6);
//    json.replace("BBBBBBBBB",tStr);
    tStr = String(iotId);
    json.replace("IIII", tStr);

    Serial.println(json);
}

void loop() {
    if (timer2.checkInterval()) {
        timer2.resetTimer();
        if (fl2 == 0) {
            fl2 = 1;
            digitalWrite(ledPin,HIGH);
        } else {
            fl2 = 0;
            digitalWrite(ledPin,LOW);
        }
        
    }

    if (wifiConnected == 0) {
        if (ssid[0] != '\0') {
            WiFi.begin(ssid, password);
            timer wifiTmr(1,minuet);

            while(WiFi.status() != WL_CONNECTED){

                delay(500);
                Serial.println("connecting to WiFi");

                if (wifiTmr.checkInterval()) {
 //                   Serial.println("Can\'t connect to wifi");
                    wifiConnected = -1;
                    break;
                }
                
            }
            if (WiFi.status() == WL_CONNECTED) {
                wifiConnected = 1;
            }

        }
    }
    

    if (btBlackOut.checkInterval()) {

        char btIn[63];
        int btInPt = 0;
        if (ESP_BT.available()) {
            while (ESP_BT.available()) {
                btIn[btInPt] = ESP_BT.read(); //Read what we recevive

                Serial.print("btIn[");
                Serial.print(btInPt);
                Serial.print("] = ");
                Serial.println(btIn[btInPt]);

                btInPt ++;

                Serial.print("btInPt = ");
                Serial.println(btInPt);
}

        if (setupStage == 2) {
            int i;
            for(i = 0; i < btInPt; i++)
            {
                ssid[i] = btIn[i];
            }
            ssid[i] = '\0';
            i++;

            initObj.setSSID(ssid,i);
            
            btIn[0] = 99;
            setupStage = 1;
        
        }
        else if (setupStage == 3) {
            int i;
            for(i = 0; i < btInPt; i++)
            {
                password[i] = btIn[i];
            }
            password[i] = '\0';
            i++;

            initObj.setWifiPassword(password,i);
           
            btIn[0] = 99;
            setupStage = 1;

        }
        else if (setupStage == 4) { // setting Longitude
            float i;
            i = atof(btIn);
            initObj.setLongitude(i);

            setupStage = 1;
        }
        else if (setupStage == 5) { // setting Latitude
            float i;
            i = atof(btIn);
            initObj.setLatitude(i);

            setupStage = 1;
        }
        else if (setupStage == 6) {
            int i;
            char pinCode1[] = btPin;
            char pinCode2[6];
            for(i = 0; i < btInPt; i++)
            {
                pinCode2[i] = btIn[i];
            }
            pinCode2[i] = '\0';
            btIn[0] = 99;

            int flag = 0;
            for(i = 0; i < 5; i++)
            {
                if (pinCode1[i] != pinCode2[i]) {
                    flag = 1;
                    btBlackOut.setDelta(2,minuet);
                    btBlackOut.resetTimer();
                    setupStage = 0;
                    for(int i = 0; i < 63; i++)
                    {
                       btIn[i] = '\0';
                    }
                }
            }
            if (flag == 0) {
                    ESP_BT.println("Entering Setup");
                    setupStage = 1;
                    for(int i = 0; i < 63; i++)
                    {
                       btIn[i] = '\0';
                    }
            }
/*        } else if (setupStage == 1 && btInPt == 2){
            if (btIn[0] == 49 && btIn[1] == 48) {
                ESP_BT.println("Ready to set IoT Id");
                setupStage = 10;
            }
*/        }
        else if (setupStage == 9) { // setting id
            Serial.println("Entered set id");
            int i;
            i = atoi(btIn);
            Serial.print("i = ");
            Serial.println(i);
            initObj.setIotId(i);

            setupStage = 1;
            initObj.getIotId(&i);
            Serial.print("i from Update = ");
            Serial.println(i);

        }
        else {
            if (btIn[0] == 48 &&  setupStage == 1)          // 0
                {
                    ESP_BT.println("Out of setup");
                    setupStage = 7;
                }
            else if (btIn[0] == 49 && setupStage == 0)      // 1
            {
                ESP_BT.println("Input Pin Code");
                setupStage = 6;
            }
            else if (btIn[0] == 49 && setupStage == 7)      // 1
            {
                ESP_BT.println("Entering Setup");
                setupStage = 1;
            }
            else if (btIn[0] == 50 && setupStage == 1) {     // 2
                ESP_BT.println("Ready to set SSID");
                setupStage = 2;
            }
            else if (btIn[0] == 51 && setupStage == 1) {    // 3
                ESP_BT.println("Ready to set WiFi Password");
                setupStage = 3;
            }
            else if (btIn[0] == 52 && setupStage == 1) {    // 4
                ESP_BT.println("Ready to set Longitude");
                setupStage = 4;
            }
            else if (btIn[0] == 53 && setupStage == 1) {    // 5
                ESP_BT.println("Ready to set WiFi Latitude");
                setupStage = 5;
            }
            else if (btIn[0] == 54 && setupStage == 1) {    // 6
                ESP_BT.println("Ready to set IoT ID");
                setupStage = 9;
            }
/*            else if (btIn[0] == 55 && setupStage == 1) {    // 7
                ESP_BT.println("Ready to set Station Name");
                setupStage = 10;
            }
            else if (btIn[0] == 56 && setupStage == 1) {    // 8
                ESP_BT.println("Ready to set Station Description");
                setupStage = 11;
            }
*/            
            else if (btIn[0] == 57 && setupStage == 1) {    // 9
                ESP_BT.println("Are you sure you want to reset?");
                setupStage = 7;
            }
            else if ((btIn[0] == 89 || btIn[0] == 121) && setupStage == 7) {
                ESP_BT.println("Resetting system!!!");
                delay(25);
                hard_restart();
            }
            else if ((btIn[0] != 89 || btIn[0] != 121) && setupStage == 7) {
                ESP_BT.println("Back to setup sequence");
                setupStage = 1;
            }
            else if (btIn[0] == 56 && setupStage == 1) { // 8
                ESP_BT.println("Are you sure you want to cleare the EEPROM?");
                setupStage = 8;
            }
            else if ((btIn[0] == 89 || btIn[0] == 121) && setupStage == 8) {
                ESP_BT.println("Clearing EEPROM!!!");
                delay(25);
                initObj.resetEEPROM();
                setupStage = 1;
            }
            else if ((btIn[0] != 89 || btIn[0] != 121) && setupStage == 8) {
                ESP_BT.println("Back to setup sequence");
                setupStage = 1;
            }
        }
    }
  delay(20);
    }
    


    if (timer1.checkInterval()){
        if (fl1 == 0) {
            timer1.setDelta(10,minuet);
            fl1 = 1;
        }
        timer1.resetTimer();

        String json2 = json;

        // Reading temperature or humidity takes about 250 milliseconds!
        // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
        float h = dht.readHumidity();
        // Read temperature as Celsius (the default)
        float t = dht.readTemperature();

        if (isnan(h) || isnan(t)) {
            Serial.println(F("Failed to read from DHT sensor!"));
        }
      Serial.print(F("Humidity: "));
        Serial.print(h);
        Serial.print(F("%  Temperature: "));
        Serial.println(t);

        if (wifiConnected > 0) {
           Serial.println("Connected to Wifi network");
           Serial.println(WiFi.localIP());

            String tStr;
            tStr = String(t,2);
            json.replace("CCCCC",tStr);
            tStr = String(h,2);
            json.replace("DDDDD",tStr);
            Serial.print("json = ");
            Serial.println(json);

            Serial.println("Opening HTTP.");
            http.begin("temp.efrati.info",44404,"/iotApi"); // http.begin("192.168.0.109",4000,"/iotApi");
            http.addHeader("Content-Type", "application/json");
            int httpCode = http.POST(json);
            if (httpCode > 0) {
                Serial.println("Data posted.");
                Serial.printf("[HTTP] Post... code: %d\n", httpCode);
            } else {
                Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }
            Serial.println("Closing HTTP.");
            http.end();
        } 
        else {
             Serial.println("WiFi Not connected");
        }
        json = json2;
    }
}

void hard_restart() {
    Serial.println("Resetting!!!");
    ESP_BT.end();
    if (WiFi.status() == WL_CONNECTED) {
        WiFi.disconnect();
    }
    Serial.end();
    
    esp_task_wdt_init(1,true);
    esp_task_wdt_add(NULL);
    while(true);
}