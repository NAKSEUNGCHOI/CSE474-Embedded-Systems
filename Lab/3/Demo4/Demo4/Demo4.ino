/**
 *   @file   Demo4.ino
 *   @author Nakseung Choi 1572578 Kejin Li 1978130
 *   @date   21-May-2022
 *   @brief  Task1, Task2, and Task3 concurrently using SRRI
 *   @detail: Task1() is blinking LED for 1000ms
 *   Task2() plays 13 different tones 200ms each. 4 seconds break in between.
 *   Task3() counts up and diplays counting on 4-bit 7-seg.
 *   Using SRRI scheduler, all tasks operate concurrently.                      
 */

#include "SRRI4.h"

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
 * @param Serial enable Serial.print at 9600.
 * @param speaker_setup speaker setup
 * @param interrupt_setup interrupt setup
 * @param seven_segment_setup 4bit seven segment setup
 * @param LED_setup  LED setup
 * @param T1 0
 * @param T2 1
 * @param T3 2
 * @param schedule_sync schedule syncler. 
 */
void setup() {
  Serial.begin(9600);
  LED_setup();
  speaker_setup();
  interrupt_setup();
  seven_segment_setup();

  for(int i = 0; i < NTASKS; i++){
    runningTasks[i] = NULL;
    sleepTasks[i] = 0;
    stateTasks[i] = READY;
    timeTasks[i] = 0;
  }
  runningTasks[T1] = Task1;
  runningTasks[T2] = Task2;
  runningTasks[T3] = Task3;
  runningTasks[NTASKS - ACTUAL_N_TASKS] = schedule_sync;
}
/**
 * loop()
 * @brief loop function that runs tasks inside the runningTasks array.
 * @param runningTasks this is now a struct that has its own variables.
 * @param arg_ptr a pointer to a task.
 * @param state state of runningTasks
 * @param n_times number of times
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
 * @param sleepTasks set to t to sleep for t period.
 * @param stateTasks set to SLEEPING to change the currentTasks state to SLEEPING.
 */
void sleep_474(long t){
  sleepTasks[currentTasks] = t;
  stateTasks[currentTasks] = SLEEPING;
}
/**
 * schedule_sync()
 * @brief This function synchronizes the time for sleeping and running time.
 *        It updates remaining sleep time, wakes up sleeping tasks, and change
 *        It changes the corresponding state to READY and resets flag at the end.
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
 * @brief with a prescale of 8, it converts the frequency to N.
 * @param PRE_SCALE OCR3A
 * @param TCNT4_COUNTER TCNT4 counter sets to 0
 * @param freq frequency from melody[]
 */
void set_OC3A_freq(uint32_t freq) {
  PRE_SCALE = freq == 0 ? 0 : 2000000 / (2 * freq);
  //Serial.println(PRE_SCALE_SPEAKER);
  TCNT4_COUNTER = 0;
}
/**
 * Task1()
 * @brief LED turns on for 250ms and off for 750ms
 * @param LED_PORT PORTL
 * @param PIN47 Actualy PIN number on ATMEGA
 * @param LED_ON 250ms on time
 * @param LED_OFF 750ms off time
 * @param LED_TOTAL 1000ms after one second, time set back to zero.
 * @param sleep_474 a function that takes a long integer and set sleeping time and state of the current task.
 */
