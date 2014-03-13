#include <pb.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <sitewhere.h>
#include <sitewhere.pb.h>

#include "uno.h"

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]  = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
byte mqtt[] = { 54, 80, 46, 154 };

/** Callback function header */
void callback(char* topic, byte* payload, unsigned int length);

/** Connectivity */
EthernetClient ethClient;
PubSubClient mqttClient(mqtt, 1883, callback, ethClient);

/** Message buffer */
uint8_t buffer[128];

static char* hardwareId = "90970e47-cfc5-40e6-ad1f-68a9146de179";
static char* specificationToken = "7dfd6d63-5e8d-4380-be04-fc5c73801dfb";
static char* outbound = "SiteWhere/input/protobuf";
static char* command = "SiteWhere/commands/90970e47-cfc5-40e6-ad1f-68a9146de179";
static char* system = "SiteWhere/system/90970e47-cfc5-40e6-ad1f-68a9146de179";

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
        }
      }
    } else {
      Serial.println(F("Unable to decode system command."));
    }
}

/** Handle a command specific to the specification for this device */
static void handleSpecificationCommand(byte* payload, unsigned int length) {
    Uno__Header header;
	pb_istream_t stream = pb_istream_from_buffer(payload, length);
    if (pb_decode_delimited(&stream, Uno__Header_fields, &header)) {
      Serial.println("Decoded header for spec command.");
      if (header.command == Uno_Command_PING) {
        Uno_ping ping;
        if (pb_decode_delimited(&stream, Uno_ping_fields, &ping)) {
          handlePing(ping, header.originator);
        }
      } else if (header.command == Uno_Command_TESTEVENTS) {
        Uno_testEvents testEvents;
        if (pb_decode_delimited(&stream, Uno_testEvents_fields, &testEvents)) {
          handleTestEvents(testEvents, header.originator);
        }
      }
      
      /*
      else if (header.command == Uno_Command_PINMODE) {
        Uno_pinMode pm;
        if (pb_decode_delimited(&stream, Uno_pinMode_fields, &pm)) {
          handlePinMode(pm, header.originator);
        }
      } else if (header.command == Uno_Command_DIGITALREAD) {
        Uno_digitalRead dr;
        if (pb_decode_delimited(&stream, Uno_digitalRead_fields, &dr)) {
          handleDigitalRead(dr, header.originator);
        }
      } else if (header.command == Uno_Command_DIGITALWRITE) {
        Uno_digitalWrite dw;
        if (pb_decode_delimited(&stream, Uno_digitalWrite_fields, &dw)) {
          handleDigitalWrite(dw, header.originator);
        }
      } else if (header.command == Uno_Command_ANALOGREAD) {
        Uno_analogRead ar;
        if (pb_decode_delimited(&stream, Uno_analogRead_fields, &ar)) {
          handleAnalogRead(ar, header.originator);
        }
      } 
      */
      
      else if (header.command == Uno_Command_SERIALPRINTLN) {
        Uno_serialPrintln sp;
        if (pb_decode_delimited(&stream, Uno_serialPrintln_fields, &sp)) {
          handleSerialPrintln(sp, header.originator);
        }
      }
    }
}

/** Handle the 'ping' command */
static void handlePing(Uno_ping ping, char* originator) {
  if (sw_acknowledge(hardwareId, "Ping received.", buffer, sizeof(buffer), originator)) {
    mqttClient.publish(outbound, (char*) buffer);
  }
}

/** Handle the 'testEvents' command */
static void handleTestEvents(Uno_testEvents testEvents, char* originator) {
  if (sw_location(hardwareId, 33.755f, -84.39f, 0.0f, NULL, buffer, sizeof(buffer), originator)) {
    mqttClient.publish(outbound, (char*) buffer);
  }
  if (sw_measurement(hardwareId, "engine.temp", 25.1f, NULL, buffer, sizeof(buffer), originator)) {
    mqttClient.publish(outbound, (char*) buffer);
  }
  if (sw_alert(hardwareId, "engine.overheat", "The engine is overheating!", NULL, buffer, sizeof(buffer), originator)) {
    mqttClient.publish(outbound, (char*) buffer);
  }
}

/** Handle 'pinMode' command */
static void handlePinMode(Uno_pinMode pm, char* originator) {
  if (pm.output == true) {
    pinMode(pm.pinNumber, OUTPUT);
    if (sw_acknowledge(hardwareId, "Pin mode set to OUTPUT.", buffer, sizeof(buffer), originator)) {
      mqttClient.publish(outbound, (char*) buffer);
    }
  } else {
    pinMode(pm.pinNumber, INPUT);
    if (sw_acknowledge(hardwareId, "Pin mode set to INPUT.", buffer, sizeof(buffer), originator)) {
      mqttClient.publish(outbound, (char*) buffer);
    }
  }
}

/** Handle 'digitalRead' command */
static void handleDigitalRead(Uno_digitalRead dr, char* originator) {
  int value = digitalRead(dr.pinNumber);
  float fval = 0.0f + value;
  if (sw_measurement(hardwareId, "digital.value", fval, NULL, buffer, sizeof(buffer), originator)) {
    mqttClient.publish(outbound, (char*) buffer);
  }
}

/** Handle 'digitalWrite' command */
static void handleDigitalWrite(Uno_digitalWrite dw, char* originator) {
  if (dw.value == true) {
    digitalWrite(dw.pinNumber, HIGH);
    if (sw_acknowledge(hardwareId, "Pin value set to HIGH.", buffer, sizeof(buffer), originator)) {
      mqttClient.publish(outbound, (char*) buffer);
    }
  } else {
    digitalWrite(dw.pinNumber, LOW);
    if (sw_acknowledge(hardwareId, "Pin value set to LOW.", buffer, sizeof(buffer), originator)) {
      mqttClient.publish(outbound, (char*) buffer);
    }
  }
}

/** Handle 'analogRead' command */
static void handleAnalogRead(Uno_analogRead ar, char* originator) {
  int value = analogRead(ar.pinNumber);
  float fval = 0.0f + value;
  if (sw_measurement(hardwareId, "analog.value", fval, NULL, buffer, sizeof(buffer), originator)) {
    mqttClient.publish(outbound, (char*) buffer);
  }
}

/** Handle 'serialPrintln' command */
static void handleSerialPrintln(Uno_serialPrintln sp, char* originator) {
  Serial.println(sp.message);
  if (sw_acknowledge(hardwareId, "Sent message.", buffer, sizeof(buffer), originator)) {
    mqttClient.publish(outbound, (char*) buffer);
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
    mqttClient.subscribe(command);
    mqttClient.subscribe(system);

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
