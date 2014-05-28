#include <pb.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <sitewhere.h>
#include <sitewhere-arduino.pb.h>

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]  = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte mqtt[] = { 192, 168, 1, 68 };

/** Callback function header */
void callback(char* topic, byte* payload, unsigned int length);

/** Connectivity */
EthernetClient ethClient;
PubSubClient mqttClient(mqtt, 1883, callback, ethClient);

/** Message buffer */
uint8_t buffer[300];

/** MQTT client name */
static char* clientName = "arduinoClient";

/** Unique hardware id for this device */
static char* hardwareId = "123-GATEWAY-234567";

/** Device specification token for hardware configuration */
static char* specificationToken = "75126a52-0607-4cca-b995-df40e73a707b";

/** Outbound MQTT topic */
static char* outbound = "SiteWhere/input/protobuf";

/** Inbound custom command topic */
static char* command = "SiteWhere/command/123-GATEWAY-234567";

/** Inbound system command topic */
static char* system = "SiteWhere/system/123-GATEWAY-234567";

/** Handle MQTT subscription responses */
void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, system) == 0) {
    handleSystemCommand(payload, length);
  } else if (strcmp(topic, command) == 0) {
    handleSpecificationCommand(payload, length);
  }
}

/** Handle a system command */
static void handleSystemCommand(byte* payload, unsigned int length) {
  Device_Header header;
  pb_istream_t stream = pb_istream_from_buffer(payload, length);

  // Read header to find what type of command follows.
  if (pb_decode_delimited(&stream, Device_Header_fields, &header)) {
  
    // Handle a registration acknowledgement.
    if (header.command == Device_Command_REGISTER_ACK) {
      Device_RegistrationAck ack;
      if (pb_decode_delimited(&stream, Device_RegistrationAck_fields, &ack)) {
        if (ack.state == Device_RegistrationAckState_NEW_REGISTRATION) {
          Serial.println(F("Registered new device."));
        } else if (ack.state == Device_RegistrationAckState_ALREADY_REGISTERED) {
          Serial.println(F("Device was already registered."));
        } else if (ack.state == Device_RegistrationAckState_REGISTRATION_ERROR) {
          Serial.println(F("Error registering device."));
        }
      }
    }
  } else {
    Serial.println(F("Unable to decode system command."));
  }
}

/** Handle a command specific to the specification for this device */
static void handleSpecificationCommand(byte* payload, unsigned int length) {
  Device_Header header;
  pb_istream_t stream = pb_istream_from_buffer(payload, length);
	
  // Use the generic device header since it is API compatible with all command headers.
  if (pb_decode_delimited(&stream, Device_Header_fields, &header)) {
    
    Serial.println("Decoded header for specification command.");
    if (header.has_nestedPath && header.has_nestedSpec) {
      handleNestedCommand(header.nestedPath, header.nestedSpec, &header);
    } else {
      Serial.println(F("Command did not address a nested device."));
    }
  }
}

/** Execute a nested command based on relative path, device specification, and header */
static void handleNestedCommand(char* path, char* specification, void* header) {
  Serial.print(F("Relative path for nested device: "));
  Serial.println(path);
  Serial.print(F("Specification for nested device: "));
  Serial.println(specification);
}

/** Set up processing */
void setup() {
  Serial.begin(9600);
  Serial.println(F("Starting up ethernet..."));
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Unable to get initialize ethernet."));
    return;
  }
  Serial.println(F("Connected to ethernet."));
  if (mqttClient.connect(clientName)) {
    Serial.println(F("Connected to MQTT."));
    
    // Subscribe to command topics.
    mqttClient.subscribe(command);
    mqttClient.subscribe(system);

    // Register device with SiteWhere.
    if (sw_register(hardwareId, specificationToken, buffer, sizeof(buffer), NULL)) {
      mqttClient.publish(outbound, (char*) buffer);
    }
    Serial.println("Sent registration.");
  } else {
    Serial.println(F("Unable to connect to MQTT."));
  }
}

/** Main MQTT processing loop */
void loop() {
  mqttClient.loop();
}