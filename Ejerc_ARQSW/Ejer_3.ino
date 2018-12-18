/* ================================== EJERCICIO 3 ======================================
 *  Programar 4 tareas de la misma prioridad que escriban el nombre de la cada 5s
 *  tarea en el monitor serie y que se sincronicen con la Tarea 1 a través de
 *  3 semáforos binarios S1, S2 y S3.
 *  La Tarea 1 se activará cada 5s y enviará S1 para activar la Tarea 2. 
 *  La Tarea 2 se activará al recibir S1 y enviará S2. La Tarea 3 se activará al
 *  recibir S2 y enviará S3. 
 *  La Tarea 4 se activará al recibir S3.
 *  Utilice las funciones xTaskCreate, vTaskDelay, xSemaphoreCreateBinary, 
 *  xSemaphoreGive y xSemaphoreTake y declare el semáforo con el tipo SemaphoreHandle_t.
 *  En la función setup:
 *  1º) Crear los semáforos binarios
 *  2º) Crear las tareas
===================================================================================== */
#include <M5Stack.h>
#include "esp_freertos_hooks.h"

void setup() {
  M5.begin();

  //Declaramos los 3 semáforos
  SemaphoreHandle_t S1 = xSemaphoreCreateBinary();
  SemaphoreHandle_t S2 = xSemaphoreCreateBinary();
  SemaphoreHandle_t S3 = xSemaphoreCreateBinary();

  //Creamos las 4 tareas
  xTaskCreate(&tarea_1, "Tarea 1", 1024, NULL, 4, NULL);
  xTaskCreate(&tarea_2, "Tarea 2", 1024, NULL, 2, NULL);
  xTaskCreate(&tarea_3, "Tarea 3", 1024, NULL, 1, NULL);
  xTaskCreate(&tarea_4, "Tarea 4", 1024, NULL, 3, NULL);
}

void loop() {
  vTaskDelay(5000);
}

//Defino las 4 tareas que escriben cada una su nombre
void tarea_1(void *pvParameters){
  while(true){
    Serial.println("Tarea 1");
    //Manda el semáforo 1
    xSemaphoreGive(S1);
    vTaskDelay(5000/portTICK_RATE_MS);
  }
}
void tarea_2(void *pvParameters){
  while(true){
    //Recoge el semáforo 1
    /* El segundo parámetro (xTicksToWait)
     *  es el tiempo en ticks que espera
     *  el semáforo para estar disponible.
     *  Si xTicksToWait = portMAX_DELAY
     *  significa que espera de forma indefinida.
     */
    xSemaphoreTake(S1, portMAX_DELAY);
    Serial.println("Tarea 2");
    //Manda el semáforo 2
    xSemaphoreGive(S2);
    /*Se comenta el vTaskDelay para que no se
     * inicialice la tarea cada X tiempo sino que
     * se inicialice cuando le llegue el semáforo
     */
    //vTaskDelay(5000/portTICK_RATE_MS);
  }
}
void tarea_3(void *pvParameters){
  while(true){
    //Recoge el semáforo 2
    xSemaphoreTake(S2, portMAX_DELAY);
    Serial.println("Tarea 3");
    //Manda el semáforo 3
    xSemaphoreGive(S3);
    vTaskDelay(5000/portTICK_RATE_MS);
  }
}
void tarea_4(void *pvParameters){
  while(true){
    //Recoge el semáforo 3
    xSemaphoreTake(S3, portMAX_DELAY);
    Serial.println("Tarea 4");
    //vTaskDelay(5000/portTICK_RATE_MS);
  }
}
