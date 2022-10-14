#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "config.h"

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");
}

void connectWiFi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
}

void connectMqttBroker()
{
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);

  while (!client.connected())
  {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("\nThe client %s connects to the public mqtt broker", client_id.c_str());

    if (client.connect(client_id.c_str(), MQTT_USER, MQTT_PASSWORD))
    {
      Serial.println("Public mqtt broker connected");
    }
    else
    {
      Serial.printf("\nFiled with state %d", client.state());
      delay(2000);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  dht.begin();

  connectWiFi();
  connectMqttBroker();
}

void loop()
{
  String payload = "";
  StaticJsonDocument<200> doc;

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }

  doc["humidity"] = h;
  doc["temperature_Celsius"] = t;
  doc["temperature_Fahrenheit"] = f;
  doc["heat_index_Fahreheit"] = dht.computeHeatIndex(f, h);
  doc["heat_index_Celsius"] = dht.computeHeatIndex(t, h, false);

  serializeJson(doc, payload);
  client.publish(MQTT_TOPIC, (char *)payload.c_str());

  Serial.println(payload);

  Serial.println();
  serializeJsonPretty(doc, payload);

  delay(5000);
}