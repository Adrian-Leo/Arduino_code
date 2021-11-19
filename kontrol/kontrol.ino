//library nodemcu 
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

//NETWORK SSID 
const char* ssid ="Adrian";
const char* pass ="bosspapa123";  

//ambil ip server 

const char* server = "192.168.0.6" ;

//pin untuk relay 
#define pin_pompa D1 // pin GPIO5 = D1 
#define pin_lampu D2 // pin GPIO4 = D2 


void setup() {
  Serial.begin(9600);

  //koneksi ke wifi 
  WiFi.hostname("NodeMcu");
  WiFi.begin(ssid,pass);

  //cek koneksi apakah berhasil atau tidak 
  while(WiFi.status() != WL_CONNECTED){
    //coba koneksi terus 
    Serial.print(".");
    delay(500);
  }
  Serial.println("connected");

  //mode untuk pin pompa 
  pinMode(pin_pompa,OUTPUT);
  digitalWrite(pin_pompa, 0); 


//  pinMode(pin_lampu,OUTPUT)   
//  digitalWrite(pin_lampu, 0); 
  

}

void loop() {
  // koneksi ke web server
  WiFiClient client ; 
  const int httpPort =80;
  //uji koneksi ke server 
  if(!client.connect(server,httpPort)){
    Serial.println("gagal koneksi ke web server ");
    return;
  }
  //apabila berhasil konek 
  Serial.println("berhasil konek ke server");
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
  digitalWrite(pin_pompa,responsePompa.toInt());


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
  digitalWrite(pin_lampu,responseLampu.toInt());
  
  delay(100);
}
