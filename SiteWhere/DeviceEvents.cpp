/**
 * Implementation class for SiteWhere device events.
 */
#include "DeviceEvents.h"
#include "SiteWhere.h"
#include <Stdio.h>

/**
 * Base constructor for device events. Event date is expected to be in the format of
 * an ISO 8601 date string (e.g. 2013-09-29T10:40Z).
 */
DeviceEvent::DeviceEvent(char* eventDate) {
	this->_eventDate = eventDate;
}

/**
 * Get the event date.
 */
char* DeviceEvent::getEventDate() {
	return this->_eventDate;
}

/**
 * Create a device alert.
 */
DeviceAlert::DeviceAlert(char* type, char* message, char* eventDate) :
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
void DeviceAlert::getJSON(char* message) {
	sprintf_P(message, PSTR("{\"type\":\"%s\",\"message\":\"%s\",\"eventDate\":\"%s\",\"metadata\":[]}"),
			this->_alertType, this->_alertMessage, this->getEventDate());
}

/**
 * Create a device location based on latitude and longitude.
 */
DeviceLocation::DeviceLocation(double latitude, double longitude, char* eventDate) :
		DeviceEvent(eventDate) {
	this->_latitude = latitude;
	this->_longitude = longitude;
	this->_elevation = 0;
}

/**
 * Create a device alert.
 */
DeviceLocation::DeviceLocation(double latitude, double longitude, double elevation, char* eventDate) :
		DeviceEvent(eventDate) {
	this->_latitude = latitude;
	this->_longitude = longitude;
	this->_elevation = elevation;
}

/**
 * Get the location latitude.
 */
double DeviceLocation::getLatitude() {
	return this->_latitude;
}

/**
 * Get the location longitude.
 */
double DeviceLocation::getLongitude() {
	return this->_longitude;
}

/**
 * Get the location elevation.
 */
double DeviceLocation::getElevation() {
	return this->_elevation;
}

/**
 * Get a JSON representation of the location.
 */
void DeviceLocation::getJSON(char* message) {
	char strLatitude[16], strLongitude[16], strElevation[16];
	dtostrf(this->_latitude, 10, 8, strLatitude);
	dtostrf(this->_longitude, 10, 8, strLongitude);
	dtostrf(this->_elevation, 10, 8, strElevation);
	sprintf_P(message,
			PSTR(
					"{\"latitude\":\"%s\",\"longitude\":\"%s\",\"elevation\":\"%s\",\"eventDate\":\"%s\",\"metadata\":[]}"),
			strLatitude, strLongitude, strElevation, this->getEventDate());
}

/**
 * Create a device measurement.
 */
DeviceMeasurement::DeviceMeasurement(char* name, char* value, char* eventDate) :
		DeviceEvent(eventDate) {
	this->_measurementName = name;
	this->_measurementValue = value;
}

/**
 * Get the measurement name.
 */
char* DeviceMeasurement::getMeasurementName() {
	return this->_measurementName;
}

/**
 * Get the measurement value.
 */
char* DeviceMeasurement::getMeasurementValue() {
	return this->_measurementValue;
}

/**
 * Get a JSON representation of the measurement.
 */
void DeviceMeasurement::getJSON(char* message) {
	sprintf_P(message,
			PSTR(
					"{\"measurements\":[{\"name\":\"%s\",\"value\":\"%s\"}],\"eventDate\":\"%s\",\"metadata\":[]}"),
			this->_measurementName, this->_measurementValue, this->getEventDate());
}
