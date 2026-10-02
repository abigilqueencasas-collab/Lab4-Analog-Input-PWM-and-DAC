#include <Arduino.h>

// Example 3: Analog Input (Potentiometer Reading)
// Reads the raw ADC value and the equivalent millivolts from a potentiometer.

const uint8_t POT_PIN = 34;   // ADC1 channel, input-only pin

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);               // 12-bit resolution -> 0-4095
  analogSetPinAttenuation(POT_PIN, ADC_11db); // allows ~0-3.3V full range
}

void loop() {
  const int raw = analogRead(POT_PIN);
  const uint32_t millivolts = analogReadMilliVolts(POT_PIN);

  Serial.print("Raw:");
  Serial.print(raw);
  Serial.print("\tMillivolts:");
  Serial.println(millivolts);

  delay(2000);
}
