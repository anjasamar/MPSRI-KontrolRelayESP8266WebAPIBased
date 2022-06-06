
//---------------------------------------------------------------------------------
//Project: Kontrol Relay Berbasis Web API Request, LCD Monitor, Dan WiFiManager   |
//Author: Anjas Amar Pradana                                                      |
//Mail: atsidev.io@gmail.com                                                      |
//Program khusus board: esp8266 (Nodemcu)                                         |
//Official Sites: http://mpsri.skom.id OR http://mpsri.my-board.org               |
//---------------------------------------------------------------------------------

//=========================================================Perpus-Fungsi======================================================================
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <Arduino_JSON.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <WiFiManager.h>         // https://github.com/tzapu/WiFiManager
#include <DNSServer.h>

//===========================================================Setup-LCD========================================================================
// atur jumlah kolom dan baris LCD
int lcdColumns = 16;
int lcdRows = 2;

// atur alamat LCD, jumlah kolom dan baris
// jika Anda tidak tahu alamat tampilan Anda, jalankan sketsa pemindai I2C
// abaikan jika error terjadi saat kompiler, yang mengatakan tidak support untuk jenis board anda selain AVR,
// ini saya menggunakan perpustakaan dan mengubahnya di properti perpustakaan lalu menambahkan perangkat modul saya.
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

//=========================================================Setup-Server=======================================================================
//Alamat Server Api
String host = "alamat.url.anda"; //Contoh: www.anjasganteng.com (tanpa tanda baca, slash akhir, dan http:// atau http://

// Set web server port number to 80
//Cek Alamat Server Dapat Terhubung
const int httpPort = 80;

//Fungsi Limit Request On/Off Dari Web Ke ESP
const long interval = 100; //10000
unsigned long previousMillis = 0;

//=========================================================Setup-WiFi=========================================================================
//Fungsi Kredensial Koneksi WiFi Manual, Silakan Isi Data Di bawah, Jika Anda Menggunakan Fitur Ini Setup Wifi Manager Tidak Akan Bekerja
const char* ssid = ""; //masukkan ssid
const char* password = ""; //masukkan password
const char* namaAP = "ESP-MPSRI-SETUP"; //Ubah Nama Akses Poin Setup
const char* passwordAP = "atsidevio"; //Ubah Sandi Akses Poin Setup

//=========================================================Setup-IDBoard======================================================================
//ID Token Board, Silakan Masuk Ke Akun Anda Atau Daftar Di http://atsidev.mpsri.skom.id Untuk Membuat Dan Melihat Token Anda Sendiri
int board = "XXXXXXXXXXXXXXXXXXXX"; //Token Contoh: 12345678901234567890

