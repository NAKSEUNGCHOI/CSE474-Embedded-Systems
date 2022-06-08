/**
 * @file ECE474Final.h
 * @author Nakseung Choi 1572578
 * @brief header file for ECE474Final.ino
 * @date June-01-2022
 */
#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include "keypad.h"
#include "motor.h"
#include "servo.h"
#include "temperature.h"
#include "security.h"

#define LED_DDRL DDRL
#define LED_PORTL PORTL
#define PIN42 1 << DDL7

extern QueueHandle_t TaskMotorQueue;
extern QueueHandle_t TaskServoQueue;
extern QueueHandle_t TaskTemperatureQueue;

typedef enum motorcontrol{
  MOTOR_RESET,
  PAUSE,
  FASTER,
  SLOWER
}motorcontrol;

typedef enum servocontrol{
  AUTO,
  SERVO_RESET,
  LEFT,
  RIGHT,
  UP,
  DOWN,
  OPENUP,
  CLOSEUP
}servocontrol;

typedef enum tempthreshold{
  NORMAL,
  SUPERHOT
}tempthreshold;

/**
 * Function Pointers
 * Detailed descriptions are written in the main ino file.
 */
void setup();
void LED_setup();
void TaskExternalLED( void *pvParameters );