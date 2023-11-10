#include <Arduino.h>
#include "HX711.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 17;
const int LOADCELL_SCK_PIN = 16;

const int ONE_WIRE_BUS = 15; // Pin para o sensor DS18B20

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

HX711 scale;

void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  sensors.begin();
}

void loop() {
  if (scale.is_ready()) {
    long reading = scale.read();
    Serial.print("HX711 reading: ");
    Serial.println(reading);
  } else {
    Serial.println("HX711 not found.");
  }

  sensors.requestTemperatures(); // Solicita a leitura da temperatura ao sensor DS18B20
  float temperatureC = sensors.getTempCByIndex(0); // Obtém a temperatura em graus Celsius

  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");

  delay(1000);
}