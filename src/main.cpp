#include <Arduino.h>
#include "HX711.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#define ConstanteAgua 0.85
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
float TemperaturaOleo = 66.0; // para armazenar a temperatura final
float ConstanteLiquido = 0;   // para armazenar a constante do liquido.
int capacidade = 0;           // para armazenar a capacidade calorifica do calorimetro

void desenvolvimento()
{ // funcao para printer todos os dados
  Serial.print("Massa Inicial ");
  Serial.println(MassaInicial);
  Serial.print("Massa Final ");
  Serial.println(MassaFinal);
  Serial.print("Diferenca de Massa ");
  Serial.println(MassaFinal - MassaInicial);
  Serial.print("Temperatura Inicial ");
  Serial.println(TemperaturaInicial);
  Serial.print("Temperatura Final ");
  Serial.println(TemperaturaFinal);
  Serial.print("Temperatura Oleo ");
  Serial.println(TemperaturaOleo);
  Serial.print("Constante Liquido ");
  Serial.println(ConstanteLiquido);
}
void calorimetro()
{
  delay(2000);                                         // aguarda um tempo pra conexao ser estabelecida                                                                                    // espera 10 segundos para estabilizar a temperatura e a massa
  Serial.println("___ Coloque a agua no calorimetro"); // joga na serial um aviso para que se carregue a agua
  desenvolvimento();
  delay(20000);                       // espera 10 segundos para estabilizar a temperatura e a massa
  MassaInicial = scale.get_units(10); // realiza a leitura da massa inicial
  sensors.requestTemperatures();
  TemperaturaInicial = sensors.getTempCByIndex(0); // realiza a leitura da temperatura inicial
  capacidade = MassaInicial * ConstanteAgua;       // calcula a capacidade calorifica do calorimetro
  // pinta a capacidade termica
  Serial.print("Capacidade termica estimada "); // joga na serial um aviso para que se carregue o liquidio dde interesse
  Serial.print(capacidade);                     // joga na serial um aviso para que se carregue o liquidio dde interesse
  Serial.println(" cal/ºC");                    // joga na serial um aviso para que se carregue o liquidio dde interesse
  Serial.println("___ Coloque o sensor no liquido de interesse");
  delay(15000); // espera 40 segundos para estabilizar a temperatura e a massa
  sensors.requestTemperatures();
  TemperaturaOleo = sensors.getTempCByIndex(0);                                   // realiza a leitura da temperatura do liquido de interesse
  Serial.println("___ Coloque o liquido de interesse e o sensor no calorimetro"); // joga na serial um aviso para que se carregue o liquidio dde interesse
  delay(50000);                                                                   // espera 10 segundos para estabilizar a temperatura e a massa
  MassaFinal = scale.get_units(10);                                               // realiza a leitura da massa final
  sensors.requestTemperatures();
  TemperaturaFinal = sensors.getTempCByIndex(0);
  // calcula o valor da constante do segundo liquido com base na formula co = C (Tf - TC) /m1(T1 - Tf)
  ConstanteLiquido = capacidade * (TemperaturaFinal - TemperaturaInicial) / ((MassaFinal - MassaInicial) * (TemperaturaOleo - TemperaturaFinal));

  Serial.print("A constante do liquido é: "); // joga na serial a constante do liquido
  Serial.println(ConstanteLiquido);
  desenvolvimento();
}

void setup()
{
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  sensors.begin();
  scale.tare(10);
  scale.set_scale(383); // ok
  calorimetro();
}

void test()
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

void loop()
{

  // test();
}