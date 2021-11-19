//#include <Firebase.h>
//#include <FirebaseArduino.h>
//#include <FirebaseCloudMessaging.h>
//#include <FirebaseError.h>
//#include <FirebaseHttpClient.h>
//#include <FirebaseObject.h>

//#include <ESP8266Firebase.h>

#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>


//
//
#define FIREBASE_HOST "sensor-e78b7-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "XlO4EIHj2vKFTgFZMyxspWpRzHikl1HOdK1yS7SH"
#define WIFI_SSID "BUDIPW2"
#define WIFI_PASSWORD "budi2712"

WiFiClient wifiClient;



#define DHTPIN D2
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);

// inisiasi variabel ssid dan pass 
//char ssid[]="Adrian";
//char pass[]="bosspapa123";
//#define WIFI_SSID "BUDIPW2712"
//#define WIFI_PASSWORD "budi2712"

// INISIASI VARIABEL HOST IP ADDRESS SERVER 
const char* server="192.168.0.6";

const int ledpin = D1;
const int ldrpin= A0;
const int soilpin=D3;
const int relay=D4;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(ledpin,OUTPUT);
  pinMode(ldrpin,INPUT);
  pinMode(soilpin,INPUT);
  pinMode(relay,OUTPUT);
  

  //inisiasi host
  WiFi.hostname("NodeMCU");
  //konek wifi
//  WiFi.begin(ssid,pass);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  //cek koneksi
  while(WiFi.status() != WL_CONNECTED){
    Serial.println(".");
    delay(500);
    
  }
 
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);

  //SETTING MODE LED
//  pinMode(PIN_LED,OUTPUT);

}

void loop(){
  float suhu = dht.readTemperature();

  int kelembapan = dht.readHumidity();

  int ldrstatus= analogRead(ldrpin);

  Serial.println("Suhu : "+String(suhu));

  Serial.println("Kelembapan : "+String(kelembapan));
  Serial.println();
  Serial.println("ldr : " + String(ldrstatus));

  int moisture =digitalRead(soilpin);
  Serial.println("soil : " + String(moisture));
  
  //SENSOR TANAH
   if(moisture == HIGH){
   digitalWrite(D4,HIGH);
    
   Serial.println("POMPA ON");
    
  }
  if( moisture == LOW){
    digitalWrite(D4,LOW);
  
    Serial.println("POMPA OFF ");
  }

  
   //if ldr <300 high
  if(ldrstatus <= 300){
    digitalWrite(ledpin,HIGH);
    Serial.println("LED ON ");
  }else {
      digitalWrite(ledpin,LOW);
    Serial.println("LED OFF ");
  }

  //KIRIM SENSOR KE FIREBASE masukan field di firebase yg dalam kasus ini value 
  //set value
  Firebase.setInt("kelembapan",kelembapan); 
  Firebase.setFloat("suhu",suhu);
  delay(100);

  // update value
  Firebase.setInt("kelembapan",kelembapan); 
  Firebase.setFloat("suhu",suhu); 
  delay(100);

  Firebase.setInt("cahaya",ldrstatus); 
  Firebase.setInt("tanah",moisture); 
  delay(100);

  Firebase.setInt("cahaya",ldrstatus); 
  Firebase.setInt("tanah",moisture); 
  delay(100);

  // remove value
//   Firebase.remove("kelembapan");
//   Firebase.remove("suhu");

    //get value 
     Serial.println("kelembapan firebase : ");
    Serial.println(Firebase.getInt("kelembapan"));

    Serial.println("Suhu firebase : ");
    Serial.println(Firebase.getInt("suhu"));
//
//    Serial.println("Kelembapan firebase : ");
//    Serial.println(Firebase.getFloat("kelembapan"));
//
     Serial.println("Cahaya firebase : ");
    Serial.println(Firebase.getInt("cahaya"));

    Serial.println("tanah firebase : ");
    Serial.println(Firebase.getInt("tanah"));

    if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(100);

  //status pompa
  int statuspompa= Firebase.getInt("nyalapompa");
  int statuslampu= Firebase.getInt("nyalalampu");
//
  if(statuspompa == 1){
    digitalWrite(relay,HIGH);
    Serial.println("pompa menyala");
  } else{
    digitalWrite(relay,LOW);
    Serial.println("pompa mati");
  }

   if(statuslampu == 1){
    digitalWrite(ledpin,HIGH);
    Serial.println("lampu menyala");
  } else{
    digitalWrite(ledpin,LOW);
    Serial.println("lampu mati");
  }
  
  
  

  //handle error firebase
//  if(Firebase.failed()){
//    Serial.println("Gagal kirim data");
//    Serial.println(Firebase.error());
//    digitalWrite(PIN_LED,LOW);
//    jika gagal coba lagi
//    return;
//    }

  //jika berhasil kirim data
//  digitalWrite(PIN_LED,HIGH);

  //Ambil nilai dari firebase 
//  Serial.print("KELEMBAPAN DARI FIREBASE : ");
//  Serial.println(Firebase.getInt("value"));


  //KIRIM DATA KE DATABASE
  //cek koneksi nodemcu ke web server 
  WiFiClient client;
  const int httpPort=80;

  if(!client.connect(server,httpPort)){
    Serial.println("gagal terkoneksi ke web server");
    return;
  }
  //jika terkoneksi ke web server kirimi data
  HTTPClient http;
  //link url untuk kirim data 
  String Link = "http://"+String(server)+"/iot-webleo/kirimdata.php?suhu=" + String(suhu) + "&kelembapan=" +String(kelembapan) +"&cahaya=" +String(ldrstatus)+ "&tanah=" +String(moisture);
  //eksekusi 
  http.begin(wifiClient, Link);
//  http.begin(Link);
  http.GET();
  //tangkap respon
  String respon = http.getString();
  Serial.println(respon);
  delay(1000);

  //LINK URL POMPA DAN LAMPU
  // baca status pompa
  String LinkPompa ; 
  HTTPClient httpPompa;
  LinkPompa = "http://" + String(server) + "/iot-webleo/dashboard-control/bacapompa.php";
  httpPompa.begin(LinkPompa);
  //ambil isi status pompa 
  httpPompa.GET();
  //baca status response 
  String responsePompa = httpPompa.getString();
  Serial.println(responsePompa);
  httpPompa.end();

  //ubah status pompa di nodemcu
  digitalWrite(relay,responsePompa.toInt());


   // baca status lampu
  String LinkLampu ; 
  HTTPClient httpLampu;
  LinkLampu = "http://" + String(server) + "/iot-webleo/dashboard-control/bacalampu.php";
  httpLampu.begin(LinkLampu);
  //ambil isi status lampu 
  httpLampu.GET();
  //baca status response 
  String responseLampu = httpLampu.getString();
  Serial.println(responseLampu);
  httpLampu.end();

   //ubah status lampu di nodemcu 
  digitalWrite(ledpin,responseLampu.toInt());
}
