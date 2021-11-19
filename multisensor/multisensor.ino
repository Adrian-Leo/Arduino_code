#include "DHT.h"

#include <ESP8266HTTPClient.h>
#include<ESP8266WiFi.h>



WiFiClient wifiClient;
#define DHTPIN D2
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);

// inisiasi variabel ssid dan pass 
char ssid[]="Adrian";
char pass[]="bosspapa123";

// INISIASI VARIABEL HOST IP ADDRESS SERVER 
const char* server="192.168.0.6";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();

    //inisiasi host
  WiFi.hostname("NodeMCU");
  //konek wifi
  WiFi.begin(ssid,pass);
  //cek koneksi
  while(WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(500);
    
  }
  Serial.println("berhasil konek wifi");

}

void loop() {
  // put your main code here, to run repeatedly:
  float suhu = dht.readTemperature();
  int kelembapan = dht.readHumidity();
  
    //tampilkan diserial
  Serial.println("Suhu : "+String(suhu));

  Serial.println("Kelembapan : "+String(kelembapan));
  Serial.println();

  WiFiClient client;
  const int httpPort =80;

  if(!client.connect(server,httpPort)){
    Serial.println("gagal terkoneksi ke web server");
    return;
  }

   HTTPClient http;
  //link url untuk kirim data 
  String Link = "http://"+String(server)+"/multisensor/kirimdata.php?suhu=" + String(suhu) + "&kelembapan=" +String(kelembapan) ;
  //eksekusi 
  http.begin(wifiClient, Link);
//  http.begin(Link);
  http.GET();
  //tangkap respon
  String respon = http.getString();
  Serial.println(respon);

  delay(1000);
}
