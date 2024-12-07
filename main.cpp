#define BLYNK_TEMPLATE_ID "BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "BLYNK_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "BLYNK_AUTH_TOKEN"
#define BLYNK_PRINT Serial
#include <Arduino.h>
#include <Wire.h>
#include <BlynkSimpleEsp32.h>
#include "DHT20.h"

// DHT20 I2C
#define DHT20_ADDRESS 0x38
DHT20 DHT(&Wire);

// Soil Moisture Sensors and Relays
#define NUM_PLANTS 3
const int soilSensorPins[NUM_PLANTS] = {38, 37, 36};
const int relayPins[NUM_PLANTS] = {26, 25, 33};
const int drainPin = 13;
const int startPin = 12;

// Moisture Threshold
const int moistureThreshold = 30;

// Blynk Virtual Pins
#define VPIN_SOIL_MOISTURE_PLANT_1 V1
#define VPIN_SOIL_MOISTURE_PLANT_2 V2
#define VPIN_SOIL_MOISTURE_PLANT_3 V3
#define VPIN_TEMP V5
#define VPIN_HUMIDITY V6

// BlynkTimer timer;

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "<YOUR_WIFI_SSID>";  // type your wifi name
char pass[] = "<YOUR_WIFI_PASSWORD>"; // type your wifi password

int read_soil_moisture(int pin)
{
  int x = analogRead(pin);
  return map(x, 3020, 1200, 0, 100);
}

void water_plant(int relayPinIndex)
{
  Serial.print("Watering Plant ");
  Serial.println(relayPinIndex);
  digitalWrite(relayPins[relayPinIndex - 1], LOW);  // Turn relay ON
  delay(1500);                                      // Water for 5 seconds
  digitalWrite(relayPins[relayPinIndex - 1], HIGH); // Turn relay OFF
}

void setup()
{
  Serial.begin(9600);
  delay(5000);
  WiFi.begin(ssid, pass); // Start Wi-Fi connection
  Serial.print("Connecting to WiFi");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print the IP address assigned to the ESP32
  Wire.begin(21, 22);             // For temp and humidity sensor
  for (int i = 0; i < NUM_PLANTS; i++)
  {
    pinMode(soilSensorPins[i], INPUT);
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], HIGH); // Set Relays to off
    pinMode(drainPin, OUTPUT);
    pinMode(startPin, OUTPUT);
  }
  pinMode(drainPin, OUTPUT);
  digitalWrite(drainPin, HIGH); // Set drain to off

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}

void loop()
{
  Blynk.run();
  if (digitalRead(startPin) == HIGH)
  {
    // Read and send temperature and humidity
    int status = DHT.read();
    switch (status)
    {
    case DHT20_OK:
      Serial.print("\tDHT 20 OK,\t");
      break;
    case DHT20_ERROR_CHECKSUM:
      Serial.print("Checksum error,\t");
      break;
    case DHT20_ERROR_CONNECT:
      Serial.print("Connect error,\t");
      break;
    case DHT20_MISSING_BYTES:
      Serial.print("Missing bytes,\t");
      break;
    case DHT20_ERROR_BYTES_ALL_ZERO:
      Serial.print("All bytes read zero");
      break;
    case DHT20_ERROR_READ_TIMEOUT:
      Serial.print("Read time out");
      break;
    case DHT20_ERROR_LASTREAD:
      Serial.print("Error read too fast");
      break;
    default:
      Serial.print("Unknown error,\t");
      break;
    }
    int err = 0;
    float temperature = DHT.getTemperature();
    float humidity = DHT.getHumidity();
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    Blynk.virtualWrite(VPIN_TEMP, temperature);
    Blynk.virtualWrite(VPIN_HUMIDITY, humidity);
    // Water plants and gather soil moisture data
    for (int i = 0; i < NUM_PLANTS; i++)
    {
      if (digitalRead(startPin) == LOW)
      {
        Blynk.virtualWrite(VPIN_SOIL_MOISTURE_PLANT_1, 0);
        Blynk.virtualWrite(VPIN_SOIL_MOISTURE_PLANT_2, 0);
        Blynk.virtualWrite(VPIN_SOIL_MOISTURE_PLANT_3, 0);
        Blynk.virtualWrite(VPIN_HUMIDITY, 0);
        Blynk.virtualWrite(VPIN_TEMP, 0);
        break;
      }
      int moistureLevel = read_soil_moisture(soilSensorPins[i]);
      Serial.print("Plant ");
      Serial.print(i + 1);
      Serial.print(": Moisture Level = ");
      Serial.println(moistureLevel);
      switch (i + 1)
      {
      case 1:
        Blynk.virtualWrite(VPIN_SOIL_MOISTURE_PLANT_1, moistureLevel);
        break;
      case 2:
        Blynk.virtualWrite(VPIN_SOIL_MOISTURE_PLANT_2, moistureLevel);
        break;
      case 3:
        Blynk.virtualWrite(VPIN_SOIL_MOISTURE_PLANT_3, moistureLevel);
        break;
      }
      // Water plant if moisture level is low
      if (moistureLevel < moistureThreshold)
      {
        water_plant(i + 1);
      }
      // delay(2000);
    }
    delay(5000);
  }
  else
  {
    Blynk.virtualWrite(VPIN_SOIL_MOISTURE_PLANT_1, 0);
    Blynk.virtualWrite(VPIN_SOIL_MOISTURE_PLANT_2, 0);
    Blynk.virtualWrite(VPIN_SOIL_MOISTURE_PLANT_3, 0);
    Blynk.virtualWrite(VPIN_HUMIDITY, 0);
    Blynk.virtualWrite(VPIN_TEMP, 0);
  }
}
