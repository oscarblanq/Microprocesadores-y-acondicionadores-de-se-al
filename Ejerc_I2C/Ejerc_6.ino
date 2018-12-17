/*
 * Realizar una función para leer 1 byte de un registro en un
 * dispositivo utilizando las funciones de la librería i2c.h del ESP-IDF
 * de ESPRESSIF. Utilizarlo para leer el ID del BMP180 cuando se
 * pulsa el botón A del M5Stack y mostrarlo por el monitor serie.
 * • Dirección del BMP180 0x77
 * • El ID del BMP180 está en la dirección 0xD0 y tiene el valor 0x55
 * • Función: uint8_t read_1byte(uint8_t device_addr, uint8_t read_addr);
 */
#include <M5Stack.h>
#include "driver/i2c.h"

#define BMP180 0x77
#define idBMP180 0xD0

const int pinSDA = 21;
const int pinSCL = 22;

// Manejador que nos permitirá leer y escribir
i2c_cmd_handle_t cmd = NULL;

void setup() {
  M5.begin();

  // Variable dónde se guarda la configuración del I2C
  i2c_config_t conf;
  //configuración del I2C, este objeto se pasa como puntero en el siguiente método i2c_param_config
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = pinSDA;
  conf.scl_io_num = pinSCL;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = 100000;

  //Establecemos la configuración del bus I2C pasándole
  //los parámetros que hemos introducido antes
  i2c_param_config(I2C_NUM_0, &conf);
  
  //Instalar el driver. Los últimos tres parámetros se
  //explican después
  i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER,0,0,0);
  /* =========== LOS TRES ÚLTIMOS PARÁMETROS ==========
   * 1. Tamaño del búfer de salida (SÓLO EN MODO ESCLAVO, sino es 0).
   * 2. Tamaño del búfer de entrada (SÓLO EN MODO ESCLAVO, sino es 0).
   * 3. Flag adicional para colocar interrupciones (Normalmente es 0).
   */
}

void loop() {
  if(M5.BtnA.wasPressed()){

  Serial.println("-----------------------------------");
  Serial.println("Prueba de control del sensor BMP180");
  Serial.println("-----------------------------------");
  Serial.print("BMP180_ID = 0x");
  
  uint8_t data = read_1byte(BMP180,idBMP180);
  Serial.println(data, HEX);
    
  }
  
}
uint8_t read_1byte(uint8_t device_addr, uint8_t read_addr){
  uint8_t res;

  /* ============= PRIMERA FASE =======================
   *  Enviar una primera trama al esclavo con la 
   *  dirección del registro donde está el dato a leer.
   *  =================================================
   */

   //Creamos el manejador
   cmd = i2c_cmd_link_create();
   //Condición de salida (Bit START)
   icd_master_start(cmd);
   //Le decimos al esclavo (dirección del dispositivo) que queremos
   // comunicarnos. Le estamos escribiendo los bits de la trama.
   i2c_master_write_byte(cmd, (cmd, device_addr << 1) | I2C_MASTER_WRITE, true);
   //A continuación le seguimos escribiendo la trama y le 
   //decimos la dirección del registro donde está el dato
   i2c_master_write_byte(cmd, read_addr, true);
   //Envíamos el bit de parada (STOP)
   i2c_master_stop();

   //Ejecutamos todos los comandos, desde el bit
   //de salida hasta el de parada
   i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_RATE_MS);
   //Cerramos el manejador
   i2c_cmd_link_delete();

  /* ============= SEGUNDA FASE =======================
   *  Después se lee el valor guardado creando un nuevo
   *  command link con el sensor.
   *  =================================================
   */

   //Creamos el manejador
   cmd = i2c_cmd_link_create();
   //Condición de salida (Bit START)
   icd_master_start(cmd);
   //Le decimos de qué dispositivo esclavo queremos leer
   i2c_master_write_byte(cmd, (device_adress << 1) | I2C_MASTER_READ, true);
   //Leemos el valor pasándole el puntero donde queremos
   //guardar el dato leído => res
   i2c_master_read_byte(cmd, &res, ACK_VAL);
   //Envíamos el bit de parada (STOP)
   i2c_master_stop();
   
   //Ejecutamos todos los comandos, desde el bit
   //de salida hasta el de parada
   i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000/portTICK_RATE_MS);
   //Cerramos el manejador
   i2c_cmd_link_delete();

   return res;
}
