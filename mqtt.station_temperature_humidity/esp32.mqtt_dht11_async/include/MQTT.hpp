#pragma once

AsyncMqttClient mqttClient;

String GetPayloadContent(char *data, size_t len)
{
	String content = "";
	for (size_t i = 0; i < len; i++)
	{
		content.concat(data[i]);
	}
	return content;
}

void SuscribeMqtt()
{
	return;
}

void PublishMqtt(String payload)
{

	mqttClient.publish(MQTT_TOPIC, 0, true, (char *)payload.c_str());
}

void OnMqttReceived(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
	return;
}