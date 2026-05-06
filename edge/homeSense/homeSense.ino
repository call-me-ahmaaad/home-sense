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
  - RGB LED (Common Cathode)
Function        :
  - Monitoring temperature, humidity, rain status, and gas levels.
  - Sends monitoring data to an MQTT topic.
  - Controls the on and off of LEDs based on the received MQTT messages.

Overview        :
An IoT-based monitoring dashboard developed to visualize environmental sensor data in real-time, including temperature, humidity, gas levels, and rain detection.

Built a responsive web interface optimized for multiple screen sizes using modern CSS techniques such as Grid and Flexbox. 
The dashboard is designed to work with microcontroller-based devices (e.g., ESP32), enabling real-time data updates and remote monitoring.
-------------------------------------------------- */

// ---------- Libraries ----------
#include <WiFi.h>
#include <HardwareSerial.h>
#include <ModbusMaster.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi, MQTT, and other credentials. See config.example.h for the template.
#include "config.h"
// ---------- Libraries ----------

// ---------- Pin Configuration ----------
// RS485 Serial pins for XY-MD02 communication via Modbus
#define RX_PIN 16
#define TX_PIN 17

#define RAIN_SENSOR_PIN 5  // Digital input - LOW = rain detected (active low)
#define GAS_SENSOR_PIN 32  // Analog input - ADC pin for MQ-5 gas sensor
// ---------- Pin Configuration ----------

// ---------- Timer Section ----------
// Non-blocking timer to control sensor read and publish interval
unsigned long previousMillis_process = 0;
unsigned long interval_process = 2000;  // Read and publish every 2 seconds
// ---------- Intervals ----------

// ---------- MQTT Configuration ----------
String TOPIC_INIT = "home_sense";  // Base topic, full topic: home_sense/<chipID>
// ---------- MQTT Configuration ----------

// ---------- Event Handlers & Object ----------
WiFiClient espClient;
PubSubClient client(espClient);

HardwareSerial RS485Serial(2);  // Use UART2 for RS485 communication
ModbusMaster node;              // Modbus master instance for XY-MD02
// ---------- Event Handlers & Object ----------

// ---------- Main Variables ----------
float temperature;
float humidity;

String rainStatus;  // Human-readable rain status: "RAIN" or "DRY"
bool isRaining;

float gasLevel;  // Gas level in ppm, converted from ADC raw value

String chipID;      // Unique device identifier, assigned once in setup()
String macAddress;  // Unique hardware network identifier (without colons), assigned once in setup()
// ---------- Main Variables ----------

// Returns the unique MAC Address of ESP32
String getMACAddress() {
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  return mac;
}

// Returns the unique chip ID derived from ESP32's eFuse MAC address
String getChipID() {
  uint64_t chipId = ESP.getEfuseMac();
  char chipIdStr[13];
  snprintf(chipIdStr, sizeof(chipIdStr), "%04X%08X", (uint32_t)(chipId >> 32), (uint32_t)chipId);
  return String(chipIdStr);
}

// Reads rain sensor and updates isRaining and rainStatus
void getRainStatus() {
  isRaining = !digitalRead(RAIN_SENSOR_PIN);  // Active low: LOW = rain detected
  rainStatus = isRaining ? "RAIN" : "DRY";
}

// Reads MQ-5 gas sensor ADC value and converts it to ppm
// Formula based on MQ-5 datasheet, calibrated for LPG with R0 = 10.0 kΩ
// Note: R0 should be calibrated in clean air for accurate readings
void getGasLevel() {
  int adcValue = analogRead(GAS_SENSOR_PIN);

  float voltage = adcValue * (3.3 / 4095.0);  // Convert ADC (12-bit) to voltage

  float RL = 10.0;                              // Load resistance in kΩ
  float Rs = ((3.3 - voltage) / voltage) * RL;  // Sensor resistance

  float R0 = 10.0;  // Baseline resistance in clean air
  float ratio = Rs / R0;
  gasLevel = 26.572 * pow(ratio, -2.045);  // Datasheet curve fitting for LPG
}

