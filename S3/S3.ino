 #include <WiFiClientSecure.h>
 #include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

WiFiClientSecure client;
PubSubClient mqtt(client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const byte pin_led = 23;

//constantes p/ broker
const String URL   = "8b46e29e75014bcba8465b77629b065c.s1.eu.hivemq.cloud";
const int PORT     = 8883;
const String USR   = "thetrain_esp";
const String broker_PASS  = "Thetrain123";
const String servo1_topic = "Servo1";
const String servo2_topic = "Servo2";
const String presenca_topic = "Presenca";
const String presenca = "Presenca";

const String Presenca1Topic = "Presenca1";
const String Presenca2Topic = "Presenca2";
const String iluminacaoTopic = "S1/Iluminacao";

const int pinTrig = 32;
const int pinEcho = 33;
float distancia;

Servo servo_1;

 void setup() {
  Serial.begin(115200);
  Serial.println("Conectando ao WiFi");
  WiFi.begin(SSID, PASS);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("\nConectado com sucesso!");
  client.setInsecure();
  Serial.println("Conectando ao Broker");
  mqtt.setServer(URL.c_str(),PORT);
  while(!mqtt.connected()){
    String ID = "S2_";
    ID += String(random(0xffff),HEX); //cria a parte aleat.
    mqtt.connect(ID.c_str(),USR.c_str(),broker_PASS.c_str());
    Serial.print(".");
    delay(200);
  }
  mqtt.subscribe(iluminacaoTopic.c_str());
  mqtt.setCallback(callBack);
  Serial.println("\nConectado com sucesso !");

  pinMode(pin_led,OUTPUT);

  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

  servo_1.attach(2);
  servo_1.write(0);

}

void loop() {
  String mensagem = "Pedro: ";
  if(Serial.available()>0){
    mensagem += Serial.readStringUntil('\n');
    mqtt.publish(presenca.c_str(),mensagem.c_str());
  }
  mqtt.loop();
  delay(1000);

}

void callBack(char* topic, byte* payload, unsigned int length){
  String mensagem = "";
  
  for(int i = 0; i < length; i++){
    mensagem += (char)payload[i];
  }
  if(String(topic) == "S1/Iluminacao"){
  if(mensagem == "1"){
    digitalWrite(pin_led, HIGH);
  }
  if(mensagem == "0"){
    digitalWrite(pin_led, LOW);
  }
  }

  
}


void medirDistancia(){
  digitalWrite(pinTrig, LOW);
  delay(0005);
  digitalWrite(pinTrig, HIGH);
  delay(0010);
  digitalWrite(pinTrig, LOW);

  distancia = pulseIn (pinEcho, HIGH);
  distancia = distancia/58;
  Serial.println (distancia);
  
  if(distancia < 50){

  }
}


void moverServo(){
  for(int i=0; i<=180; i=+90){
    servo_1.write(i);
    delay(100);
  }
}






