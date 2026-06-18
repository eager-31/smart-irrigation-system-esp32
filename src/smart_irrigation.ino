/************************************************************
 * Project: Smart Irrigation and Environmental Monitoring System
 * Board  : ESP32
 * Sensors: DHT22, Soil Moisture Sensor
 * Cloud  : Blynk IoT + MQTT HiveMQ Broker
 ************************************************************/

#define BLYNK_TEMPLATE_ID   "My_BLYNK_TEMPLATE_ID"   /* that i use here */
#define BLYNK_TEMPLATE_NAME "Smart Irrigation ESP32"
#define BLYNK_AUTH_TOKEN    "My_BLYNK_AUTH_TOKEN"    /* that i use here */

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>

/* Pin Configuration */
#define DHT_PIN     4
#define DHT_TYPE    DHT22
#define SOIL_PIN    35
#define PUMP_PIN    26

/* Irrigation Thresholds */
#define PUMP_THRESHOLD      60
#define HYSTERESIS          5
#define HIGH_TEMP_ALERT     35
#define LOW_HUMIDITY_ALERT  25

/* WiFi Credentials */
const char* WIFI_SSID     = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

/* MQTT Configuration */
const char* MQTT_SERVER = "broker.hivemq.com";
const int   MQTT_PORT   = 1883;
const char* MQTT_TOPIC  = "olus/intern/irrigation/data";

/* Blynk Virtual Pins */
#define VPIN_PUMP_CONTROL  V0
#define VPIN_TEMPERATURE   V1
#define VPIN_HUMIDITY      V2
#define VPIN_SOIL_MOISTURE V3
#define VPIN_PUMP_STATUS   V4

WiFiClient espClient;
PubSubClient mqttClient(espClient);
DHT dht(DHT_PIN, DHT_TYPE);

bool pumpState = false;
unsigned long lastPublishTime = 0;
const unsigned long PUBLISH_INTERVAL = 5000;

/* Manual Pump Control from Blynk App */
BLYNK_WRITE(VPIN_PUMP_CONTROL) {
  pumpState = param.asInt();
  digitalWrite(PUMP_PIN, pumpState ? HIGH : LOW);
}

void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT... ");

    String clientId = "ESP32_Irrigation_" + String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" | Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void updatePumpControl(int soilPercent) {
  if (soilPercent < (PUMP_THRESHOLD - HYSTERESIS)) {
    pumpState = true;
  } 
  else if (soilPercent > (PUMP_THRESHOLD + HYSTERESIS)) {
    pumpState = false;
  }

  digitalWrite(PUMP_PIN, pumpState ? HIGH : LOW);
}

void sendDataToBlynk(float temperature, float humidity, int soilPercent) {
  Blynk.virtualWrite(VPIN_TEMPERATURE, temperature);
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity);
  Blynk.virtualWrite(VPIN_SOIL_MOISTURE, soilPercent);
  Blynk.virtualWrite(VPIN_PUMP_STATUS, pumpState ? 1 : 0);
}

void publishDataToMQTT(float temperature, float humidity, int soilPercent) {
  String payload = "{";
  payload += "\"soil_percent\":" + String(soilPercent) + ",";
  payload += "\"temperature\":" + String(temperature, 1) + ",";
  payload += "\"humidity\":" + String(humidity, 1) + ",";
  payload += "\"pump_status\":\"" + String(pumpState ? "ON" : "OFF") + "\"";
  payload += "}";

  mqttClient.publish(MQTT_TOPIC, payload.c_str());
}

void printSensorData(int rawSoilValue, int soilPercent, float temperature, float humidity) {
  Serial.println("--------------- Sensor Data ---------------");
  Serial.print("Raw Soil Value: ");
  Serial.println(rawSoilValue);

  Serial.print("Soil Moisture: ");
  Serial.print(soilPercent);
  Serial.println("%");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  Serial.print("Pump Status: ");
  Serial.println(pumpState ? "ON" : "OFF");

  if (temperature > HIGH_TEMP_ALERT) {
    Serial.println("Alert: High temperature detected!");
  }

  if (humidity < LOW_HUMIDITY_ALERT) {
    Serial.println("Alert: Low humidity detected!");
  }

  Serial.println("Data published to Blynk and MQTT Cloud");
  Serial.println("-------------------------------------------");
}

void setup() {
  Serial.begin(115200);

  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);

  dht.begin();

  Serial.println("Connecting to WiFi and Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);

  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);

  Serial.println("Smart Irrigation System Started");
}

void loop() {
  Blynk.run();

  if (!mqttClient.connected()) {
    connectMQTT();
  }
  mqttClient.loop();

  unsigned long currentTime = millis();

  if (currentTime - lastPublishTime >= PUBLISH_INTERVAL) {
    lastPublishTime = currentTime;

    int rawSoilValue = analogRead(SOIL_PIN);
    int soilPercent = map(rawSoilValue, 0, 4095, 0, 100);

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Error: Failed to read from DHT22 sensor");
      return;
    }

    updatePumpControl(soilPercent);
    sendDataToBlynk(temperature, humidity, soilPercent);
    publishDataToMQTT(temperature, humidity, soilPercent);
    printSensorData(rawSoilValue, soilPercent, temperature, humidity);
  }
}
