#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Ultrasonic.h>
#include <Bonezegei_DHT11.h>

WiFiClientSecure client;
PubSubClient mqtt(client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

//Constantes do Broker
const String URL   = "8b46e29e75014bcba8465b77629b065c.s1.eu.hivemq.cloud";
const int PORT     = 8883;
const String USR   = "thetrain_esp";
const String broker_PASS   = "Thetrain123";
const String TempTopic     = "S1/Temp";
const String UmidTopic     = "S1/Umid";
const String PresencaTopic = "S1/Presenca";
const String LedTopic      = "S1/Iluminacao";

int ldrPin = 34;
int ledPin = 19;
int echoPin = 23;
int trigPin = 22;

int sensibilidadeLdr = 2500;

 Bonezegei_DHT11 dht(4);

Ultrasonic ultrasonic(trigPin, echoPin);

void setup() {
  // put your setup code here, to run once:
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
  dht.begin();
  Serial.begin(115200);
  Serial.println("Conectando  ao WiFi");
  WiFi.begin(SSID, PASS);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("\nConectado com sucesso!");
  client.setInsecure();
  Serial.println("Conectando com o Broker");
  mqtt.setServer(URL.c_str(), PORT);
  while(!mqtt.connected()){
    String ID = "S1_";
    ID += String(random(0xffff), HEX);
    mqtt.connect(ID.c_str(), USR.c_str(), broker_PASS.c_str());
    Serial.print(".");
    delay(200);
  }
  //mqtt.subscribe(MyTopic.c_str());
  mqtt.subscribe(LedTopic.c_str());
  mqtt.setCallback(callback);
  Serial.println("\nConectado com sucesso ao broker!");
}

void loop() {
  dht.getData();
  int valorLdr = analogRead(ldrPin);
  int distance = ultrasonic.read();
  int acenderLed = (valorLdr > sensibilidadeLdr)? 1 : 0;
  int presencaDetectada = (distance > 30)? 0 : 1;
  mqtt.publish(LedTopic.c_str(), String(acenderLed).c_str());
  mqtt.publish(PresencaTopic.c_str(), String(presencaDetectada).c_str());
  mqtt.publish(TempTopic.c_str(), String(dht.getTemperature()).c_str());
  mqtt.publish(UmidTopic.c_str(), String(dht.getHumidity()).c_str());
  
  mqtt.loop();
  delay(1000);

}

void callback(char* topic, byte* payload, unsigned int length){
  String mensagem = "";
  for(int i = 0; i < length; i++){
    mensagem += (char)payload[i];
  }
  if(mensagem == "1"){
    digitalWrite(ledPin, HIGH);
  }
  if(mensagem == "0"){
    digitalWrite(ledPin, LOW);
  }

  
}