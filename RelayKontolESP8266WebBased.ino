//---------------------------------------------------
//Project: Kontrol Relay Berbasis Web API Request   |
//Author: Anjas Amar Pradana                        |
//Program khusus board: esp8266 (Nodemcu)           |
//---------------------------------------------------

//=========================================================Perpus-Fungsi======================================================================
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <Arduino_JSON.h>

//=========================================================Setup-Server=======================================================================
//Web/Server API address to read/write from 
const char *HostServer = "http://mpsri.my-board.org";   //Default: http://mpsri.my-board.org website or IP address of server
//Fungsi Limit Request On/Off Dari Web Ke ESP
const long interval = 10; //3000
unsigned long previousMillis = 0;

//=========================================================Setup-WiFi=========================================================================
//Fungsi Kredensial Koneksi WiFi
const char* ssid = "Keluarga"; //masukkan ssid
const char* password = "MyFamily"; //masukkan password

//=========================================================Setup-IDBoard======================================================================
//ID Board Bebas Angka 1-seterusnya
int board = 1;

//=========================================================Void-Setup=========================================================================
void setup () {
  Serial.begin(115200);
//=========================================================WiFi-Module========================================================================
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Connected!!!");
  }
  else{
    Serial.println("Connected Failed!!!");
  }
  
}

//=========================================================Void-Loop=========================================================================
void loop() {
//====================================================WebAPIRequest-Module===================================================================
  //Declare object of class HTTPClient
  WiFiClient client;
  HTTPClient http;    
  String DeviceData, GetData, Link, url;
  //String to interger conversion
  DeviceData = String(board);
  Link = String(HostServer);  
      //GET Data
      GetData = "?board=" + DeviceData ;  //Note "?" added at front
      url = Link + "/proses.php" + GetData;

      http.begin(client, url); //Specify request destination
  
     //Fungsi Mendapatkan Balasan HTTP Request     
     int httpCode = http.GET();
    //Fungsi Untuk Mengituli Limit Request On/Off Yang Tadi Di Awal Program
    unsigned long currentMillis = millis();

  //Fungsi Payload Atau Pemanggialan Dan Penerimaan Data Status I/O High/Low Pada Pin GPIO
  if(currentMillis - previousMillis >= interval) {
      if (WiFi.status() == WL_CONNECTED) {
        if (httpCode > 0) {
            String payload = http.getString();
            JSONVar myObject = JSON.parse(payload);
            Serial.print("JSON object = ");
            Serial.println(myObject);
            JSONVar keys = myObject.keys();
        
            for (int i = 0; i < keys.length(); i++) {
                JSONVar value = myObject[keys[i]];
                Serial.print("GPIO: ");
                Serial.print(keys[i]);
                Serial.print(" - SET to: ");
                Serial.println(value);
                pinMode(atoi(keys[i]), OUTPUT);
                digitalWrite(atoi(keys[i]), atoi(value));
             }  
           previousMillis = currentMillis;    
         }
        http.end();
      }
  }
  
}
