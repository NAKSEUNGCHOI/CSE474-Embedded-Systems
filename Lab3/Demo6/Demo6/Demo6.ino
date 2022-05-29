/**
 *   @file   Demo6.ino
 *   @author Nakseung Choi 1572578 Kejin Lin 1978130
 *   @date   23-May-2022
 *   @brief  Running Task 5 using Data Driven Scheduler (DDS.)
 *   
 *   Detailed Description: Task5() is the supervisor of all other tasks. It starts and stops other tasks.
 *   Task1() runs all the time. Task2() runs at the start but stops after playing two cycles.
 *   Task4_2() starts to countdown for 3000ms and the countdown is displayed on 7-seg.
 *   Task5_1() display a smile for 2000ms. Lastly, all tasks stop except for Task1(). 
 */

#include "DDS6.h"

int reset = 0;

tcb runningTasks[NTASKS];
tcb deadTasks[NTASKS];
volatile int id;
volatile int currentTasks;
volatile FLAG sFlag;
volatile int currentFreq;
volatile int little_comp;

/**
 * setup()
 * @brief Initialize all registers for LED, speaker, and interrupt as well as Tasks being stored in an array called running Tasks.
 * @param Serial enable Serial.print at 9600.
 * @param speaker_setup A function that enables all speaker setups.
 * @param interrupt_setup A function that enables all interrupt setups. 
 * @param seven_segment_setup A function that enables all GI/O PINs for 4-bit seven segment display.
 * @param DDS_setup  A function that enables all DDS setups.
 * @param LED_setup LED setup
 * @param load_up_task This loads up tasks.
 * @param find_dead_task a function that finds a dead task to run it.
 * @param start_task a function that takes a function named find_dead_task where it goes look for a dead task to run.
 */
