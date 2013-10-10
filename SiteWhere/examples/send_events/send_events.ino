#include <SPI.h>
#include <PubSubClient.h>
#include <Ethernet.h>
#include <SiteWhereEvents.h>
#include <SiteWhere.h>

/**
 * Send a measurement, alert, and location to SiteWhere.
 */

/** Local MAC/IP and MQTT IP */
byte mac[]    = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte ip[]     = { 192, 168, 1, 20 };
byte mqtt[]   = { 192, 168, 1, 68 };

/** Buffer for JSON message */
char messageBuffer[225];

/** Callback function header */
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;
PubSubClient mqttClient(mqtt, 1883, callback, ethClient);
SiteWhere sitewhere;

SiteWhereMeasurement measurement("heart.rate", "100", NULL);
SiteWhereAlert alert("nurse.call", "Nurse call button pressed!", "2013-10-03T15:15:12Z");
SiteWhereLocation location(34.12142594794674, -84.20103406853741, 0, NULL);

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  if (mqttClient.connect("arduinoClient")) {
    Serial.println(F("Connected to MQTT."));
    mqttClient.subscribe("SiteWhere/response");
    sitewhere.createMessage(messageBuffer, "2394738380-MEI-001", location, "SiteWhere/response");
    mqttClient.publish("SiteWhere/input", messageBuffer);
    sitewhere.createMessage(messageBuffer, "2394738380-MEI-001", measurement, "SiteWhere/response");
    mqttClient.publish("SiteWhere/input", messageBuffer);
    sitewhere.createMessage(messageBuffer, "2394738380-MEI-001", alert, "SiteWhere/response");
    mqttClient.publish("SiteWhere/input", messageBuffer);
    Serial.println(F("Messages sent to SiteWhere successfully."));
  } else {
    Serial.println(F("Unable to connect to MQTT."));
  }
}

void loop() {
  mqttClient.loop();
}

/** Print the response message */
void handleResponse(char* topic, char* payload) {
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(payload);
}

/** Called when a response comes in from a subscribed topic */
void callback(char* topic, byte* payload, unsigned int length) {
  char* payloadCopy = (char*) malloc(length+1);
  memcpy(payloadCopy, payload, length);
  payloadCopy[length] = NULL;
  handleResponse(topic, payloadCopy);
  free(payloadCopy);
}