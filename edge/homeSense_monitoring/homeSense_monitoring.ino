/* --------------------------------------------------
Author          : Muhammad
Author's Github : @call-me-ahmaaad
Instagram       : @just.type_miguel
LinkedIn        : linkedin.com/in/muhammad-ahmad-9a1857266

Project         : HomeSense
Project GitHub  : https://github.com/call-me-ahmaaad/home-sense

Hardware        :
  - ESP32 DEVKIT V1 Microcontroller
  - Temperature & Humidity Sensor XY-MD02
  - Raindrop Sensor Module
  - MQ-5 Gas Sensor
  - 2 LEDs (WiFi and MQTT connection indicators)

Function        :
  - Reads temperature and humidity data from the XY-MD02 sensor.
  - Detects rain status using the raindrop sensor module.
  - Reads gas levels from the MQ-5 gas sensor.
  - Processes and formats sensor data.
  - Publishes sensor data to MQTT topics in real-time.
  - Indicates WiFi and MQTT connection status using LEDs.

Overview        :
A real-time environmental monitoring system developed as part of the HomeSense project. 
This program uses an ESP32 DEVKIT V1 to collect temperature, humidity, rain status, and gas level data from multiple sensors, process the readings, and publish the monitoring data to MQTT topics.

The system is designed for IoT-based communication, enabling sensor data transmission between microcontroller devices and external applications through the MQTT protocol.
-------------------------------------------------- */

// ---------- Libraries ----------
#include <WiFi.h>
#include <HardwareSerial.h>
#include <ModbusMaster.h>  // Modbus RTU master — used to communicate with XY-MD02
#include <PubSubClient.h>

// WiFi, MQTT, and other credentials. See config.example.h for the template.
#include "config.h"
// ---------- Libraries ----------

// ---------- Pin Configuration ----------
// RS485 Serial pins for XY-MD02 communication via Modbus RTU
#define RX_PIN 16
#define TX_PIN 17

#define RAIN_SENSOR_PIN 4  // Digital input — LOW = rain detected (active low)
#define GAS_SENSOR_PIN 32  // Analog input — ADC pin for MQ-5 gas sensor

#define WIFI_LED 13  // ON = WiFi connected
#define MQTT_LED 12  // ON = MQTT connected
// ---------- Pin Configuration ----------

// ---------- Timer ----------
// Non-blocking timer to control sensor read and publish interval
unsigned long previousMillis_process = 0;
unsigned long interval_process = 2000;  // Read and publish every 2 seconds
// ---------- Timer ----------

// ---------- MQTT Configuration ----------
// Base topic used for all MQTT publications.
// Full topic format: home_sense/<macAddress>/...
String TOPIC_INIT = "home_sense";
// ---------- MQTT Configuration ----------

// ---------- Objects ----------
WiFiClient espClient;
PubSubClient client(espClient);  // MQTT client, uses espClient for TCP transport

HardwareSerial RS485Serial(2);  // UART2 assigned for RS485 communication
ModbusMaster node;              // Modbus master instance for XY-MD02
// ---------- Objects ----------

// ---------- Global Variables ----------
float temperature;
float humidity;

String rainStatus;  // Human-readable rain status: "RAIN" or "DRY"
bool isRaining;

float gasLevel;  // Gas level in ppm, converted from ADC raw value

String chipID;      // Unique device identifier, assigned once in setup()
String macAddress;  // Device MAC address (no colons), assigned once in setup()
// ---------- Global Variables ----------


// ==================== Device Identity ====================

// Returns the device's MAC address with colons removed (e.g., "AABBCCDDEEFF")
String getMACAddress() {
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  return mac;
}

// Returns a unique chip ID string derived from the ESP32's eFuse MAC address.
// The 64-bit value is split: upper 32 bits formatted as 4 hex digits, lower 32 bits as 8.
String getChipID() {
  uint64_t chipId = ESP.getEfuseMac();
  char chipIdStr[13];
  snprintf(chipIdStr, sizeof(chipIdStr), "%04X%08X", (uint32_t)(chipId >> 32), (uint32_t)chipId);
  return String(chipIdStr);
}


