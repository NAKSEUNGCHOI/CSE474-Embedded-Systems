/**
 *   @file   Demo3.ino
 *   @author Nakseung Choi 1572578 Kejin Li 1978130
 *   @date   21-May-2022
 *   @brief  Task1 and Task2 concurrently using Data Driven Scheduler (DDS.)
 *   
 *   Detailed Description: In setup function, LED, speaker, interrupt, and DDS function are called for initial setup.
 *                         Struct tcb is initialized in DDS.h file. task_load loads up tasks and initializes all variables
 *                         in tcb. start_task starts a task and this task gets updated as dead_tasks.
 *                         As it goes through loop() function, (*runningTasks[i].arg_ptr)() calls each Task 1 and 2, and
 *                         both of them start to run concurrently. Sleep function, which takes a number of sleep time,
 *                         is employed to make a task go sleep for a certain period of time. In schedule sync function, 
 *                         the amount of the time that a task sleeps is synchronized by sleepTasks decrementing by -2. 
 *                         Similary, timeTasks increments by 2 (meaning 2 ms) because hardware timer (interrupt) is set to occur 
 *                         every 2 ms.   
 */

#include "DDS.h"

int reset = 0;
tcb runningTasks[NTASKS];
tcb deadTasks[NTASKS];
volatile int id;
volatile int currentTasks;
volatile FLAG sFlag;
int little_comp;

/**
 * setup()
 * @brief Initialize all registers for LED, speaker, and interrupt as well as Tasks being stored in an array called running Tasks.
 * @param Serial enable Serial.print at 9600.
 * @param LED_setup A function that contains all LED setups.
 * @param speaker_setup A function that contains all speaker setups.
 * @param interrupt_setup A function that contains all interrupt setups. 
 * @param DDS_setup  A function that contains all DDS setups.
 * @param task_load This loads up tasks.
 * @param find_dead_task a function that finds a dead task to run it.
 * @param start_task a function that takes a function named find_dead_task where it goes look for a dead task to run.
 */
void setup() {
  Serial.begin(9600);
  id = 0;
  LED_setup();
  speaker_setup();
  interrupt_setup();
  DDS_setup();

  task_load(Task1, "Task1");
  task_load(Task2, "Task2");
  task_load(schedule_sync, "schedule_sync");
  start_task(find_dead_task("Task1"));
  start_task(find_dead_task("Task2"));
  start_task(find_dead_task("schedule_sync"));
  

}
/**
 * loop()
 * @brief loop function that runs whats inside infinitely.
 * @param runningTasks this is now a struct that has its own variables.
 * @param arg_ptr a pointer to a task.
 * @param state state of runningTasks
 * @param n_times number of times
 * @param currentTasks keeping track of current task running.
 */
