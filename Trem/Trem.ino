#include <WiFi.h> 
#include <PubSubClient.h>

WiFiClient client;
PubSubClient mqtt(client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

//constantes p/ broker
const String URL   = "test.mosquitto.org";
const int PORT     = 1883;
const String USR   = "";
const String broker_PASS  = "";
const String MyTopic = "rogui";
const String OtherTopic = "pega";

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
  String mensagem = "Rodrigo: ";
  if (Serial.available()>0){
    mensagem += Serial.readStringUntil('\n');
    mqtt.publish(OtherTopic.c_str(),mensagem.c_str());
  }
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
  if (mensagem == "Pedro: liga led" || mensagem == "Gabriel: liga led"){
    digitalWrite(ledPin, HIGH);
  } 
  if (mensagem == "Pedro: desliga led" || mensagem == "Gabriel: desliga led"){
    digitalWrite(ledPin, LOW);
  }
}
