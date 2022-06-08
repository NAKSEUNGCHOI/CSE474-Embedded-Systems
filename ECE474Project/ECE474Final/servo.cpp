/**
 *   @file   servo.cpp
 *   @author Nakseung Choi 1572578 Kejin Li 1978130
 *   @date   01-June-2022
 *   @brief  Two servos to move the fan up and down and left and right. one servo to open and close up the food bank to feed the cows.
 *   @description: Receiving data from the membrane keypad, three servos operate for different functionalities.
 */
#include "servo.h"
#include "ECE474Final.h"
#include <Servo.h>

Servo myservo;
Servo myservo2;
Servo myservo3;
QueueHandle_t TaskServoQueue;

/**
 * TaskServo( void *pvParameters )
 * @brief TaskServo
 * @param Serve_setup Servo setup
 * @param TaskServoQueue Createdd xQueue to receive data from keypad.
 * @param servocontrol to receive the data from keypad.
 * @param xQueueReceive (Name of function, pointer to receive, portMAX_DELAY)
 * @param xQueueReceive This receives data from keypad. Keypad controls the direction of the fan.
 * @param myservo1 servo 1 and servo 2 for the direction of the fan(up, down, left, and right)
 * @param myservo3 servo 3 for the food bank.
 * @param NONE automatic mode. The fan moves left and right automatically until users press B. 
 * @description: Receiving data from keypad, it updates the state of the servo(up, down, left and right) and moves the direction of the fan.
 * Keypad 4 and 6 is to open and close the food bank to feed the cows.
 */
void TaskServo( void *pvParameters ){
  vTaskDelay(pdMS_TO_TICKS(100));
  TaskServoQueue = xQueueCreate(1, sizeof(TaskServoQueue));
  Serve_setup();

  servocontrol temp1;
  int degree = 0;
  int *automatic = (int*)pvParameters;
  int counter = 5;
  for(;;){
    xQueueReceive(TaskServoQueue, &temp1, portMAX_DELAY);
    Serial.print("SERVO: ");
    Serial.println(temp1);
    switch(temp1){

      case SERVO_RESET:
          degree = 90;
          Serial.println("RESET");
          myservo.write(degree);
          myservo2.write(degree);
          break;
        
        case UP:
          degree = 75;
          Serial.println("UP");
          myservo2.write(degree);
          break;
        
        case DOWN:
          degree = 110;
          myservo2.write(degree);
          Serial.println("DOWN");
          Serial.println(degree);
          break;

        case LEFT:
          degree = 125;
          Serial.println("LEFT");
          myservo.write(degree);
          //vTaskDelay(pdMS_TO_TICKS(1000));
          break;
        
        case RIGHT:
          degree = 65;
          Serial.println("RIGHT");
          myservo.write(degree);
          // vTaskDelay(pdMS_TO_TICKS(1000));
          break;

        case AUTO:
          degree = 65;
          *automatic = 0;
          while(*automatic != 1){
            degree = degree + counter;
            myservo.write(degree);
            vTaskDelay(pdMS_TO_TICKS(100));
            if(degree == 125){
              while(degree != 65){
                degree = degree - counter;
                myservo.write(degree);
                vTaskDelay(pdMS_TO_TICKS(100));
              }
            }
          }
          break;
        
        case OPENUP:
          degree = 120;
          Serial.println("OPEN");
          myservo3.write(degree);
          break;

        case CLOSEUP:
          degree = 90;
          Serial.println("CLOSE");
          myservo3.write(degree);
          break;


     
    } // switch end
  }

}
/**
 * Serve_setup()
 * @brief servo setup
 * @param myservo set up three servos to PIN13, 22, and 23. Pins are chosen based on the physical location proximity to the board and servos.
 */
void Serve_setup(){
  Serial.begin(9600);
  
  myservo.attach(PIN13);
  myservo2.attach(PIN22);
  myservo3.attach(PIN23);

  myservo.write(90); // move servos to center position -> 90°
  myservo2.write(90);
  myservo3.write(90);

}