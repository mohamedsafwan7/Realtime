#include "DHT.h"

#define MOISTURE_PIN A0
#define DHT_PIN 4
#define RELAY_PIN 7
#define DHT_TYPE DHT11   // change to DHT22 if needed

DHT dht(DHT_PIN, DHT_TYPE);

// --- Calibration values for UNO (0-1023) ---
const int RAW_DRY = 536;   // replace after measuring
const int RAW_WET = 270;   // replace after measuring

const float TARGET = 60.0;
const float DEADBAND = 5.0;

bool pumpOn = false;

float readMoisturePercent() {
  long sum = 0;
  for (int i = 0; i < 20; i++) {
    sum += analogRead(MOISTURE_PIN);
    delay(5);
  }
  int raw = sum / 20;

  float pct = 100.0 * (RAW_DRY - raw) / (float)(RAW_DRY - RAW_WET);
  if (pct < 0) pct = 0;
  if (pct > 100) pct = 100;
  return pct;
}

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);   // relay OFF
  dht.begin();
}

void loop() {
  float moisture = readMoisturePercent();
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  bool dhtOk = !(isnan(temp) || isnan(hum));

  // Pump control with hysteresis
  if (!pumpOn && moisture < TARGET - DEADBAND) {
    pumpOn = true;
  }
  else if (pumpOn && moisture > TARGET + DEADBAND) {
    pumpOn = false;
  }

  digitalWrite(RELAY_PIN, pumpOn ? HIGH : LOW);

  // --- single clean JSON line ---
  Serial.print("{\"time\":");
  Serial.print(millis());
  Serial.print(",\"pct\":");
  Serial.print(moisture, 1);

  Serial.print(",\"temp\":");
  if (dhtOk) Serial.print(temp, 1); else Serial.print("null");

  Serial.print(",\"humidity\":");
  if (dhtOk) Serial.print(hum, 1); else Serial.print("null");

  Serial.print(",\"pump\":");
  Serial.print(pumpOn ? "true" : "false");
  Serial.println("}");

  delay(2000);
}