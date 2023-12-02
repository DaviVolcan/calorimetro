#include <Arduino.h>
#include "HX711.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#define ConstanteAgua 1
// Pinos dos sensores
const int LOADCELL_DOUT_PIN = 17;
const int LOADCELL_SCK_PIN = 16;
const int ONE_WIRE_BUS = 15; // Pin para o sensor DS18B20
// objetos dos sensores
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
HX711 scale;
// variavies globais
int MassaInicial = 0;         // para medir a quantidade de liquido base
int MassaFinal = 0;           // para medir a quantidade de liquido final
float TemperaturaInicial = 0; // para armazenar a temperatura inicial
float TemperaturaFinal = 0;   // para armazenar a temperatura final
float ConstanteLiquido = 0;   // para armazenar a constante do liquido.

// funcao para calcular a capacidade termica do calorimetro
int CapacidadeCalorimetro(int massa, float ConstanteLiquido)
{
  int capacidade = massa * ConstanteLiquido;
  return capacidade;
}

void setup()
{
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  sensors.begin();
  scale.tare(10);
  scale.set_scale(383);
}

void loop()
{
  if (scale.is_ready())
  {
    long reading = scale.get_units(10);
    Serial.print("HX711 reading: ");
    Serial.println(reading);
  }
  else
  {
    Serial.println("HX711 not found.");
  }

  sensors.requestTemperatures();                   // Solicita a leitura da temperatura ao sensor DS18B20
  float temperatureC = sensors.getTempCByIndex(0); // Obtém a temperatura em graus Celsius

  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");

  delay(1000);
}

void calorimetro()
{
  delay(10000);                                                    // espera 10 segundos para estabilizar a temperatura e a massa
  MassaInicial = scale.get_units(10);                              // realiza a leitura da massa inicial
  TemperaturaInicial = sensors.getTempCByIndex(0);                 // realiza a leitura da temperatura inicial
  Serial.println("Coloque o liquido de interesse no calorimetro"); // joga na serial um aviso para que se carregue o liquidio dde interesse
  delay(10000);                                                    // espera 10 segundos para estabilizar a temperatura e a massa
MassaFinal = scale.get_units(10);                                // realiza a leitura da massa final
  TemperaturaFinal = sensors.getTempCByIndex(0);                   // realiza a leitura da temperatura final
  ConstanteLiquido = ConstanteAgua * (TemperaturaFinal - TemperaturaInicial) / (MassaFinal - MassaInicial); // calcula a constante do liquido
  Serial.print("A constante do liquido e: ");                     // joga na serial a constante do liquido
  Serial.println(ConstanteLiquido);
  Serial.print("A capacidade calorifica do calorimetro e: ");      // joga na serial a capacidade calorifica do calorimetro
  Serial.println(CapacidadeCalorimetro(MassaInicial, ConstanteLiquido));
}