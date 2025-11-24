 #include <WiFiClientSecure.h>
 #include <WiFi.h>
 #include <PubSubClient.h>
 #include <Ultrasonic.h>

WiFiClientSecure client;
PubSubClient mqtt(client);

const String SSID = "FIESC_IOT_EDU";
const String PASS = "8120gv08";
const byte pino_led = 18;
const int pino_trigger1 = 19;
const int pino_echo1 = 21;
const int pino_trigger2 = 12;
const int pino_echo2 = 13;

//constantes p/ broker
const String URL   = "8b46e29e75014bcba8465b77629b065c.s1.eu.hivemq.cloud";
const int PORT     = 8883;
const String USR   = "thetrain_esp";
const String broker_PASS  = "Thetrain123";
const String MyTopic = "teste";
const String OtherTopic = "teste";
const String iluminacaoTopic = "S1/Iluminacao";
const String presenca1Topic = "S2/Presenca1";
const String presenca2Topic = "S2/Presenca2";
float distanciaPresenca1;
float distanciaPresenca2;

Ultrasonic ultrasonic(pino_trigger1, pino_echo1);
Ultrasonic ultrasonic2(pino_trigger2, pino_echo2);


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
  mqtt.setCallback(callback);
  Serial.println("\nConectado ao broker com sucesso !");

  pinMode(pino_led, OUTPUT);
}

void loop() {
  int distance = ultrasonic.read();
  int distance2 = ultrasonic2.read();

  if(distance < 30){
    mqtt.publish(presenca1Topic.c_str(),String(1).c_str());
  }else{
    mqtt.publish(presenca1Topic.c_str(),String(0).c_str());
  }

  if(distance2 < 30){
    mqtt.publish(presenca2Topic.c_str(),String(1).c_str());
  }else{
    mqtt.publish(presenca2Topic.c_str(),String(0).c_str());
  }
  
  mqtt.loop();
  delay(1000);
}

void callback(char* topic, byte* payload, unsigned int length){
  String mensagem = "";
  for(int i = 0; i < length; i++){
    mensagem += (char)payload[i];
  }
  Serial.println(mensagem);
  if(mensagem == "1"){
    digitalWrite(pino_led, HIGH);
  } else if(mensagem == "0"){
    digitalWrite(pino_led, LOW);
  }
}

