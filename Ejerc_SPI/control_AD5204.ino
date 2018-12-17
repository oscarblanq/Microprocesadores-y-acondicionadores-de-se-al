/* Control del Potenciometro digital AD5204*/

#include <M5Stack.h>
#include <SPI.h>

#define DEV1_CS 21        
#define DEV1_PR 22
#define DEV1_SHDN 2 

// Estructura para almacenar los comandos A y D
typedef struct{  
      uint8_t addr; // A: Address
      uint8_t data_val; // D: Data
} comando;

// Funcion para leer los comandos A y D desde el monitor serie
comando lee_comando(void);

// Funcion a realizar en los ejercicios
void write_ad5204(SPISettings SPI_settings,uint8_t dev_cs_pin,uint8_t write_addr, uint8_t data);

uint8_t flag ;
void setup() {
  // put your setup code here, to run once:
  M5.begin();
  pinMode (DEV1_CS, OUTPUT);
  digitalWrite(DEV1_CS, HIGH);
  pinMode (DEV1_PR, OUTPUT);
  digitalWrite(DEV1_PR, HIGH);
  pinMode (DEV1_SHDN, OUTPUT);
  digitalWrite(DEV1_SHDN, HIGH);
  
//SPI.begin();
  Serial.begin(115200);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("Test lectura SPI\r\n");
  Serial.println("  ");
  Serial.println("------------------------------------");
  Serial.println("Prueba de lectura en bus SPI");
  Serial.println("------------------------------------");

  flag = 0;
}


void loop() {
   // Lee comandos y datos
   comando cmd;
   cmd = lee_comando();
   uint8_t addr = cmd.addr;
   uint8_t data_val = cmd.data_val;

  M5.BtnA.read();
    if(M5.BtnA.wasPressed()) {
	  // Escribir el AD5204 en la direccion addr con el dato data_val
	  write_ad5204(DEV1_CS, 0X01, 0XFF);
    write
	  
	  Serial.print("ESCRITURA DEV1 = Dir 0x");
      Serial.print(addr,HEX);
      Serial.print(" - Dato 0x");
      Serial.println(data_val,HEX);
  }
  M5.BtnB.read();
    if(M5.BtnB.wasPressed()) {
	  // Generar un flanco ascendente en DEV1_PR
	  
	  
	  
	  
  }
  M5.BtnC.read();
    if(M5.BtnC.wasPressed()) {
      // Activacion o inhibicion del DEV1_SHDN
/* =========================================== EJERCICIO 4b ==================================================
 *  Completar el programa para:
 *  1. generar un flanco positivo en el pin PR cada vez que se pulsa el botón B
 *  2. activar o inhibir el “shutdown” (pin SHDN) cada vez que se pulsa el botón C
 */	  
	  
  }
}

/* =========================================== EJERCICIO 4a ==================================================
 * Escribir una función para configurar (escribir) los 4 potenciómetros digitales del AD5204 utilizando
 * la librería SPI de Arduino. Realizar la configuración cuando se pulsa el botón A del M5Stack. Mostrar la
 * trama escrita por el monitor serie y visualizar con el analizador lógico las líneas del bus SPI.
 * • Utilizar el programa control_AD5204.ino
 * • Usar la función lee_comando(void) para detectar los comandos A (Address) o D (Data) introducidos mediante el
 * monitor serie y sus valores
 * • Tener en cuenta tanto para la programación como para el uso del analizador lógico que:
 * - CE se activa a nivel bajo y está conectado al pin 21.
 * - PR está conectado al pin 22.
 * - SHDN está conectado al pin 2.
 * . el SPI se configura en SPI_MODE0
 * • Función: void write_ad5204(uint8_t dev_cs_pin,uint8_t write_addr, uint8_t data)
4.b Completar el programa para:
1. generar un flanco positivo en el pin PR cada vez que se pulsa el botón B (
2. activar o inhibir el “shutdown” (pin SHDN) cada vez que se pulsa el botón C
 */
void write_ad5204(uint8_t dev_cs_pin,uint8_t write_addr, uint8_t data){
  // Escribir la dirección del registro que se quiere escribir 
  // y despuéss escribir su valor
  
}

  ////////////////////////////////////////////////////////////////////////////////
// Funcion para leer los comandos A (Address) y D (Data) desde el monitor serie
////////////////////////////////////////////////////////////////////////////////
comando lee_comando(void){
  char dato = 'A';
  char num = 0;
  uint16_t dig;
  static  comando cmd;
  
  uint8_t num_bytes = Serial.available();
   if (num_bytes>0) // Lee comando "Dir" o "Dato"
   {
     dato = Serial.read();
     if ((dato == 'A') || dato == 'D')
     {
        Serial.print("Comando recibido : ");
        Serial.println(dato);
        num_bytes = 0;
        num = 0;
        dig = 0;
        Serial.println("Introduce el valor");
  
        while (num_bytes == 0) // Lee número
        {
          num_bytes = Serial.available();
         }
        uint8_t cont_dig = 0;
         while (num_bytes > 0)
         {
            num_bytes -= 1;
            num = Serial.read();
            num -= '0';
            dig += num*pow(10,num_bytes);      
         }
         Serial.print(dato);
         Serial.print(" = ");
         Serial.println(dig);
         
         Serial.println("Introduce un comando (A: Address; D: Data) ");
         
         if (dato == 'A')
         {
            cmd.addr = (uint8_t)dig;
            if (dig >3){
              Serial.println("ERROR: A debe ser menor que 4");
            }
         }
         if (dato == 'D')
         {
            cmd.data_val = (uint8_t)dig;
            if (dig >255){
              Serial.println("ERROR: D debe ser menor que 256");
            }
         } 
     }
     else
     {
        Serial.println("Comando NO válido: debe ser A o D");
     } 
    } 
return cmd;
}
