/**
 * Library for interacting with SiteWhere via MQTT.
 */
#include "SiteWhere.h"
#include <PubSubClient.h>

SiteWhere::SiteWhere(Client& client, uint8_t* mqttIp, uint16_t mqttPort) {
	this->_client = &client;
	this->_mqtt = new PubSubClient(mqttIp, mqttPort, onResponseMessage, client);
}

boolean SiteWhere::connect(char* clientId) {
	_mqtt->connect(clientId);
}

boolean SiteWhere::loop() {
	_mqtt->loop();
}

void SiteWhere::onResponseMessage(char* topic, byte* payload, unsigned int length) {
}
