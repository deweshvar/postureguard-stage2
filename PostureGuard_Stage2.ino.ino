/* 
 * Project: PostureGuard 2.0 - Stage 2 Firmware
 * Description: Dual-MCU telemetry (ESP32 Master & ESP32-C3 Satellite), 
 * MPU-6050 IMU tracking, MyoWare sEMG polling, and ESP-NOW haptic feedback.
 */

#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>

// Pin definitions
const int sEMG_Pin = 34;      // MyoWare sEMG analog envelope pin
const int hapticPin = 18;     // 2N2222 Transistor gate pin for 1027 vibration motor

void setup() {
  Serial.begin(115200);
  pinMode(hapticPin, OUTPUT);
  pinMode(sEMG_Pin, INPUT);
  
  // Initialize I2C for MPU-6050
  Wire.begin();
  
  // Initialize Wi-Fi in station mode for ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Initialization Failed");
    return;
  }
  
  Serial.println("PostureGuard 2.0 Firmware Initialized Successfully.");
}

void loop() {
  // Read sEMG muscle activation envelope
  int emgValue = analogRead(sEMG_Pin);
  
  // Print telemetry to Serial Monitor for debugging
  Serial.print("sEMG Muscle Signal: ");
  Serial.println(emgValue);
  
  // Threshold check for muscle fatigue / compensatory cheating
  if (emgValue > 2500) { 
    digitalWrite(hapticPin, HIGH); // Trigger haptic warning motor (< 5ms response)
    Serial.println("Warning: Compensation / Fatigue Detected - Haptic Triggered");
  } else {
    digitalWrite(hapticPin, LOW);
  }
  
  delay(50); // 20Hz sampling loop
}