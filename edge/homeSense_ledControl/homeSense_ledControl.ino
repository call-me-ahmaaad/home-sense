/* --------------------------------------------------
Author          : Muhammad
Author's Github : @call-me-ahmaaad
Instagram       : @just.type_miguel
LinkedIn        : linkedin.com/in/muhammad-ahmad-9a1857266

Project         : HomeSense
Project GitHub  : https://github.com/call-me-ahmaaad/home-sense

Hardware        :
  - WEMOS D1 MINI ESP8266
  - LEDs (Red, Green, Blue)

Function        :
  - Connects the device to a WiFi network.
  - Connects the device to an MQTT broker.
  - Subscribes to MQTT topics for LED control.
  - Controls LED states based on received MQTT messages.
  - Supports real-time remote LED control over a wireless network.

Overview        :
An MQTT-based LED control system developed as part of the HomeSense project. 
This program uses a WEMOS D1 MINI ESP8266 to control three LEDs (Red, Green, and Blue) through MQTT messages in real-time.

The system subscribes to dedicated MQTT topics and changes the LED states based on the received payloads, enabling simple remote device control over a wireless network.
-------------------------------------------------- */

// ---------- Libraries ----------
#include <ESP8266WiFi.h>   // Core WiFi library for ESP8266
#include <PubSubClient.h>  // MQTT client library

// WiFi, MQTT, and other credentials. See config.example.h for the template.
#include "config.h"
// ---------- Libraries ----------

// ---------- Pin Configuration ----------
#define RED_PIN D1
#define GREEN_PIN D2
#define BLUE_PIN D3
// ---------- Pin Configuration ----------

// ---------- MQTT Configuration ----------
// Base topic used for all MQTT subscriptions.
// Full topic format: home_sense/<macAddress>/led/<color>
String TOPIC_INIT = "home_sense";
// ---------- MQTT Configuration ----------

// ---------- Objects ----------
WiFiClient espClient;
PubSubClient client(espClient);  // MQTT client, uses espClient for TCP transport
// ---------- Objects ----------

// ---------- Global Variables ----------
String macAddress;  // Device MAC address (no colons), used as a unique device identifier

// LED color labels — used to dynamically build MQTT subscription topics
String leds[] = {
  "red",
  "green",
  "blue"
};
// ---------- Global Variables ----------


// ==================== WiFi ====================

// Returns the device's MAC address with colons removed (e.g., "AABBCCDDEEFF")
String getMACAddress() {
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  return mac;
}

// Initiates WiFi connection using credentials from config.h
void connectToWiFi() {
  Serial.println("Connecting device to WiFi access point...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

// Event: fired when the device associates with the access point (before IP assignment)
void onStationModeConnected(const WiFiEventStationModeConnected& evt) {
  Serial.println("Device connected to WiFi access point!");
}

// Event: fired when DHCP assigns an IP — WiFi is fully ready at this point
void onStationModeGotIP(const WiFiEventStationModeGotIP& evt) {
  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// Event: fired on disconnection — logs the reason and retries the connection
void onStationModeDisconnected(const WiFiEventStationModeDisconnected& evt) {
  Serial.println("Device disconnected from WiFi access point!");
  Serial.print("Reason: ");
  Serial.println(evt.reason);

  connectToWiFi();
}

// ==================== MQTT ====================

// Configures the MQTT broker address and port (does not connect yet)
void initMQTT() {
  Serial.println("Start connecting device to MQTT...");
  client.setServer(MQTT_SERVER, MQTT_PORT);
}

// Attempts to (re)connect to the MQTT broker and subscribe to all LED topics.
// Skips if WiFi is not connected. Retries every 5 seconds on failure.
void reconnectMQTT() {
  if (WiFi.status() != WL_CONNECTED) return;

  while (!client.connected()) {
    Serial.println("Attempting connection to MQTT...");

    // Unique client ID prevents broker conflicts when multiple devices are online
    String clientID = TOPIC_INIT + "-" + macAddress;

    if (client.connect(clientID.c_str())) {
      Serial.println("Connected to MQTT!");

      // Subscribe to each LED's topic: home_sense/<mac>/led/<color>
      Serial.println("Subscribed to: ");
      for (String led : leds) {
        String fullTopic = TOPIC_INIT + "/" + "led" + "/" + led;
        client.subscribe(fullTopic.c_str());

        Serial.print("   - ");
        Serial.println(fullTopic);
      }
    } else {
      Serial.print("Failed. RC: ");
      Serial.print(client.state());
      Serial.print(" . Try again in 5 seconds");
      delay(5000);
    }
  }
}

// MQTT message handler — called by PubSubClient whenever a subscribed topic receives a message.
// Parses the topic suffix (/red, /green, /blue) and sets the corresponding LED HIGH or LOW.
// Expected payload: "ON" to turn on, anything else to turn off.
void callback(char* topic, byte* payload, unsigned int length) {
  String incomingTopic = String(topic);
  String message;

  // Reconstruct the payload string from raw bytes
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  message.trim();

  Serial.print("Topic   : ");
  Serial.println(topic);
  Serial.print("Message : ");
  Serial.println(message);

  // Route the message to the correct LED based on the topic suffix
  if (incomingTopic.endsWith("/red")) {
    digitalWrite(RED_PIN, message == "ON");
  } else if (incomingTopic.endsWith("/green")) {
    digitalWrite(GREEN_PIN, message == "ON");
  } else if (incomingTopic.endsWith("/blue")) {
    digitalWrite(BLUE_PIN, message == "ON");
  }
}

// ==================== Arduino Entry Points ====================

void setup() {
  Serial.begin(115200);
  Serial.println("Setup starting...");

  // Initialize LED pins as output and ensure they start in OFF state
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  // Reset WiFi state and set to station (client) mode before connecting
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  delay(1000);

  // Register WiFi event callbacks
  WiFi.onStationModeConnected(onStationModeConnected);
  WiFi.onStationModeGotIP(onStationModeGotIP);
  WiFi.onStationModeDisconnected(onStationModeDisconnected);
  delay(1000);

  connectToWiFi();

  // MAC address is retrieved once here and reused for topic building and client ID
  macAddress = getMACAddress();
  delay(1000);

  initMQTT();
  client.setCallback(callback);  // Register MQTT message handler
  delay(1000);
}

void loop() {
  // Reconnect to MQTT if connection was lost (e.g., broker restart, network drop)
  if (!client.connected()) {
    reconnectMQTT();
  }

  // Process incoming MQTT messages and maintain the keep-alive heartbeat
  client.loop();
}
