
Š
sitewhere-arduino.proto"Ý
Model4
Metadata
name (	Rname
value (	RvalueÓ
DeviceLocation

hardwareId (	R
hardwareId
latitude (Rlatitude
	longitude (R	longitude
	elevation (R	elevation
	eventDate (R	eventDate+
metadata (2.Model.MetadataRmetadataº
DeviceAlert

hardwareId (	R
hardwareId
	alertType (	R	alertType"
alertMessage (	RalertMessage
	eventDate (R	eventDate+
metadata (2.Model.MetadataRmetadata_
Measurement$
measurementId (	RmeasurementId*
measurementValue (RmeasurementValueµ
DeviceMeasurements

hardwareId (	R
hardwareId4
measurement (2.Model.MeasurementRmeasurement
	eventDate (R	eventDate+
metadata (2.Model.MetadataRmetadata™
DeviceStream

hardwareId (	R
hardwareId
streamId (	RstreamId 
contentType (	RcontentType+
metadata (2.Model.MetadataRmetadataÕ
DeviceStreamData

hardwareId (	R
hardwareId
streamId (	RstreamId&
sequenceNumber (RsequenceNumber
data (Rdata
	eventDate (R	eventDate+
metadata (2.Model.MetadataRmetadata"º
	SiteWhereV
Header,
command (2.SiteWhere.CommandRcommand

originator (	R
originator«
RegisterDevice

hardwareId (	R
hardwareId.
specificationToken (	RspecificationToken+
metadata (2.Model.MetadataRmetadata
	siteToken (	R	siteTokenG
Acknowledge

hardwareId (	R
hardwareId
message (	Rmessage}
DeviceStreamDataRequest

hardwareId (	R
hardwareId
streamId (	RstreamId&
sequenceNumber (RsequenceNumber"Þ
Command
SEND_REGISTRATION
SEND_ACKNOWLEDGEMENT
SEND_DEVICE_LOCATION
SEND_DEVICE_ALERT
SEND_DEVICE_MEASUREMENTS
SEND_DEVICE_STREAM
SEND_DEVICE_STREAM_DATA
REQUEST_DEVICE_STREAM_DATA"š
Device“
Header)
command (2.Device.CommandRcommand

originator (	R
originator

nestedPath (	R
nestedPath

nestedSpec (	R
nestedSpec¥
RegistrationAck2
state (2.Device.RegistrationAckStateRstate:
	errorType (2.Device.RegistrationAckErrorR	errorType"
errorMessage (	RerrorMessagea
DeviceStreamAck
streamId (	RstreamId2
state (2.Device.DeviceStreamAckStateRstate"V
Command
ACK_REGISTRATION
ACK_DEVICE_STREAM
RECEIVE_DEVICE_STREAM_DATA"\
RegistrationAckState
NEW_REGISTRATION
ALREADY_REGISTERED
REGISTRATION_ERROR"g
RegistrationAckError
INVALID_SPECIFICATION
SITE_TOKEN_REQUIRED
NEW_DEVICES_NOT_ALLOWED"P
DeviceStreamAckState
STREAM_CREATED
STREAM_EXISTS
STREAM_FAILEDB5
1com.sitewhere.device.communication.protobuf.protoH