
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

//Create the DHT temperature and humidity sensor object
DHT dht1(D4, DHT11);
DHT dht2(D3, DHT11);

void setup(void) {
  Serial.begin(9600);
  dht1.begin();
  dht2.begin();
}

void loop() {
  //Read DHT temeperature and humidity values
  float DHT11_t = dht1.readTemperature();
  float DHT11_h = dht1.readHumidity();

  float DHT11_t2 = dht2.readTemperature();
  float DHT11_h2 = dht2.readHumidity();
  
  Serial.print("DHT11 ");
  Serial.print(DHT11_t,1); Serial.print(String(char(176))+"C ");
  Serial.print(DHT11_h,1); Serial.println("%RH");
  Serial.println();
  
  Serial.print("DHT11_2 ");
  Serial.print(DHT11_t2,1); Serial.print(String(char(176))+"C ");
  Serial.print(DHT11_h2,1); Serial.println("%RH");
  Serial.println();


  delay(3000);
}


const char* ssid = "";//WiFi SSID
const char* password =  "";//WiFi password
const char* mqttServer = "192.168.1.10";
const int mqttPort = 1883;
const char* mqttUser = "student";
const char* mqttPassword = "P@ssw0rd";
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup_MQTT() {
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  client.publish("temp1", DHT11_t);
  client.subscribe("temp1");
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
 
void loop() {
  client.loop();
}