void setup(){
  id = 0;
  Serial.begin(9600);
  interrupt_setup();
  speaker_setup();
  seven_segment_setup();
  LED_setup();
  DDS_setup();

  load_up_task(Task5, "Task5");
  load_up_task(schedule_sync, "schedule_sync");
  start_task(find_dead_task("Task5"));
  start_task(find_dead_task("schedule_sync"));
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
  for (int i = 0; i < NTASKS; i++){
    if (runningTasks[i].arg_ptr != NULL && runningTasks[i].state == READY){
      runningTasks[i].state = RUNNING;
      currentTasks = i;
      (*(runningTasks[i].arg_ptr))();
      runningTasks[i].n_times++;
      if (runningTasks[i].state == RUNNING){
        runningTasks[i].state = READY;
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
 * load_up_task(void (*arg_ptr)(), const char * name)
 * @brief A function that takes void function pointer and a char pointer.
 *        It loads up a new task and finds the next stack in the dead task array.
 * @param sFlag Set to DONE, meaning triggered.
 */
 void load_up_task(void (*arg_ptr)(), const char * name){
 
  int i;
  for (i = 0; i < NTASKS; i++){
    if (deadTasks[i].arg_ptr == NULL){
    break;
    }
  }

  // if there's no space, don't do anything
  if (i == NTASKS){
    return;
  }

  strcpy(deadTasks[i].name, name);
  deadTasks[i].arg_ptr = arg_ptr;
  deadTasks[i].id = id;
  id++;
  deadTasks[i].n_times = 0;
  deadTasks[i].sleepytime = 0;
  deadTasks[i].time = 0;
  deadTasks[i].state = DEAD;
}
/**
 * start_task(tcb * task)
 * @brief A function that takes void function pointer and a char pointer.
 *        It loads up a new task and finds the next stack in the dead task array.
 * @param sFlag Set to DONE, meaning triggered.
 */
void start_task(tcb * task){
  if (task == NULL || task->state != DEAD){
    return;
  }
  
  int i;
  for (i = 0; i < NTASKS; i++){
    if (runningTasks[i].arg_ptr == NULL){
      break;
    }
  }

  if (i == NTASKS){
    return;
  }
  copy_tcb(&(runningTasks[i]), task);
  runningTasks[i].state = READY;
  task->state = DEAD;
  task->arg_ptr = NULL;
}
/**
 * find_dead_task(const char * name)
 * @brief A function that takes char pointer which points to the name of tasks like Task1 or Task2.
 *        This function finds a dead task and return deadTasks[count].
 * @param sFlag Set to DONE, meaning triggered.
 * @param strcmp compare char pointer with deadTasks[count].name and the deadTasks will be returned.
 * @return &(deadTasks[count])
 */
tcb * find_dead_task(const char * name){
  int i;
  for (i = 0; i < NTASKS; i++) {
    if (strcmp(name, deadTasks[i].name) == 0){
      break;
    }
  }

  if (i == NTASKS){
    return NULL;
  }
  return &(deadTasks[i]);
}
/**
 * copy_tcb(tcb * running, tcb * dead)
 * @brief This function just points to variable to update runningTasks to deadTasks. 
 */
void copy_tcb(tcb * running, tcb * dead){
  running->arg_ptr = dead->arg_ptr;
  strcpy(running->name, dead->name);
  running->id = dead->id;
  running->n_times = dead->n_times;
  running->state = dead->state;
  running->time = dead->time;
  running->sleepytime = dead->sleepytime;
}
/**
 * task_self_quit()
 * @brief This function sets the task state to DEAD and update it to the dead task array. 
 */
void task_self_quit(){

  int i;
  for (i = 0; i < NTASKS; i++) {
    if (deadTasks[i].arg_ptr == NULL){
      break;
    }
  }

  if (i == NTASKS){
    return;
  }
  copy_tcb(&(deadTasks[i]), &(runningTasks[currentTasks]));
  deadTasks[i].state = DEAD;
  runningTasks[currentTasks].state = DEAD;
  runningTasks[currentTasks].arg_ptr = NULL;
}
/**
 * sleep_474(long t)
 * @brief this function takes a long t and set sleepTasks[currentTasks]'s time to t and set state to SLEEPING to sleep.
 * @param sleepTasks set to t to sleep for t period.
 * @param stateTasks set to SLEEPING to change the currentTasks state to SLEEPING.
 */
void sleep_474(long t){
  runningTasks[currentTasks].sleepytime = t;
  runningTasks[currentTasks].state = SLEEPING;
  return;
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
  for (int i = 0; i < NTASKS; i++){
    if (runningTasks[i].state == SLEEPING){
      runningTasks[i].sleepytime -= TIMING;
      if (runningTasks[i].sleepytime < TIMING){
        runningTasks[i].sleepytime = 0;
        runningTasks[i].state = READY;
      }
    }
    runningTasks[i].time += TIMING;
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
void set_OC3A_freq(uint32_t freq){
  PRE_SCALE = freq == 0 ? 0 : 2000000 / (2 * freq);
  TCNT4_COUNTER = 0;
}
/**
 * Task1()
 * @brief LED turns on for 250ms and off for 750ms
 * @param sleep_474 a function that makes a task sleeps for a certain period of time.
 * @param LED_ON 250ms
 * @param LED_OFF 750ms
 * @param PIN47 1 << DDL2
 * @param LED_PORT PORTL
 * @param timeTasks when timeTasks[currentTasks] >= 1000ms, timeTasks[currentTasks] is set back to 0.
 */
void Task1(){

  if(reset){
    LED_PORT |= PIN47;
    runningTasks[currentTasks].time = 0;
    reset = 0;
    return;
  }

  if(runningTasks[currentTasks].time < (1 * LED_DURATION) + 1) {
    // Serial.println(timeTasks[currentTasks]);
    LED_PORT |= PIN47;
    sleep_474(LED_ON); // 250ms
    return;
  }
  
  if(runningTasks[currentTasks].time < (4 * LED_DURATION) + 1) {
    // Serial.println("hello");
    // Serial.println(timeTasks[currentTasks]);
    LED_PORT &= ~(PIN47);
    sleep_474(LED_OFF); // 750ms
    return;
  }

  if(runningTasks[currentTasks].time >= LED_TOTAL){
    runningTasks[currentTasks].time = 0;
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
 * @param TIME_COUNTING (NTONE * TONE_DURATION) + SLEEP_DURATION = (13*200)+4000 = 6600ms
 * @param set_OC3A_freq a function that does calculation for a tone to play at a certain frequency.
 * @param task_self_quit turn the task off.
 */
void Task2(){

if(reset){
  set_OC3A_freq(0);
  runningTasks[currentTasks].time = 0;
  reset = 0;
  return;
}

  for(int i = 0; i < NTONE; i++){
    if(runningTasks[currentTasks].time >= (i * TONE_DURATION) && 
       runningTasks[currentTasks].time < ((i + 1) * TONE_DURATION) ){
      set_OC3A_freq(melody[i]);
      sleep_474(TONE_DURATION);
      return;
    }
  }
  
  if(runningTasks[currentTasks].time < TIME_COUNTING){
      set_OC3A_freq(0);
      sleep_474(SLEEP_DURATION);
      return;
  }

  for(int i = 0; i < NTONE; i++){
    if(runningTasks[currentTasks].time >= (TIME_COUNTING + ((unsigned long) i) * TONE_DURATION) &&
       runningTasks[currentTasks].time < (TIME_COUNTING + ((unsigned long) i + 1) * TONE_DURATION)){
      set_OC3A_freq(melody[i]);
      sleep_474(TONE_DURATION);
      return;
    }
  }

  if(runningTasks[currentTasks].time >= (TIME_COUNTING + (unsigned long)NTONE * TONE_DURATION)){
     runningTasks[currentTasks].time = 0;
     task_self_quit();
  }

}
/**
 * Task4_2()
 * @brief This function displays countdown from 30, which is 3 seconds.
 * @param convert In Task4_2(), Instead of taking frequency to display, it takes (3000ms / 100) - count to display the countdown.
 *                This time increments by one and subtract the (3000ms /100) by one every time loop function calls Task4.
 * @param displayStates an int array that stores which bit to display.
 * @param digits an int array that stores the number of each bit field to display on 7 seg.
 * @param disp a byte pointer that stores the addresses of the number of each bit field and goes into display_segment() to display.
 * @param sleep_474 a function that makes a task sleep for a set period of time.
 * @param NTONE a number of tones = 13.
 * @param currentTasks a variable that keeps track of which task is running.
 * @param TONE_DURATION 200ms each tone plays for 200ms
 * @param time it is used to compare with the duration of smile on 7-seg to turn off.
 * @param count Count increments by one every time this function is run, and it subtracts the countdown.
 * @param TASK2_SLEEP_DURATION 3000ms of sleeping time
 * @param task_self_quit turn the task off.
 */
void Task4_2() {
  static int time;
  static int count;
  static int digits[4];
  static int displayStates[4] = {DS1, DS2, DS3, DS4};

  // take digits out of count
  convert(digits, (TASK2_SLEEP_DURATION / 100) - count);

  // display count on the 7seg display
  for (int h = 0; h < 5; h++){
    for (int i = 0; i < 4; i++){
      int pin = 10 + i;
      if ((runningTasks[currentTasks].time / 5) >= (4 * h) + i && (runningTasks[currentTasks].time / 5) < (4 * h) + (i + 1)){
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
  if (runningTasks[currentTasks].time >= 100){
    time += runningTasks[currentTasks].time;
    runningTasks[currentTasks].time = 0;
    count = count == 9999 ? 0 : count + 1;
  }

  if (time >= TASK2_SLEEP_DURATION){
    PORTB = 0xFF;
    time = 0;
    runningTasks[currentTasks].time = 0;
    count = 0;
    task_self_quit();
  }
}
/**
 * Task5()
 * @brief This function supervises all tasks, starts and stops other tasks.
 * @param load_up_task Load up task in order.
 * @param start_task start the task
 * @param TASK2_DURATION (2 * (NTONE * TONE_DURATION)) + SLEEP_DURATION = 2*13*200 + 4000 = 8600ms
 * @param TASK2_SLEEP_DURATION 3000ms of sleeping time.
 * @param SMILE_DURATION display smile on 7-seg for 2000ms. 
 * @param task_self_quit turn the task off.
 * @param currentTasks a variable that keeps track of which task is running.
 */
void Task5(){

  if(runningTasks[currentTasks].n_times == 0){
    load_up_task(Task1, "Task1");
    start_task(find_dead_task("Task1"));
    load_up_task(Task2, "Task2");
    load_up_task(Task4_2, "countdown");
    load_up_task(Task5_1, "smile");
  }

  if(runningTasks[currentTasks].time < TASK2_DURATION){
    start_task(find_dead_task("Task2"));
    sleep_474(TASK2_DURATION);
    return;
  }

  if(runningTasks[currentTasks].time >= TASK2_DURATION &&
     runningTasks[currentTasks].time < (TASK2_DURATION + TASK2_SLEEP_DURATION)){
    start_task(find_dead_task("countdown"));
    sleep_474(TASK2_SLEEP_DURATION);
    return;
  }

  if(runningTasks[currentTasks].time >= (TASK2_DURATION + TASK2_SLEEP_DURATION) &&
      runningTasks[currentTasks].time < ((2 * (TASK2_DURATION)) + TASK2_SLEEP_DURATION) ){
    start_task(find_dead_task("Task2"));
    sleep_474(TASK2_DURATION);
    return;
  }

  if(runningTasks[currentTasks].time >= (2 * (TASK2_DURATION) + TASK2_SLEEP_DURATION) &&
      runningTasks[currentTasks].time < (2 * (TASK2_DURATION) + TASK2_SLEEP_DURATION + SMILE_DURATION)){
    start_task(find_dead_task("smile"));
    sleep_474(SMILE_DURATION);
    return;
  }

  if (runningTasks[currentTasks].time >= (( 2 * (TASK2_DURATION) + TASK2_SLEEP_DURATION + SMILE_DURATION)) ){
    task_self_quit();
    return;
  }
}
/**
 * Task5_1()
 * @brief This function displays smile on 7-segment.
 * @param time it is used to compare with the duration of smile on 7-seg to turn off.
 * @param start_task start the task
 * @param smile an 4x7 2 dimensional array that is employed to display smile.
 * @param TASK2_DURATION (2 * (NTONE * TONE_DURATION)) + SLEEP_DURATION = 2*13*200 + 4000 = 8600ms
 * @param TASK2_SLEEP_DURATION 3000ms of sleeping time.
 * @param SMILE_DURATION display smile on 7-seg for 2000ms. 
 * @param task_self_quit turn the task off.
 * @param currentTasks a variable that keeps track of which task is running.
 * @param displayStates an int array that stores which bit to display.
 * @param digits an int array that stores the number of each bit field to display on 7 seg.
 * @param disp a byte pointer that stores the addresses of the number of each bit field and goes into display_segment() to display.
 * @param sleep_474 a function that makes a task sleep for a set period of time.
 */
void Task5_1() {
  static int time;
  static int displayStates[4] = {DS1, DS2, DS3, DS4};

  // display count on the 7seg display
  for (int h = 0; h < 5; h++) {
    for (int i = 0; i < 4; i++) {
      if ((runningTasks[currentTasks].time / 5) >= (4 * h) + i && (runningTasks[currentTasks].time / 5) < (4 * h) + (i + 1)) {
        // turn 7seg & specified digit on
        PORTB = displayStates[i];
        byte *disp = smile[i];
        display_segment(disp);
        sleep_474(2);
        return;
      }
    }
  }

  // reset
  if (runningTasks[currentTasks].time >= 100) {
    time += runningTasks[currentTasks].time;
    runningTasks[currentTasks].time = 0;
  }
  if (time >= SMILE_DURATION) {
    time = 0;
    PORTB = 0xFF;
    task_self_quit();
  }
}
/**
 * display_segment(byte temp[7])
 * @brief This functions takes a byte array from Task4_1 and Task4_2 and displays the corresponding numbers on 7-seg.
 *        (e.g. take digits out of currentFreq or countdown.)
 * @param data an int variable that is added by all 7 elements of the byte array to display the number on 7-seg.
 */
void display_segment(byte temp[7]){
  int data = 0;
  for (int i = 0; i < 7; i++){
    data |= (temp[i] << i);
  }
  PORTA = data;
}
/**
 * convert(int * digits, int temp)
 * @brief this function takes a number and computes each bit field for 7-seg display.
 * @param temp the number to be computed.
 * @param digits 0 = 1's 1 = 10's 2 = 100's 3 = 1000's
 */
void convert(int * digits, int temp){
	digits[0] = temp % 10;
	digits[1] = (temp / 10) % 10;
	digits[2] = (temp / 100) % 10;
	digits[3] = (temp / 1000) % 10;
}
/**
 * LED_setup()
 * @brief LED set-up
 * @param LED_DDR DDRL
 */
void LED_setup(){
  // Set output pin for LED
  LED_DDR |= PIN47;  
}
/**
 * interrupt_setup()
 * @brief Initialize all registers for interrupts
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

  DDRH |= 1 << DDH3;
  PORTH |= 1 << DDH3;

  // so-called "master interrupt" This needs to be enabled to be able to use interrupt
  SREG |= (1<<7);
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
void seven_segment_setup(){
  // set up 7-seg outputs
  DDRA |= PIN29 | PIN28 | PIN27 | PIN26 | PIN25 | PIN24 | PIN23 | PIN22;
  // set up digit selectors
  DDRB |= PIN13 | PIN12 | PIN11 | PIN10;
  PORTB |= PIN13 | PIN12 | PIN11 | PIN10;
}
/**
 * DDS_setup()
 * @brief This function initializes two tcbs named runningTasks and deadTasks and sets to 0 or NULL.
 * @param runningTasks tasks that are going to be run are stored in this tcb
 * @param deadTasks tasks that have been run are going to be stored in this tcb.
 */
void DDS_setup(){
  // populate task array
  for (int i = 0; i < NTASKS; i++){
    runningTasks[i].id = 0;
    runningTasks[i].n_times = 0;
    runningTasks[i].sleepytime = 0;
    runningTasks[i].time = 0;
    runningTasks[i].arg_ptr = NULL;
    runningTasks[i].state = DEAD;
    deadTasks[i].id = 0;
    deadTasks[i].n_times = 0;
    deadTasks[i].sleepytime = 0;
    deadTasks[i].time = 0;
    deadTasks[i].arg_ptr = NULL;
    deadTasks[i].state = DEAD;
  }
}