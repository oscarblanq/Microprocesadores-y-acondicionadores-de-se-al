/* ================================== EJERCICIO 1 ======================================
 * Completar el código del programa Ejerc_1.ino para configurar el Timer 0 del ESP32 
 * utilizando funciones de Arduino para que genere interrupciones periódicas de 1 seg y 
 * que ejecute la rutina de atención a la interrupción ISR_Timer0. El timer se activará 
 * cuando se pulse el botón A y se deshabilitará cuando se pulse el botón B.
 * Tenga en cuenta que el reloj de entrada al prescaler es de 80 MHz.
 * Utilice las funciones timerBegin, timerAttachInterrupt, timerAlarmWrite y 
 * timerAlarmEnable para configurar el timer y timerStart y timerStop para activarlo y 
 * detenerlo.
===================================================================================== */
#include <M5Stack.h>
#define x0 30
#define y0 30
#define r 20

// Declaracion objeto Timer0
hw_timer_t *Timer0 = NULL;


volatile int Flag_ISR_Timer0 = 0;

void IRAM_ATTR ISR_Timer0(){
  Flag_ISR_Timer0 = 1;
}

void setup() {
  M5.begin();
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("\n\n\n\n");
  M5.Lcd.printf("    Test Timers\r\n");
  Serial.begin(115200);
  Serial.println("  ");
  Serial.println("------------------------------------");
  Serial.println("Prueba Timers");
  Serial.println("------------------------------------");

  //Configuracion del Timer0 
  Timer0 = timerBegin(0, 80, true);
 // Configurar interrupcion
 timerAttachInterrupt(Timer0, &ISR_Timer0 , true);
 // Escribir el valor de la alarma con autorecarga
 timerAlarmWrite(Timer0, 1000000, true);
 // Habilitar la alarma
 timerAlarmEnable(Timer0);
 }

int Flagcirc_0 = 1;
void loop() {
  
  // Control de pulsadores
  M5.BtnA.read();
    if(M5.BtnA.wasPressed()) {
		// Activar el timer
   timerStart(Timer0);
  }
  M5.BtnB.read();
    if(M5.BtnB.wasPressed()) {
      // Detener el timer
      timerStop(Timer0);
  }
  
  // Acciones de la ISR 1
  if (Flag_ISR_Timer0==1){
    Serial.println(String("ISR_Timer0 ")+String(((float) millis()/1000))+" seg");
    Flag_ISR_Timer0 = 0;
    if (Flagcirc_0==0){                  
      M5.Lcd.fillCircle(x0,y0,r,RED);// Dibujar circulo
      Flagcirc_0++;
    }
    else{                                
      M5.Lcd.fillCircle(x0,y0,r,BLACK);// Borrar circulo
      Flagcirc_0--;
    }  
  }
 
}
