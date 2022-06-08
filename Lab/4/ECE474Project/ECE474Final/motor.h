/**
 * @file motor.h
 * @author Nakseung Choi 1572578
 * @brief header file for motor.cpp
 * @date June-01-2022
 */
#include "Arduino.h"
#include <Arduino_FreeRTOS.h>
#include "keypad.h"

#define MOTOR_ENABLE DDRH
#define MOTOR_IN1 DDRG
#define MOTOR_IN2 DDRE
#define PIN6 1 << DDH3
#define PIN4 1 << DDG5
#define PIN3 1 << DDE5

/**
 * Function Pointers
 * Detailed descriptions are written in the motor.cpp file.
 */
void TaskMotor( void *pvParameters );
void Motor_setup();