/* ================================== EJERCICIO 6 ======================================
 * Dos tareas (Tarea A y Tarea B) están sincronizadas con dos rutinas de atención a las 
 * interrupciones generadas con los botones A y B (ISR A e ISR B). Las tareas cuentan 
 * las veces que se han pulsado los botones A y B utilizando la variable compartida 
 * cEvents. Use el fichero Ejer_6_tarea_seccritica.ino
 * a) Lea e interprete el código del programa
 * b) Compruebe el funcionamiento del programa pulsando los botones A y B a la vez
 * c) Utilice un semáforo mutex para proteger la sección crítica de las tareas. 
 * Utilice las funciones: xSemaphoreCreateMutex, xSemaphoreGive y xSemaphoreTake. 
 * Declare el mutex con el tipo SemaphoreHandle_t.
=====================================================================================*/
#define PIN_PULSADOR_A 39
#define PIN_PULSADOR_B 38

//declarar el manejador para el semáforo binario como variable global
SemaphoreHandle_t xSemaphore_A = NULL;
SemaphoreHandle_t xSemaphore_B = NULL;

//declarar el manejador para el semáforo mutex como variable global
SemaphoreHandle_t SemaphoreMutex = NULL;

static int cEvents; 
int const a_end = 32767;
int const b_end = 199;
volatile int incr_2 = 0;

// Rutina de interrupción, llamada cuando se presiona el pulsador
void IRAM_ATTR pulsadorA_ISR(){
    // da el semáforo para que quede libre para la tarea pulsador
   xSemaphoreGiveFromISR(xSemaphore_A, NULL);
}
void IRAM_ATTR pulsadorB_ISR() {
    // da el semáforo para que quede libre para la tarea pulsador
   xSemaphoreGiveFromISR(xSemaphore_B, NULL);
}

void task_pulsador_A(void* arg) {

   while(1) {
      int incr = 1;
      int cuenta;
      // Espero por la notificación de la ISR
      if(xSemaphoreTake(xSemaphore_A,portMAX_DELAY) == pdTRUE) {
            cuenta = cEvents;
            Delay_c(a_end,b_end);
            cuenta += incr;
            cEvents = cuenta;
            Serial.print("Pulsador A presionado. cEvents = ");
            Serial.println(cEvents);
      }
      xSemaphoreGive(SemaphoreMutex);
   }
}
void task_pulsador_B(void* arg) {

   while(1) {
      // Espero por la notificación de la ISR
      if(xSemaphoreTake(SemaphoreMutex,portMAX_DELAY) == pdTRUE) {
            cEvents ++;
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
   
   //Se crea el Mutex
   SemaphoreMutex = xSemaphoreCreateMutex();
   
   // creo la tarea task_pulsador
   xTaskCreate(task_pulsador_A, "task_pulsador", 2048, NULL, 5, NULL);
   xTaskCreate(task_pulsador_B, "task_pulsador", 2048, NULL, 5, NULL);
}

void Delay_c(int const a_end, int const b_end){
    int a, b;
      for (a = 1; a <= a_end; a++){
        for (b = 1; b <= b_end; b++)
          {    
           incr_2++;
           }
        };
}
void loop() {}
