/**
 *   @file   Demo1.ino
 *   @author Nakseung Choi 1572578 Kejin Li 1978130
 *   @date   18-May-2022
 *   @brief  Task1 and Task2 concurrently using basic Round Robin(RR) scheduler.
 *   
 *   @Description 
 *    Task1 is to flash an external LED for 250ms every one second.
 *    Task2 is to make the speaker emit a theme called "Close Encounters of the Third Kind,"
 *    using hardware timer technique. Task2 plays the theme, sleep for 4 seconds, and then play the theme again.
 *    In lab 3, we learned to implement three schedulers(basic RR, SRRI, and DDS) for different tasks.
 *    Note that these schedulers are not pre-emptive. For this demo, I implemented the basic RR to run two tasks concurrently.
 */
#include "RR.h"

int reset = 0;
/**
 * setup()
 * @brief Initialize all registers for CTC LED, and speaker.
 * @param SYSTEM_REG_A TCCR3A
 * @param SYSTEM_REG_B TCCR3B 
 * @param PRE_SCALE Set OCR3A to 0  
 * @param TCNT4_COUNTER initialize TCNT4 to be 0. 
 * @param SPEAKER_DDRE Enable DDE3 (PIN 5)
 * @param SPEAKER_PORTE Enable PIN5 to be an output mode.
 * @param LED_DDR Enable PIN47
 */
void setup() {
  Serial.begin(9600);
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
  SPEAKER_DDRE |= 1 << DDE3; // Enable PIN 5(PE3)
  SPEAKER_PORTE |= 1 << DDE3; // Set PORTE to output mode.

  // Set output pin for LED
  LED_DDR |= 1 << DDL2;
}
/**
 * loop() 
 *  @brief Call Task1 and Task2 infinitely every 1ms.
 *  @param Task1 Flash an external LED for 250ms every one second.
 *  @param Task2 Make the speaker emit a theme called "Close Encounters of the Third Kind."
 */
void loop() {
  Task1();
  Task2();
  delay(1);
}
/**
 * set_OC3A_freq(uint32_t freq)
 * @brief This function takes a frequency and convert it to N.
 * @param PRE_SCALE = OCR3A is an output compare register.
 *                    This takes N to generate a certain tone for a set perid of time.
 *                    N can be calculated using this formula:
 *                    
 *                    System Clock = 16Mhz, 62.5ns per tick.
 *                    With a pre-scale of 8, 16Mhz / 8 = 2MHz
 *                    2,000,000 / (2*desired frequency) = N 
 * @param freq this is a frequency defined as an unsigned 32-bit int. 
 * @param TCNT4_COUTER Set this to be equal to 0 so that counting starts at 0. 
 * @warning Tones can be changed based on pre-scaling setup.
 */
void set_OC3A_freq(uint32_t freq) {
  PRE_SCALE = freq == 0 ? 0 : 2000000 / (2 * freq);
  //Serial.println(PRE_SCALE);
  TCNT4_COUNTER = 0;
}

/**
 * Task1()
 * @brief Flash an external LED for one second. (one for 250ms and off for 750ms)
 * @param time This is the time variable in the function that controls how long an LED flash.
 * @param LED_PORT Enable PORTL as an output mode.  
 * @param LED_DURATION is 250ms, which is the time that LED will remain ON. 
 * 
 * Detailed Description: Static unsigned time increments by one every time loops runs. (every 1 ms)
 * LED turns on for 250ms and turns off for 750ms.
 * when time crement = 1000 (one second,) this time is reset to zero.
 */
void Task1() {
  static unsigned long time;
  time++;

  // initialize to zero.
  if (reset) {
    LED_PORT |= 1 << DDL2; 
    time = 0;
    reset = 0;
    return;
  }

  // Start Fflashing LED at time = 1 (ms)
  if (time == (0 * LED_DURATION) + 1) {
    LED_PORT |= (1 << DDL2);
  }

  //if time = 251, turn off
  if (time == (1 * LED_DURATION) + 1) {
    LED_PORT &= ~(1 << DDL2);
  }

  // reset time at one second.
  if (time == 1000) {
    time = 0;
  }
  return;
}

/**
 * Task2()
 * @brief Task2 is to make a speaker emit a theme called "Close Encounters of the Third Kind."
 * @param time This is the time variable in the function that controls how long each tone plays.
 * @param LED_PORT Enable PORTL as an output mode.  
 * @param TONE_DURATION is 200ms, which is the time that it plays each tone for.
 * 
 * Detailed Description: Static unsigned time increments by one every time loops runs. (every 1 ms)
 * The first loop runs for NTONE = 13 times, and each tone plays for 200 milli second.
 * if this time constantly increments and reach to 13 * 200 + 1 seconds,
 * it sleeps for 4 seconds. Then, it plays again after 4 seconds of sleep.
 * Lastly, when time reaches to (2 * 13 * 200) + 4000 seconds + 1, 
 * time is set back to zero.
 */
void Task2() {
  static unsigned long time;
  time++;

  if (reset) {
    set_OC3A_freq(0);
    time = 0;
    reset = 0;
    return;
  }

  for (int i = 0; i < NTONE; i++) {
    if (time == (i * TONE_DURATION) + 1) {
      //Serial.println(i); //293, 329, 261, 130, 196
      set_OC3A_freq(melody[i]);
    }
  }

  // stop playing for 4 seconds
  if (time == (NTONE * TONE_DURATION) + 1) {
    set_OC3A_freq(0);
  }

  // start playing again after 4 seconds of stop
  for (int i = 0; i < NTONE; i++) {
    if (time == (TIME_COUNTING + i * TONE_DURATION) + 1) {
      set_OC3A_freq(melody[i]); //293, 329, 261, 130, 196
    }
  }

  // reset time to zero
  if (time == (TIME_COUNTING + NTONE * TONE_DURATION) + 1) {
    time = 0;
  }
}
