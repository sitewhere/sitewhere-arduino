#include <pb.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <sitewhere.h>
#include <sitewhere-arduino.pb.h>

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]  = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
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
static char* hardwareId = "123-TEST-234567";

/** Device specification token for hardware configuration */
static char* specificationToken = "82043707-9e3d-441f-bdcc-33cf0f4f7260";

/** Outbound MQTT topic */
static char* outbound = "SiteWhere/input/protobuf";

/** Inbound custom command topic */
static char* command = "SiteWhere/commands/123-TEST-234567";

/** Inbound system command topic */
static char* system = "SiteWhere/system/123-TEST-234567";

/** Handle MQTT subscription responses */
void callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, system) == 0) {
    handleSystemCommand(payload, length);
  } else if (strcmp(topic, command) == 0) {
    // No custom commands in this example.
  }
}

/** Handle a system command */
static void handleSystemCommand(byte* payload, unsigned int length) {
    Device_Header header;
    pb_istream_t stream = pb_istream_from_buffer(payload, length);

    if (pb_decode_delimited(&stream, Device_Header_fields, &header)) {
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
          
          // If we registered successfully, send test messages for event types.
          if (ack.state != Device_RegistrationAckState_REGISTRATION_ERROR) {
            testEvents();
          }
        }
      }
    } else {
      Serial.println(F("Unable to decode system command."));
    }
}

/** Test each event type */
static void testEvents() {
  unsigned int len = 0;
  if (len = sw_acknowledge(hardwareId, "Ack from Arduino", buffer, sizeof(buffer), NULL)) {
    mqttClient.publish(outbound, buffer, len);
  }
  if (len = sw_location(hardwareId, 33.755f, -84.39f, 0.1f, NULL, buffer, sizeof(buffer), NULL)) {
    mqttClient.publish(outbound, buffer, len);
  }
  if (len = sw_measurement(hardwareId, "engine.temp", 25.1f, NULL, buffer, sizeof(buffer), NULL)) {
    mqttClient.publish(outbound, buffer, len);
  }
  if (len = sw_alert(hardwareId, "engine.overheat", "The engine is overheating!", NULL, buffer, sizeof(buffer), NULL)) {
    mqttClient.publish(outbound, buffer, len);
  }
}

/** Set up processing */
void setup() {
  Serial.begin(9600);
  Serial.println(F("Starting up ethernet..."));
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Unable to get DHCP address."));
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