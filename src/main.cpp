#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Dirección I2C de la LCD 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin del potenciómetro
const int potPin = 34; // GPIO 34 (ADC1)

// Variables para el reloj (Inicia en 03:26:00 como en tu ejemplo)
int horas = 3;
int minutos = 26;
int segundos = 0;

unsigned long previoMillis = 0;
const long intervalo = 1000; // 1 segundo (1000 ms)

void setup() {
  // Inicialización del puerto serie
  Serial.begin(115200);
  delay(1000); // Pausa para estabilizar la comunicación serial

  // Inicialización del bus I2C (SDA = 21, SCL = 22)
  Wire.begin(21, 22);

  // Inicializar pantalla LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  unsigned long actualMillis = millis();

  // Se ejecuta exactamente cada 1 segundo
  if (actualMillis - previoMillis >= intervalo) {
    previoMillis = actualMillis;

    // --- LÓGICA DE TIEMPO ---
    segundos++;
    if (segundos >= 60) {
      segundos = 0;
      minutos++;
      if (minutos >= 60) {
        minutos = 0;
        horas++;
        if (horas >= 24) {
          horas = 0;
        }
      }
    }

    // Leer el valor instantáneo del potenciómetro
    int valorPot = analogRead(potPin);

    // --- 1. MOSTRAR EN MONITOR SERIAL ---
    // Formato: Hora actual: HH:MM:SS | Potenciometro: XXXX
    char bufferSerial[65];
    snprintf(bufferSerial, sizeof(bufferSerial), 
             "Hora actual: %02d:%02d:%02d | Potenciometro: %d", 
             horas, minutos, segundos, valorPot);
    Serial.println(bufferSerial);

    // --- 2. MOSTRAR EN LA PANTALLA LCD 16x2 ---
    // Fila superior: Hora centrada "03:26:15"
    char bufferHora[9];
    snprintf(bufferHora, sizeof(bufferHora), "%02d:%02d:%02d", horas, minutos, segundos);
    lcd.setCursor(4, 0); // Columna 4 para centrar "HH:MM:SS" (8 caracteres)
    lcd.print(bufferHora);

    // Fila inferior: "POT: 1308"
    lcd.setCursor(0, 1); // Inicio de la Fila 1
    lcd.print("POT: ");
    lcd.print(valorPot);
    lcd.print("    "); // Espacios en blanco para limpiar dígitos sobrantes
  }
}