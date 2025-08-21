#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "YourWifiName";
const char* password = "YourWifiPassword";
String apiKey = "****************"; // Replace with your actual ThingSpeak API key

// LED pins
#define RED_LED    2
#define YELLOW_LED 15
#define GREEN_LED  4

// Ultrasonic sensor pins
#define TRIG_PIN  5
#define ECHO_PIN  18

long duration;
float distance;
int fillPercent;
int statusCode;
String statusLabel;

const int binHeight = 30; // cm (adjust based on your bin's actual depth)

void setup() {
  Serial.begin(115200);

  // LED setup
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Ultrasonic setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Wi-Fi connection with timeout
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  unsigned long startAttemptTime = millis();
  unsigned long timeout = 10000; // 10 seconds

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected");
  } else {
    Serial.println("\nWiFi connection failed");
    // Optional: blink RED LED to indicate failure
    digitalWrite(RED_LED, HIGH);
  }
}

void loop() {
  // Measure distance
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * 0.0343) / 2; // cm

  // Calculate fill percentage
  fillPercent = map(distance, binHeight, 5, 0, 100); // 5 cm = full, binHeight = empty
  fillPercent = constrain(fillPercent, 0, 100);

  // LED logic + status assignment
  if (fillPercent > 80) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    statusCode = 2;
    statusLabel = "FULL";
  }
  else if (fillPercent > 40) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    statusCode = 1;
    statusLabel = "HALF";
  }
  else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    statusCode = 0;
    statusLabel = "EMPTY";
  }

  // Debug output
  Serial.println("Distance: " + String(distance) + " cm");
  Serial.println("Fill %: " + String(fillPercent));
  Serial.println("Status: " + statusLabel + " (" + String(statusCode) + ")");

  // Send data to ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(fillPercent) +
                 "&field2=" + String(statusCode);

    http.begin(url);
    int httpCode = http.GET();
    http.end();
    Serial.println("ThingSpeak update: " + String(httpCode));
  } else {
    Serial.println("WiFi not connected. Skipping ThingSpeak update.");
  }

  delay(200); // Wait 2 seconds before next update
}