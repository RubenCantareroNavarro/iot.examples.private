#include <WiFi.h>
#include <AsyncMqttClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

#include "config.h"
#include "MQTT.hpp"
#include "ESP32_Utils.hpp"
#include "ESP32_Utils_MQTT_Async.hpp"
#include "DHT11.hpp"

void setup()
{
  Serial.begin(9600);

  delay(500);

  WiFi.onEvent(WiFiEvent);
  InitMqtt();

  ConnectWiFi_STA();
}

void loop()
{
  String payload = read_vaues();
  Serial.println(payload);

  PublishMqtt(payload);

  delay(5000);
}
