/**
 * Implementation class for SiteWhere device events.
 */
#include "DeviceEvents.h"
#include <Stdio.h>

/**
 * Base constructor for device events.
 */
DeviceEvent::DeviceEvent(time_t eventDate) {
	this->_eventDate = eventDate;
}

/**
 * Get the event date.
 */
time_t DeviceEvent::getEventDate() {
	return this->_eventDate;
}

/**
 * Create a device alert.
 */
DeviceAlert::DeviceAlert(char* type, char* message, time_t eventDate) :
		DeviceEvent(eventDate) {
	this->_alertType = type;
	this->_alertMessage = message;
}

/**
 * Get the alert type.
 */
char* DeviceAlert::getAlertType() {
	return this->_alertType;
}

/**
 * Get the alert message.
 */
char* DeviceAlert::getAlertMessage() {
	return this->_alertMessage;
}

/**
 * Get a JSON representation of the alert.
 */
char* DeviceAlert::getJSON() {
	char message[256];
	sprintf(message, "{'type': '%s'}, 'message': '%s', 'eventDate': '2013-09-26T06:40:30.362Z', 'metadata': []}",
			this->_alertType, this->_alertMessage);
	return message;
}
