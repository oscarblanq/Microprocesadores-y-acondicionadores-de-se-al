/* ================================== EJERCICIO 8 ======================================
 * Complete el programa Ejer_8_tareas_a_cola.ino para enviar datos a través de una cola 
 * desde tres tareas transmisoras a una receptora. La tarea receptora enviará un 
 * mensaje al monitor serie identificando la tarea transmisora y escribiendo el dato.
 * Los datos se envían mediante la estructura type_msg, con la que se identificará 
 * al transmisor
=====================================================================================*/
#define TAM_COLA 20 /*20 mensajes*/

// Estructura para envio de datos
typedef struct{
  uint8_t id_sender;
  uint8_t data;
}type_msg;

// Declarar manejador de la cola
QueueHandle_t xQueue = NULL;

int msg1 = 0;
void escribe1(void *pvParameter)
{
    type_msg my_msg;
    my_msg.id_sender = 1;
    uint8_t cont_t1 = 0;
    while(1) {
    cont_t1++;
    my_msg.data = cont_t1;
    printf("Ejecuta Tarea escribe1\n");
    // Enviar datos a la cola

    vTaskDelay(2000 / portTICK_RATE_MS);
    }
}
void escribe2(void *pvParameter)
{
    type_msg my_msg;
    my_msg.id_sender = 2;
    uint8_t cont_t2 = 0;
    while(1) {
    cont_t2++;
    my_msg.data = cont_t2;
    printf("Ejecuta Tarea escribe2\n");
    
    // Enviar datos a la cola
    xQueueSend(xQueue, &my_msg, portMAX_DELAY);
    
     vTaskDelay(2000 / portTICK_RATE_MS);
 }
}

void escribe3(void *pvParameter)
{
  type_msg my_msg;
    my_msg.id_sender = 3;
    uint8_t cont_t3 = 0;
    while(1) {
    cont_t3++;
    my_msg.data = cont_t3;
    printf("Ejecuta Tarea escribe3\n");
    
    // Enviar datos a la cola
    xQueueSend(xQueue, &my_msg, portMAX_DELAY);

    vTaskDelay(2000 / portTICK_RATE_MS);
    }
}

void lee1(void *pvParameter)
{
   type_msg rx_msg;
   while(1) { 
    
      // Leer datos de la cola
      xQueueReceive(xQueue, &rx_msg, portMAX_DELAY);

      //Escribir en el monitor serie
      printf("Lee: %d\n", rx_msg.data);

   }
}

void setup() {

    // Crear cola
    xQueue = xQueueCreate(TAM_COLA, siezeof(type_msg));


    // Crear tareas
    xTaskCreate(&lee1,     "lee1",     1024*10, NULL, 5, NULL);
    xTaskCreate(&escribe1, "escribe1", 1024, NULL, 1, NULL);
    xTaskCreate(&escribe2, "escribe2", 1024, NULL, 1, NULL);
    xTaskCreate(&escribe3, "escribe3", 1024, NULL, 1, NULL);
}

void loop() {}
