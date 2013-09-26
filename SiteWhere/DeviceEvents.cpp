/**
 * Implementation class for SiteWhere device events.
 */
#include "DeviceEvents.h"
#include "SiteWhere.h"
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
void DeviceAlert::getJSON(char* message) {
	sprintf(message,
			"{\"type\":\"%s\",\"message\":\"%s\",\"eventDate\":\"2013-09-26T06:40:30.362Z\",\"metadata\":[]}",
			this->_alertType, this->_alertMessage);
}

/**
 * Create a device location based on latitude and longitude.
 */
DeviceLocation::DeviceLocation(double latitude, double longitude,
		time_t eventDate) :
		DeviceEvent(eventDate) {
	this->_latitude = latitude;
	this->_longitude = longitude;
	this->_elevation = 0;
}

/**
 * Create a device alert.
 */
DeviceLocation::DeviceLocation(double latitude, double longitude,
		double elevation, time_t eventDate) :
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
	sprintf(message,
			"{\"latitude\":\"%s\",\"longitude\":\"%s\",\"elevation\":\"%s\",\"eventDate\":\"2013-09-26T06:40:30.362Z\",\"metadata\":[]}",
			strLatitude, strLongitude, strElevation);
}

/**
 * Create a device measurement.
 */
DeviceMeasurement::DeviceMeasurement(char* name, char* value, time_t eventDate) :
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
	sprintf(message,
			"{\"measurements\":[{\"name\":\"%s\",\"value\":\"%s\"}],\"eventDate\":\"2013-09-26T06:40:30.362Z\",\"metadata\":[]}",
			this->_measurementName, this->_measurementValue);
}
