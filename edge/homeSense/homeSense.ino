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

#include "config.h"
// ---------- Libraries ----------

// ---------- Pin Configuration ----------
#define RX_PIN 16
#define TX_PIN 17

#define RAIN_SENSOR_PIN 5
#define GAS_SENSOR_PIN 32
// ---------- Pin Configuration ----------

// ---------- Timer Section ----------
unsigned long previousMillis_process = 0;
unsigned long interval_process = 2000;
// ---------- Intervals ----------

// ---------- WiFi Credential ----------
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
// ---------- WiFi Credential ----------

// ---------- MQTT Configuration ----------
#define MQTT_SERVER "YOUR_MQTT_SERVER"
#define MQTT_PORT "YOUR_MQTT_PORT" // Without double quote

String TOPIC_INIT = "home_sense";
// ---------- MQTT Configuration ----------

// ---------- Event Handlers & Object ----------
WiFiClient espClient;
PubSubClient client(espClient);

HardwareSerial RS485Serial(2);
ModbusMaster node;
// ---------- Event Handlers & Object ----------

// ---------- Main Variables ----------
float temperature;
float humidity;

String rainStatus;
bool isRaining;

float gasLevel;

String chipID;
// ---------- Main Variables ----------

String getChipID() {
  uint64_t chipId = ESP.getEfuseMac();
  char chipIdStr[13];
  snprintf(chipIdStr, sizeof(chipIdStr), "%04X%08X", (uint32_t)(chipId >> 32), (uint32_t)chipId);
  return String(chipIdStr);
}

void getRainStatus() {
  isRaining = !digitalRead(RAIN_SENSOR_PIN);
  rainStatus = isRaining ? "RAIN" : "DRY";
}

void getGasLevel() {
  int adcValue = analogRead(GAS_SENSOR_PIN);

  float voltage = adcValue * (3.3 / 4095.0);

  float RL = 10.0;
  float Rs = ((3.3 - voltage) / voltage) * RL;

  float R0 = 10.0;
  float ratio = Rs / R0;
  gasLevel = 26.572 * pow(ratio, -2.045);
}

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

void printData() {
  Serial.print("WiFi Status : ");
  Serial.println(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");

  Serial.print("WiFi RSSI   : ");
  Serial.println(WiFi.RSSI());

  Serial.print("IP Address  : ");
  Serial.println(WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : "0.0.0.0");

  Serial.print("MAC Address : ");
  Serial.println(WiFi.macAddress());

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

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Device disconnected from WiFi access point!");
  Serial.print("Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);

  WiFiStationConnecting();
}

void publishMQTT(String topic, String payload) {
  bool publish = client.publish(topic.c_str(), payload.c_str());

  Serial.print("Publish Status  : ");
  Serial.println(publish ? "DELIVERED" : "FAILED");

  Serial.print("Payload         : ");
  Serial.println(publish ? payload : "-");

  Serial.println();
}

void reconnectMQTT() {
  if (WiFi.status() != WL_CONNECTED) return;

  while (!client.connected()) {
    Serial.println("Attempting connection to MQTT...");
    String clientID = TOPIC_INIT + chipID;

    if (client.connect(clientID.c_str())) {
      Serial.println("Connected to MQTT!");
    } else {
      Serial.print("Failed. RC: ");
      Serial.print(client.state());
      Serial.print(" . Try again in 5 seconds");
      delay(5000);
    }
  }
}

void sendSensorPayload() {
  String topic = TOPIC_INIT + "/" + chipID;

  StaticJsonDocument<256> doc;

  doc["ip"] = WiFi.status() == WL_CONNECTED ? WiFi.localIP().toString() : "0.0.0.0";
  doc["mac"] = WiFi.macAddress();
  doc["chip_id"] = chipID;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["rain_status"] = isRaining;
  doc["gas_level"] = round(gasLevel * 100) / 100.0;

  String payload;
  serializeJson(doc, payload);

  publishMQTT(topic, payload);
}

void setup() {
  Serial.begin(115200);

  RS485Serial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  node.begin(1, RS485Serial);  // Modbus slave ID = 1, might be different with yours. Please check again.

  Serial.println("Setup starting...");

  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(GAS_SENSOR_PIN, INPUT);

  chipID = getChipID();

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  delay(1000);

  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  delay(1000);

  Serial.println("Start connecting device to WiFi access point...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  delay(1000);

  Serial.println("Start connecting device to MQTT...");
  client.setServer(MQTT_SERVER, MQTT_PORT);

  delay(1000);
}

void loop() {
  unsigned long currentMillis = millis();

  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  if (currentMillis - previousMillis_process >= interval_process) {
    getTemperatureHumidity();

    getRainStatus();

    getGasLevel();

    sendSensorPayload();

    printData();

    previousMillis_process = currentMillis;
  }
}
