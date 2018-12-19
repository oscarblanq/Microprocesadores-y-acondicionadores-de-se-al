/* ================================== EJERCICIO 4 ======================================
 * Complete el programa Ejer_4_sincr_ISR_tarea.ino para generar una interrupción al 
 * pulsar el botón A del M5Stack y que la rutina de atención a la interrupción active 
 * un semáforo binario (S1) para sincronizar una tarea que escribirá en el monitor 
 * serie el mensaje “Botón A pulsado”. Utilice las funciones xSemaphoreCreateBinary,
 * xSemaphoreGiveFromISR, xSemaphoreTake,xTaskCreate y declare el semáforo con el tipo 
 * SemaphoreHandle_t.
===================================================================================== */
#define PIN_PULSADOR_A 39

//Crear el manejador para el semáforo
SemaphoreHandle_t S1 = NULL;

// Rutina de interrupción, llamada cuando se presiona el pulsador
void IRAM_ATTR pulsadorA_ISR(){
  // da el semáforo para que quede libre para la tarea pulsador
  xSemaphoreGiveFromISR(S1, NULL);
}

void task_pulsador(void *arg) {
  while(true){
    // Esperar la notificación de la ISR
    xSemaphoreTake(S1, portMAX_DELAY);
    // Escribe mensaje
    Serial.println("Botón A pulsado\n");
  }
}
void setup() {
  Serial.begin(115200);
  Serial.println("Sincronizacion ISR --> tarea: ");

  // Crear semaforo binario
  S1 = xSemaphoreCreateBinary();

  // Crear la tarea task_pulsador
  xTaskCreate(&task_pulsador, "task_pulsador", 2048, NULL, 1, NULL);

  // Configurar interrupcion del pulsador A
  pinMode(PIN_PULSADOR_A, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_PULSADOR_A), pulsadorA_ISR, FALLING);
}

void loop() {}
