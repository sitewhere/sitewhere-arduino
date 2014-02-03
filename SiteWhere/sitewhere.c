#include "sitewhere.h"
#include "sitewhere.pb.h"
#include "pb_encode.h"

// Signals end of stream.
uint8_t zero = 0;

boolean sw_register(char* hardwareId, char* specificationToken, char* replyTo, uint8_t* buffer, size_t length) {
	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);

	// Send header.
	SiteWhere_Header header = {};
	header.command = SiteWhere_Command_REGISTER;
	if (!pb_encode_delimited(&stream, SiteWhere_Header_fields, &header)) {
		return false;
	}

	// Send messsage.
	SiteWhere_RegisterDevice registerDevice = {};
	strcpy(registerDevice.hardwareId, hardwareId);
	strcpy(registerDevice.specificationToken, specificationToken);
	if (replyTo != NULL) {
		registerDevice.has_replyTo = true;
		strcpy(registerDevice.replyTo, replyTo);
	}
	if (!pb_encode_delimited(&stream, SiteWhere_RegisterDevice_fields, &registerDevice)) {
		return false;
	}

	return pb_write(&stream, &zero, 1);
}
