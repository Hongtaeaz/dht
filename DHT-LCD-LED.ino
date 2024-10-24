#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2      // Pin where the DHT sensor is connected
#define DHTTYPE DHT11 // DHT 11 sensor type

DHT dht(DHTPIN, DHTTYPE);            // Initialize DHT sensor
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars, 2 line display

#define GREEN_LED 4
#define YELLOW_LED 5
#define RED_LED 6
#define ON HIGH
#define OFF LOW

void setup() {
  Serial.begin(9600);                // Start serial communication
  Serial.println(F("DHTxx test!"));

  dht.begin();                       // Initialize the DHT sensor
  lcd.begin();                       // Initialize the LCD
  lcd.backlight();                   // Turn on the LCD backlight

  // Set LED pins as outputs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Turn off all LEDs initially
  digitalWrite(GREEN_LED, OFF);
  digitalWrite(YELLOW_LED, OFF);
  digitalWrite(RED_LED, OFF);
}

void loop() {
  delay(2000);                       // Wait 2 seconds between readings

  float h = dht.readHumidity();      // Read humidity
  float t = dht.readTemperature();   // Read temperature as Celsius
  
  // Check if any reads failed and exit early
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    lcd.clear();
    lcd.print("Sensor error");
    return;
  }

  // Print to Serial Monitor
  Serial.print(F("Temperature: "));
  Serial.print(t);
  Serial.print(F("°C  Humidity: "));
  Serial.print(h);
  Serial.println(F("%"));

  // Convert the temperature and humidity to strings
  String tempStr = "Temp: " + String(t) + " C";
  String humidStr = "Humidity: " + String(h) + " %";

  // Center temperature on line 1
  int tempStartPos = (16 - tempStr.length()) / 2;
  lcd.setCursor(tempStartPos, 0);    // Set cursor to center
  lcd.print(tempStr);

  // Center humidity on line 2
  int humidStartPos = (16 - humidStr.length()) / 2;
  lcd.setCursor(humidStartPos, 1);   // Set cursor to center
  lcd.print(humidStr);

  // LED control logic based on temperature
  if (t < 50) {
    digitalWrite(GREEN_LED, ON);      // Green LED on
    digitalWrite(YELLOW_LED, OFF);    // Yellow LED off
    digitalWrite(RED_LED, OFF);       // Red LED off
  } else if (t > 50 && t <= 60) {
    digitalWrite(GREEN_LED, OFF);     // Green LED off
    digitalWrite(YELLOW_LED, ON);     // Yellow LED on
    digitalWrite(RED_LED, OFF);       // Red LED off
  } else if (t > 60) {
    digitalWrite(GREEN_LED, OFF);     // Green LED off
    digitalWrite(YELLOW_LED, OFF);    // Yellow LED off
    digitalWrite(RED_LED, ON);        // Red LED on
  }
}
