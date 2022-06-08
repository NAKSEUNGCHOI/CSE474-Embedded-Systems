/**
 * @file temperature.h
 * @author Nakseung Choi 1572578
 * @brief header file for temperature.cpp
 * @date June-01-2022
 */
#include <Arduino_FreeRTOS.h>
#include "Arduino.h"

/**
 * Function Pointers
 * Detailed descriptions are written in the motor.cpp file.
 */
static bool measure_environment( float *temperature, float *humidity );
void TaskTemperature( void *pvParameters );
void temperature_setup();