/**
 *   @file   Demo2.ino
 *   @author Nakseung Choi 1572578 Kejin Li 1978130
 *   @date   20-May-2022
 *   @brief  Task1 and Task2 concurrently using Synchronized Round Robin(SRRI) scheduler.
 *   
 *   Detailed Description: In setup function, LED, speaker, interrupt functions are called for initial setup.
 *                         Task1() and Task2() are stored in void function pointer that points to an array called
 *                         runningTasks[NTasks]. The rest of the space in runningTasks is stored with schedule_sync.
 *                         As it goes through loop() function, (*runningTasks[i])() calls each Task 1 and 2, and
 *                         both of them start to run concurrently. Sleep function, which takes a number of sleep time
 *                         is employed to make a task go sleep for a certain period of time. In schedule sync function, 
 *                         the amount of the time that a task sleeps is synchronized by sleepTasks decrementing by -2. 
 *                         At the same time, timeTasks increments by 2 (meaning 2 ms) because interrupt is set to occur 
 *                         every 2 ms.   
 */

#include "SRRI.h"

int reset = 0;
void (*runningTasks[NTASKS])();
volatile int sFlag = PENDING;
volatile int currentTasks;
volatile int sleepTasks[NTASKS];
volatile int stateTasks[NTASKS];
volatile long timeTasks[NTASKS];
unsigned long int little_comp;


/**
 * setup()
 * @brief Initialize all registers for LED, speaker, and interrupt as well as Tasks being stored in an array called running Tasks.
 * @param LED_setup A function that contains all LED setups.
 * @param speaker_setup A function that contains all speaker setups.
 * @param interrupt_setup A function that contains all interrupt setups. 
 * @param runningTasks initialized to NULL at first, but Task 1 and 2 are stored in runningTasks[0] and runningTasks[1]
 * @param sleepTasks initialized to 0.
 * @param stateTasks initialized to READY.
 * @param timeTasks initialized to 0.
 */
void setup() {
  Serial.begin(9600);
  LED_setup();
  speaker_setup();
  interrupt_setup();

  for(int i = 0; i < NTASKS; i++){
    runningTasks[i] = NULL;
    sleepTasks[i] = 0;
    stateTasks[i] = READY;
    timeTasks[i] = 0;
  }
  runningTasks[0] = Task1;
  runningTasks[1] = Task2;
  runningTasks[NTASKS - ACTUAL_N_TASKS] = schedule_sync;
}
/**
 * loop()
 * @brief Loop function that run tasks infinitely.
 * @param stateTasks Set to RUNNING.
 * @param runningTasks this function pointer constantly calls all stored tasks for NTASKS times. 
 * @param stateTasks As long as state = RUNNING, it keeps running.
 */
void loop() {
  for (int i = 0; i < NTASKS; i++) {
    if (runningTasks[i] != NULL && stateTasks[i] == READY) {
      stateTasks[i] = RUNNING;
      currentTasks = i;
      (*runningTasks[i])();
      if (stateTasks[i] == RUNNING){
        stateTasks[i] = READY;
      }
    }
  }
}
/**
 * ISR(TIMER4_COMPA_vect)
 * @brief Interupt Service Routine
 * @param sFlag Set to DONE, meaning triggered.
 */
ISR(TIMER4_COMPA_vect){
  sFlag = DONE;
}
/**
 * sleep_474(long t)
 * @brief this function takes a long t and set sleepTasks[currentTasks]'s time to t and set state to SLEEPING to sleep.
 * @param sleepTasks set to t to sleep for "t" period.
 * @param stateTasks set to SLEEPING to change the currentTasks state to SLEEPING.
 */
void sleep_474(long t){
  sleepTasks[currentTasks] = t;
  stateTasks[currentTasks] = SLEEPING;
}
/**
 * schedule_sync()
 * @brief This function synchronizes the time for sleeping and running time.
 * @param little_comp It increments by one every time when interupt occurs.
 * @param sleepTasks This decrement by 2 to count how long a task sleeps.
 * @param timeTasks This increment by 2 to count how long a task runs.
 */
void schedule_sync(){
  while (sFlag == PENDING){
    little_comp = little_comp + 1;
    //Serial.println(little_comp);
    if(little_comp == 10000){
      little_comp = 0;
    }
  }
  for(int i = 0; i < NTASKS; i++){
    if(stateTasks[i] == SLEEPING){
      //Serial.println(sleepTasks[i]);
      sleepTasks[i] -= ACTUAL_N_TASKS;
      if(sleepTasks[i] < ACTUAL_N_TASKS){
        sleepTasks[i] = 0;
        stateTasks[i] = READY;
      }
    }
    timeTasks[i] += ACTUAL_N_TASKS;
  }
  sFlag = PENDING;
  return;
}
/**
 * set_OC3A_freq(uint32_t freq)
 * @brief This sets frequencices for speaker
 * @param PRE_SCALE OCR3A
 * @param freq frequencies
 * @param TCNT4_COUNTER set this to zero to restart counting.
 */