//==========================================================Void-Setup=========================================================================
void setup () {

Serial.begin(115200);
  
  String messageStartUp = "Memulai Sistem";
  String messageSetupWifi = "Silakan Atur Koneksi Wifi Pada Akses Poin ESP-MPSRI-SETUP Untuk Menggunakan MPSRI";
  Serial.print("Memulai Sistem Tunggu...");
  // initialize LCD
  lcd.init();
  // nyalakan lampu latar LCD                      
  lcd.backlight();
  lcd.setCursor(0,0);
  // print pesan statis
  lcd.print(messageStartUp);
  lcd.blink();
  delay(7000);
  lcd.setCursor(0,1);
  lcd.print("Memulai I/O");
  lcd.blink();
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Atur Koneksi!>");
  lcd.blink();
  lcd.setCursor(0,0);
  // print pesan skrol
  scrollText(0, messageSetupWifi, 260, lcdColumns);
  delay(3000);
  
//=====================================================WiFiManager-Module========================================================================
//Fungsi Kredensial Koneksi WiFi Sistem Web WifiManager
WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    WiFiManager wm;
    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    wm.resetSettings();
    // Terhubung secara otomatis menggunakan kredensial yang disimpan,
    // jika koneksi gagal, itu memulai jalur akses dengan nama yang ditentukan ( "ESP-MPSRI-SETUP"),
    // jika kosong akan otomatis menghasilkan SSID, jika kata sandi kosong akan menjadi AP anonim (wm.autoConnect())
    // kemudian masuk ke loop pemblokiran menunggu konfigurasi dan akan mengembalikan hasil sukses
    bool res;
    // res = wm.autoConnect(); // auto generated nama AP dari chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect(namaAP,passwordAP); // Mengamankan Aksespoint ESP8266
    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //jika Anda sampai di sini, Anda telah terhubung ke WiFi    
        Serial.println("connected...yeey :)");
    }

  delay(6000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WiFi: OK");
  delay(3000);
  lcd.setCursor(0,1);
  lcd.print("Status WiFi: ");
  lcd.print(WiFi.status());
  delay(6000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("IP Wifi:");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Web Client: OK");
  delay(2000);
  lcd.setCursor(0,1);
  lcd.print("Program: OK");
  delay(2000);
  lcd.clear();
  lcd.print("Memulai Program");
  lcd.blink();
  lcd.setCursor(0,1);
  lcd.print("Mohon Tunggu...");
  delay(6000);
  lcd.noBlink();
  lcd.clear();
//=========================================================WiFi-Module========================================================================
//Module Wifi Jika Menggunakan Pengaturan Manual
  WiFi.hostname("Perangkat Driver Relay");
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan Ke Jaringan WiFi..");
  }
  if(WiFi.status() == WL_CONNECTED){
  Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else{
    Serial.println("Connected Failed!!!");
  }
  
}

// Sring Data Untuk Kebutuhan Text Scrolling LCD
String messageToScroll = "Metode Pembelajaran Saklar Relay Dengan ESP8266";

// Fungsi untuk text skrol Judul projek
// Fungsi menerima argumen berikut::
// row: nomor baris tempat teks akan ditampilkan
// message: pesan ke skrol
// delayTime: penundaan antara setiap perpindahan karakter
// lcdColumns: angka dari kolom dari LCD
void scrollText(int row, String message, int delayTime, int lcdColumns) {
  for (int i=0; i < lcdColumns; i++) {
    message = " " + message;  
  } 
  message = message + " "; 
  for (int pos = 0; pos < message.length(); pos++) {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

//=========================================================Void-Loop=========================================================================
void loop() {
  
//====================================================WebAPIRequest-Module===================================================================
  //Declare object of class HTTPClient
  WiFiClient client;
  HTTPClient http;

  //Konfrensi Alamat Server Api
  String LinkRelay;
  String URLServer = "http://"+String(host);
  LinkRelay = "http://"+String(host)+"/proses.php?board="+String(board); //Sesuaikan Dengan Direktori Web Server Anda Ya Pada (/relaykontrol/)
  http.begin(client, LinkRelay);
  
    //Fungsi Mendapatkan Balasan HTTP Request     
    int httpCode = http.GET();
    
    //Fungsi Untuk Mengituli Limit Request On/Off Yang Tadi Di Awal Program
    unsigned long currentMillis = millis();

  //Fungsi Payload Atau Pemanggialan Dan Penerimaan Data Status I/O High/Low Pada Pin GPIO
  if(currentMillis - previousMillis >= interval) {
      if (WiFi.status() == WL_CONNECTED) {

        //Jika Status Server Tidak Terhubung Maka Akan Mengeluarkan Hasil Output -Digit Contoh -1 Pada LCD, Sebaliknya Jika Status Server Terhubung Maka Akan Mengrluarkan Output >Digit Contoh 200
        if (httpCode > 0) {
          
            //Membaca Status Server Api
            String payload = http.getString();
            JSONVar myObject = JSON.parse(payload);
            Serial.print("Payload = ");
            Serial.println(payload);
            Serial.print("JSON object payload = ");
            Serial.print(httpCode);
            Serial.println(myObject);
            JSONVar keys = myObject.keys();

            //Membaca Status Value Output Server Api, Lalu Memulai Program Ke Relay
            for (int i = 0; i < keys.length(); i++) {
                JSONVar value = myObject[keys[i]];
                Serial.print("GPIO KE: ");
                Serial.print(keys[i]);
                Serial.print(" - SET Value KE: ");
                Serial.println(value);
                pinMode(atoi(keys[i]), OUTPUT);
                digitalWrite(atoi(keys[i]), atoi(value));
             }  
           previousMillis = currentMillis;   
         }
        http.end();
      }
  }

  //Menampilkan Status Kode HTTP Respond, Dan Status Alamat WiFi
  Serial.println(LinkRelay);
  
  //Cek Status Port Server Apakah Dapat Terhubung
  if(!client.connect(host, httpPort)){
     Serial.print("Gagal Terhubung Ke Server Api -> ");
  }
  Serial.print("Berhasil Terhubung Ke Server Api -> ");
  
//====================================================LCD-Info-Module===================================================================
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Status Updated >");
  delay(6000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(F("Status API: "));lcd.print(httpCode);
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("URL Server:");
  // print pesan skrol
  scrollText(1, URLServer, 260, lcdColumns);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Pada Port: ");lcd.print(httpPort);
  delay(3000);
  lcd.clear();
  lcd.print("Cek Status Relay");
  lcd.setCursor(0,1);
  lcd.print("Dalam 3 detik...");
  delay(3000);
}

//---------------------------------------------------------------------------------
//Project: Kontrol Relay Berbasis Web API Request, LCD Monitor, Dan WiFiManager   |
//Author: Anjas Amar Pradana                                                      |
//Mail: atsidev.io@gmail.com                                                      |
//Program khusus board: esp8266 (Nodemcu)                                         |
//Official Sites: http://mpsri.skom.id OR http://mpsri.my-board.org               |
//---------------------------------------------------------------------------------
