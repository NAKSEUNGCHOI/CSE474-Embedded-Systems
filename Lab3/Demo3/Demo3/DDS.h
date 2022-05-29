/**
 *   @file   DDS.h
 *   @author Nakseung Choi 1572578
 *   @date   21-May-2022
 *   @brief  This is a header file for Demo3.ino
 */

// 16-bit timer/counter set up for speaker
#define SYSTEM_REG_3A TCCR3A
#define SYSTEM_REG_3B TCCR3B
#define TCNT4_COUNTER TCNT4
#define PRE_SCALE OCR3A
#define SPEAKER_DDRE DDRE
#define SPEAKER_PORTE PORTE

// set up for LEDs
#define LED_DDR DDRL
#define LED_PORT PORTL
#define PIN47 DDL2
#define LED_ON 250
#define LED_OFF 750
#define LED_TOTAL 1000

// durations for LED, tones, sleep, and time counting.
#define LED_DURATION 250
#define N_FLASH 3
#define TONE_DURATION 200
#define SLEEP_DURATION 4000
#define TIME_COUNTING (NTONE * TONE_DURATION) + SLEEP_DURATION

// frequencies for tones
#define NOTE_D4 293
#define NOTE_E4 329
#define NOTE_C4 261
#define NOTE_C3 130
#define NOTE_G3 196
#define NOTE_R 0
#define NTONE 13
int melody[] = { NOTE_D4, NOTE_R, NOTE_E4, NOTE_R, NOTE_C4, NOTE_R, NOTE_C3,
                 NOTE_R, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_R };

// number of tasks
#define NTASKS 10
#define ACTUAL_N_TASKS 2
#define TIMING 2

typedef enum{
  READY, 
  RUNNING, 
  SLEEPING,
  DEAD
}STATE;
typedef enum flagState{
  PENDING, 
  DONE
}FLAG;

typedef struct TCB{
  char name[20];
  void (*arg_ptr)();
  int id;
  int n_times;
  int sleepytime;
  int time;
  STATE state;
}tcb;

/* 
 * Function prototypes
*/
void sleep_474(long t);
void schedule_sync();
void copy_tcb(tcb * dst, tcb * src);
void task_self_start(tcb * task);
void task_load(void * arg_ptr(), const char * name);
tcb * find_dead_task(const char * name);
void set_OC3A_freq(uint32_t freq);
void Task1();
void Task2();
void LED_setup();
void speaker_setup();
void interrupt_setup();