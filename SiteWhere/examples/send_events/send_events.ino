#include <SPI.h>
#include <PubSubClient.h>
#include <Ethernet.h>
#include <SiteWhereEvents.h>
#include <SiteWhere.h>

/**
 * Send message to SiteWhere when a button is pressed.
 */

/** Constants */
const int BUTTON_PIN = 2;
const int LED_PIN =  13;
const int HEART_RATE_SEND_INTERVAL = 10000;
const int NURSE_CALL_MIN_INTERVAL = 1000;

/** Global variables */
int buttonState = 0;
long lastMeasurementTime = -HEART_RATE_SEND_INTERVAL;
long lastAlertTime = -NURSE_CALL_MIN_INTERVAL;

/** Local MAC/IP and MQTT IP */
byte mac[]    = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte ip[]     = { 192, 168, 1, 20 };
byte mqtt[]   = { 192, 168, 1, 68 };

/** Variables for simulating heart rate */
const int MIN_HEART_RATE = 60;
const int MAX_HEART_RATE = 170;
int delta = 5;
int currentHeartRate = MIN_HEART_RATE;

/** Buffer for JSON message */
char messageBuffer[250];

/** Callback function header */
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;
PubSubClient mqttClient(mqtt, 1883, callback, ethClient);
SiteWhere sitewhere;

SiteWhereMeasurement measurement("heart.rate", "100", "2013-10-03T15:15:12Z");
SiteWhereAlert alert("nurse.call", "Nurse call button pressed!", "2013-10-03T15:15:12Z");

void setup() {
  pinMode(LED_PIN, OUTPUT);      
  pinMode(BUTTON_PIN, INPUT);
  
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  if (mqttClient.connect("arduinoClient")) {
    Serial.println(F("Connected to MQTT."));
    if (!mqttClient.subscribe("SiteWhere/response")) {
      Serial.println(F("Failed on subscribe to response topic."));
    }
  } else {
    Serial.println(F("Unable to connect to SiteWhere via MQTT."));
  }
}

void loop() {
  buttonState = digitalRead(BUTTON_PIN);
  
  if ((millis() - lastMeasurementTime) > HEART_RATE_SEND_INTERVAL) {
    lastMeasurementTime = millis();
    sitewhere.createMessage(messageBuffer, "2394738380-MEI-001", measurement, "SiteWhere/response");
    mqttClient.publish("SiteWhere/input", messageBuffer);
  }

  if ((buttonState == HIGH) && ((millis() - lastAlertTime) > NURSE_CALL_MIN_INTERVAL)) {
    lastAlertTime = millis();
    sitewhere.createMessage(messageBuffer, "2394738380-MEI-001", alert, "SiteWhere/response");
    mqttClient.publish("SiteWhere/input", messageBuffer);
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW); 
  }

  mqttClient.loop();
}

/** Handle the response message */
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