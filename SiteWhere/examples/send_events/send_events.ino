#include <SPI.h>
#include <Time.h>
#include <Ethernet.h>
#include <SiteWhere.h>
#include <PubSubClient.h>

byte mac[]    = { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte ip[]     = { 192, 168, 1, 20 };
byte mqtt[]   = { 192, 168, 1, 68 };

EthernetClient ethClient;
SiteWhere sitewhere(ethClient, mqtt, 1883);
DeviceAlert alert("fire.alarm", "The house is on fire!", 0);
DeviceLocation location(34.12377073007217, -84.19863080926007, 0);
DeviceMeasurement measurement("heart.rate", "100", 0);

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  Serial.println("Ethernet started. Attempting connect...");
  if (sitewhere.connect("arduinoClient")) {
    Serial.println("Connected!");
    if (sitewhere.sendDeviceAlert("SiteWhere/input", "2394738380-MEI-001", alert)) {
      Serial.println("Sent alert!");
    }
    if (sitewhere.sendDeviceLocation("SiteWhere/input", "2394738380-MEI-001", location)) {
      Serial.println("Sent location!");
    }
    if (sitewhere.sendDeviceMeasurement("SiteWhere/input", "2394738380-MEI-001", measurement)) {
      Serial.println("Sent measurement!");
    }
  } else {
    Serial.println("Connected failed.");
  }
}

void loop() {
  sitewhere.loop();
}