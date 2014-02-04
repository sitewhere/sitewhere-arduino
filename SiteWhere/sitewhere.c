#include "sitewhere.h"
#include "sitewhere.pb.h"
#include "pb_encode.h"

// Signals end of stream.
uint8_t zero = 0;

boolean sw_register(char* hardwareId, char* specificationToken, uint8_t* buffer, size_t length,
		char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	// Send header.
	SiteWhere_Header header = { };
	header.command = SiteWhere_Command_REGISTER;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Header_fields, &header)) {
		return false;
	}

	// Send messsage.
	SiteWhere_RegisterDevice registerDevice = { };
	strcpy(registerDevice.hardwareId, hardwareId);
	strcpy(registerDevice.specificationToken, specificationToken);
	if (!pb_encode_delimited(&stream, SiteWhere_RegisterDevice_fields, &registerDevice)) {
		return false;
	}

	return pb_write(&stream, &zero, 1);
}

boolean sw_acknowledge(char* hardwareId, uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	// Send header.
	SiteWhere_Header header = { };
	header.command = SiteWhere_Command_ACKNOWLEDGE;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Header_fields, &header)) {
		return false;
	}

	// Send messsage.
	SiteWhere_Acknowledge ack = { };
	strcpy(ack.hardwareId, hardwareId);
	if (!pb_encode_delimited(&stream, SiteWhere_Acknowledge_fields, &ack)) {
		return false;
	}

	return pb_write(&stream, &zero, 1);
}

boolean sw_location(char* hardwareId, float lat, float lon, float elevation, int64_t eventDate,
		uint8_t* buffer, size_t length, char* originator) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	// Send header.
	SiteWhere_Header header = { };
	header.command = SiteWhere_Command_DEVICELOCATION;
	if (originator != NULL) {
		header.has_originator = true;
		strcpy(header.originator, originator);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_Header_fields, &header)) {
		return false;
	}

	// Send messsage.
	SiteWhere_DeviceLocation location = { };
	strcpy(location.hardwareId, hardwareId);
	location.latitude = lat;
	location.longitude = lon;
	location.elevation = elevation;
	location.eventDate = eventDate;
	if (!pb_encode_delimited(&stream, SiteWhere_DeviceLocation_fields, &location)) {
		return false;
	}

	return pb_write(&stream, &zero, 1);
}
