/**
 *   @file   Lab4_Part3.ino
 *   @author Nakseung Choi 1572578 Kejin Li 1978130
 *   @date   28-May-2022 
 *   @brief  running a speaker, an external LED, and FFTs using free-RTOS.
 *   
 *   Detailed Description: In this demo, a real-time pre-emptive operating system is employed
 *   to run three tasks: 
 *   1. flash an external LED, one for 100ms and off for 200ms. 
 *   2. speaker that plays "Close Encounters Theme"
 *   3. measure the performance of an existing FFT function based on a random signal
 *   the purpose of this demo is to verify students understanding of free RT-OS.
 *   All of these tasks are running concurrently in real time operating system.
 */

#include "setup.h"
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <arduinoFFT.h>

QueueHandle_t RT3Queue;
QueueHandle_t RT4Queue;
TaskHandle_t RT3p1Handle;
TaskHandle_t RT4p0Handle;

/**
 * setup()
 * @brief setup function
 * @param xTaskCreate set up tasks to run.
 * @param xTaskCreate (Name of function, any name, stack memory, parameter passing, priority, Task handle) 
 * @param vTaskStartScheduler The task scheduler, which takes over control of scheduling individual tasks, is automatically started.
 */
void setup() {
  Serial.begin(9600);
  
  while(!Serial){
    ;
  }

  xTaskCreate(TaskSpeaker, "TaskSpeaker", 128, NULL, 1, NULL);
  xTaskCreate(TaskExternalLED, "ExternalLED", 128, NULL, 1, NULL);
  RT3p0();
  vTaskStartScheduler();

}

/**
 * loop()
 * @brief general loop function
 * @description: Empty as things are done in Task.
 */
void loop() {
  // Empty as things are done in Task.
}

/**
 * set_OC3A_freq(uint32_t freq)
 * @brief This sets frequencices for speaker
 * @param PRE_SCALE OCR3A
 * @param freq frequencies
 * @param TCNT4_COUNTER set this to zero to restart counting.
 */
void set_OC3A_freq(uint32_t freq){
  PRE_SCALE = freq == 0 ? 0 : 2000000 / (2 * freq);
  TCNT4_COUNTER = 0;
}

/**
 * TaskExternalLED( void *pvParameters )
 * @brief flash an external LED for 100ms and off for 200ms.
 * @param LED_setup LED setup function.
 * @param LED_PORTL PORTL
 * @param PIN47 1 << DDL2
 * @param vTaskDelay = delay();
 * @param portTICK_PERIOD_MS 100 / portTICK_PERIOD_MS means 100 per tick.
 */
void TaskExternalLED( void *pvParameters ){
  
  LED_setup();

  for(;;){
    LED_PORTL |= PIN47;
    vTaskDelay( 100 / portTICK_PERIOD_MS );
    LED_PORTL &= ~(PIN47);
    vTaskDelay( 200 / portTICK_PERIOD_MS );
  }

}

/**
 * TaskSpeaker( void *pvParameters )
 * @brief plays "Close Encounters Theme" 3 times 1.5 seconds of pause between playbacks.
 * @param speaker_setup speaker setup function.
 * @param melody an array that frequencies are stored.
 * @param TONE_DURATION 200ms
 * @param PAUSE_DURATION 1500ms pause.
 * @param portTICK_PERIOD_MS 100 / portTICK_PERIOD_MS means 100 per tick.
 * @param vTaskDelete stop the task.
 */
void TaskSpeaker( void *pvParameters ){
  
  speaker_setup();

  for(int i = 0; i < NPLAY; i++){
    for(int k = 0; k < NTONE; k++){
      set_OC3A_freq(melody[k]);
      vTaskDelay( TONE_DURATION / portTICK_PERIOD_MS);
    }
     set_OC3A_freq(0);

     if(i != 2){
       vTaskDelay( PAUSE_DURATION / portTICK_PERIOD_MS );
     }

  }

  vTaskDelete(NULL);


}
/**
 * RT3p0()
 * @brief Scheduler for RT3p1 and RT4p0
 * @param xTaskCreate (Name of function, any name, stack memory, parameter passing, priority, Task handle)
 * @param speaker_setup speaker setup function.
 * @param melody an array that frequencies are stored.
 * @param RANDOM Minimum requirement is 256. We could do it until 512. 1024 did not work. 
 * @param TONE_DURATION 200ms
 * @param PAUSE_DURATION 1500ms pause.
 * @param portTICK_PERIOD_MS 100 / portTICK_PERIOD_MS means 100 per tick.
 * @param vTaskDelete stop the task.
 */
