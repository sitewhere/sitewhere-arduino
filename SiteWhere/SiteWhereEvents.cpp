/**
 * Implementation class for SiteWhere device events.
 */
#include "SiteWhereEvents.h"

/**
 * Base constructor for device events. Event date is expected to be in the format of
 * an ISO 8601 date string (e.g. 2013-09-29T10:40Z).
 */
SiteWhereEvent::SiteWhereEvent(char* eventDate) {
	this->_eventDate = eventDate;
}

/**
 * Get the event date.
 */
char* SiteWhereEvent::getEventDate() {
	return this->_eventDate;
}

/**
 * Set the event date.
 */
void SiteWhereEvent::setEventDate(char* date) {
	this->_eventDate = date;
}

/**
 * Create a device alert.
 */
SiteWhereAlert::SiteWhereAlert(char* type, char* message, char* eventDate) :
		SiteWhereEvent(eventDate) {
	this->_alertType = type;
	this->_alertMessage = message;
}

/**
 * Get the alert type.
 */
char* SiteWhereAlert::getAlertType() {
	return this->_alertType;
}

/**
 * Set the alert type.
 */
void SiteWhereAlert::setAlertType(char* type) {
	this->_alertType = type;
}

/**
 * Get the alert message.
 */
char* SiteWhereAlert::getAlertMessage() {
	return this->_alertMessage;
}

/**
 * Set the alert message.
 */
void SiteWhereAlert::setAlertMessage(char* message) {
	this->_alertMessage = message;
}

/**
 * Create a device location based on latitude and longitude.
 */
SiteWhereLocation::SiteWhereLocation(double latitude, double longitude, char* eventDate) :
		SiteWhereEvent(eventDate) {
	this->_latitude = latitude;
	this->_longitude = longitude;
	this->_elevation = 0;
}

/**
 * Create a device alert.
 */
SiteWhereLocation::SiteWhereLocation(double latitude, double longitude, double elevation, char* eventDate) :
		SiteWhereEvent(eventDate) {
	this->_latitude = latitude;
	this->_longitude = longitude;
	this->_elevation = elevation;
}

/**
 * Get the location latitude.
 */
double SiteWhereLocation::getLatitude() {
	return this->_latitude;
}

/**
 * Set the location latitude.
 */
void SiteWhereLocation::setLatitude(double latitude) {
	this->_latitude = latitude;
}

/**
 * Get the location longitude.
 */
double SiteWhereLocation::getLongitude() {
	return this->_longitude;
}

/**
 * Set the location longitude.
 */
void SiteWhereLocation::setLongitude(double longitude) {
	this->_longitude = longitude;
}

/**
 * Get the location elevation.
 */
double SiteWhereLocation::getElevation() {
	return this->_elevation;
}

/**
 * Set the location elevation.
 */
void SiteWhereLocation::setElevation(double elevation) {
	this->_elevation = elevation;
}

/**
 * Create a device measurement.
 */
SiteWhereMeasurement::SiteWhereMeasurement(char* name, char* value, char* eventDate) :
		SiteWhereEvent(eventDate) {
	this->_measurementName = name;
	this->_measurementValue = value;
}

/**
 * Get the measurement name.
 */
char* SiteWhereMeasurement::getMeasurementName() {
	return this->_measurementName;
}

/**
 * Get the measurement value.
 */
char* SiteWhereMeasurement::getMeasurementValue() {
	return this->_measurementValue;
}
