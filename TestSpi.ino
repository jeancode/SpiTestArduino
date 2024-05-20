#include <SPI.h>
#include <stdlib.h> // Necesario para la función random()

#define PIN_MISO 12 // Pin MISO
#define PIN_MOSI 13 // Pin MOSI
#define PIN_SCLK 14 // Pin SCLK
#define PIN_CS   15 // Pin CS
#define MAX_ARRAY_SIZE 200 // Tamaño máximo del array de bytes aleatorios
#define NUM_TESTS 25 // Número de pruebas a realizar

void runTest(uint8_t arraySize, uint32_t speed) {
  SPISettings settings(speed, MSBFIRST, SPI_MODE0); // Configuración de velocidad
  unsigned long totalTime = 0; // Tiempo total acumulado

  uint8_t dataOut[arraySize]; // Array para almacenar los bytes aleatorios
  uint8_t dataIn[arraySize]; // Array para almacenar los bytes recibidos

  // Generar el array de bytes aleatorios
  for (int i = 0; i < arraySize; i++) {
    dataOut[i] = random(256); // Generar byte aleatorio
  }

  digitalWrite(PIN_CS, LOW); // Seleccionar dispositivo
  SPI.beginTransaction(settings);
  unsigned long startTime = micros(); // Tiempo de inicio
  SPI.transferBytes(dataOut, dataIn, arraySize); // Enviar y recibir arrays de bytes
  unsigned long endTime = micros(); // Tiempo de fin
  totalTime += (endTime - startTime); // Sumar al tiempo total
  SPI.endTransaction();
  digitalWrite(PIN_CS, HIGH); // Deseleccionar dispositivo

  Serial.print("Array Size: ");
  Serial.print(arraySize);
  Serial.print(" | Speed: ");
  Serial.print(speed / 1000000.0); // Mostrar velocidad en Mbps
  Serial.print(" Mbps | Total Time (us): ");
  Serial.print(totalTime); // Mostrar tiempo total acumulado

  // Mostrar tiempo de cada interacción
  Serial.print(" | Interactions Time (us): ");
  Serial.print(endTime - startTime);

  // Comprobar si los bytes enviados y recibidos son iguales
  bool equal = true;
  for (int i = 0; i < arraySize; i++) {
    if (dataOut[i] != dataIn[i]) {
      equal = false;
      break;
    }
  }

  Serial.print(" | Bytes Match: ");
  Serial.println(equal ? "Yes" : "No"); // Mostrar si los bytes coinciden o no
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_MISO, INPUT);
  pinMode(PIN_MOSI, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  pinMode(PIN_CS, OUTPUT);
  digitalWrite(PIN_CS, HIGH); // Deseleccionar dispositivo
  SPI.begin(PIN_SCLK, PIN_MISO, PIN_MOSI, PIN_CS);
  randomSeed(analogRead(0)); // Inicializar generador de números aleatorios

  // Ejecutar múltiples pruebas con diferentes tamaños de array y velocidades
  for (int i = 0; i < NUM_TESTS; i++) {
    uint8_t arraySize = random(MAX_ARRAY_SIZE) + 1; // Generar tamaño aleatorio entre 1 y MAX_ARRAY_SIZE
    uint32_t speed = random(1000000, 50000000); // Generar velocidad aleatoria entre 1 Mbps y 50 Mbps
    runTest(arraySize, speed);
    delay(1000); // Esperar antes de la siguiente prueba
  }
}

void loop() {
  // No se necesita código en el bucle loop
}