void Task1(){

  if(timeTasks[currentTasks] < (1 * LED_DURATION) + 1) {
    // Serial.println(timeTasks[currentTasks]);
    LED_PORT |= PIN47;
    sleep_474(LED_ON); // 250ms
    return;
  }
  
  if(timeTasks[currentTasks] < (4 * LED_DURATION) + 1) {
    // Serial.println("hello");
    // Serial.println(timeTasks[currentTasks]);
    LED_PORT &= ~(PIN47);
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
 * @param sleep_474 a function that makes a task sleep for a set period of time.
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
 * Task2()
 * @brief It counts up and displays on 4bit 7-segment.
 * @param digits 0 = 1's 1 = 10's 2 = 100's 3 = 1000's
 * @param convert converts count and places in array to display.
 * @param disp pointer that stores the addresses of seven_seg_counting.
 * @param display_segment takes a pointer pointing to the addresses of seven_seg_counting and displays the number.
 * @param count increments by one. used for count displayed on 7-seg.
 */
void Task3() {
  static int count;
  static int digits[4];
  static int displayStates[4] = {DS1, DS2, DS3, DS4};

  // take digits out of count
  convert(digits, count);

  // display count on the 7seg display
  for (int h = 0; h < 5; h++) {
    for (int i = 0; i < 4; i++) {
      if ((timeTasks[currentTasks] / 5) >= (4 * h) + i && (timeTasks[currentTasks] / 5) < (4 * h) + (i + 1)) {
        // turn 7seg & specified digit on
        PORTB = displayStates[i];
        byte *disp = seven_seg_counting[digits[i]];
        display_segment(disp);
        sleep_474(2);
        return;
      }
    }
  }

  // reset
  if (timeTasks[currentTasks] >= 100) {
    timeTasks[currentTasks] = 0;
    count = count == 9999 ? 0 : count + 1;
  }
}
/**
 * convert(int * digits, int temp)
 * @brief this function takes a number and computes each bit field for 7-seg display.
 * @param temp the number to be computed.
 * @param digits 0 = 1's 1 = 10's 2 = 100's 3 = 1000's
 */
void convert(int * digits, int count) {
	digits[0] = count % 10;
	digits[1] = (count / 10) % 10;
	digits[2] = (count / 100) % 10;
	digits[3] = (count / 1000) % 10;
}
/**
 * display_segment(byte temp[7])
 * @brief This functions takes a byte array from Task4_1 and Task4_2 and displays the corresponding numbers on 7-seg.
 * @param data an int variable that is added by all 7 elements of the byte array to display the number on 7-seg.
 */
void display_segment(byte temp[7]) {
  int data = 0;
  for (int i = 0; i < 7; i++) {
    data |= (temp[i] << i);
  }
  PORTA = data;
}
/**
 * LED_setup()
 * @brief LED setup
 * @param PIN47 activate the actual PIN47 on ATMEGA
 */
void LED_setup(){
  // Set output pin for LED
  LED_DDR |= PIN47;  
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

  // Initialize system registers to zero.
  SYSTEM_REG_3A = 0;
  SYSTEM_REG_3B = 0;

  // Enable compare output mode with TCCR3A 
  // because TCCRnB does not have compare output mode.
  SYSTEM_REG_3A |= (0 << COM3A1) | (1 << COM3A0);

  // Enable Waveform Generation Mode (set WGM32 to 1)
  SYSTEM_REG_3A |= (0 << WGM31) | (0 << WGM30);
  SYSTEM_REG_3B |= (0 << WGM33) | (1 << WGM32);

  // Set the precale to 8 
  SYSTEM_REG_3B |= (0 << CS32) | (1 << CS31) | (0 << CS30);
  PRE_SCALE = 0;
  TCNT4_COUNTER = 0;

  // Set speaker output
  SPEAKER_DDRE |= PIN5; // Enable PIN 5(PE3)
  SPEAKER_PORTE |= PIN5; // Set PORTE to output mode.

}
/**
 * seven_segment_setup()
 * @brief 7 segment display setup 
 * @param PIN29 Each PIN represents the physical PIN number on ATMEGA 2560. 
 */
void seven_segment_setup() {
  // set up 7-seg outputs
  DDRA |= PIN29 | PIN28 | PIN27 | PIN26 | PIN25 | PIN24 | PIN23 | PIN22;
  // set up digit selectors
  DDRB |= PIN13 | PIN12 | PIN11 | PIN10;
  PORTB |= PIN13 | PIN12 | PIN11 | PIN10;
}
/**
 * interrupt_setup()
 * @brief hardware timer is employed to generate an interrupt for each tick (2ms)
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

  // Enable compare output mode with TCCR3A 
  // because TCCRnB does not have compare output mode.
  TCCR4A |= (0 << COM4A1) | (1 << COM4A0);

  // Enable Waveform Generation Mode (set WGM42 to 1)
  TCCR4A |= (0 << WGM41) | (0 << WGM40);
  TCCR4B |= (0 << WGM43) | (1 << WGM42);

  // enable interrupt on counter 4A (output compare A match interrupt)
  TIMSK4 |= (1 << OCIE4A);

  // Set the precale to 1 
  TCCR4B |= (0 << CS42) | (0 << CS41) | (1 << CS40);
  OCR4A = 32000;
  TCNT3H = 0;

  // so-called "master interrupt" This needs to be enabled to be able to use interrupt
  SREG |= (1<<7);
}