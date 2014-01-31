#include "sitewhere.h"
#include "sitewhere.pb.h"
#include "pb_encode.h"

boolean sw_register(char* hardwareId, char* specificationToken, char* replyTo, uint8_t* buffer, size_t length) {
	sitewhere sw = { };
	sw._command = sitewhere_Command_REGISTER;
	sw.has_registerDevice = true;
	sitewhere__type_registerDevice registerDevice = { };
	strcpy(registerDevice.hardwareId, hardwareId);
	strcpy(registerDevice.specificationToken, specificationToken);
	if (replyTo != NULL) {
		registerDevice.has_replyTo = true;
		strcpy(registerDevice.replyTo, replyTo);
	}

	sw.registerDevice = registerDevice;

	pb_ostream_t stream = pb_ostream_from_buffer(buffer, length);
	return pb_encode(&stream, sitewhere_fields, &sw);
}
