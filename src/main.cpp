#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

// Wifi Credentials
const char *ssid = "******";
const char *password = "******";

// MQTT broker credentials
const char *mqttBroker = "*******";
const char *userName = "espClient";
const int port = 1883;
const char *clientId = "user001";
const char *topic = "esp32/blink";

// Pins used
int ledPin = 23;

WiFiClient clientWifi;
PubSubClient client(clientWifi);

// callback to listen for messages
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived on topic");
  Serial.println(topic);

  String message;
  for (int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }

  if (message == "ON")
  {
    digitalWrite(ledPin, HIGH);
  }
  else if (message == "OFF")
  {
    digitalWrite(ledPin, LOW);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqttBroker, port);
  client.setCallback(callback);
}

void reconnectMqtt()
{
  while (!client.connected())
  {
    Serial.println("Connecting to mqtt...");
    if (client.connect(clientId))
    {
      Serial.println("Connected to broker");
      client.subscribe(topic);
    }
    else
    {
      Serial.print("Failed with state: ");
      Serial.print(client.state());
      Serial.println();
      delay(2000);
    }
  }
}

void loop()
{
  if (!client.connected())
  {
    reconnectMqtt();
  }
  client.loop();
}