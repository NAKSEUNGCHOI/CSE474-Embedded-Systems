/**
 * @file servo.h
 * @author Nakseung Choi 1572578
 * @brief header file for servo.cpp
 * @date June-01-2022
 */
#include <Arduino_FreeRTOS.h>
#include "Arduino.h"

#define PIN13 13
#define PIN22 22
#define PIN23 23

/**
 * Function Pointers
 * Detailed descriptions are written in the motor.cpp file.
 */
void TaskServo(void *pvParameters );
void Serve_setup();