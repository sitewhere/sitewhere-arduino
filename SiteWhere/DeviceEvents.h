/**
 * DeviceEvents.h - Device events that may be sent to SiteWhere.
 * Copyright 2013 Reveal Technologies LLC.
 */
#ifndef DeviceEvents_h
#define DeviceEvents_h

#include <Time.h>

/**
 * Common base class for SiteWhere device events.
 */
class DeviceEvent {
public:
	DeviceEvent(time_t eventDate);
	time_t getEventDate();
	virtual char* getJSON() = 0;
protected:
	time_t _eventDate;
};

/**
 * Class for SiteWhere device alerts.
 */
class DeviceAlert: protected DeviceEvent {
public:
	DeviceAlert(char* type, char* message, time_t eventDate);
	char* getAlertType();
	char* getAlertMessage();
	char* getJSON();
protected:
	char* _alertType;
	char* _alertMessage;
};

#endif
