/* ================================== EJERCICIO 2 ======================================
 * Añada una función para ejecutarla en la tarea idle. Esta función realizará únicamente 
 * el incremento de un contador asociado a una variable global. Por otro lado, la tarea 1 
 * escribirá en el monitor serie, además de su mensaje, el valor de la cuenta. Para 
 * realizar este ejercicio debe bloquear la tarea en la que se ejecuta la función loop
 * de Arduino mediante vTaskDelay.
 * Funciones a utilizar:
 * • esp_register_freertos_idle_hookvTaskDelay
 * No olvide añadir:
 * • #include "esp_freertos_hooks.h"
===================================================================================== */
#include <M5Stack.h>
#include "esp_freertos_hooks.h"

int contador;

void setup() {
  M5.begin();

  //Registra una devolución de llamada al enlace inactivo del núcleo
  //que llama a esta función. La devolución de llamada debe devolver
  //verdadero si debe ser llamado por el enlace inactivo una vez por
  //interrupción (o FreeRTOS tick), y devolver falso si debe llamarse
  //repetidamente lo más rápido posible por el enlace inactivo.
  esp_register_freertos_idle_hook(vApplicationIdleHook);

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
    Serial.print("Contador: ");
    Serial.println(contador);
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

//Función con contador que devuelve TRUE
bool vApplicationIdleHook(void){
  contador++;
  return true;
}
