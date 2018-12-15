/*
 * Realizar una función para leer 1 byte de un registro en un 
 * dispositivo utilizando las funciones de la librería Wire de 
 * Arduino. Utilizarlo para leer el ID del BMP180 cuando se 
 * pulsa el botón A del M5Stack y mostrarlo por el monitor serie.
 * • Dirección del BMP180 0x77
 * • El ID del BMP180 está en la dirección 0xD0 y tiene el valor 0x55
 * • Función: uint8_t read_1byte(uint8_t device_addr, uint8_t read_addr);
 */
#include <M5Stack.h>
#include <Wire.h>

#define BMP180 0x77
#define idBMP180 0xD0

const int pinSDA = 21;
const int pinSCL = 22;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  Wire.begin(pinSDA, pinSCL);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(M5.BtnA.wasPressed()){
  Serial.println("-----------------------------------");
  Serial.println("Prueba de control del sensor BMP180");
  Serial.println("-----------------------------------");
  Serial.print("BMP180_ID = 0x");
  
  uint8_t data = read_1byte(BMP180, idBMP180);
  
  Serial.println(data, HEX);
  }
  
  M5.update();

}

uint8_t read_1byte(uint8_t device_addr, uint8_t read_addr){
  uint8_t ret;
  // 1º FASE: Escribir la dirección del registro que se quiere leer
  Wire.beginTransmission(BMP180);
  Wire.write(read_addr);
  Wire.endTransmission();
  // 2ª FASE: Leer el registro
  Wire.requestFrom(device_addr, 1);
  ret = Wire.read(); // receive DATA

  return ret;
}
