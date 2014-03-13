#include "sitewhere.h"
#include "sitewhere.pb.h"
#include "pb_encode.h"

// Signals end of stream.
uint8_t zero = 0;

// Shared header.
SiteWhere_Header header = { };

// Buffer for converting floats to strings.
char fstr[16];

boolean sw_register(char* hardwareId, char* specificationToken, uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	header.command = SiteWhere_Command_REGISTER;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Header_fields, &header)) {
		return false;
	}

	SiteWhere_RegisterDevice registerDevice = { };
	strcpy(registerDevice.hardwareId, hardwareId);
	strcpy(registerDevice.specificationToken, specificationToken);
	if (!pb_encode_delimited(&stream, SiteWhere_RegisterDevice_fields, &registerDevice)) {
		return false;
	}

	return pb_write(&stream, &zero, 1);
}

boolean sw_acknowledge(char* hardwareId, char* message, uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	header.command = SiteWhere_Command_ACKNOWLEDGE;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Header_fields, &header)) {
		return false;
	}

	SiteWhere_Acknowledge ack = { };
	strcpy(ack.hardwareId, hardwareId);
	if (message != NULL) {
		ack.has_message = true;
		strcpy(ack.message, message);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Acknowledge_fields, &ack)) {
		return false;
	}

	return pb_write(&stream, &zero, 1);
}

boolean sw_measurement(char* hardwareId, char* name, float value, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	header.command = SiteWhere_Command_DEVICEMEASUREMENT;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Header_fields, &header)) {
		return false;
	}

	SiteWhere_DeviceMeasurement measurement = { };
	strcpy(measurement.hardwareId, hardwareId);
	strcpy(measurement.measurementId, name);

	dtostrf(value, 10, 8, fstr);
	strcpy(measurement.measurementValue, fstr);

	if (eventDate != NULL) {
		measurement.has_eventDate = true;
		measurement.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, SiteWhere_DeviceMeasurement_fields, &measurement)) {
		return false;
	}

	return pb_write(&stream, &zero, 1);
}

boolean sw_location(char* hardwareId, float lat, float lon, float ele, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	header.command = SiteWhere_Command_DEVICELOCATION;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Header_fields, &header)) {
		return false;
	}

	SiteWhere_DeviceLocation location = { };
	strcpy(location.hardwareId, hardwareId);
	dtostrf(lat, 10, 8, fstr);
	strcpy(location.latitude, fstr);
	dtostrf(lon, 10, 8, fstr);
	strcpy(location.longitude, fstr);
	dtostrf(ele, 10, 8, fstr);
	strcpy(location.elevation, fstr);
	if (eventDate != NULL) {
		location.has_eventDate = true;
		location.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, SiteWhere_DeviceLocation_fields, &location)) {
		return false;
	}

	return pb_write(&stream, &zero, 1);
}

boolean sw_alert(char* hardwareId, char* alertType, char* alertMessage, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	header.command = SiteWhere_Command_DEVICEALERT;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Header_fields, &header)) {
		return false;
	}

	SiteWhere_DeviceAlert alert = { };
	strcpy(alert.hardwareId, hardwareId);
	strcpy(alert.alertType, alertType);
	strcpy(alert.alertMessage, alertMessage);
	if (eventDate != NULL) {
		alert.has_eventDate = true;
		alert.eventDate = eventDate;
	}
	if (!pb_encode_delimited(&stream, SiteWhere_DeviceAlert_fields, &alert)) {
		return false;
	}

	return pb_write(&stream, &zero, 1);
}
