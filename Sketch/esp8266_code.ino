//hello

#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

#include <WiFiClient.h>

// WiFi credentials

const char* ssid = "Hardware_Hackathon-2";
const char* password = "hackathon@2025";

// Server details
const char* serverIP = "192.168.78.221";
const int serverPort = 8000;

// Define the DHT sensor type and pin
#define DHTPIN D3      // Pin connected to the DHT sensor
#define DHTTYPE DHT22  // DHT11 or DHT22
#define SERVO_PIN D4
#define BUZZER_PIN D8


DHT dht(DHTPIN, DHTTYPE);  // Initialize the DHT sensor

// Define the rain sensor analog pin
#define RAIN_SENSOR_PIN A0  // Rain sensor connected to A0 pin

// Define the MQ-6 sensor analog pin
#define MQ6_SENSOR_PIN D0  // MQ-6 gas sensor connected to D0 pin

WiFiClient client;
Servo servo;

void setup() {
  Serial.begin(115200);  // Start the Serial Monitor at 115200 baud rate
  dht.begin();           // Initialize the DHT sensor
  delay(2000);           // Wait for the DHT sensor to initialize

  servo.attach(SERVO_PIN);
  pinMode(BUZZER_PIN, OUTPUT);


  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Read temperature and humidity from the DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();  // Default is Celsius
  servo.attach(4);

  // Check if reading from the DHT sensor was successful
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Read the rain sensor value (0-1023 from A0 pin)
  int rainSensorValue = analogRead(RAIN_SENSOR_PIN);

  // Read the MQ-6 gas sensor value (0-1023 from D0 pin)
  int mq6SensorValue = analogRead(MQ6_SENSOR_PIN);

  // Output the sensor readings to the Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.print("°C  Rain Sensor Value: ");
  Serial.print(rainSensorValue);
  Serial.print("  MQ-6 Sensor Value: ");
  Serial.println(mq6SensorValue);

  //performing curdulling action


  // Check the rain sensor value and provide a simple indication
  if (rainSensorValue < 1023) {  // Adjust this threshold based on your sensor's behavior
    Serial.println("Rain detected!");
  } else {
    Serial.println("No rain detected.");
  }

  // Check the MQ-6 gas sensor value for detecting gases (threshold can be adjusted)
  if (mq6SensorValue > 200) {  // Adjust the threshold depending on your environment
    Serial.println("Gas detected by MQ-6!");
  } else {
    Serial.println("No gas detected.");
  }

  // Send data to the server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String serverPath = String("http://") + serverIP + ":" + serverPort + "/sensor-data";

    String postData = String("{\"temperature\":") + temperature + ",\"humidity\":" + humidity + ",\"moisture\":" + rainSensorValue + ",\"mq\":" + mq6SensorValue + "}";

    http.begin(client, serverPath);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      Serial.print("Server response: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending data: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    http.end();

  } else {
    Serial.println("WiFi not connected. Unable to send data.");
  }
  // for buzzer and cradle
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("wifi connected.");
    HTTPClient http;
    String craddlePath = String("http://") + serverIP + ":" + serverPort + "/craddle";
    http.begin(client, craddlePath);
    int craddleCode = http.GET();
    if (craddleCode == 200) {
      int mappedPos;

      Serial.println("Performing cradling action...");
      for (float pos = 0; pos <= 45; pos += 0.5) {  // Smaller step size for smoothness
        mappedPos = map(pos, -90, 90, 0, 180);      // Map -90 to 90 range to 0 to 180
        servo.write(mappedPos);
        delay(10);  // Adjust delay for smoothness
      }

      // Backward swing: 45 to 0 degrees
      for (float pos = 45; pos >= 0; pos -= 0.5) {
        mappedPos = map(pos, -90, 90, 0, 180);
        servo.write(mappedPos);
        delay(10);
      }

      // Downward swing: 0 to -45 degrees
      for (float pos = 0; pos >= -45; pos -= 0.5) {
        mappedPos = map(pos, -90, 90, 0, 180);
        servo.write(mappedPos);
        delay(10);
      }

      // Return swing: -45 to 0 degrees
      for (float pos = -45; pos <= 0; pos += 0.5) {
        mappedPos = map(pos, -90, 90, 0, 180);
        servo.write(mappedPos);
        delay(10);
      }
    } else {
      Serial.printf("No craddling action: %d", craddleCode);
    }

    http.end();


    String buzzerPath = String("http://") + serverIP + ":" + serverPort + "/buzzer";
    http.begin(client, buzzerPath);
    int buzzerCode = http.GET();
    if (buzzerCode == 200) {
      digitalWrite(BUZZER_PIN, HIGH);
      Serial.printf("Buzzer on: %d", buzzerCode);

    } else {
      Serial.printf("Buzzer off: %d", buzzerCode);
      digitalWrite(BUZZER_PIN, LOW);
    }
    http.end();
  } else {
    Serial.println("WiFi not connected. Unable to send data.");
  }


  delay(2000);  // Wait for 2 seconds before the next reading
}