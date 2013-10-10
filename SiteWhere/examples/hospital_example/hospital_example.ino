#include <SPI.h>
#include <PubSubClient.h>
#include <Ethernet.h>
#include <SiteWhereEvents.h>
#include <SiteWhere.h>

/**
 * Send a heart rate measurement to SiteWhere at a given interval and an alert
 * if the "nurse call" button is pressed.
 */

/** Pin locations on the Arduino */
const int BUTTON_PIN = 2;
const int LED_PIN =  13;

/** Keep from sending messages too often. Intervals are in milliseconds */
const int HEART_RATE_SEND_INTERVAL = 5000;
const int NURSE_CALL_MIN_INTERVAL = 1000;

/** Unique hardware id known by SiteWhere */
char HARDWARE_ID[] = "2394738380-MEI-001";

/** Topic used for sending/receiving data from MQTT */
char SITEWHERE_TOPIC[] = "SiteWhere/input";
char RESPONSE_TOPIC[] = "SiteWhere/response";

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
  pinMode(LED_PIN, OUTPUT);      
  pinMode(BUTTON_PIN, INPUT);
  
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  if (mqttClient.connect("arduinoClient")) {
    Serial.println(F("Connected to MQTT."));
    mqttClient.subscribe(RESPONSE_TOPIC);
    sitewhere.createMessage(messageBuffer, HARDWARE_ID, location, RESPONSE_TOPIC);
    mqttClient.publish(SITEWHERE_TOPIC, messageBuffer);
  } else {
    Serial.println(F("Unable to connect to MQTT."));
  }
}

/**
 * Main message loop. This is executed repeatedly by the hardware.
 */
void loop() {
  // Find whether the button is pressed.
  buttonState = digitalRead(BUTTON_PIN);
  
  // If we have waited longer than the interval, send a new measurement.
  if ((millis() - lastMeasurementTime) > HEART_RATE_SEND_INTERVAL) {
    lastMeasurementTime = millis();
    char heartRateStr[5];
    sprintf(heartRateStr, "%d", currentHeartRate);
    measurement.setMeasurementValue(heartRateStr);
    sitewhere.createMessage(messageBuffer, HARDWARE_ID, measurement, RESPONSE_TOPIC);
    mqttClient.publish(SITEWHERE_TOPIC, messageBuffer);
    
    // Bounce heart rate between min and max.
    currentHeartRate += delta;
   if ((currentHeartRate < MIN_HEART_RATE) || (currentHeartRate > MAX_HEART_RATE)) {
      delta = -delta;
      currentHeartRate += delta;
    }
  }

  // If we have waited longer than the alert interval, send an alert message.
  if ((buttonState == HIGH) && ((millis() - lastAlertTime) > NURSE_CALL_MIN_INTERVAL)) {
    lastAlertTime = millis();
    sitewhere.createMessage(messageBuffer, HARDWARE_ID, alert, RESPONSE_TOPIC);
    mqttClient.publish(SITEWHERE_TOPIC, messageBuffer);
  }

  // Check for MQTT responses. They will be handled in the callback.
  mqttClient.loop();
}

/** Handle the response message */
void handleResponse(char* topic, char* payload) {
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(payload);
  if (strcmp(topic, RESPONSE_TOPIC) == 0) {
    if (strcmp(payload, "A:FLASH") == 0) {
      digitalWrite(LED_PIN, HIGH);
      delay(1000);
      digitalWrite(LED_PIN, LOW); 
    }
  }
}

/** 
 * Called when a response comes in from a subscribed topic. This code just creates a copy as
 * a string an executes handleResponse() with it.
 */
void callback(char* topic, byte* payload, unsigned int length) {
  char* payloadCopy = (char*) malloc(length+1);
  memcpy(payloadCopy, payload, length);
  payloadCopy[length] = NULL;
  handleResponse(topic, payloadCopy);
  free(payloadCopy);
}