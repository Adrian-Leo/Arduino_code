#include<CTBot.h>
#include "DHT.h"
#define DHTPIN 2     
int SensorPin=A0;
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE); 
//variable bot telegram
CTBot myBot;
//konfigurasi wifi
String ssid = "";
String pass = "";
//variable token dan id telegram
String token = "1508550489:AAGc22EfTNe7sa0nC_dqHAdEpBJUSRKcYwg";
const int id = 1482187153;

void setup() {
  Serial.begin(9600);

  dht.begin();
  pinMode(8,OUTPUT);
  //koneksi ke wifi 
  myBot.wifiConnect(ssid,pass);
  //set token telegram
  myBot.setTelegramToken(token);
  //cek koneksi wifi
  myBot.sendMessage(id,"===TABLE PERINTAH===");
  myBot.sendMessage(id,"ketik 'cek'");
  myBot.sendMessage(id,"ketik 'nyalakan'");
  myBot.sendMessage(id,"ketik 'matikan'");
  if(myBot.testConnection()){
    Serial.println("Connection Succesful");
  }
  else{
    Serial.println("Connection Failed");
  }

}

void loop() {
  // baca pesan masuk
  TBMessage msg;
  //sensor tanah
  int SensorValue = analogRead(SensorPin);   
  float SensorVolts = analogRead(SensorPin)*0.0048828125;  
  
  //read humidity
  float humidity = dht.readHumidity();
  
  //sensor DHT11
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(temperature, humidity, false);
  //panggil otomasi
   if(SensorValue <= 600){
    digitalWrite(8,LOW);
    
    Serial.println("OFF");
    
  }
  if(SensorValue > 600){
    digitalWrite(8,HIGH);
  
    Serial.println("ON ");
    
  }
  //bot kirim pesan
  String stringOne,stringOnee,stringTwo,stringTwoo,stringThree,stringThreee,stringFour,stringFourr,stringFive,stringSix,stringSixx;
  stringOne ="temperature : ";
  stringOnee=" °C";
  stringTwo= stringOne + temperature;
  stringTwoo = stringTwo + stringOnee;

  stringThree="humidity : ";
  stringThreee=" %";
  stringFour= stringThree + humidity;
  stringFourr = stringFour + stringThreee;

  stringFive ="heat index : ";
  stringSix= stringFive + hic;
  stringSixx = stringSix + stringThreee;
  if(myBot.getNewMessage(msg)){
    Serial.println("Incoming Message : "+msg.text);

    //tampung isi pesan
    String pesan = msg.text;
    if(pesan=="cek"){
   
      digitalWrite(LED_BUILTIN,HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN,LOW);
          if(SensorValue <= 600){
            myBot.sendMessage(id,"Tanah Basah"); 
          }
          if(SensorValue > 600){
            myBot.sendMessage(id,"Tanah Kering");  
          }
       myBot.sendMessage(id,stringTwoo);
       myBot.sendMessage(id,stringFourr);  
       myBot.sendMessage(id,stringSixx);  
         
      
      
    }
    else if(pesan=="nyalakan"){
      
      digitalWrite(LED_BUILTIN,HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN,LOW);
      digitalWrite(8,HIGH);
      myBot.sendMessage(id,"Pompa menyala"); 
      
    }
    else if(pesan =="matikan"){
      digitalWrite(LED_BUILTIN,HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN,LOW);
      digitalWrite(8,LOW);
      myBot.sendMessage(id,"Pompa dimatikan"); 
    }
  }
  Serial.print("Sensor value : " );
  Serial.println(SensorValue);
  Serial.print("Voltage value : ");
  Serial.println(SensorVolts);

  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.println("%");

  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.print(F("°C "));

  Serial.print(F("Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  delay(2000);



}
