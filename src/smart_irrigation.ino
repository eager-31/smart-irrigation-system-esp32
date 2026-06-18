#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define SOIL_PIN 35
#define PUMP_PIN 26

#define PUMP_THRESHOLD 60
#define HIGH_TEMP_ALERT 35
#define LOW_HUMIDITY_ALERT 25

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(PUMP_PIN, OUTPUT);

  Serial.println("Smart Irrigation and Environmental Monitoring System Started");
}

void loop() {
  int soilValue = analogRead(SOIL_PIN);
  int soilPercent = map(soilValue, 0, 4095, 0, 100);

  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT22 sensor!");
    return;
  }

  Serial.println("--------------- Sensor Data ---------------");

  Serial.print("Raw Soil Value: ");
  Serial.println(soilValue);

  Serial.print("Soil Moisture: ");
  Serial.print(soilPercent);
  Serial.println("%");

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  if (soilPercent < PUMP_THRESHOLD) {
  digitalWrite(PUMP_PIN, HIGH);
  Serial.println("Pump Status: ON");
  Serial.println("Reason: Soil moisture is low");
} else {
  digitalWrite(PUMP_PIN, LOW);
  Serial.println("Pump Status: OFF");
  Serial.println("Reason: Soil moisture is sufficient");
}

  if (temp > HIGH_TEMP_ALERT) {
    Serial.println("Alert: High temperature detected!");
  }

  if (humidity < LOW_HUMIDITY_ALERT) {
    Serial.println("Alert: Low humidity detected!");
  }

  Serial.println("Data logged successfully");
  Serial.println("-------------------------------------------");

  delay(2000);
}