void set_OC3A_freq(uint32_t freq) {
  PRE_SCALE = freq == 0 ? 0 : 2000000 / (2 * freq);
  //Serial.println(PRE_SCALE_SPEAKER);
  TCNT4_COUNTER = 0;
}
/**
 * Task1()
 * @brief LED turns on for 250ms and off for 750ms
 * @param sleep_474 a function that makes a task sleeps for a certain period of time.
 * @param LED_ON 250ms
 * @param LED_OFF 750ms
 * @param PIN5 DDL2
 * @param LED_PORT PORTL
 * @param timeTasks when timeTasks[currentTasks] >= 1000ms, timeTasks[currentTasks] is set back to 0.
 */
void Task1(){

  if(timeTasks[currentTasks] < (1 * LED_DURATION) + 1) {
    // Serial.println(timeTasks[currentTasks]);
    LED_PORT |= (1 << PIN5);
    sleep_474(LED_ON); // 250ms
    return;
  }
  
  if(timeTasks[currentTasks] < (4 * LED_DURATION) + 1) {
    // Serial.println("hello");
    // Serial.println(timeTasks[currentTasks]);
    LED_PORT &= ~(1 << PIN5);
    sleep_474(LED_OFF); // 750ms
    return;
  }

  if(timeTasks[currentTasks] >= LED_TOTAL){
    timeTasks[currentTasks] = 0;
  }
  return;
}
/**
 * Task2()
 * @brief This function plays 13 different tones.
 * @param sleep_474 a function that makes a task sleeps for a certain period of time.
 * @param TONE_DURATION 200ms each tone plays for 200ms
 * @param SLEEP_DURATION 4000ms sleeps for 4 seconds after all tones are played.
 * @param melody an array that all frequencies are stored.
 * @param set_OC3A_freq a function that does calculation for a tone to play at a certain frequency.
 */
void Task2(){

  for(int i = 0; i < NTONE; i++){
    if(timeTasks[currentTasks] >= i * TONE_DURATION && timeTasks[currentTasks] < (i + 1) * TONE_DURATION){
      set_OC3A_freq(melody[i]);
      sleep_474(TONE_DURATION);
      return;
    }
  }
  
  if(timeTasks[currentTasks] < TIME_COUNTING){
      set_OC3A_freq(0);
      sleep_474(SLEEP_DURATION);
  }

  for(int i = 0; i < NTONE; i++){
    if(timeTasks[currentTasks] >= (TIME_COUNTING + ((unsigned long) i) * TONE_DURATION) &&
       timeTasks[currentTasks] < (TIME_COUNTING + ((unsigned long) i + 1) * TONE_DURATION)){
      set_OC3A_freq(melody[i]);
      sleep_474(TONE_DURATION);
    }
  }

  if(timeTasks[currentTasks] >= (TIME_COUNTING + NTONE * TONE_DURATION)){
    timeTasks[currentTasks] = 0;
  }

}
/**
 * LED_setup()
 * @brief LED set-up
 * @param LED_DDR DDRL
 */
void LED_setup(){
  LED_DDR |= 1 << DDL2;  
}
/**
 * speaker_setup()
 * @brief Speaker setup
 * @param SYSTEM_REG_A Enable compare output mode with TCCR3A 
 * @param SYSTEM_REG_B Enable Waveform Generation Mode (set WGM32 to 1) and Set prescale to 8
 * @param PRE_SCALE Set OCR3A to 0  
 * @param TCNT4_COUNTER initialize TCNT4 to be 0. 
 * @param SPEAKER_DDRE Enable DDE3 (PIN 5)
 * @param SPEAKER_PORTE Enable PIN5 to be an output mode.
 */
void speaker_setup(){

  SYSTEM_REG_3A = 0;
  SYSTEM_REG_3B = 0;

  SYSTEM_REG_3A |= (0 << COM3A1) | (1 << COM3A0);

  SYSTEM_REG_3A |= (0 << WGM31) | (0 << WGM30);
  SYSTEM_REG_3B |= (0 << WGM33) | (1 << WGM32);

  SYSTEM_REG_3B |= (0 << CS32) | (1 << CS31) | (0 << CS30);
  PRE_SCALE = 0;
  TCNT4_COUNTER = 0;

  SPEAKER_DDRE |= 1 << DDE3; 
  SPEAKER_PORTE |= 1 << DDE3; 

}
/**
 * interrupt_setup()
 * @brief hardware timer to generate an interrupt for each tick (2ms).
 * @param TCCR4A Enable compare output mode
 * @param TCCR4B Enable Waveform Generation Mode (set WGM42 to 1) and set the precale to 1
 * @param OCR4A set to 32000 for 2ms 
 * @param TCNT3H initially set to 0 to start counting.
 * @param SREG so-called "master interrupt" This needs to be enabled to be able to use interrupt
 * @param TIMSK4 enable interrupt on counter 4A (output compare A match interrupt)
 */
void interrupt_setup(){

  // Initialize system registers to zero.
  TCCR4A = 0;
  TCCR4B = 0;

  TCCR4A |= (0 << COM4A1) | (1 << COM4A0);

  TCCR4A |= (0 << WGM41) | (0 << WGM40);
  TCCR4B |= (0 << WGM43) | (1 << WGM42);

  TIMSK4 |= (1 << OCIE4A);

  TCCR4B |= (0 << CS42) | (0 << CS41) | (1 << CS40);
  OCR4A = 32000;
  TCNT3H = 0;

  SREG |= (1<<7);
}