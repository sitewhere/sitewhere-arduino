/**
 * SiteWhere.h - Library for interacting with SiteWhere via MQTT.
 * Copyright 2013 Reveal Technologies LLC.
 */
#ifndef SiteWhere_h
#define SiteWhere_h

#include <Arduino.h>
#include <Client.h>
#include <PubSubClient.h>
#include "SiteWhereEvents.h"

class SiteWhere {
public:
	void createMessage(char* buffer, char* hardwareId, SiteWhereAlert& alert);
	void createMessage(char* buffer, char* hardwareId, SiteWhereAlert& alert, char* replyTo);
	void createMessage(char* buffer, char* hardwareId, SiteWhereLocation& location);
	void createMessage(char* buffer, char* hardwareId, SiteWhereLocation& location, char* replyTo);
	void createMessage(char* buffer, char* hardwareId, SiteWhereMeasurement& measurement);
	void createMessage(char* buffer, char* hardwareId, SiteWhereMeasurement& measurement, char* replyTo);
private:
	char* jsonify(char* field);
};

#endif
