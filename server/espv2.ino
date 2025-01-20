#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Servo.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


// WiFi credentialsServo.h"
const char* ssid = "Hardware_Hackathon-2";
const char* password = "hackathon@2025";

// Server details
const char* serverIP = "192.168.78.221";
const int serverPort = 8000;

// Pin definitions
#define MQ_SENSOR_PIN D0
#define LED_PIN D1
#define SERVO_PIN D4
#define RAIN_SENSOR_PIN A0
#define DHT_PIN D3
#define BUZZER_PIN D8

// DHT22 setup
#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);

// Servo setup
Servo servo;

WiFiClient client;

void setup() {


  Serial.begin(115200);
  delay(100);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize components
  pinMode(MQ_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  dht.begin();
  servo.attach(SERVO_PIN);
}

void loop() {
  // Read sensor data
  String moistureValue = "";
  String mqValueData = "";
  float mqThreshold = 100.0;
  float moistureThreshold = 100.0;
  int mqValue = digitalRead(MQ_SENSOR_PIN);
  int rainValue = analogRead(RAIN_SENSOR_PIN);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  bool buzzerOn = false;
  bool craddleOn = false;

servo.attach(4);


  // Log sensor data
  Serial.println("----- Sensor Data -----");
  Serial.print("MQ Sensor Value: ");
  Serial.println(mqValue);
  Serial.print("Rain Sensor Value: ");
  Serial.println(rainValue);
  Serial.print("Temperature (°C): ");
  Serial.println(temperature);
  Serial.print("Humidity (%): ");
  Serial.println(humidity);

  // Control LED and buzzer for entertainment
  Serial.println("Activating LED and buzzer...");
  digitalWrite(LED_PIN, HIGH);  // Turn LED on
    // Play tone for 500ms
  delay(500);

  // Perform cradling action
  Serial.println("Performing cradling action...");
  servo.write(0);
  delay(1000);
  servo.write(90);
  delay(1000);

  if (rainValue > moistureThreshold) {
    moistureValue += "on";
  }


  if (mqValue > mqThreshold) {
    mqValueData += "on";
  }

  // Prepare data payload
  String postData = "mq=" + String(mqValueData) + "&moisture=" + rainValue + "&temperature=" + String(temperature) + "&humidity=" + String(humidity);

  Serial.println("----- Data to Send -----");
  Serial.println(postData);
  String serverSensorURL = String("http://") + serverIP + ":" + serverPort + "/sensor-data";
  // Send data to server
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connecting to server at ");
    Serial.print(serverIP);
    Serial.print(":");
    Serial.println(serverPort);

    //   if (client.connect(serverIP, serverPort)) {
    //     Serial.println("Connected to server");
    //     client.println("POST / HTTP/1.1");
    //     client.println("Host: " + String(serverIP));
    //     client.println("Content-Type: application/x-www-form-urlencoded");
    //     client.println("Content-Length: " + String(postData.length()));
    //     client.println();
    //     client.print(postData);

    //     // Read server response
    //     Serial.println("----- Server Response -----");
    //     while (client.connected() || client.available()) {
    //       if (client.available()) {
    //         String response = client.readString();
    //         Serial.println(response);
    //         break;
    //       }
    //     }
    //     client.stop();
    //   } else {
    //     Serial.println("Connection to server failed");
    //   }
    // } else {
    //   Serial.println("WiFi disconnected");
    // }

    WiFiClient client;
    HTTPClient http;


    if (http.begin(client, serverSensorURL)) {
      // Add header for form data (x-www-form-urlencoded)
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // sending data
      // Send POST request with the sensor data
      int httpCode = http.POST(postData);

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

      http.end();
    }




    // getting data
    String buzzerUrl = String("http://") + serverIP + ":" + serverPort + "/buzzer";
    if (http.begin(client, buzzerUrl)) {
      int buzzerCode = http.GET();

      if (buzzerCode == 200)
        buzzerOn = true;
      else {
        Serial.printf("Buzzer off: %d", buzzerCode);
      }
    }

    if (buzzerOn) {
      tone(BUZZER_PIN, 1000, 500);
    }
    http.end();


    String craddleURL = String("http://") + serverIP + ":" + serverPort + "/craddle";

    if (http.begin(client, buzzerUrl)) {
      int craddleCode = http.GET();

      if (craddleCode == 200)
        craddleOn = true;
      else {
        Serial.printf("Craddle Off: %d", craddleCode);
      }
    }

    if (craddleOn) {

      // servo motor on
    int pos;

  for (pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(15);           // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    servo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15ms for the servo to reach the position
  }

    }
    http.end();
  }
  // loop
  Serial.println("------------------------");
  delay(5000);  // Wait 5 seconds before the next loop
}