// ==================== Sensors ====================

// Reads rain sensor and updates isRaining and rainStatus.
// Sensor is active low: LOW signal means rain is detected.
void getRainStatus() {
  isRaining = !digitalRead(RAIN_SENSOR_PIN);
  rainStatus = isRaining ? "RAIN" : "DRY";
}

// Reads MQ-5 gas sensor ADC value and converts it to ppm.
// Formula based on MQ-5 datasheet, calibrated for LPG with R0 = 10.0 kΩ.
// Note: R0 should be re-calibrated in clean air for accurate real-world readings.
void getGasLevel() {
  int adcValue = analogRead(GAS_SENSOR_PIN);

  float voltage = adcValue * (3.3 / 4095.0);  // Convert 12-bit ADC value to voltage

  float RL = 10.0;                              // Load resistance in kΩ
  float Rs = ((3.3 - voltage) / voltage) * RL;  // Sensor resistance at current gas level

  float R0 = 10.0;  // Baseline resistance in clean air (calibrate this value)
  float ratio = Rs / R0;
  gasLevel = 26.572 * pow(ratio, -2.045);  // Datasheet curve fit for LPG
}

// Reads temperature and humidity from XY-MD02 via Modbus RTU (slave ID = 1).
// Register map:
//   0x0001 — Temperature (raw / 10.0 = °C)
//   0x0002 — Humidity    (raw / 10.0 = %)
// Silently skips update if the Modbus read fails.
void getTemperatureHumidity() {
  uint8_t result;
  uint16_t data[2];

  result = node.readInputRegisters(0x0001, 2);

  if (result == node.ku8MBSuccess) {
    data[0] = node.getResponseBuffer(0);  // Temperature raw value
    data[1] = node.getResponseBuffer(1);  // Humidity raw value

    temperature = data[0] / 10.0;
    humidity = data[1] / 10.0;
  }
}


// ==================== Diagnostics ====================

