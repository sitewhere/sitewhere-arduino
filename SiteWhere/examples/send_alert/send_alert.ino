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

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  Serial.println("Ethernet started. Attempting connect...");
  if (sitewhere.connect("arduinoClient")) {
    Serial.println("Connected!");
    if (sitewhere.sendDeviceAlert("SiteWhere/input", "2394738380-MEI-001", alert)) {
      Serial.println("Sent alert!");
    }
  } else {
    Serial.println("Connected failed.");
  }
}

void loop() {
  sitewhere.loop();
}