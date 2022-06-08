/**
 * @file keypad.cpp
 * @author Nakseung Choi 1572578
 * @brief The keypad controls all of the tasks.
 * @date June-01-2022
 * @description: The keypad runs three servos, one fan, and security systems. It activates two modes: automation system and security system.
 * "1": fan speed(255)   "2": fan speed(180)   "3": fan off
 * "4": food bank open   "6": food bank close 
 * "5": fan moves up     "0": fan moves down   "C": Fan moves left and right automatically
 * "7": fan moves left   "9": fan moves right  "8": fan goes to center.
 * "A": Activates the security system. "B": Fan resets.
 * Users must type the password to activate the automation system.
 */
#include "keypad.h"
#include "ECE474Final.h"
#include "motor.h"


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {34, 35, 36, 37}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {38, 39, 40, 41}; //connect to the column pinouts of the keypad
char* password = "4592#";
int count = 0;

 
/**
 * TaskKeypad( void *pvParameters )
 * @brief TaskKeypad
 * @param keypad_setup keypad setup
 * @param temp int pointer to receive a global data(mode) from scheduler.
 * @param temp Keypad only runs if mode = 1, meaning the user must type the right passcode to activate the system.
 * @param customKey a variable that receives keypad data
 * @param password 4592#
 * @param count count increments by one as an user types the passcode in order. if count = 5, it activates the system.
 * @param count to turn off the system and activates the security system, press A.
 * @param motorcontrol If users press a key(1, 2, or 3), it updates the state of the motor and send the data to motor.cpp and controls the speed of the fan.
 * @param servocontrol If users press a key(4, 5, 6, 7, 8, 9, 0), it updates the state of the servos and send the data to servo.cpp and controls the direction of the fan.
 * @description: An user interface. Users must type the password initially to activate the cattle farm automation system.
 * Otherwise, the security system is on. This sends data to three servos, 5V DC motors. 
 */
void TaskKeypad( void *pvParameters ){
  keypad_setup();
  Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
  //TaskKeypadQueue = xQueueCreate(1, sizeof(motorcontrol));
  motorcontrol mctrl;
  servocontrol sctrl;
  int *temp = (int*)pvParameters;

  while(1){
    
    mctrl = MOTOR_RESET;
    sctrl = SERVO_RESET;
    char customKey = customKeypad.getKey();

    if(customKey == password[count]){
      count++;
    }
    if(count == 5){
      Serial.println("Access Granted");
      *temp = 1;
      Serial.println(*temp);
      count++;
    }
    switch(customKey){

      case '1':
        if(*temp == 1){
          mctrl = FASTER;
          if(xQueueSendToBack(TaskMotorQueue, &mctrl, 0) != pdTRUE){
            Serial.println("Failed to send to queue");
          }
        }
        break;

      case '2':
        if(*temp == 1){
          mctrl = SLOWER;
          if(xQueueSendToBack(TaskMotorQueue, &mctrl, 0) != pdTRUE){
            Serial.println("Failed to send to queue");
          }
        }
        break;

      case '3':
        if(*temp == 1){
          mctrl = PAUSE;
          if(xQueueSendToBack(TaskMotorQueue, &mctrl, 0) != pdTRUE){
            Serial.println("Failed to send to queue");
          }
        }
        break;
      
      case 'B':
        if(*temp == 1){
          mctrl = MOTOR_RESET;
          if(xQueueSendToBack(TaskMotorQueue, &mctrl, 0) != pdTRUE){
            Serial.println("Failed to send to queue");
          }
        }
        break;

        // servo now
      case '5':
        if(*temp == 1){
          sctrl = UP;
          if(xQueueSendToBack(TaskServoQueue, &sctrl, 0) != pdTRUE){
            Serial.println("Failed to send to queue");
          }
        }
        break;

      case '0':
        if(*temp == 1){
          sctrl = DOWN;
          if(xQueueSendToBack(TaskServoQueue, &sctrl, 0) != pdTRUE){
            Serial.println("Failed to send to queue");
          }
        }
        break;

      case '7':
        if(*temp == 1){
          sctrl = LEFT;
          if(xQueueSendToBack(TaskServoQueue, &sctrl, 0) != pdTRUE){
            Serial.println("Failed to send to queue");
          }
        }
        break;

      case '9':
        if(*temp == 1){
          sctrl = RIGHT;
          if(xQueueSendToBack(TaskServoQueue, &sctrl, 0) != pdTRUE){
            Serial.println("Failed to send to queue");
          }
        }
        break;

      case '8':
        if(*temp == 1){
          sctrl = SERVO_RESET;
          if(xQueueSendToBack(TaskServoQueue, &sctrl, 0) != pdTRUE){
            Serial.println("Failed to send to queue");
          }
        }
        break;
        // Food bank now
      case '4':
        if(*temp == 1){
          sctrl = OPENUP;
          if(xQueueSendToBack(TaskServoQueue, &sctrl, 0) != pdTRUE){
            Serial.println("Failed to send to queue");
          }
        }
        break;

      case '6':
        if(*temp == 1){
          sctrl = CLOSEUP;
          if(xQueueSendToBack(TaskServoQueue, &sctrl, 0) != pdTRUE){
            Serial.println("Failed to send to queue");
          }
        }
        break;

      case 'A':
        if(*temp == 1){
          *temp = 0;
          count = 0;
          // mctrl = PAUSE;
          // if(xQueueSendToBack(TaskMotorQueue, &mctrl, 0) != pdTRUE){
          //   Serial.println("Failed to send to queue");
          // }
          Serial.println("Security Activated");
        }
        break;

      case 'C':
        if(*temp == 1){
          mctrl = FASTER;
          if(xQueueSendToBack(TaskMotorQueue, &mctrl, 0) != pdTRUE){
            Serial.println("Failed to send to queue");
          }
          sctrl = AUTO;
          if(xQueueSendToBack(TaskServoQueue, &sctrl, 0) != pdTRUE){
            Serial.println("Failed to send to queue");
          }
          Serial.println("Security Activated");
        }
        break;
    }

  }
}
/**
 * keypad_setup()
 * @brief keypad setup.
 * @param Serial used for Serial.print();
 */
void keypad_setup(){
  Serial.begin(9600);
}
