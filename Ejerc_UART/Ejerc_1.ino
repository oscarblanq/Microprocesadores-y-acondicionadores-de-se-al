/*Realizar un programa para enviar el carácter ‘A’ a la vez al
monitor serie mediante la UART0 y a través de la UART1 conectando el TX al pin 21.

El carácter se enviará cuando se pulse el botón A del M5Stack utilizando la función
print() y cuando se pulse el botón B con la función println().

Las UARTs se configurarán para la transmisión a 115200 baudios
de 1 byte con 1 bit de START y uno de STOP.

Utilizar el analizador lógico conectado al pin 21 para:
a) Visualizar la trama enviada y detectar el bit de START, STOP y el código ASCII del carácter enviado 
(tenga en cuenta que primero se envía el LSB); 
b) Medir la tasa de transmisión en baudios.*/
#include <M5Stack.h>

#define TX1_pin  21
#define RX1_pin  -1

//HardwareSerial Serial1(1);
//HardwareSerial Serial2(2);

void setup() {
  M5.begin();

// Inicializacion UARTS
Serial.begin(115200, SERIAL_8N1, -1, -1);
Serial2.begin(115200, SERIAL_8N1, TX1_pin, RX1_pin);  

}

void loop() {
   
  M5.BtnA.read();
  if(M5.BtnA.wasPressed()) {
    Serial.print('A');
    Serial1.print('A');
  }

  M5.BtnB.read();
  if(M5.BtnB.wasPressed()) {
    Serial.println('A');
    Serial1.println('A');
  }
}
