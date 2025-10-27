 #include <WiFi.h>
#include <PubSubClient.h>

WiFiClient client;
PubSubClient mqtt(client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";

const byte pin_led = 2;

//constantes p/ broker
const String URL   = "test.mosquitto.org";
const int PORT     = 1883;
const String USR   = "";
const String broker_PASS  = "";
const String MyTopic = "pega";
const String OtherTopic = "rogui";

 void setup() {
  Serial.begin(115200);
  Serial.println("Conectando ao WiFi");
  WiFi.begin(SSID, PASS);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("\nConectado com sucesso!");
  Serial.println("Conectando ao Broker");
  mqtt.setServer(URL.c_str(),PORT);
  while(!mqtt.connected()){
    String ID = "S2_";
    ID += String(random(0xffff),HEX); //cria a parte aleat.
    mqtt.connect(ID.c_str(),USR.c_str(),broker_PASS.c_str());
    Serial.print(".");
    delay(200);
  }
  mqtt.subscribe(MyTopic.c_str());
  mqtt.setCallback(callBack);
  Serial.println("\nConectado ao broker com sucesso !");

  pinMode(pin_led,OUTPUT);
}

void loop() {
  String mensagem = "Pedro: ";
  if(Serial.available()>0){
    mensagem += Serial.readStringUntil('\n');
    mqtt.publish(OtherTopic.c_str(),mensagem.c_str());
  }
  mqtt.loop();
  delay(1000);

}

void callBack(char* topic, byte* payload, unsigned int length){
  String mensagem = "";
  
  for(int i = 0; i < length; i++){
    mensagem += (char)payload[i];
  }
  Serial.print("Recebido: ");
  Serial.println(mensagem);
  if(mensagem == "Guilherme: liga led" || mensagem == "Rodrigo: liga led"){
    digitalWrite(pin_led, HIGH);
  }
  if(mensagem == "Guilherme: desliga led" || mensagem == "Rodrigo: desliga led"){
    digitalWrite(pin_led, LOW);
  }
}











