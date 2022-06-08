/**
 *   @file   temperature.cpp
 *   @author Nakseung Choi 1572578 Kejin Li 1978130
 *   @date   01-June-2022
 *   @brief  DHT11 sensor to measure the temperature and the humidity of the farm and display it on the LCD display in real time.
 */
#include <dht_nonblocking.h>
#include "ECE474Final.h"
#include "motor.h"
#include "temperature.h"
#include <LiquidCrystal.h> 
#define DHT_SENSOR_TYPE DHT_TYPE_11


static const int DHT_SENSOR_PIN = 43;//2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
LiquidCrystal lcd(26, 27, 28, 29, 30, 31);

/**
 * TaskTemperature( void *pvParameters )
 * @brief TaskTemperature
 * @param temperature_setup temperature setup
 * @param temperature variable to store temperature from the sensor.
 * @param humidity variable to store humidity from the sensor.
 * @param temp int pointer to receive a global variable (mode) to turn off the display when the system is de-activated.
 * @description: DHT11 sensor measures the temperature and humidity of the cattle farm and displays them on the LCD display.
 * When the system turns off by pressing A, it displays "*security on*", and "type password: ." When an user presses the passcode
 * It resumes to display the temp and humidity.
 */
void TaskTemperature( void *pvParameters ){
  
  temperature_setup();
  float temperature;
  float humidity;
  static long count = 0;
  int *temp = (int*)pvParameters;
  tempthreshold threshold;

  lcd.print("Wecome to farm!");
  lcd.setCursor(0,1);
  lcd.print("Choi & Kejin");

  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  while(1){
    if( measure_environment( &temperature, &humidity ) == true ){

      if(*temp == 1){
      //LCD
        lcd.setCursor(0,0); 
        lcd.print("Temp: ");
        lcd.print(temperature, 1);
        lcd.print((char)223);
        lcd.print("C    ");
        lcd.setCursor(0,1);
        lcd.print("Humidity: ");
        lcd.print(humidity, 1);
        lcd.print("%  ");

      //Serial Print
        Serial.print(count);
        Serial.print(". T = ");
        Serial.print(temperature, 1);
        Serial.print(" deg. C, ");
        Serial.print("H = ");
        Serial.print(humidity, 1);
        Serial.println("%  ");
    
        if(temperature >= 27 || humidity >= 70){
          
          //LCD
          lcd.setCursor(0,0);
          lcd.print("Crazy HOTTTTT!   ");
          lcd.setCursor(0,1);
          lcd.print("*alarm activated*");

          //Serial Print
          Serial.println("Crazy HOTTTTT!   ");
          Serial.println("*alarm activated*");
          
        }
        count++;
      }else{

        lcd.setCursor(0,0);
        lcd.print("* Security ON *");
        lcd.setCursor(0,1);
        lcd.print("Enter Password:   ");

        Serial.print(count);
        Serial.print(". T = ");
        Serial.print(temperature, 1);
        Serial.print(" deg. C, ");
        Serial.print("H = ");
        Serial.print(humidity, 1);
        Serial.println("%  ");
        if(temperature >= 27 || humidity >= 70){
          
          //LCD
          lcd.setCursor(0,0);
          lcd.print("Crazy HOTTTTT!   ");
          lcd.setCursor(0,1);
          lcd.print("*alarm activated*");
          threshold = SUPERHOT;
          if(xQueueSendToBack(TaskTemperatureQueue, &threshold, 0) != pdTRUE){
            Serial.println("Failed to send to queue");
          }

          //Serial Print
          Serial.println("Crazy HOTTTTT!   ");
          Serial.println("*alarm activated*");
          
        }
        count++;

      }
    }
  }
}
/**
 * measure_environment( float *temperature, float *humidity )
 * @brief measure_environment. It sets how ofen the sensor measures the parameters.
 * @param measurement_timestamp set to millis.
 */
static bool measure_environment( float *temperature, float *humidity ){
  
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}
/**
 * temperature_setup()
 * @brief temperature setup
 * @param lcd set to begin the lcd display
 */
void temperature_setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
}
