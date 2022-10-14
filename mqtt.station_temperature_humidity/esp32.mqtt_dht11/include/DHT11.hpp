#pragma once

DHT dht(DHTPIN, DHTTYPE);

String read_vaues()
{
    StaticJsonDocument<200> doc;
    String payload = "";

    doc["humidity"] = dht.readHumidity();
    doc["temperature_Celsius"] = dht.readTemperature();
    doc["temperature_Fahrenheit"] = dht.readTemperature(true);

    doc["heat_index_Fahreheit"] = dht.computeHeatIndex(doc["temperature_Celsius"], doc["humidity"], false);
    doc["heat_index_Celsius"] = dht.computeHeatIndex(doc["temperature_Fahrenheit"], doc["humidity"]);

    serializeJson(doc, payload);

    return payload;
}