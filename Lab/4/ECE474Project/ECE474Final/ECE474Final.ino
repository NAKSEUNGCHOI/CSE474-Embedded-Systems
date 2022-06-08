/**
 *   @file   ECE474Final.ino
 *   @author Nakseung Choi 1572578 Kejin Li 1978130
 *   @date   01-June-2022
 *   @brief  ECE474 Final Automation and Security Systems for cattle farms. 
 *   @description: This is an automation system for cattle farms. Users must type the password to activate the automation system.
 *   Otherwise, the security system(HC-SR501) turns on. If an movement is detected, the alarm system (alarm and LEDs) turns on.
 *   To turn it off, users must type the password to de-activate the security system. When users activates the automation system,
 *   the LCD display starts to display the temperature and humidity inside the farm in real time. (DHT11 sensor is always on.)
 *   Users can control the direction and speed of the fan using membrane keypad. Using the keypad, it can also open and close up the food
 *   bank to feed the cows. If users press "A", the automation system turns off and the security system turns back on to detect theives
 *   trying to sneak into the farm to steal the cows at night. 
 */
#include "ECE474Final.h"

TaskHandle_t TaskMotorHandle;

/**
 * setup()
 * @brief setup function
 * @param xTaskCreate set up tasks to run.
 * @param xTaskCreate (Name of function, any name, stack memory, parameter passing, priority, Task handle) 
 * @param vTaskStartScheduler The task scheduler, which takes over control of scheduling individual tasks, is automatically started.
 * @param mode mode 0 deactivates the keyboard and turns on the security system on.
 * @param mode mode 1 activates the system.
 * @param auto_mode The fan moves left and right automatically until users press B.
 * @description: This funtion works as a scheduler for all of the tasks created using free-rtos.
 */
void setup() {
  Serial.begin(9600);
  while(!Serial){;}

  xTaskCreate(TaskExternalLED, "ExternalLED", 128, NULL, 3, NULL);

  volatile int auto_mode = 0;
  xTaskCreate(TaskMotor, "Motor", 500, &auto_mode, 1, NULL);
  xTaskCreate(TaskServo, "Servo", 500, &auto_mode, 1, NULL);
  volatile int mode = 0;
  xTaskCreate(TaskKeypad, "Keypad", 500, &mode, 1, NULL); // 0
  xTaskCreate(TaskTemperature, "temperature", 500, &mode, 1, NULL); // 0
  xTaskCreate(TaskSecurity, "security", 128, &mode, 1, NULL); 
  
  
  // Write this at the end. 
  vTaskStartScheduler();

}
void loop() {
  // put your main code here, to run repeatedly:

}
/**
 * TaskExternalLED( void *pvParameters )
 * @brief flash an external LED for 100ms and off for 200ms.
 * @param LED_setup LED setup function.
 * @param LED_PORTL PORTL
 * @param PIN42 1 << DDL7
 * @param vTaskDelay same as delay();
 * @param portTICK_PERIOD_MS 100 / portTICK_PERIOD_MS means 100 per tick.
 */
void TaskExternalLED( void *pvParameters ){
  
  LED_setup();

  for(;;){
    LED_PORTL |= PIN42;
    vTaskDelay( 100 / portTICK_PERIOD_MS );
    LED_PORTL &= ~(PIN42);
    vTaskDelay( 200 / portTICK_PERIOD_MS );
  }

}
/**
 * LED_setup()
 * @brief LED setup
 * @param LED_DDRL DDRL
 * @param LED_PORTL PORTL
 * @param PIN42 1 << DDL7
 */
void LED_setup(){
  LED_DDRL |= PIN42;
  LED_PORTL |= PIN42;
}
