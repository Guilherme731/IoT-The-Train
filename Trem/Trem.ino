#include <WiFi.h> 
#include <WiFiClientSecure.h> 
#include <PubSubClient.h>

//constantes p/ trem
const byte resistorPin = 13;
const byte caboPin = 12;

WiFiClientSecure client;
PubSubClient mqtt(client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

//constantes p/ broker
const String URL   = "8b46e29e75014bcba8465b77629b065c.s1.eu.hivemq.cloud";
const int PORT     = 8883;
const String USR   = "thetrain_esp";
const String broker_PASS  = "Thetrain123";
const String MyTopic = "teste";

int ledPin = 2;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Conectando WiFi");
  WiFi.begin(SSID,PASS);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("\n Conectado com sucesso!");
  client.setInsecure();
  Serial.println("Conectando ao Broker");
  mqtt.setServer(URL.c_str(),PORT);
  while(!mqtt.connected()){
    String ID = "S1_";
    ID += String(random(0xffff),HEX); //cria a parte aleat.
    mqtt.connect(ID.c_str(),USR.c_str(),broker_PASS.c_str());
    Serial.print(".");
    delay(200);
  }
  mqtt.subscribe(MyTopic.c_str());
  mqtt.setCallback(callback);
  Serial.println("\nConectado com sucesso ao broker !");
}

void loop() {
  mqtt.loop();
  delay(1000);
}

void callback(char* topic, byte* payload, unsigned int length){
  String mensagem = "";
  for(int i = 0; i < length; i++){
    mensagem += (char)payload[i];
  }
  Serial.print("Recebido: ");
  Serial.println(mensagem);

  if (mensagem > 0) {
    digitalWrite(resistorPin, HIGH);
  }

  if (mensagem < 0) {
    digitalWrite(caboPin, LOW);
  }

  if (mensagem == "Pedro: liga led" || mensagem == "Gabriel: liga led"){
    digitalWrite(ledPin, HIGH);
  } 
  if (mensagem == "Pedro: desliga led" || mensagem == "Gabriel: desliga led"){
    digitalWrite(ledPin, LOW);
  }
}
