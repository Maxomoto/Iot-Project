
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

const char* ssid = "TALKTALK013DA3";
const char* password =  "CM36XCQH";
const char* mqttServer = "192.168.1.10";
const int mqttPort = 1883;
const char* mqttUser = "student";
const char* mqttPassword = "P@ssw0rd";

WiFiClient espClient;
PubSubClient client(espClient);

//Create the DHT temperature and humidity sensor object
DHT dht1(D4, DHT11);
DHT dht2(D3, DHT11);

void setup(void) {
  Serial.begin(115200);
  dht1.begin();
  dht2.begin();
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
 
  client.publish("temp1", temp_1);
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
  //Read DHT temeperature and humidity values
  float DHT11_t = dht1.readTemperature();
  float DHT11_h = dht1.readHumidity();

  string temp_1;
  temp_1 = String(DHT11_t);
  
  float DHT11_t2 = dht2.readTemperature();
  float DHT11_h2 = dht2.readHumidity();
  
  client.loop();
}











