/*
 * Realizar una función para leer una palabra de 16 bits (2 bytes)
 * almacenada en 2 registros cuyas direcciones son consecutivas.
 * Utilizarlo para leer la el valor digitalizado de la temperatura 
 * del BMP180 cuando se pulsa el botón A del M5Stack y mostrarlo por 
 * el monitor serie. Utilizar el analizador lógico para visualizar y
 * analizar las tramas I2C.
 * • Dirección del BMP180 0x77
 * • Dirección del registro con el byte MSB 0x6F
 * • Dirección del registro con el byte LSB 0x7F
 * • Función: uint16_t read_2bytes(uint8_t device_addr, uint8_t read_addr);
 */
#include <M5Stack.h>
#define BMP180 0x77
#define MSB 0x6F
#define LSB 0x7F // No hace falta (*1)
const int pinSDA = 21;
const int pinSCL = 22;
void setup() {
  M5.begin();
  Wire.begin(pinSDA, pinSCL);
}

void loop() {
  if(M5.BtnA.wasPressed()){
    uint16_t temp = read_2bytes(BMP180, MSB);
    Serial.println("-----------------------------------");
    Serial.println("Prueba de control del sensor BMP180");
    Serial.println("-----------------------------------");
    Serial.print("Lectura de 2 bytes= 0x");
    Serial.println(temp, HEX);
  }
  M5.update();
}
uint16_t read_2bytes(uint8_t device_addr, uint8_t read_addr){
  Wire.beginTransmission(device_addr);
  Wire.write(read_addr);
  Wire.endTransmission();

  Wire.requestFrom(device_addr, 2);
  uint16_t temp = Wire.read() << 8 | Wire.read()
  return temp;
}
