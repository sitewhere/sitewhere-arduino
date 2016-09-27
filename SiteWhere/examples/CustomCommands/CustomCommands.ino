#include <pb.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <sitewhere.h>
#include <sitewhere-arduino.pb.h>

#include "custom.pb.h"

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]  = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte mqtt[] = { 10, 0, 0, 32 };

/** Callback function header */
void callback(char* topic, byte* payload, unsigned int length);

/** Connectivity */
EthernetClient ethClient;
PubSubClient mqttClient(mqtt, 1883, callback, ethClient);

/** Message buffer */
uint8_t buffer[300];

/** Unqiue hardware id for this device */
static char* hardwareId = "123-ARDUINO-TEST";

/** Device specification token for hardware configuration */
static char* specificationToken = "417b36a8-21ef-4196-a8fe-cc756f994d0b";

/** Outbound MQTT topic */
static char* outbound = "SiteWhere/input/protobuf";

/** Inbound custom command topic */
static char* cmdtopic = "SiteWhere/commands/123-ARDUINO-TEST";

/** Inbound system command topic */
static char* systopic = "SiteWhere/system/123-ARDUINO-TEST";

/** Handle MQTT subscription responses */
void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, systopic) == 0) {
    handleSystemCommand(payload, length);
  } else if (strcmp(topic, cmdtopic) == 0) {
    handleSpecificationCommand(payload, length);
  }
}

/** Handle a system command */
static void handleSystemCommand(byte* payload, unsigned int length) {
    Device_Header header;
    pb_istream_t stream = pb_istream_from_buffer(payload, length);

    if (pb_decode_delimited(&stream, Device_Header_fields, &header)) {
      // Handle a registration acknowledgement.
      if (header.command == Device_Command_ACK_REGISTRATION) {
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
  ArduinoCustom__Header header;
  pb_istream_t stream = pb_istream_from_buffer(payload, length);
  if (pb_decode_delimited(&stream, ArduinoCustom__Header_fields, &header)) {
	Serial.println("Decoded header for custom command.");
	if (header.command == ArduinoCustom_Command_PING) {
	  Serial.println("Handling ping command...");
	  ArduinoCustom_ping ping;
	  if (pb_decode_delimited(&stream, ArduinoCustom_ping_fields, &ping)) {
	    handlePing(ping, header.originator);
	  }
	} else if (header.command == ArduinoCustom_Command_TESTEVENTS) {
	  Serial.println("Handling testEvents command...");
	  ArduinoCustom_testEvents testEvents;
	  if (pb_decode_delimited(&stream, ArduinoCustom_testEvents_fields, &testEvents)) {
	    handleTestEvents(testEvents, header.originator);
	  }
	} else if (header.command == ArduinoCustom_Command_SERIALPRINTLN) {
	  Serial.println("Handling serialPrintln command...");
	  ArduinoCustom_serialPrintln serialPrintln;
	  if (pb_decode_delimited(&stream, ArduinoCustom_serialPrintln_fields, &serialPrintln)) {
	    handleSerialPrintln(serialPrintln, header.originator);
	  }
	}
  }
}

/** Handle the 'ping' command */
static void handlePing(ArduinoCustom_ping ping, char* originator) {
  unsigned int len = 0;
  if (len = sw_acknowledge(hardwareId, "Ping received.", buffer, sizeof(buffer), originator)) {
    mqttClient.publish(outbound, buffer, len);
  }
}

/** Handle the 'testEvents' command */
static void handleTestEvents(ArduinoCustom_testEvents testEvents, char* originator) {
  unsigned int len = 0;
  if (len = sw_location(hardwareId, 33.755f, -84.39f, 0.0f, NULL, buffer, sizeof(buffer), originator)) {
    mqttClient.publish(outbound, buffer, len);
  }
  if (len = sw_measurement(hardwareId, "engine.temp", 25.1f, NULL, buffer, sizeof(buffer), originator)) {
    mqttClient.publish(outbound, buffer, len);
  }
  if (len = sw_alert(hardwareId, "engine.overheat", "The engine is overheating!", NULL, buffer, sizeof(buffer), originator)) {
    mqttClient.publish(outbound, buffer, len);
  }
}

/** Handle the 'serialPrintln' command */
static void handleSerialPrintln(ArduinoCustom_serialPrintln serialPrintln, char* originator) {
  unsigned int len = 0;
  Serial.println(serialPrintln.message);
  if (len = sw_acknowledge(hardwareId, "Message sent to Serial.println().", buffer, sizeof(buffer), originator)) {
    mqttClient.publish(outbound, buffer, len);
  }
}

/** Set up processing */
void setup() {
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Unable to get DHCP address."));
    return;
  }
  Serial.println(F("Connected to ethernet."));
  if (mqttClient.connect("arduinoClient")) {
    Serial.println(F("Connected to MQTT."));
    mqttClient.subscribe(cmdtopic);
    mqttClient.subscribe(systopic);

    if (sw_register(hardwareId, specificationToken, buffer, sizeof(buffer), NULL)) {
      mqttClient.publish(outbound, (char*) buffer);
    }
    Serial.println("Sent registration.");
  } else {
    Serial.println(F("Unable to connect to MQTT."));
  }
}

void loop() {
  mqttClient.loop();
}