// Prints current WiFi, device, and sensor data to Serial Monitor for debugging.
void printData() {
  Serial.print("WiFi Status : ");
  Serial.println(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");

  Serial.print("WiFi RSSI   : ");
  Serial.println(WiFi.RSSI());

  Serial.print("IP Address  : ");
  Serial.println(WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : "0.0.0.0");

  Serial.print("MAC Address : ");
  Serial.println(macAddress);

  Serial.print("Chip ID     : ");
  Serial.println(chipID);

  Serial.println();

  Serial.print("Temperature : ");
  Serial.print(temperature);
  Serial.println("°C");

  Serial.print("Humidity    : ");
  Serial.print(humidity);
  Serial.println("%");

  Serial.print("Rain Status : ");
  Serial.println(rainStatus);

  Serial.print("Gas Level   : ");
  Serial.print(gasLevel);
  Serial.println(" ppm");

  Serial.println();
}

// Updates WiFi and MQTT indicator LEDs based on current connection state.
// Called every loop iteration so LEDs always reflect live status.
void connectionIndicator() {
  digitalWrite(WIFI_LED, WiFi.status() == WL_CONNECTED);
  digitalWrite(MQTT_LED, client.connected());
}


// ==================== WiFi ====================

// Initiates WiFi connection using credentials from config.h
void connectToWiFi() {
  Serial.println("Connecting device to WiFi access point...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

// Event: fired when the device associates with the access point (before IP assignment)
void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Device successfully connected to WiFi access point!");
}

// Event: fired when DHCP assigns an IP — WiFi is fully ready at this point
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// Event: fired on disconnection — logs the reason and retries the connection
void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Device disconnected from WiFi access point!");
  Serial.print("Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);

  connectToWiFi();
}


// ==================== MQTT ====================

// Publishes a payload to the given MQTT topic and logs the result to Serial.
void publishMQTT(String topic, String payload, bool retain) {
  bool published = client.publish(topic.c_str(), payload.c_str(), retain);

  Serial.print("Publish Status  : ");
  Serial.println(published ? "DELIVERED" : "FAILED");

  Serial.print("Payload         : ");
  Serial.println(published ? payload : "-");

  Serial.println();
}

// Configures the MQTT broker address and port (does not connect yet).
void initMQTT() {
  Serial.println("Start connecting device to MQTT...");
  client.setServer(MQTT_SERVER, MQTT_PORT);
}

// Attempts to (re)connect to the MQTT broker. Skips if WiFi is not available.
// Retries every 5 seconds on failure.
//
// LWT (Last Will and Testament): the broker is instructed to publish
// {"status":"offline"} to the status topic if the device disconnects unexpectedly.
// On successful connection, the device immediately publishes {"status":"online"}.
void reconnectMQTT() {
  if (WiFi.status() != WL_CONNECTED) return;

  String lwtTopic = TOPIC_INIT + "/" + macAddress + "/status";
  String lwtMessageOffline = "{\"status\":\"offline\"}";
  String lwtMessageOnline = "{\"status\":\"online\"}";

  while (!client.connected()) {
    Serial.println("Attempting connection to MQTT...");

    // Unique client ID prevents broker conflicts when multiple devices are online
    String clientID = TOPIC_INIT + "-" + macAddress;

    if (client.connect(
          clientID.c_str(),           // Client ID
          lwtTopic.c_str(),           // LWT topic
          1,                          // LWT QoS
          true,                       // LWT retain
          lwtMessageOffline.c_str())  // LWT payload (sent by broker on unexpected disconnect)
    ) {
      publishMQTT(lwtTopic, lwtMessageOnline, true);  // Announce device is online
      Serial.println("Connected to MQTT!");
    } else {
      Serial.print("Failed. RC: ");
      Serial.print(client.state());
      Serial.println(" . Try again in 5 seconds");
      delay(5000);
    }
  }
}

// Builds and publishes a JSON payload with all current sensor readings.
// Topic: home_sense/<macAddress>/data
// Payload fields: temperature (°C), humidity (%), rain_status (0/1), gas_level (ppm)
void sendSensorPayload() {
  String topic = TOPIC_INIT + "/" + macAddress + "/data";

  char payload[256];
  snprintf(
    payload,
    sizeof(payload),
    "{\"temperature\":%.2f, \"humidity\":%.2f, \"rain_status\":%d, \"gas_level\":%.2f}",
    temperature, humidity, isRaining, gasLevel);

  publishMQTT(topic, payload, false);
}


// ==================== Arduino Entry Points ====================

void setup() {
  Serial.begin(115200);

  // Initialize RS485 UART and register Modbus slave (ID = 1 for XY-MD02)
  // Note: verify the slave ID matches your sensor's DIP switch configuration
  RS485Serial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  node.begin(1, RS485Serial);

  Serial.println("Setup starting...");

  // Configure sensor and LED pins
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(GAS_SENSOR_PIN, INPUT);
  pinMode(WIFI_LED, OUTPUT);
  pinMode(MQTT_LED, OUTPUT);

  // Ensure LEDs start in OFF state
  digitalWrite(WIFI_LED, LOW);
  digitalWrite(MQTT_LED, LOW);

  // Reset WiFi state and set to station (client) mode before connecting
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  delay(1000);

  // Register WiFi event callbacks
  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  delay(1000);

  connectToWiFi();

  // MAC and Chip ID are retrieved once here and reused throughout the program
  macAddress = getMACAddress();
  chipID = getChipID();
  delay(1000);

  initMQTT();
  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();

  // Reconnect to MQTT if connection was lost (e.g., broker restart, network drop)
  if (!client.connected()) {
    reconnectMQTT();
  }

  // Process incoming MQTT messages and maintain the keep-alive heartbeat
  client.loop();

  // Reflect current connection state on indicator LEDs
  connectionIndicator();

  // Non-blocking interval: read all sensors and publish every interval_process ms
  if (currentMillis - previousMillis_process >= interval_process) {
    getTemperatureHumidity();
    getRainStatus();
    getGasLevel();
    sendSensorPayload();
    printData();

    previousMillis_process = currentMillis;
  }
}
