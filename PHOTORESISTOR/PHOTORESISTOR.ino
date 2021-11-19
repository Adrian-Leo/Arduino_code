const int ledpin =13;
const int ldrpin=A0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledpin,OUTPUT);
  pinMode(ldrpin,INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int ldrstatus= analogRead(ldrpin);

  //if ldr <300 high
  if(ldrstatus <= 300){
    digitalWrite(ledpin,HIGH);
    Serial.println("LED ON ");
  }else {
      digitalWrite(ledpin,LOW);
    Serial.println("LED OFF ");
  }
}
