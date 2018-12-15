/*
 * Analizar el código de Ejerc_6.ino en el que 
 * utilizando funciones del entorno IDF se 
 * configura la UART0 para generar una interrupción
 * cada vez que recibe un byte, lo lee y lo envía 
 * de nuevo (al monitorserie) por la UART0.
 */
#include <M5Stack.h>
#include "driver/uart.h"
#include "esp_intr_alloc.h"

#define TX0_pin  1
#define RX0_pin  3
#define TX1_pin  21
#define RX1_pin  13
#define TX2_pin  4
#define RX2_pin  22

#define BUF_SIZE (1024)

#define UART_EMPTY_THRESH_DEFAULT  (0)
#define UART_FULL_THRESH_DEFAULT  (1) // numero de bytes recibidos para generar la interrupcion
#define UART_TOUT_THRESH_DEFAULT   (0)


// Manejador para la ISR
static intr_handle_t handle_console;


 // Buffer de recepcion
volatile uint8_t  rxbuf[BUF_SIZE]; 
// Flag para activar rutina asociada a la ISR
volatile uint8_t Flag_uart_int = 0; //Se utiliza para la comunicación con el enlace pricipal

// Rutina de atencion a la interrupcion de la UART
static void  IRAM_ATTR ISR_UART(void *arg)
{
  uint8_t rx_fifo_len;
  uint8_t i = 0;
  
  // Leer el numero de bytes del buffer de la UART
  rx_fifo_len = UART0.status.rxfifo_cnt; 
  
  // Leer los bytes del buffer mientras que la variable sea >0
  while(rx_fifo_len){
   rxbuf[i++] = UART0.fifo.rw_byte; // read all bytes
   rx_fifo_len--;
   Flag_uart_int++;
  }

 // Limpiar el bit de status de la interrupcion de la UART
 uart_clear_intr_status(UART_NUM_0, UART_RXFIFO_FULL_INT_CLR);
 
}

void setup() {
  M5.begin();
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("\n\n");
  M5.Lcd.printf("      Test\r\n"); 
  M5.Lcd.printf("\r\n"); 
  M5.Lcd.printf("   Interrupcion\r\n"); 
  M5.Lcd.printf("\r\n");
  M5.Lcd.printf("      UART0\r\n");
  
  // Deshabilita la UART0 para configurarla con funciones idf
  Serial.end(); // 

  // Inincializacion UARTS
  uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 120,
        .use_ref_tick = false
    };
  
  //uart_param_config(uart_port_t uart_num, const uart_config_t *uart_config);
  uart_param_config(UART_NUM_0, &uart_config);

  
  //uart_set_pin(uart_port_t uart_num, int tx_io_num, int rx_io_num, int rts_io_num, int cts_io_num);
  uart_set_pin(UART_NUM_0, TX0_pin, RX0_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

  
  // uart_driver_install(uart_port_t uart_num, int rx_buffer_size, int tx_buffer_size, 
  uart_driver_install(UART_NUM_0, BUF_SIZE,0, 0, NULL, 0);

  
  uart_write_bytes(UART_NUM_0, "***************\r\n", 17);
  uart_write_bytes(UART_NUM_0, "Interrup. UARTs\r\n", 17);
  uart_write_bytes(UART_NUM_0, "***************\r\n", 17);


  // Liberar el manejador de la ISR UART
  uart_isr_free(UART_NUM_0);
  
  // Registrar la ISR de la UART
  uart_isr_register(UART_NUM_0,ISR_UART, NULL, ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_LEVEL1, &handle_console);
  
  // Configurar la interrupcion UART
    uart_intr_config_t uart0_intr;
    uart0_intr.intr_enable_mask = UART_RXFIFO_FULL_INT_ENA_M ;
    uart0_intr.rxfifo_full_thresh = UART_FULL_THRESH_DEFAULT;
    uart0_intr.rx_timeout_thresh = UART_TOUT_THRESH_DEFAULT;
    //uart0_intr.txfifo_empty_intr_thresh = UART_EMPTY_THRESH_DEFAULT;
   
  uart_intr_config(UART_NUM_0, &uart0_intr);

  // Habilitar la máscara de interrupciones
  uart_enable_intr_mask(UART_NUM_0, UART_RXFIFO_FULL_INT_ENA );
  
}



  void loop() {
    if (Flag_uart_int >0 ){
         uart_write_bytes(UART_NUM_0, (const char*) rxbuf, Flag_uart_int);
      Flag_uart_int = 0;
    }
 
  // Control de pulsadores
  M5.BtnA.read();
    if(M5.BtnA.wasPressed()) { // Habilitar interrupciones
      uart_enable_intr_mask(UART_NUM_0, UART_RXFIFO_FULL_INT_ENA );
  }
  M5.BtnB.read();
    if(M5.BtnB.wasPressed()) { // Deshabilitar interrupciones
      uart_disable_intr_mask(UART_NUM_0, UART_RXFIFO_FULL_INT_ENA );// OPCIÓN 1 ///
  }
}