void RT3p0(){
  double temp[RANDOM];
  for(int i = 0; i < RANDOM; i++){
    temp[i] = random(-200, 200);
    //Serial.println(temp[i]);
  }

  RT3Queue = xQueueCreate(1, (unsigned int)sizeof(double *));
  RT4Queue = xQueueCreate(1, (unsigned int)sizeof(long));

  xTaskCreate(RT3p1, "RT3p1", 128, &temp, 1, &RT3p1Handle);
  xTaskCreate(RT4p0, "RT4p0", 4500, NULL, 0, &RT4p0Handle);

}
/**
 * RT3p1()
 * @brief This function receives computed double data from RT4p0 and prints it out.
 * @param wall_clock_time a long variable that stores an elapsed time for 5 FFTs from RT4Queue via xQueueReceive. 
 * @param xQueueSendToBack A double array of the 512 random variables received from RT3p0.
 *                         This array goes to RTp40 to compute 5FFTs and wall_clock_time.
 * @param xQueueReceive It receives data into wall_clock_time and divide by 5 to print out.
 * @param NFFTS 5 times FFTs.
 * @param wall_clock_time a long variable that stores the total time for 5FFTs computed in RT4p0.
 */
void RT3p1( void *pvParameters ){
  while(1){
  long wall_clock_time;
  double *temp = (double*)pvParameters;
  
  xQueueSendToBack(RT3Queue, temp, 0);
  xQueueReceive(RT4Queue, &wall_clock_time, portMAX_DELAY);

  Serial.print("Elapsed time for 5 FFTs: ");
  Serial.println( (double)wall_clock_time / NFFTS );

  //vTaskDelete(NULL);
  }

}
/**
 * RT4p0()
 * @brief This function receives 512 random double variables from RT3p1 and computes FFTs and send wall_clock_time to RT3p1.
 * @param vReal a real part for FFT computation. 
 * @param vImag an imag part for FFT computation.
 * @param arduinoFFT initializes the arduinoFFT library.
 * @param time time is set to millis before the computation. 
 *             this time is subtracted by millis again such that we can find the total time that took for 5 ffts computation.
 * @param temp a variable that stores 512 random double variables sent from RT3p0 for FFT.
 * @param xQueueSendToBack send the elasped time for 5 FFTs back to RT3p1 to print out. 
 */
void RT4p0( void *pvParamters ){
  while(1){
  double vReal[RANDOM];
  double vImag[RANDOM];
  arduinoFFT FFT = arduinoFFT();

  double * temp;
  unsigned long time = millis();
  xQueueReceive(RT3Queue, &temp, portMAX_DELAY);
  for(int i = 0; i < NFFTS; i++){
    for(int k = 0; k < RANDOM; k++){
      vReal[k] = temp[k];
      vImag[k] = 0;
    }
    FFT.Compute(vReal, vImag, RANDOM, FFT_FORWARD);
    //Serial.println(i);
  }

  time = millis() - time;
  xQueueSendToBack(RT4Queue, &time, 0);
  }
  //vTaskDelete(NULL);

}

/**
 * LED_setup()
 * @brief LED setup
 * @param LED_DDRL DDRL
 * @param LED_PORTL PORTL
 * @param PIN47 1 << DDL2
 */
void LED_setup(){
  LED_DDRL |= PIN47;
  LED_PORTL |= PIN47;
}

/**
 * speaker_setup()
 * @brief Speaker setup
 * @param SYSTEM_REG_A Enable compare output mode with TCCR3A 
 * @param SYSTEM_REG_B Enable Waveform Generation Mode (set WGM32 to 1) and set prescale to 8
 * @param PRE_SCALE Set OCR3A to 0  
 * @param TCNT4_COUNTER initialize TCNT4 to be 0. 
 * @param SPEAKER_DDRE Enable DDE3 (PIN 5)
 * @param SPEAKER_PORTE Enable PIN5 to be an output mode.
 */
void speaker_setup(){
  
  // Initialize system registers to zero.
  SYSTEM_REG_3A = 0;
  SYSTEM_REG_3B = 0;

  // Enable compare output mode with TCCR3A 
  // because TCCRnB does not have compare output mode.
  SYSTEM_REG_3A |= (0 << COM3A1) | (1 << COM3A0);

  // Enable Waveform Generation Mode (set WGM32 to 1)
  SYSTEM_REG_3A |= (0 << WGM31) | (0 << WGM30);
  SYSTEM_REG_3B |= (0 << WGM33) | (1 << WGM32);

  // Set the precale to 8 
  SYSTEM_REG_3B |= (0 << CS32) | (1 << CS31) | (0 << CS30);
  PRE_SCALE = 0;
  TCNT4_COUNTER = 0;

  // Set speaker output
  SPEAKER_DDRE |= PIN5; // Enable PIN 5(PE3)
  SPEAKER_PORTE |= PIN5; // Set PORTE to output mode.
}