#include "HX711.h"

// Pinos no ESP32
const int PINO_DT = 13;   // Ou GPIO 16 se você mudou os fios
const int PINO_SCK = 14;  // Ou GPIO 17 se você mudou os fios

HX711 balanca;

// Fator de calibração padrão (deve ser diferente de ZERO!)
float fatorCalibracao = 4900.0; 

// Limite máximo suportado (1 kg)
const float CAPACIDADE_MAXIMA = 1000.0; 

void setup() {
  Serial.begin(115200);
  delay(1000); 

  Serial.println("\n======================================");
  Serial.println("  BALANÇA DIGITAL ESP32 - CORREÇÃO INF ");
  Serial.println("======================================");

  balanca.begin(PINO_DT, PINO_SCK);

  // Aguarda resposta do módulo HX711
  while (!balanca.is_ready()) {
    Serial.println("Aguardando módulo HX711...");
    delay(500);
  }

  // Define o fator de calibração ANTES de zerar
  balanca.set_scale(fatorCalibracao);
  
  Serial.println("Zerando a balança (Tara)... Mantenha a base sem peso!");
  delay(2000);
  balanca.tare(); 

  Serial.println("[OK] Balança pronta!");
  Serial.println("--------------------------------------");
}

void loop() {
  if (balanca.is_ready()) {
    // Pega o peso em gramas
    float peso = balanca.get_units(5); 

    // Filtra pequenas oscilações de ruído
    if (peso < 0.5 && peso > -0.5) {
      peso = 0.0;
    }

    // Alerta de limite máximo (1 kg)
    if (peso > CAPACIDADE_MAXIMA) {
      Serial.print("[ALERTA SOBRECARGA] Peso: ");
      Serial.print(peso, 1);
      Serial.println(" g (Limite do sensor: 1000g!)");
    } else {
      Serial.print("Peso: ");
      Serial.print(peso, 1);
      Serial.println(" g");
    }

  } else {
    Serial.println("[ERRO] Falha na comunicação com o HX711.");
  }

  delay(500);
}
