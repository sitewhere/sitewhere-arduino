/**
 * Library for interacting with SiteWhere via MQTT.
 */
#include "SiteWhere.h"
#include <Stdio.h>
#include <PubSubClient.h>
#include <avr/pgmspace.h>

/**
 * Create a JSON message for the given alert.
 */
void SiteWhere::createMessage(char* buffer, char* hardwareId, SiteWhereAlert& alert) {
	return createMessage(buffer, hardwareId, alert, NULL);
}

/**
 * Create a JSON message for the given alert.
 */
void SiteWhere::createMessage(char* buffer, char* hardwareId, SiteWhereAlert& alert, char* replyTo) {
	if (replyTo != NULL) {
		sprintf_P(buffer,
				PSTR(
						"{\"hardwareId\":\"%s\",\"replyTo\":\"%s\",\"alerts\":[{\"type\":\"%s\",\"message\":\"%s\",\"eventDate\":\"%s\",\"metadata\":[]}]}"),
				hardwareId, replyTo, alert.getAlertType(), alert.getAlertMessage(), alert.getEventDate());
	} else {
		sprintf_P(buffer,
				PSTR(
						"{\"hardwareId\":\"%s\",\"alerts\":[{\"type\":\"%s\",\"message\":\"%s\",\"eventDate\":\"%s\",\"metadata\":[]}]}"),
				hardwareId, alert.getAlertType(), alert.getAlertMessage(), alert.getEventDate());
	}
}

/**
 * Create a JSON message for the given location.
 */
void SiteWhere::createMessage(char* buffer, char* hardwareId, SiteWhereLocation& location) {
	return createMessage(buffer, hardwareId, location, NULL);
}

/**
 * Create a JSON message for the given location.
 */
void SiteWhere::createMessage(char* buffer, char* hardwareId, SiteWhereLocation& location, char* replyTo) {
	char strLatitude[16], strLongitude[16], strElevation[16];
	dtostrf(location.getLatitude(), 10, 8, strLatitude);
	dtostrf(location.getLongitude(), 10, 8, strLongitude);
	dtostrf(location.getElevation(), 10, 8, strElevation);

	if (replyTo != NULL) {
		sprintf_P(buffer,
				PSTR(
						"{\"hardwareId\":\"%s\",\"replyTo\":\"%s\",\"locations\":[{\"latitude\":\"%s\",\"longitude\":\"%s\",\"elevation\":\"%s\",\"eventDate\":\"%s\",\"metadata\":[]}]}"),
				hardwareId, replyTo, strLatitude, strLongitude, strElevation, location.getEventDate());
	} else {
		sprintf_P(buffer,
				PSTR(
						"{\"hardwareId\":\"%s\",\"locations\":[{\"latitude\":\"%s\",\"longitude\":\"%s\",\"elevation\":\"%s\",\"eventDate\":\"%s\",\"metadata\":[]}]}"),
				hardwareId, strLatitude, strLongitude, strElevation, location.getEventDate());
	}
}

/**
 * Create a JSON message for the given measurement.
 */
void SiteWhere::createMessage(char* buffer, char* hardwareId, SiteWhereMeasurement& measurement) {
	return createMessage(buffer, hardwareId, measurement, NULL);
}

/**
 * Create a JSON message for the given measurement.
 */
void SiteWhere::createMessage(char* buffer, char* hardwareId, SiteWhereMeasurement& measurement,
		char* replyTo) {
	if (replyTo != NULL) {
		sprintf_P(buffer,
				PSTR(
						"{\"hardwareId\":\"%s\",\"replyTo\":\"%s\",\"measurements\":[{\"measurements\":[{\"name\":\"%s\",\"value\":\"%s\"}],\"eventDate\":\"%s\",\"metadata\":[]}]}"),
				hardwareId, replyTo, measurement.getMeasurementName(), measurement.getMeasurementValue(),
				measurement.getEventDate());
	} else {
		sprintf_P(buffer,
				PSTR(
						"{\"hardwareId\":\"%s\",\"measurements\":[{\"measurements\":[{\"name\":\"%s\",\"value\":\"%s\"}],\"eventDate\":\"%s\",\"metadata\":[]}]}"),
				hardwareId, measurement.getMeasurementName(), measurement.getMeasurementValue(),
				measurement.getEventDate());
	}
}
