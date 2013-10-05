/**
 * Library for interacting with SiteWhere via MQTT.
 */
#include "SiteWhere.h"
#include <PubSubClient.h>
#include <avr/pgmspace.h>

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
	sendDeviceAlert(topic, hardwareId, alert, NULL);
}

/**
 * Send a device alert to the given topic on the MQTT broker.
 */
boolean SiteWhere::sendDeviceAlert(char* topic, char* hardwareId, DeviceAlert& alert, char* replyTo) {
	if (_mqtt->connected()) {
		char json[MAX_JSON_SIZE];
		alert.getJSON(json);
		char message[MAX_MQTT_PAYLOAD_SIZE];
		if (replyTo != NULL) {
			sprintf_P(message, PSTR("{\"hardwareId\":\"%s\",\"replyTo\":\"%s\",\"alerts\":[%s]}"), hardwareId,
					replyTo, json);
		} else {
			sprintf_P(message, PSTR("{\"hardwareId\":\"%s\",\"alerts\":[%s]}"), hardwareId, json);
		}
		return _mqtt->publish(topic, message);
	}
	return false;
}

/**
 * Send a device location to the given topic on the MQTT broker.
 */
boolean SiteWhere::sendDeviceLocation(char* topic, char* hardwareId, DeviceLocation& location) {
	sendDeviceLocation(topic, hardwareId, location, NULL);
}

/**
 * Send a device location to the given topic on the MQTT broker.
 */
boolean SiteWhere::sendDeviceLocation(char* topic, char* hardwareId, DeviceLocation& location,
		char* replyTo) {
	if (_mqtt->connected()) {
		char json[MAX_JSON_SIZE];
		location.getJSON(json);
		char message[MAX_MQTT_PAYLOAD_SIZE];
		if (replyTo != NULL) {
			sprintf_P(message, PSTR("{\"hardwareId\":\"%s\",\"replyTo\":\"%s\",\"locations\":[%s]}"),
					hardwareId, replyTo, json);
		} else {
			sprintf_P(message, PSTR("{\"hardwareId\":\"%s\",\"locations\":[%s]}"), hardwareId, json);
		}
		return _mqtt->publish(topic, message);
	}
	return false;
}

/**
 * Send a device measurement to the given topic on the MQTT broker.
 */
boolean SiteWhere::sendDeviceMeasurement(char* topic, char* hardwareId, DeviceMeasurement& measurement) {
	sendDeviceMeasurement(topic, hardwareId, measurement, NULL);
}

/**
 * Send a device measurement to the given topic on the MQTT broker.
 */
boolean SiteWhere::sendDeviceMeasurement(char* topic, char* hardwareId, DeviceMeasurement& measurement,
		char* replyTo) {
	if (_mqtt->connected()) {
		char json[MAX_JSON_SIZE];
		measurement.getJSON(json);
		char message[MAX_MQTT_PAYLOAD_SIZE];
		if (replyTo != NULL) {
			sprintf_P(message, PSTR("{\"hardwareId\":\"%s\",\"replyTo\":\"%s\",\"measurements\":[%s]}"),
					hardwareId, replyTo, json);
		} else {
			sprintf_P(message, PSTR("{\"hardwareId\":\"%s\",\"measurements\":[%s]}"), hardwareId, json);
		}
		return _mqtt->publish(topic, message);
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
void SiteWhere::onResponseMessage(char* topic, byte* payload, unsigned int length) {
}

