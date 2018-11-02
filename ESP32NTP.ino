#include <time.h> //the ESP32 native time library, does graceful NTP server synchronization
#include <WiFi.h>
#include "WiFiInfo.h" // stroes the SSID and PASSWORD in seperate file so it will not be uploaded to github
                      // this file can be included in a folder by the same name in the libraries folder
#include <WiFiMulti.h> // Connect to the best AP based on a given list

WiFiMulti wifiMulti;  // create and instance of WiFiMulti called wifiMulti


unsigned long beginTime = 0;

long timezone = -8; // Pacific time zone
byte daysavetime = 1; // 1 specifies daylight savings time 0 specifies standard time

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store the last time the serial port was updated

// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)

void setup(){
    Serial.begin(115200);
    // We start by connecting to a WiFi network
    Serial.println("This sketch is called ESP32NTP.ino");
    Serial.println();

    connectWiFi();

 // wait for connection to WiFi
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println(WiFi.status());
        Serial.print(".");
    }

    time_t now;

    Serial.print("Time() before connecting to the time server is ");
    //Serial.printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct.tm_year)+1900,( tmstruct.tm_mon)+1, tmstruct.tm_mday,tmstruct.tm_hour , tmstruct.tm_min, tmstruct.tm_sec);
    time(&now);
    Serial.println(now); //this is the number of seconds since January 1st 1970)

    Serial.println("Contacting Time Server");
	configTime(3600*timezone, daysavetime*3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org"); // I think this is the function to connect to time server
	
    struct tm tmstruct ;
    
    delay(2000);
    Serial.print("Time() after connecting to the time server is ");
//    Serial.println(time_t());  //this is the number of seconds since January 1st 1970)
    time(&now);
    Serial.println(now);  //this is the number of seconds since January 1st 1970)

    tmstruct.tm_year = 0;

    getLocalTime(&tmstruct, 5000);  // I think this reterives the time from the ESP32
    WiFi.disconnect(true);  // disconnect the WiFi
    WiFi.mode(WIFI_OFF); // turn off the WiFi module to save power
    btStop();   // turn off the blue tooth module to save power
    Serial.printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct.tm_year)+1900,( tmstruct.tm_mon)+1, tmstruct.tm_mday,tmstruct.tm_hour , tmstruct.tm_min, tmstruct.tm_sec);
 //   Serial.println(());  //this is the number of seconds since January 1st 1970)
    Serial.println("");
    Serial.print("WiFi IP address is: ");
    Serial.println(WiFi.localIP());
    Serial.print("WiFi status: ");
    Serial.println(WiFi.status());      // show the WiFi status
    Serial.print("WiFi connection status: ");
    if (WiFi.isConnected() == 1 ){
        Serial.println("Connected"); // show if WiFi is connected, 1 for yes, 0 for no
    }
    else{
        Serial.println("Disconnected"); // show if WiFi is connected, 1 for yes, 0 for no
    }
}

void loop(){

  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to write the currect internal time to the serial port;
  // that is, if the difference between the current time and last time you wrote
  // the the time is longer than the interval at which you want to write the time to the serial port.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {

    previousMillis = currentMillis; // save the last time you wrote the time to the serial port

    // write the time from the internal clock:
    
    time_t now;
    time(&now);

    struct tm tmstruct;
    tmstruct.tm_year = 0;
    getLocalTime(&tmstruct, 5000);
    Serial.print("time_t(now) = ");
    Serial.print(time_t(now));  //this is the number of seconds since January 1st 1970
    Serial.printf("\nThe formatted time is: %d-%02d-%02d %02d:%02d:%02d\n",(tmstruct.tm_year)+1900,( tmstruct.tm_mon)+1, tmstruct.tm_mday,tmstruct.tm_hour , tmstruct.tm_min, tmstruct.tm_sec);
    Serial.println(""); //without this line the clock updates to the serial port every five seconds instead of interval.
  }
}

void connectWiFi(){
    wifiMulti.addAP(ssid_from_AP_1, your_password_for_AP_1); // this is defined in the the WiFiInfo.h file
    wifiMulti.addAP(ssid_from_AP_2, your_password_for_AP_2); // this is defined in the the WiFiInfo.h file
    wifiMulti.addAP(ssid_from_AP_3, your_password_for_AP_3); // this is defined in the the WiFiInfo.h file

    Serial.print("Connecting Wifi to ");
    Serial.print(ssid_from_AP_1);
    Serial.print(", "),   
    Serial.print(ssid_from_AP_2);   
    Serial.print(", or "),   
    Serial.println(ssid_from_AP_3);   

    if(wifiMulti.run() == WL_CONNECTED) {
        Serial.println("");
        Serial.print("WiFi connected to: ");
        Serial.println(WiFi.SSID());
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("Signal strength: ");
        Serial.println(WiFi.RSSI());
    }
}