void loop() {
  for (int i = 0; i < NTASKS; i++) {
    if (runningTasks[i].arg_ptr != NULL && runningTasks[i].state == READY) {
      runningTasks[i].state = RUNNING;
      currentTasks = i;
      runningTasks[i].n_times++;
      (*(runningTasks[i].arg_ptr))();
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
 * task_load(void (*arg_ptr)(), const char * name)
 * @brief A function that takes void function pointer and a char pointer.
 *        It loads up a new task and finds the next stack in the dead task array.
 * @param sFlag Set to DONE, meaning triggered.
 */
void task_load(void (*arg_ptr)(), const char * name) {

  int i;
  for (i = 0; i < NTASKS; i++) {
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
  if(task == NULL || task->state !=DEAD){
    return;
  }
  int count;
  for(count = 0; count < NTASKS; count++){
    if(runningTasks[count].arg_ptr == NULL){
      break;
    }
  }

  //if there is no space, do nothing.
  if(count == NTASKS){
    return;
  }

  copy_tcb(&(runningTasks[count]), task);
  runningTasks[count].state = READY;
  task->state = DEAD;
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
  int count;
  for(count = 0; count < NTASKS; count++){
    if(strcmp(name, deadTasks[count].name) == 0){
      break;
    }
  }
  if(count == NTASKS){
    return NULL;
  }
  return &(deadTasks[count]);
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
  int count;
  for(count = 0; count < NTASKS; count++){
    if(deadTasks[count].arg_ptr == NULL){
      break;
    }
  }
  if(count == NTASKS){
    return;
  }
  copy_tcb(&(deadTasks[count]), &(runningTasks[currentTasks]));
  deadTasks[count].state = DEAD;
  runningTasks[currentTasks].state = DEAD;
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
    if(runningTasks[i].state == SLEEPING){
      //Serial.println(sleepTasks[i]);
      runningTasks[i].sleepytime -= TIMING;
      if(runningTasks[i].sleepytime < ACTUAL_N_TASKS){
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
 * @param PIN47 DDL2
 * @param LED_PORT PORTL
 * @param timeTasks when timeTasks[currentTasks] >= 1000ms, timeTasks[currentTasks] is set back to 0.
 */
void Task1(){

  if(reset){
    LED_PORT |= 1 << PIN47;
    runningTasks[currentTasks].time = 0;
    reset = 0;
    return;
  }

  if(runningTasks[currentTasks].time < (1 * LED_DURATION) + 1) {
    // Serial.println(timeTasks[currentTasks]);
    LED_PORT |= (1 << PIN47);
    sleep_474(LED_ON); // 250ms
    return;
  }
  
  if(runningTasks[currentTasks].time < (4 * LED_DURATION) + 1) {
    // Serial.println("hello");
    // Serial.println(timeTasks[currentTasks]);
    LED_PORT &= ~(1 << PIN47);
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
 * @param set_OC3A_freq a function that does calculation for a tone to play at a certain frequency.
 */
void Task2(){

if(reset){
  set_OC3A_freq(0);
  runningTasks[currentTasks].time = 0;
  reset = 0;
  return;
}

  for(int i = 0; i < NTONE; i++){
    if(runningTasks[currentTasks].time >= i * TONE_DURATION && runningTasks[currentTasks].time < (i + 1) * TONE_DURATION){
      set_OC3A_freq(melody[i]);
      sleep_474(TONE_DURATION);
      return;
    }
  }
  
  if(runningTasks[currentTasks].time < TIME_COUNTING){
      set_OC3A_freq(0);
      sleep_474(SLEEP_DURATION);
  }

  for(int i = 0; i < NTONE; i++){
    if(runningTasks[currentTasks].time >= (TIME_COUNTING + ((unsigned long) i) * TONE_DURATION) &&
       runningTasks[currentTasks].time < (TIME_COUNTING + ((unsigned long) i + 1) * TONE_DURATION)){
      set_OC3A_freq(melody[i]);
      sleep_474(TONE_DURATION);
    }
  }

  if(runningTasks[currentTasks].time >= (TIME_COUNTING + NTONE * TONE_DURATION)){
    runningTasks[currentTasks].time = 0;
  }

}
/**
 * LED_setup()
 * @brief LED set-up
 * @param LED_DDR DDRL
 */
void LED_setup(){
  // Set output pin for LED
  LED_DDR |= 1 << PIN47;  
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
  SPEAKER_DDRE |= 1 << DDE3; // Enable PIN 5(PE3)
  SPEAKER_PORTE |= 1 << DDE3; // Set PORTE to output mode.

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

  // so-called "master interrupt" This needs to be enabled to be able to use interrupt
  SREG |= (1<<7);
}
/**
 * DDS_setup()
 * @brief This function initializes two tcbs named runningTasks and deadTasks and sets to 0 or NULL.
 * @param runningTasks tasks that are going to be run are stored in this tcb
 * @param deadTasks tasks that have been run are going to be stored in this tcb.
 */
void DDS_setup(){
  for(int i = 0; i < NTASKS; i++){
    runningTasks[i].id = 0;
    runningTasks[i].arg_ptr = NULL;
    runningTasks[i].n_times = 0;
    runningTasks[i].sleepytime = 0;
    runningTasks[i].time = 0;
    deadTasks[i].id = 0;
    deadTasks[i].arg_ptr = NULL;
    deadTasks[i].n_times = 0;
    deadTasks[i].sleepytime = 0;
    deadTasks[i].time = 0;
  }
}