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
	char* replyToW = jsonify(replyTo);
	char* eventDateW = jsonify(alert.getEventDate());
	sprintf_P(buffer,
			PSTR(
					"{\"hardwareId\":\"%s\",\"replyTo\":%s,\"alerts\":[{\"type\":\"%s\",\"message\":\"%s\",\"eventDate\":%s,\"metadata\":[]}]}"),
			hardwareId, replyToW, alert.getAlertType(), alert.getAlertMessage(), eventDateW);
	free(replyToW);
	free(eventDateW);
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
	char* replyToW = jsonify(replyTo);
	char* eventDateW = jsonify(location.getEventDate());

	sprintf_P(buffer,
			PSTR(
					"{\"hardwareId\":\"%s\",\"replyTo\":%s,\"locations\":[{\"latitude\":\"%s\",\"longitude\":\"%s\",\"elevation\":\"%s\",\"eventDate\":%s,\"metadata\":[]}]}"),
			hardwareId, replyToW, strLatitude, strLongitude, strElevation, eventDateW);
	free(replyToW);
	free(eventDateW);
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
	char* replyToW = jsonify(replyTo);
	char* eventDateW = jsonify(measurement.getEventDate());

	sprintf_P(buffer,
			PSTR(
					"{\"hardwareId\":\"%s\",\"replyTo\":%s,\"measurements\":[{\"measurements\":[{\"name\":\"%s\",\"value\":\"%s\"}],\"eventDate\":%s,\"metadata\":[]}]}"),
			hardwareId, replyToW, measurement.getMeasurementName(), measurement.getMeasurementValue(),
			eventDateW);
	free(replyToW);
	free(eventDateW);
}

/** Creates a new field that either holds 'null' or a quote-wrapped version of the string */
char* SiteWhere::jsonify(char* field) {
	if (field == NULL) {
		char* nullResponse = (char*) malloc(5);
		memcpy(nullResponse, "null", 4);
		nullResponse[4] = NULL;
		return nullResponse;
	}
	int length = strlen(field);
	char* quoted = (char*) malloc(length + 3);
	quoted[0] = '\"';
	memcpy(quoted + 1, field, length);
	quoted[length + 1] = '\"';
	quoted[length + 2] = NULL;
	return quoted;
}