// Reads temperature and humidity from XY-MD02 via Modbus RTU
// Register 0x0001: Temperature (raw value / 10.0 = °C)
// Register 0x0002: Humidity    (raw value / 10.0 = %)
void getTemperatureHumidity() {
  uint8_t result;
  uint16_t data[2];

  result = node.readInputRegisters(0x0001, 2);

  if (result == node.ku8MBSuccess) {
    data[0] = node.getResponseBuffer(0);  // Temperature
    data[1] = node.getResponseBuffer(1);  // Humidity

    temperature = data[0] / 10.0;
    humidity = data[1] / 10.0;
  }
}

// Prints current device and sensor data to Serial Monitor for debugging
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

// ---------- WiFi Event Handlers ----------
void WiFiStationConnecting() {
  Serial.println("Connecting device to WiFi access point...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Device successfully connected to WiFi access point!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// Auto-reconnect to WiFi when disconnected
void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Device disconnected from WiFi access point!");
  Serial.print("Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);

  WiFiStationConnecting();
}
// ---------- WiFi Event Handlers ----------

// Publishes a payload to the given MQTT topic and logs the result
void publishMQTT(String topic, String payload, bool retain) {
  bool publish = client.publish(topic.c_str(), payload.c_str(), retain);

  Serial.print("Publish Status  : ");
  Serial.println(publish ? "DELIVERED" : "FAILED");

  Serial.print("Payload         : ");
  Serial.println(publish ? payload : "-");

  Serial.println();
}

// Attempts to reconnect to MQTT broker if disconnected
// Skips if WiFi is not connected to avoid blocking
void reconnectMQTT() {
  if (WiFi.status() != WL_CONNECTED) return;

  String lwtTopic = TOPIC_INIT + "/" + macAddress + "/status";
  String lwtMessageOffline = "{\"status\":\"offline\"}";
  String lwtMessageOnline = "{\"status\":\"online\"}";

  while (!client.connected()) {
    Serial.println("Attempting connection to MQTT...");
    String clientID = TOPIC_INIT + macAddress;  // Unique client ID per device

    if (client.connect(
          clientID.c_str(),           // Client ID
          lwtTopic.c_str(),           // LWT Topic
          1,                          // QoS
          true,                       // Retain
          lwtMessageOffline.c_str())  // LWT Message
    ) {
      publishMQTT(lwtTopic.c_str(), lwtMessageOnline.c_str(), true);
      Serial.println("Connected to MQTT!");
    } else {
      Serial.print("Failed. RC: ");
      Serial.print(client.state());
      Serial.print(" . Try again in 5 seconds");
      delay(5000);
    }
  }
}

// Builds and publishes a JSON payload containing all sensor data to MQTT
void sendSensorPayload() {
  String topic = TOPIC_INIT + "/" + macAddress + "/" + "sensor_data";

  StaticJsonDocument<256> doc;

  doc["ip"] = WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : "0.0.0.0";
  doc["mac"] = macAddress;
  doc["chip_id"] = chipID;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["rain_status"] = isRaining;
  doc["gas_level"] = round(gasLevel * 100) / 100.0;  // Rounded to 2 decimal places

  String payload;
  serializeJson(doc, payload);

  publishMQTT(topic, payload, false);
}

void setup() {
  Serial.begin(115200);

  RS485Serial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  node.begin(1, RS485Serial);  // Modbus slave ID = 1, might be different with yours. Please check again.

  Serial.println("Setup starting...");

  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(GAS_SENSOR_PIN, INPUT);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  delay(1000);

  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  delay(1000);

  Serial.println("Start connecting device to WiFi access point...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  macAddress = getMACAddress();  // Assigned once, used throughout the program
  chipID = getChipID();          // Assigned once, used throughout the program

  delay(1000);

  Serial.println("Start connecting device to MQTT...");
  client.setServer(MQTT_SERVER, MQTT_PORT);

  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();

  // Ensure MQTT stays connected, reconnect if dropped
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();  // Handle incoming MQTT messages and maintain connection

  // Non-blocking interval: read sensors and publish every interval_process ms
  if (currentMillis - previousMillis_process >= interval_process) {
    getTemperatureHumidity();

    getRainStatus();

    getGasLevel();

    sendSensorPayload();

    printData();

    previousMillis_process = currentMillis;
  }
}
