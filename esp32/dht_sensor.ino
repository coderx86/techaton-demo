#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 4        // GPIO pin connected to DHT22
#define DHTTYPE DHT22   // Sensor type DHT22

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Moto50";           // 🔑 Your WiFi SSID
const char* password = "12345678";   // 🔑 Your WiFi password
const char* server = "http://192.168.64.107:8000/data"; // 🌐 Flask/FastAPI endpoint

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("📡 Starting ESP32...");

  dht.begin();
  Serial.println("🌡️ DHT22 initialized.");

  WiFi.begin(ssid, password);
  Serial.print("🔗 Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("✅ WiFi connected. IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.println("🔄 Reading sensor data...");

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("❌ Failed to read from DHT sensor!");
  } else {
    Serial.print("🌡️ Temperature: ");
    Serial.print(temp);
    Serial.print("°C, 💧 Humidity: ");
    Serial.print(hum);
    Serial.println("%");

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(server);
      http.addHeader("Content-Type", "application/json");

      String body = "{\"temperature\": " + String(temp, 2) + ", \"humidity\": " + String(hum, 2) + "}";
      Serial.print("📤 Sending data: ");
      Serial.println(body);

      int httpCode = http.POST(body);
      String payload = http.getString();

      Serial.print("✅ HTTP Response Code: ");
      Serial.println(httpCode);
      Serial.print("📝 Server Response: ");
      Serial.println(payload);

      http.end();
    } else {
      Serial.println("❌ WiFi not connected!");
    }
  }

  Serial.println("⏳ Waiting 10 seconds...\n");
  delay(10000);
}
