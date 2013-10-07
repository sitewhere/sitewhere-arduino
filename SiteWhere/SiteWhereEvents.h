/**
 * SiteWhereEvents.h - Device events that may be sent to SiteWhere.
 * Copyright 2013 Reveal Technologies LLC.
 */
#ifndef SiteWhereEvents_h
#define SiteWhereEvents_h

#include <Time.h>

/**
 * Common base class for SiteWhere device events.
 */
class SiteWhereEvent {
public:
	SiteWhereEvent(char* eventDate);
	char* getEventDate();
	void setEventDate(char* date);
protected:
	char* _eventDate;
};

/**
 * Class for SiteWhere device alert data.
 */
class SiteWhereAlert: public SiteWhereEvent {
public:
	SiteWhereAlert(char* type, char* message, char* eventDate);
	char* getAlertType();
	char* getAlertMessage();
	void setAlertType(char* type);
	void setAlertMessage(char* message);
protected:
	char* _alertType;
	char* _alertMessage;
};

/**
 * Class for SiteWhere device location data.
 */
class SiteWhereLocation: public SiteWhereEvent {
public:
	SiteWhereLocation(double latitude, double longitude, char* eventDate);
	SiteWhereLocation(double latitude, double longitude, double elevation, char* eventDate);
	double getLatitude();
	double getLongitude();
	double getElevation();
	void setLatitude(double latitude);
	void setLongitude(double longitude);
	void setElevation(double elevation);
	void getJSON(char* message);
protected:
	double _latitude;
	double _longitude;
	double _elevation;
};

/**
 * Class for SiteWhere device measurements data.
 */
class SiteWhereMeasurement: public SiteWhereEvent {
public:
	SiteWhereMeasurement(char* name, char* value, char* eventDate);
	char* getMeasurementName();
	char* getMeasurementValue();
	void setMeasurementName(char* name);
	void setMeasurementValue(char* value);
protected:
	char* _measurementName;
	char* _measurementValue;
};

#endif
