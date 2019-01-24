/*
 Configura el Timer 0 del grupo 0 para generar interrupciones periódicas de 5 seg.
 Configura un pulsador para generar interrupciones.
 Se puede cambiar el nivel de interrupciones para que una rutina interrumpa a otra.
 El inicio y fin de la rutina de atención a la interrupción se monitoriza por el puerto serie.
 */
#include <M5Stack.h>
#include <esp_task_wdt.h>
#include "driver\timer.h"

#define TIMER_INTR_SEL  TIMER_INTR_LEVEL  /*!< Timer level interrupt */
#define TIMER_GROUP     TIMER_GROUP_0     /*!< Test on timer group 0 */
#define TIMER_DIVIDER   80             /*!< Hardware timer clock divider, 80 to get 1MHz clock to timer */
#define TIMER_SCALE     (TIMER_BASE_CLK / TIMER_DIVIDER)  /*!< used to calculate counter value */
#define TIMER_FINE_ADJ  (0*(TIMER_BASE_CLK / TIMER_DIVIDER)/1000000) /*!< used to compensate alarm value */
#define TIMER_INTERVAL0_SEC   (5)   /*!< test interval for timer 0 */
#define ESP_INTR_FLAG_TIMER ESP_INTR_FLAG_LEVEL1
#define ESP_INTR_FLAG_BUTTON ESP_INTR_FLAG_LEVEL3

int c, d, e;
int const c_end = 32767;
int const d_end = 1767;
int const e_end = 10;

int c2, d2, e2;
int const c2_end = 32767;
int const d2_end = 1767;
int const e2_end = 10;
volatile int TimerInterruptCounter = 0;
volatile int PulsAInterruptCounter = 0;

void IRAM_ATTR buttonA_isr_handler(void* arg) {
  //portENTER_CRITICAL_ISR(&mux);
  PulsAInterruptCounter++;
  Serial.print("INI Puls A int n. ");
  Serial.println(PulsAInterruptCounter);
  for (c2 = 1; c2 <= c2_end; c2++)
  {
     esp_task_wdt_reset();
      for (d2 = 1; d2 <= d2_end; d2++)
          for (e2 = 1; e2 <= e2_end; e2++)
         {};
  }
  Serial.print("END Puls A int n. ");
  Serial.println(PulsAInterruptCounter);;
  //portEXIT_CRITICAL_ISR(&mux);
}

void IRAM_ATTR timer_group0_isr(void *para){// timer group 0, ISR
    int timer_idx = (int) para;
    uint32_t intr_status = TIMERG0.int_st_timers.val;
      if((intr_status & BIT(timer_idx)) && timer_idx == TIMER_0) {
          TIMERG0.hw_timer[timer_idx].update = 1;
          TIMERG0.int_clr_timers.t0 = 1;
          TIMERG0.hw_timer[timer_idx].config.alarm_en = 1;
          Serial.println("Timer ON");
          TimerInterruptCounter++;
          Serial.print("INI Timer int n. ");
          Serial.println(TimerInterruptCounter);
            for (c = 1; c <= c_end; c++)
            {
                delay(500);
              for (d = 1; d <= d_end; d++)
                 for (e = 1; e <= e_end; e++)
                   {};
            }
          Serial.print("END Timer int n. ");
          Serial.println(TimerInterruptCounter);
        }
}
static void example_tg0_timer0_init()
{
    timer_group_t timer_group = TIMER_GROUP_0;
    timer_idx_t timer_idx = TIMER_0;
    timer_config_t conf;
    conf.alarm_en = 1;
    conf.auto_reload = 1;
    conf.counter_dir = TIMER_COUNT_UP;
    conf.divider = (uint16_t)80;
    conf.intr_type = TIMER_INTR_SEL;
    conf.counter_en = TIMER_PAUSE;
    /*Configure timer*/
    timer_init(timer_group, timer_idx, &conf);
    /*Stop timer counter*/
    timer_pause(timer_group, timer_idx);
    /*Load counter value */
    timer_set_counter_value(timer_group, timer_idx, 0x00000000ULL);
    /*Set alarm value*/
    timer_set_alarm_value(timer_group, timer_idx, (TIMER_INTERVAL0_SEC * TIMER_SCALE) - TIMER_FINE_ADJ);
    /*Enable timer interrupt*/
    timer_enable_intr(timer_group, timer_idx);
    /*Set ISR handler*/
    timer_isr_register(timer_group, timer_idx, timer_group0_isr, (void*) timer_idx, ESP_INTR_FLAG_TIMER, NULL);
    /*Start timer counter*/
    timer_start(timer_group, timer_idx);
}
static void PulsA_int_init()
{
  // Configurar pines
  gpio_set_direction(GPIO_NUM_39,GPIO_MODE_INPUT);
  // Configurar interrupciones en pines
  gpio_set_intr_type(GPIO_NUM_39,GPIO_INTR_NEGEDGE);
    // install ISR service with default configuration
  gpio_install_isr_service(ESP_INTR_FLAG_BUTTON);
    // attach the interrupt service routine
  gpio_isr_handler_add(GPIO_NUM_39, buttonA_isr_handler, NULL);
}
void setup() {
  // put your setup code here, to run once:
  M5.begin(false, false, true);
  Serial.println("\nPrueba del timer");
  PulsA_int_init();
  example_tg0_timer0_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (M5.BtnC.isPressed()) M5.powerOFF();
  delay(100);
  M5.update();
}
