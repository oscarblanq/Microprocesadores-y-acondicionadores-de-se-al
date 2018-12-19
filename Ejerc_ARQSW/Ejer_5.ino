/* ================================== EJERCICIO 5 ======================================
 * Dos tareas (Tarea A y Tarea B) están sincronizadas con dos rutinas de atención a las 
 * interrupciones generadas con los botones A y B (ISR A e ISR B). Las tareas cuentan 
 * las veces que se han pulsado los botones A y B utilizando la variable compartida 
 * cEvents. Use el fichero Ejer_5_int_seccritica.ino
 * a) Lea e interprete el código del programa
 * b) Compruebe el funcionamiento del programa pulsando los botones A y B a la vez
 * c) Utilice portENTER_CRITICAL(&mux) y portEXIT_CRITICAL(&mux) para proteger la 
 * sección crítica de las tareas
===================================================================================== */
#define PIN_PULSADOR_A 39
#define PIN_PULSADOR_B 38

void Delay_c(int const a_end, int const b_end);

//creo el manejador para el semáforo como variable global
SemaphoreHandle_t xSemaphore_A = NULL;
SemaphoreHandle_t xSemaphore_B = NULL;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

static int cEvents; 

// Rutina de interrupción, llamada cuando se presiona el pulsador
void IRAM_ATTR pulsadorA_ISR() {
    // da el semáforo para que quede libre para la tarea pulsador
   xSemaphoreGiveFromISR(xSemaphore_A, NULL);
}
void IRAM_ATTR pulsadorB_ISR() {
    // da el semáforo para que quede libre para la tarea pulsador
   xSemaphoreGiveFromISR(xSemaphore_B, NULL);
}

int const a_end = 32767;
int const b_end = 199;
volatile int incr_2 = 0;

void task_pulsador_A(void* arg) {
   while(1) {
      int incr = 1;
      int cuenta;
      // Espero por la notificación de la ISR
      if(xSemaphoreTake(xSemaphore_A,portMAX_DELAY) == pdTRUE) {
            cuenta = cEvents;
            Delay_c(a_end,b_end);
            cuenta += incr;
            portENTER_CRITICAL(&mux);
            cEvents = cuenta;
            portEXIT_CRITICAL(&mux);
//            cEvents ++;
            Serial.print("Pulsador A presionado. cEvents = ");
            Serial.println(cEvents);
      }
   }
}
void task_pulsador_B(void* arg) {
   while(1) {
      // Espero por la notificación de la ISR
      if(xSemaphoreTake(xSemaphore_B,portMAX_DELAY) == pdTRUE) {
        portENTER_CRITICAL(&mux);
            cEvents ++;
        portEXIT_CRITICAL(&mux);
            Serial.print("Pulsador B presionado. cEvents = ");
            Serial.println(cEvents);
      }
   }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Monitoring interrupts: ");
  pinMode(PIN_PULSADOR_A, INPUT_PULLUP);
  pinMode(PIN_PULSADOR_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_PULSADOR_A), pulsadorA_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_PULSADOR_B), pulsadorB_ISR, RISING);

   // se crea el semáforo binario
   xSemaphore_A = xSemaphoreCreateBinary();
   xSemaphore_B = xSemaphoreCreateBinary();
    
   // creo la tarea task_pulsador
   xTaskCreate(task_pulsador_A, "task_pulsador_A", 2048, NULL, 1, NULL);
   xTaskCreate(task_pulsador_B, "task_pulsador_B", 2048, NULL, 1, NULL);
}

void Delay_c(int const a_end, int const b_end){
    int a, b;
      for (a = 1; a <= a_end; a++){
        for (b = 1; b <= b_end; b++)
          {    
           incr_2++;
           }
        }
}

void loop() {}
