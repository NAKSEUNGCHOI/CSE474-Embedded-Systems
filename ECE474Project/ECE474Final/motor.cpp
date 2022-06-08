/**
 *   @file   motor.cpp
 *   @author Nakseung Choi 1572578 Kejin Li 1978130
 *   @date   01-June-2022
 *   @brief  5V DC motor for a fan operation. 
 *   @description: By receiving data from the membrane keypad, the motor runs at different speeds.
 */
#include "motor.h"
#include "ECE474Final.h"

QueueHandle_t TaskMotorQueue;

/**
 * TaskMotor( void *pvParamters )
 * @brief TaskMotor
 * @param Motor_setup motor setup
 * @param TaskMotorQueue Created xQueue to receive data from keypad.
 * @param xQueueReceive (Name of function, pointer to receive, portMAX_DELAY)
 * @param xQueueReceive This receives data from keypad. Keypad can turn on and off the fan.
 * @param speed 0 180(slow) 255(fast) speed of the fan.
 * @param reset it stops the fan moving left and right automatically by making the global variable to 1.
 * @description: This receives data from keypad and turns on the fan at different speeds.
 */
void TaskMotor( void *pvParameters ){
  vTaskDelay(pdMS_TO_TICKS(100));
  TaskMotorQueue = xQueueCreate(1, sizeof(motorcontrol));
  Motor_setup();
  int *reset = (int*)pvParameters;

  motorcontrol temp;
  int speed = 0;
  for(;;){
    temp = MOTOR_RESET;
    xQueueReceive(TaskMotorQueue, &temp, portMAX_DELAY);
    Serial.print("Hey I got it!1: ");
    Serial.println(temp);
    switch(temp){
      case MOTOR_RESET:
          speed = 0;
          analogWrite(6, speed);
          *reset = 1;
          break;
        
        case PAUSE:
          speed = 0;
          analogWrite(6, speed);
          Serial.print("Fan off.");
          break;
        
        case SLOWER:
          speed = 180;
          analogWrite(6, speed);
          Serial.print("SPEED: ");
          Serial.println(speed);
          break;

        case FASTER:
          speed = 255;
          analogWrite(6, speed);
          Serial.print("SPEED: ");
          Serial.println(speed);
          break;

     
    } // switch end
  }// for end


}
/**
 * Motor_setup()
 * @brief LED setup
 * @param analogWrite set up a 5V DC motor
 * @param MOTOR_IN2 MOTOR IN2
 * @param MOTOR_IN1 MOTOR IN1
 * @param pinMode set up PIN30, PIN31, and PIN32 as an output mode.
 */
void Motor_setup(){
  analogWrite(6, 0);
  //MOTOR_ENABLE |= PIN6;
  MOTOR_IN2 |= PIN3;
  PORTE |= PIN3;

  MOTOR_IN1 |= PIN4;
  PORTG &= ~PIN4;
  pinMode(30, OUTPUT);
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
}