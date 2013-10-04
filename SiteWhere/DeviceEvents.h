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
	DeviceEvent(char* eventDate);
	DeviceEvent(char* eventDate, char* replyTo);
	char* getEventDate();
	char* getReplyTo();
	virtual void getJSON(char* message) = 0;
protected:
	char* _eventDate;
	char* _replyTo;
};

/**
 * Class for SiteWhere device alert data.
 */
class DeviceAlert: public DeviceEvent {
public:
	DeviceAlert(char* type, char* message, char* eventDate);
	DeviceAlert(char* type, char* message, char* eventDate, char* replyTo);
	char* getAlertType();
	char* getAlertMessage();
	void getJSON(char* message);
protected:
	char* _alertType;
	char* _alertMessage;
};

/**
 * Class for SiteWhere device location data.
 */
class DeviceLocation: public DeviceEvent {
public:
	DeviceLocation(double latitude, double longitude, char* eventDate);
	DeviceLocation(double latitude, double longitude, double elevation, char* eventDate);
	DeviceLocation(double latitude, double longitude, double elevation, char* eventDate, char* replyTo);
	double getLatitude();
	double getLongitude();
	double getElevation();
	void getJSON(char* message);
protected:
	double _latitude;
	double _longitude;
	double _elevation;
};

/**
 * Class for SiteWhere device measurements data.
 */
class DeviceMeasurement: public DeviceEvent {
public:
	DeviceMeasurement(char* name, char* value, char* eventDate);
	DeviceMeasurement(char* name, char* value, char* eventDate, char* replyTo);
	char* getMeasurementName();
	char* getMeasurementValue();
	void getJSON(char* message);
protected:
	char* _measurementName;
	char* _measurementValue;
};

#endif
