/**
 *   @file   security.cpp
 *   @author Nakseung Choi 1572578 Kejin Li 1978130
 *   @date   01-June-2022
 *   @brief  Security system using HC-SR501 sensor.
 *   @description: When users activates the security system, HC-SR501 sensor turns on.
 *   If there is any motion detected, the alarm and LEDs go off.
 */
#include "security.h"
#include "ECE474Final.h"

int ledPin1 = 12;  // LED on Pin 12 of Arduino
int ledPin2 = 11;  // LED on Pin 11 of Arduino
int ledPin3 = 10;  // LED on Pin 10 of Arduino
int pirPin = 48; // Input for HC-S501

int pirValue; // Place to store read PIR Value
int melody[] = { NOTE_E7, NOTE_C7, NOTE_E7, NOTE_C7, NOTE_E7, NOTE_C7, NOTE_E7, NOTE_C7,
                 NOTE_E7, NOTE_C7, NOTE_E7, NOTE_C7, NOTE_E7, NOTE_C7, NOTE_E7, NOTE_C7,
                 NOTE_E7, NOTE_C7, NOTE_E7, NOTE_C7, NOTE_E7, NOTE_C7, NOTE_E7, NOTE_C7,
                 NOTE_E7, NOTE_C7, NOTE_E7, NOTE_C7, NOTE_E7, NOTE_C7, NOTE_E7, NOTE_C7 };

int melody2[] = { NOTE_C6, NOTE_E6, NOTE_G6 };
QueueHandle_t TaskTemperatureQueue;


/**
 * TaskSecurity( void *pvParameters )
 * @brief TaskSecurity
 * @param Security_setup Security system setup
 * @param Speaker_setup Security system setup
 * @param temp int pointer to receive a global variable (mode). This security system activates when the main system is de-activiated.
 * @param time duration of the time to turn the LED and speaker on when an objected is detected.
 * @param count number of tone. It sets back to zero to play again when it plays all of the tones.
 * @param deactivate deactivate the alarm and LEDs and turns the system back on. Used to indicate when to turn on the second melody.
 * @description: As an user activates the security system by pressing A on the keyboard, the security system turns on.
 * if an objected is detected by the HC-SR501 sensor, the alarm and LEDs turn on. To turn it off, the user must type the password.
 * It makes the second tone indicating that the alarm and LEDs are off. 
 */
void TaskSecurity( void *pvParameters ){
  Security_setup();
  Speaker_setup();
  TaskTemperatureQueue = xQueueCreate(1, sizeof(tempthreshold));
  int *temp = (int*)pvParameters;
  unsigned long time = 0;
  int count = 0;
  int deactivate = 0;
  tempthreshold threshold;

  while(1){
    threshold = NORMAL;
    // xQueueReceive(TaskTemperatureQueue, &threshold, portMAX_DELAY);
    if(*temp == 0){
      pirValue = digitalRead(pirPin);
      if(pirValue == 1){
        time = time + 10000;
        deactivate = 1;
      }
      if(threshold == SUPERHOT){
        time = time + 10000;
        deactivate = 1;
      }
      while(time != 0){
        digitalWrite(ledPin1, pirValue);
        vTaskDelay(pdMS_TO_TICKS(100));
        LED_PORTB &= ~(PIN12);
        digitalWrite(ledPin2, pirValue);
        vTaskDelay(pdMS_TO_TICKS(100));
        LED_PORTB &= ~(PIN11);
        digitalWrite(ledPin3, pirValue);
        vTaskDelay(pdMS_TO_TICKS(100));
        LED_PORTB &= ~(PIN10);
        set_OC3A_freq(melody[count]);
        vTaskDelay(pdMS_TO_TICKS(50));
        time--;
        count++;
        if(*temp == 1){
          time = 0;
        }
        if(count == 32){
          count = 0;
        }
      }// end while(time != 0)
    }else{
      LED_PORTB &= ~(PIN12);
      LED_PORTB &= ~(PIN11);
      LED_PORTB &= ~(PIN10);
      SPEAKER_DDRE &= ~(PIN5);
      SPEAKER_PORTE &= ~(PIN5);
      if(deactivate == 1){
        set_OC3A_freq(melody2[0]);
        vTaskDelay(pdMS_TO_TICKS(100));
        set_OC3A_freq(melody2[1]);
        vTaskDelay(pdMS_TO_TICKS(100));
        set_OC3A_freq(melody2[2]);
        vTaskDelay(pdMS_TO_TICKS(300));
        deactivate = 0;
      } // end if
    } // end else 
  } // end while(1)
}
/**
 * Security_setup()
 * @brief Security system setup
 * @param LED_DDRB DDRB
 * @param DDRL this is for the sensor input.
 * @param PIN12 PIN 12 ~ 10 for LEDs. They start LOW.
 */
void Security_setup(){
  Serial.begin(9600);
  LED_DDRB |= PIN12;
  LED_DDRB |= PIN11;
  LED_DDRB |= PIN10;
  DDRL &= ~(1 << DDL1);
  // pinMode(ledPin, OUTPUT);
  // pinMode(pirPin, INPUT);
 
  //digitalWrite(ledPin, LOW);
  LED_PORTB &= ~(PIN12);
  LED_PORTB &= ~(PIN11);
  LED_PORTB &= ~(PIN10);
}
/**
 * Speaker_setup()
 * @brief Speaker setup. same as lab 2 and 3.
 * @param SYSTEM_REG_A TCCR3A to enable "compare output mode"
 * @param SYSTEM_REG_B TCCR3B to enable WGM and pre-scale
 * @param PRE_SCALE pre-scale is set to 8 for this application.
 * @param TCNT4_COUNTER set to 0 to start at 0.
 * @param SPEAKER_DDRE TCCR3A is designated to PIN5.
 * @param SPEAKER_PORTE TCCR3A is designated to PIN5.
 */
void Speaker_setup(){
  // Initialize system registers to zero.
  SYSTEM_REG_A = 0;
  SYSTEM_REG_B = 0;

  // Enable compare output mode with TCCR3A 
  // because TCCRnB does not have compare output mode.
  SYSTEM_REG_A |= (0 << COM3A1) | (1 << COM3A0);

  // Enable Waveform Generation Mode (set WGM32 to 1)
  SYSTEM_REG_A |= (0 << WGM31) | (0 << WGM30);
  SYSTEM_REG_B |= (0 << WGM33) | (1 << WGM32);

  // Set the precale to 8 
  SYSTEM_REG_B |= (0 << CS32) | (1 << CS31) | (0 << CS30);
  PRE_SCALE = 0;
  TCNT4_COUNTER = 0;

  // Set speaker output
  SPEAKER_DDRE |= PIN5; // Enable PIN 5(PE3)
  SPEAKER_PORTE |= PIN5; // Set PORTE to output mode.

}
/**
 * set_OC3A_freq(uint32_t freq)
 * @brief a function to generate tones. same as lab 2 and 3.
 * @param PRE_SCALE The function receives one frequency at a time and set N to OCR3A.
 * @param TCNT4_COUNTER set to 0 to start at 0.
 */
void set_OC3A_freq(uint32_t freq) {
  SPEAKER_DDRE |= PIN5;
  SPEAKER_PORTE |= (PIN5);
  PRE_SCALE = freq == 0 ? 0 : 2000000 / (2 * freq);
  //Serial.println(PRE_SCALE);
  TCNT4_COUNTER = 0;
}
