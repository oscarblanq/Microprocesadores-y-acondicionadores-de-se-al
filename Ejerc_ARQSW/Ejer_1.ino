/* ================================== EJERCICIO 1 ======================================
 * Programar 4 tareas que se activen cada 5 segundos y escriban el nombre de la
 * tarea en el monitor serie. Inicialmente asigne una prioridad distinta (de 1 a 4) a 
 * cada tarea.
 * Utilizar las funciones xTaskCreate y vTaskDelay. Al crear cada tarea asígnele a la
 * pila un tamaño de 1024 words.
 * Compruebe e interprete cómo afecta:
 * a) la modificación de las prioridades en las tareas
 * b) la asignación de la misma prioridad a todas las tareas
 * c) la reducción del tamaño de la pila en una tarea (bájelo en la tarea 1 a 256 words)
 * d) eliminar la función de bloqueo en una tarea (por ejemplo en la tarea con la
 * prioridad 2 comente la función vTaskDelay e incluya un retardo (sin usar delay)
===================================================================================== */
#include <M5Stack.h>

void setup() {
  M5.begin();

  //Creamos las 4 tareas
  xTaskCreate(&tarea_1, "Tarea 1", 1024, NULL, 4, NULL);
  xTaskCreate(&tarea_2, "Tarea 2", 1024, NULL, 2, NULL);
  xTaskCreate(&tarea_3, "Tarea 3", 1024, NULL, 1, NULL);
  xTaskCreate(&tarea_4, "Tarea 4", 1024, NULL, 3, NULL);
}

void loop() {
  vTaskDelay(5000);
}

//Definos 4 tareas que escriben cada una su nombre
void tarea_1(void *pvParameters){
  while(true){
    Serial.println("Tarea 1");
    vTaskDelay(5000/portTICK_RATE_MS);
  }
}
void tarea_2(void *pvParameters){
  while(true){
    Serial.println("Tarea 2");
    vTaskDelay(5000/portTICK_RATE_MS);
  }
}
void tarea_3(void *pvParameters){
  while(true){
    Serial.println("Tarea 3");
    vTaskDelay(5000/portTICK_RATE_MS);
  }
}
void tarea_4(void *pvParameters){
  while(true){
    Serial.println("Tarea 4");
    vTaskDelay(5000/portTICK_RATE_MS);
  }
}
