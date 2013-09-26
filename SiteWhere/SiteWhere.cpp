/**
 * Library for interacting with SiteWhere via MQTT.
 */
#include "SiteWhere.h"
#include <PubSubClient.h>

/**
 * Create a SiteWhere instance with TCP/IP client and details about MQTT server.
 */
SiteWhere::SiteWhere(Client& client, uint8_t* mqttIp, uint16_t mqttPort) {
	this->_client = &client;
	this->_mqtt = new PubSubClient(mqttIp, mqttPort, onResponseMessage, client);
}

/**
 * Connect to the MQTT server.
 */
boolean SiteWhere::connect(char* clientId) {
	_mqtt->connect(clientId);
}

/**
 * Send a device alert to the given topic on the MQTT broker.
 */
boolean SiteWhere::sendDeviceAlert(char* topic, char* hardwareId, DeviceAlert& alert) {
	if (_mqtt->connected()) {
		char* json = alert.getJSON();
		return _mqtt->publish(topic, json);
	}
	return false;
}

/**
 * Handle processing loop for receiving messages on the socket.
 */
boolean SiteWhere::loop() {
	_mqtt->loop();
}

/**
 * Callback for response messages.
 */
void SiteWhere::onResponseMessage(char* topic, byte* payload,
		unsigned int length) {
}
