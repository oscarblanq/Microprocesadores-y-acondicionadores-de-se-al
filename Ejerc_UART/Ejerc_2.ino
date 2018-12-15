/*
 * Realizar un programa que nos permita enviar por la
 * UART1 la cadena de caracteres recibida por la UART0 y
 * enviar por la UART0 la cadena de caracteres recibida
 * por la UART2.
 * 
 * Se conectará la salida TX de la UART1 a la entrada RX
 * de la UART2, de esa forma al enviar una cadena de
 * caracteres a través del monitor serie, esta cadena se
 * escribirá en el monitor serie después de ser 
 * retransmitida por la UART1 y recibida por la UART2.
 * 
 * Configurar la transmisión a 115200 baudios de 1 byte con 
 * 1 bit de START y uno de STOP.
 * 
 * • Pines UART1: TX 21 RX -1, Pines UART2: TX -1 RX 22
 * • Utilizar las funciones: available(), read() y print() 
 */
#include <M5Stack.h>

#define TX1_pin  21
#define RX1_pin  -1

#define TX2_pin  -1
#define RX2_pin  22

// HardwareSerial Serial1(1);
// HardwareSerial Serial2(2);

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("\n\n\n\n");
  M5.Lcd.printf("   Test 3 UARTs\r\n");
  Serial.println("  ");

// Inicialización de las 3 UARTS
  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, TX1_pin, RX1_pin);
  Serial2.begin(115200, SERIAL_8N1, TX1_pin, RX1_pin);

}

char dato1, dato2;
uint8_t num_bytes;

void loop() {
  num_bytes = Serial.available();

  if(num_bytes > 0){
   // Lee datos recibidos por UART0 y los envia a UART1
   dato1 = Serial.read();
   Serial1.print(dato1);
   
   // Lee datos recibidos por UART2 y los envia a UART0
   dato2 = Seria.read();
   Serial.print(dato2);
  }
}
