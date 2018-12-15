/*
 * Realizar una función para escribir un byte en un registro de un
 * dispositivo. Utilizarlo para escribir el registro de control del 
 * BMP180 e inicializar la conversión de temperatura cuando se pulsa 
 * el botón A del M5Stack. Una vez se ha escrito, leer el valor del 
 * registro de control y mostrarlo por el monitor serie para comprobar 
 * que se ha escrito correctamente. Utilizar el analizador lógico para 
 * visualizar y analizar las tramas I2C.
 * • Dirección del BMP180 0x77
 * • Dirección del registro de control 0xF4
 * • Control de inicio de medida de temperatura 0x2A
 * • Función: void write_1byte(uint8_t device_addr, uint8_t write_addr, uint8_t data);
 */
 #define BMP180 0x77;
 #define registroControl 0xF4;
 #define inicioTemperatura 0x2A

 const int pinSDA = 21;
 const int pinSCL = 22;
void setup() {
  M5.begin();
  Wire.begin(pinSDA, pinSCL);
}

void loop() {

  if(M5.BtnA.wasPressed()){
    uint8_t temp = read_1byte(BMP180, inicioTemperatura);
    write_1byte(BMP180, registroControl, temp);
  }
  
  M5.update();
}

void write_1byte(uint8_t device_addr, uint8_t write_addr, uint8_t data){
  uint8_t ret;

  Wire.beginTransmission(device_addr);
  Wire.write(write_addr); // dónde queremos escribir
  Wire.write(data); // Qué queremos escribir
  Wire.endTransmission(); // Finalizar transmisión

  //Comprobación el dato escrito a través del Monitor Serie.
  //Leemos el dato en el registro donde lo hemos escrito y
  //luego lo mostramos por el serial.  
  Wire.requestFrom(device_adress, 1); // De qué dispositivo queremos leer y cuantos bytes queremos leer
  ret = Wire.read(); // lectura
  
  Serial.print("El valor guardado en la dirección 0xF4 (Registro de control) es: " );
  Serial.println(ret);
}

uint8_t read_1byte(uint8_t device_addr, uint8_t read_addr){
  uint8_t ret;
  // 1º FASE: Escribir la dirección del registro que se quiere leer
  Wire.beginTransmission(device_addr);
  Wire.write(read_addr);
  Wire.endTransmission();
  // 2ª FASE: Leer el registro
  Wire.requestFrom(device_addr, 1);
  ret = Wire.read(); // receive DATA

  return ret;
}
