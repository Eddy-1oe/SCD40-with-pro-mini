// Include necessary libraries
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <SensirionI2CScd4x.h>
#include <Wire.h>

// Initialize the sensor
SensirionI2CScd4x scd4x;

// Define LED pins
const int greenled = 12 ;
const int redled = 10;
const int yellowled = 11;

// Define SoftwareSerial pins
#define RX 3
#define TX 2

// Define LDR and buzzer pins
const int ldr = A2;
int ldrReading = 0;
const int led = 13;
const int buzzer = 7;

// Initialize SoftwareSerial
SoftwareSerial BTSerial(RX, TX);  // (RX, TX)
#define BAUDRATE 9600

// Function to print a uint16_t value in hexadecimal
void printUint16Hex(uint16_t value) {
  Serial.print(value < 4096 ? "0" : "");
  Serial.print(value < 256 ? "0" : "");
  Serial.print(value < 16 ? "0" : "");
  Serial.print(value, HEX);
}

// Function to print the sensor's serial number
void printSerialNumber(uint16_t serial0, uint16_t serial1, uint16_t serial2) {
  Serial.print("Serial: 0x");
  printUint16Hex(serial0);
  printUint16Hex(serial1);
  printUint16Hex(serial2);
  Serial.println();
}

// Setup function
void setup() {
  // Begin serial communication
  Serial.begin(BAUDRATE);
  BTSerial.begin(BAUDRATE);

  // Wait for serial port to connect
  while (!Serial) {
    delay(100);
  }
  // Begin I2C communication
  Wire.begin();

  uint16_t error;
  char errorMessage[256];

  // Initialize the sensor
  scd4x.begin(Wire);
  // Set pin modes
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(yellowled, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(ldr, INPUT);
  pinMode(buzzer, OUTPUT);

  // Stop potentially previously started measurement
  error = scd4x.stopPeriodicMeasurement();
  if (error) {
    Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
  }

  uint16_t serial0;
  uint16_t serial1;
  uint16_t serial2;
  // Get the sensor's serial number
  error = scd4x.getSerialNumber(serial0, serial1, serial2);
  if (error) {
    Serial.print("Error trying to execute getSerialNumber(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
    digitalWrite(buzzer, LOW);
  } else {
    printSerialNumber(serial0, serial1, serial2);
  }

  // Start Measurement
  error = scd4x.startPeriodicMeasurement();
  if (error) {
    Serial.print("Error trying to execute startPeriodicMeasurement(): ");
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
    digitalWrite(buzzer, LOW);
  }

  Serial.println("Waiting for first measurement... (5 sec)");
}

// Function to check conditions and control LEDs and buzzer accordingly
void checkConditions(uint16_t co2_ppm, float temperature_C, float humidity_percent) {
  if (co2_ppm < 1200 && temperature_C < 30 && humidity_percent < 75) {
    digitalWrite(greenled, HIGH);
    digitalWrite(yellowled, LOW);
    digitalWrite(redled, LOW);
    digitalWrite(buzzer, LOW);
  } else if (co2_ppm < 1700 && temperature_C < 37 && humidity_percent < 90) {
    digitalWrite(greenled, HIGH);
    digitalWrite(yellowled, HIGH);
    digitalWrite(redled, LOW);
    digitalWrite(buzzer, LOW);
  }
  else {
    digitalWrite(greenled, HIGH);
    digitalWrite(yellowled, HIGH);
    digitalWrite(redled,HIGH);
    digitalWrite(buzzer,HIGH);
  }
}

// Function to check light conditions and control LED accordingly
void checkLight(int ldrReading){
  if (ldrReading < 300) {
    digitalWrite(led, HIGH);
  } 
  else {
    delay(5000);
    digitalWrite(led, LOW);
  }
}

// Main loop
void loop() {
  // Read LDR value
  ldrReading = analogRead(ldr);
  delay(100);

  uint16_t error;
  char errorMessage[256];

  // Read Measurement
  uint16_t co2;
  float temperature;
  float humidity;
  error = scd4x.readMeasurement(co2, temperature, humidity);
  if (error) {
    Serial.print("Error trying to execute readMeasurement(): ");
    BTSerial.print("Error trying to execute readMeasurement(): ");
    errorToString(error, errorMessage, 256);
    errorToString(error, errorMessage, 256);
    Serial.println(errorMessage);
    BTSerial.println(errorMessage);
  } else if (co2 == 0) {
    Serial.println("Invalid sample detected, skipping.");
    BTSerial.println("Invalid sample detected, skipping.");
  } else {
    // Print CO2, temperature, and humidity values
    Serial.print("Co2:");
    Serial.print(co2);
    Serial.print("ppm");
    Serial.print("\t");
    BTSerial.print(co2);
    BTSerial.print(",");
    Serial.print("Temperature:");
    Serial.print(temperature);
    Serial.print("°C");
    Serial.print("\t");
    BTSerial.print(temperature);
    BTSerial.print(",");
    Serial.print("Humidity:");
    Serial.print(humidity);
    Serial.print("%");
    Serial.print("\t");
    BTSerial.print(humidity);
    BTSerial.print(",");
    Serial.print("\t");
    Serial.print("L_Val: ");
    Serial.print(ldrReading);
    Serial.println("Cd");
    BTSerial.print(ldrReading);
  }
  // Check conditions and control LEDs and buzzer
  checkConditions(co2, temperature, humidity);
  // Check light conditions and control LED
  checkLight(ldrReading);
  delay(5000);
}
