#include "DHT.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define WIFI_AP ""//WiFi SSID
#define WIFI_PASSWORD ""//WiFi password

#define TOKEN "ESP8266_DEMO_TOKEN"

// DHT
#define DHTPIN D2
#define DHTTYPE DHT11
#define DHTPIN2 D3
#define DHTTYPE2 DHT11

char server[] = "192.168.1.10";//IoT Hub address

WiFiClient wifiClient;

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE2);

PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;
unsigned long lastSend;

void setup()
{
  Serial.begin(115200);
  dht.begin();
  delay(10);
  InitWiFi();
  client.setServer( server, 1883 );
  lastSend = 0;
}

void loop()
{
  if ( !client.connected() ) {
    reconnect();
  }

  if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
    getAndSendTemperatureAndHumidityData();
    lastSend = millis();
  }

  client.loop();
}

void getAndSendTemperatureAndHumidityData()
{
  Serial.println("Collecting temperature data.");

  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");

  String temperature = String(t);
  String humidity = String(h);

  Serial.print("Humidity: ");
  Serial.print(h2);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t2);
  Serial.print(" *C ");
  
  if (t2>t)
  {
    digitalWrite(4, HIGH);
  }
  else if (t2<t)
  {
    digitalWrite(4, HIGH);
  }
  else if (t2=t)
  {
    digitalWrite(4, LOW);
  }

  String temperature2 = String(t2);
  String humidity2 = String(h2);

  // Just debug messages
  Serial.print( "Sending temperature and humidity : [" );
  Serial.print( temperature ); Serial.print( "," );
  Serial.print( humidity );
  Serial.print( "]   -> " );

    Serial.print( "Sending temperature and humidity : [" );
  Serial.print( temperature2 ); Serial.print( "," );
  Serial.print( humidity2 );
  Serial.print( "]   -> " );


  // Prepare a JSON payload string
  String payload = "Temperature 1: "; payload += temperature; payload +="*C  , ";
  payload += "Humidity 1: "; payload += humidity; payload +=" %\t";

  String payload2 = "Temperature 2: "; payload2 += temperature2; payload2 += "*C  , ";
  payload2 += "Humidity 2: "; payload2 += humidity2; payload2 +=" %\t";

  // Send payload
  char attributes[100];
  payload.toCharArray( attributes, 100 );
  client.publish( "temp1", attributes );
  Serial.println( attributes );

   char attributes2[100];
  payload2.toCharArray( attributes2, 100 );
  client.publish( "temp2", attributes2 );
  Serial.println( attributes2 );


}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
    Serial.print("Connecting to ThingsBoard node ...");
    // Attempt to connect (clientId, username, password)
    if ( client.connect("ESP8266 Device", TOKEN, NULL) ) {
      Serial.println( "[DONE]" );
    } else {
      Serial.print( "[FAILED] [ rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
