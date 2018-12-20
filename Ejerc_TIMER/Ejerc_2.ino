/* ================================== EJERCICIO 1 ======================================
 * Completar el código del programa Ejerc_2.ino utilizando funciones de Arduino para 
 * configurar los 4 timers del ESP32 para que generen interrupciones periódicas de 1, 
 * 2, 0.5 y 1.5 seg y que ejecuten las rutinas de atención a la interrupciones 
 * ISR_Timer0, ISR_Timer1, ISR_Timer2 y ISR_Timer3. Los timers se activarán cuando se 
 * pulse el botón A y se deshabilitarán cuando se pulse el botón B.
===================================================================================== */
#include <M5Stack.h>
#define x0 40
#define y0 30
#define r 30
#define x_offset_1 75 
#define x_offset_2 2*x_offset_1
#define x_offset_3 3*x_offset_1

// Declarar objetos timer0, timer1, timer2 y timer3
hw_timer_t *Timer0 = NULL;
hw_timer_t *Timer1 = NULL;
hw_timer_t *Timer2 = NULL;
hw_timer_t *Timer3 = NULL;

volatile int Flag_ISR_Timer0 = 0;
volatile int Flag_ISR_Timer1 = 0;
volatile int Flag_ISR_Timer2 = 0;
volatile int Flag_ISR_Timer3 = 0;

int Flagcirc_0 = 1;
int Flagcirc_1 = 1;
int Flagcirc_2 = 1;
int Flagcirc_3 = 1;

void IRAM_ATTR ISR_Timer0(){
  Flag_ISR_Timer0 = 1;
}
void IRAM_ATTR ISR_Timer1(){
  Flag_ISR_Timer1 = 1;
}
void IRAM_ATTR ISR_Timer2(){
  Flag_ISR_Timer2 = 1;
}
void IRAM_ATTR ISR_Timer3(){
  Flag_ISR_Timer3 = 1;
}  

void setup() {
  M5.begin();
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("\n\n\n\n");
  M5.Lcd.printf("   Test 4 Timers\r\n");
  Serial.begin(115200);

  Serial.println("  ");
  Serial.println("------------------------------------");
  Serial.println("Prueba 4 Timers");
  Serial.println("------------------------------------");

  // Timer 0: alarma 1s
  Timer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(Timer0, &ISR_Timer0 , true);
  timerAlarmWrite(Timer0, 1000000, true);

  // Timer 1: alarma 2s
  Timer1 = timerBegin(0, 80, true);
  timerAttachInterrupt(Timer1, &ISR_Timer1, true);
  timerAlarmWrite(Timer0, 2000000, true);
  
  // Timer 2: alarma 0.5s
  Timer2 = timerBegin(2, 80, true);
  timerAttachInterrupt(Timer2, &ISR_Timer2 , true);
  timerAlarmWrite(Timer2, 500000, true);
  // Timer 3: alarma 1.5s
  Timer3 = timerBegin(3, 80, true);
  timerAttachInterrupt(Timer3, &ISR_Timer3 , true);
  timerAlarmWrite(Timer3, 1500000, true);
  
  // Habilitar alarma de los 4 timers
  timerAlarmEnable(Timer0);
  timerAlarmEnable(Timer1);
  timerAlarmEnable(Timer2);
  timerAlarmEnable(Timer3);
}

void loop() {
  // Control de pulsadores
  M5.BtnA.read();
    if(M5.BtnA.wasPressed()) {
		// Habilitar alarma
    timerStart(Timer0);
    timerStart(Timer1);
    timerStart(Timer2);
    timerStart(Timer3);
  
  }
  M5.BtnB.read();
    if(M5.BtnB.wasPressed()) {
       // Deshabilitar alarma
       timerStop(Timer0);
       timerStop(Timer1);
       timerStop(Timer2);
       timerStop(Timer3);
  
  }
  
  
  if (Flag_ISR_Timer0) {
    Serial.println("Timer 0");
    if (Flagcirc_0 == 0) {
      M5.Lcd.fillCircle(x0, y0, r, RED); // Dibujar circulo
      Flagcirc_0++;
    }
    else {
      M5.Lcd.fillCircle(x0, y0, r, BLACK); // Borrar circulo
      Flagcirc_0--;
    }
    Flag_ISR_Timer0 = 0;
  }

  if (Flag_ISR_Timer1) {
    Serial.println("       Timer 1");
    if (Flagcirc_1 == 0) {
      M5.Lcd.fillCircle(x0 + x_offset_1, y0, r, GREEN); // Dibujar circulo
      Flagcirc_1++;
    }
    else {
      M5.Lcd.fillCircle(x0 + x_offset_1, y0, r, BLACK); // Borrar circulo
      Flagcirc_1--;
    }
    Flag_ISR_Timer1 = 0;
  }

  if (Flag_ISR_Timer2) {
    Serial.println("              Timer 2");
    if (Flagcirc_2 == 0) {
      M5.Lcd.fillCircle(x0 + x_offset_2, y0, r, BLUE); // Dibujar circulo
      Flagcirc_2++;
    }
    else {
      M5.Lcd.fillCircle(x0 + x_offset_2, y0, r, BLACK); // Borrar circulo
      Flagcirc_2--;
    }
    Flag_ISR_Timer2 = 0;
  }

  if (Flag_ISR_Timer3) {
    Serial.println("                        Timer 3");
    if (Flagcirc_2 == 0) {
      M5.Lcd.fillCircle(x0 + x_offset_3, y0, r, WHITE); // Dibujar circulo
      Flagcirc_3++;
    }
    else {
      M5.Lcd.fillCircle(x0 + x_offset_3, y0, r, BLACK); // Borrar circulo
      Flagcirc_3--;
    }
    Flag_ISR_Timer3 = 0;
  }
}
