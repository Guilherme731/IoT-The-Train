 #include <WiFiClientSecure.h>
 #include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include <Ultrasonic.h>

WiFiClientSecure client;
PubSubClient mqtt(client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const byte pin_led = 23;

//Nome dos servos
Servo servo_superior; // pin 21
Servo servo_inferior; // pin 19


byte pos_S1 = 120;

//constantes p/ broker
const String URL   = "8b46e29e75014bcba8465b77629b065c.s1.eu.hivemq.cloud";
const int PORT     = 8883;
const String USR   = "thetrain_esp";
const String broker_PASS  = "Thetrain123";
const String servo1_topic = "S3/Servo1";
const String servo2_topic = "S3/Servo2";
const String presenca_topic = "S3/Presenca";
const String presenca = "Presenca";

const String Presenca1Topic = "S2/Presenca1";
const String Presenca2Topic = "S2/Presenca2";
const String Presenca3Topic = "S3/Presenca";
const String iluminacaoTopic = "S1/Iluminacao";

const int pinTrig = 32;
const int pinEcho = 33;
float distancia;
Ultrasonic ultrasonic(pinTrig, pinEcho);


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
  mqtt.setCallback(callBack);
  mqtt.subscribe(iluminacaoTopic.c_str());
  mqtt.subscribe(Presenca1Topic.c_str());
  mqtt.subscribe(Presenca2Topic.c_str());
  mqtt.subscribe(Presenca3Topic.c_str());
  Serial.println("\nConectado com sucesso !");

  pinMode(pin_led,OUTPUT);

  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

  servo_superior.attach(23);
  servo_inferior.attach(22);

} 

void loop() {
  
  int distancia = ultrasonic.read();
  Serial.println(distancia);
  int presencaLed = (distancia < 30)? mqtt.publish(presenca_topic.c_str(), "1") : 0;
  // mqtt.publish(presenca_topic.c_str(), String(presencaLed).c_str());

  mqtt.loop();
  delay(1000);

}

void callBack(char* topic, byte* payload, unsigned int length){
  String mensagem = "";
  
  for(int i = 0; i < length; i++){
    mensagem += (char)payload[i];
  }
  Serial.printf("Recebido: %s\n",mensagem);

  if(String(topic) == "S1/Iluminacao"){
    if(mensagem == "1"){
      digitalWrite(pin_led, HIGH);
    }
      if(mensagem == "0"){
        digitalWrite(pin_led, LOW);
      }
  }

  if(strcmp(topic,Presenca2Topic.c_str()) == 0 && mensagem == "1"){
    servo_inferior.write(120);
    Serial.println("Movendo s_inf 120");
  }else if(strcmp(topic,Presenca3Topic.c_str()) == 0 && pos_S1 == 120 && mensagem == "1"){
    servo_superior.write(60);
    Serial.println("Movendo s_sup 60");
    pos_S1 = 60;
  }else if(strcmp(topic,Presenca3Topic.c_str()) == 0 && pos_S1 == 60 && mensagem == "1"){
    servo_superior.write(120);
    Serial.println("Movendo s_sup 120");
    pos_S1 = 120;
  }else if(strcmp(topic,Presenca1Topic.c_str()) == 0 && mensagem == "1"){
    servo_inferior.write(60);
    Serial.println("Movendo s_inf 60");
  }
}







