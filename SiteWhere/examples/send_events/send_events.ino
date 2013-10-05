#include <SPI.h>
#include <Ethernet.h>
#include <SiteWhere.h>
#include <PubSubClient.h>

byte mac[]    = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte ip[]     = { 192, 168, 1, 20 };
byte mqtt[]   = { 192, 168, 1, 68 };

EthernetClient ethClient;
SiteWhere sitewhere(ethClient, mqtt, 1883);
DeviceAlert alert("fire.alarm", "The house is on fire!", "2013-10-03T15:15:12Z");
DeviceLocation location(34.12377073007217, -84.19863080926007, 0, "2013-10-03T15:15:12Z");
DeviceMeasurement measurement("heart.rate", "100", "2013-10-03T15:15:12Z");

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  Serial.println("Ethernet started. Attempting connect...");
  if (sitewhere.connect("arduinoClient")) {
    Serial.println("Connected!");
    if (sitewhere.sendDeviceAlert("SiteWhere/input", "2394738380-MEI-001", alert, "SiteWhere/response")) {
      Serial.println("Sent alert!");
    }
    if (sitewhere.sendDeviceLocation("SiteWhere/input", "2394738380-MEI-001", location, "SiteWhere/response")) {
      Serial.println("Sent location!");
    }
    if (sitewhere.sendDeviceMeasurement("SiteWhere/input", "2394738380-MEI-001", measurement, "SiteWhere/response")) {
      Serial.println("Sent measurement!");
    }
  } else {
    Serial.println("Connected failed.");
  }
}

void loop() {
  sitewhere.loop();
}