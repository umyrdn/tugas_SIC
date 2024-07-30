#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <DHT.h>

// Pengaturan DHT11
#define DHTPIN 13     // Pin yang terhubung ke DHT11
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// URL server Flask
const char* serverName = "http://localhost:5000/sensor/data"; // Gantilah dengan IP lokal server Anda

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

void setup(void) {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ");
  Serial.print(WIFI_SSID);
  
  // Wait for connection
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" Connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println(" Connection failed!");
    return;
  }

  dht.begin();
}

void loop() {
  // Membaca kelembapan dan suhu
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  unsigned long timestamp = millis(); // Menggunakan waktu sejak ESP8266/ESP32 dihidupkan sebagai timestamp

  // Periksa apakah pembacaan berhasil
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    return;
  }

  // Mengirim data ke server Flask
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");
    
    // Membuat JSON untuk dikirim
    String httpRequestData = "{\"temperature\":"
        + String(temperature) + ",\"kelembapan\":"
        + String(humidity) + "}";

    Serial.print("Sending data: ");
    Serial.println(httpRequestData);

    int httpResponseCode = http.POST(httpRequestData);

    // Cetak kode respons HTTP
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(response);
    } else {
      Serial.print("Error pada kode: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  delay(5000); // Jeda 5 detik setiap post request
}
