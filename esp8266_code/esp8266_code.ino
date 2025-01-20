#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT22.h>

// Replace with your Wi-Fi credentials
const char* ssid = "Hardware_Hackathon-2";
const char* password = "hackathon@2025";

// Replace with your server's IP address and port
const char* serverIP = "192.168.78.221";
const int serverPort = 8000;

#define DHTPIN D2           // Pin where the DHT sensor is connected
#define DHTTYPE DHT22      // DHT 22 (AM2302)
DHT22 dht(DHTPIN);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Wi-Fi connected!");
  Serial.print("ESP8266 IP: ");
  Serial.println(WiFi.localIP());  // Print ESP8266's IP
}

void loop() {
  // Read temperature and humidity from the DHT sensor
  float temperature = dht.getTemperature();  // Read temperature in Celsius
  float humidity = dht.getHumidity();       // Read humidity percentage

  if (dht.getLastError() != dht.OK) {
    Serial.print("Last error: ");
    Serial.println(dht.getLastError());
  }

  // Ensure valid sensor readings
  if (temperature != NAN && humidity != NAN) {
    // Print the readings to the Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Check if connected to Wi-Fi
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      // Form the server URL
      String serverURL = String("http://") + serverIP + ":" + serverPort +"/sensor-data";
      Serial.print("Sending POST request to: ");
      Serial.println(serverURL);

      if (http.begin(client, serverURL)) {
        // Add header for form data (x-www-form-urlencoded)
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        // Data to send with HTTP POST
        String httpRequestData = "temperature=" + String(temperature) + "&humidity=" + String(humidity);

        // Send POST request with the sensor data
        int httpCode = http.POST(httpRequestData);

        if (httpCode > 0) {
          // If the HTTP request was successful
          Serial.printf("HTTP Response Code: %d\n", httpCode);
          String payload = http.getString();
          Serial.println("Response from server: ");
          Serial.println(payload);
        } else {
          // If the HTTP request failed
          Serial.printf("HTTP Request failed: %s\n", http.errorToString(httpCode).c_str());
        }
        

        http.end();  // End the connection
      } else {
        Serial.println("Unable to connect to the server!");
      }
    } else {
      Serial.println("Wi-Fi not connected!");
    }
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  delay(10000);  // Wait 10 seconds before sending the next request
}
