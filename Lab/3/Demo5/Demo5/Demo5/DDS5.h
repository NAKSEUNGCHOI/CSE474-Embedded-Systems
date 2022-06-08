/**
 *   @file   DDS5.h
 *   @author Nakseung Choi 1572578 Kejin Li 1978130
 *   @date   22-May-2022
 *   @brief  header file for Demo5.ino
 */

// 16-bit timer/counter set up for speaker
#define SYSTEM_REG_3A TCCR3A
#define SYSTEM_REG_3B TCCR3B
#define TCNT4_COUNTER TCNT4H
#define PRE_SCALE OCR3A
#define SPEAKER_DDRE DDRE
#define SPEAKER_PORTE PORTE
#define PIN5 1 << DDE3

// durations tones, sleep, and time counting.
#define TONE_DURATION 200
#define SLEEP_DURATION 4000 
#define TIME_COUNTING (NTONE * TONE_DURATION) + SLEEP_DURATION

//Frequencies for each note stored in melody[]
#define NOTE_D4 293
#define NOTE_E4 329
#define NOTE_C4 261
#define NOTE_C3 130
#define NOTE_G3 196
#define NOTE_R 0
#define NTONE 13
#define NTASKS 10
#define TIMING 2
int melody[] = { NOTE_D4, NOTE_R, NOTE_E4, NOTE_R, NOTE_C4, NOTE_R, NOTE_C3,
                 NOTE_R, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_R };

// 4bit 7-segment setup
#define PIN13 1 << DDB7 
#define PIN12 1 << DDB6 
#define PIN11 1 << DDB5 
#define PIN10 1 << DDB4
#define PIN22 1 << DDA0 
#define PIN23 1 << DDA1 
#define PIN24 1 << DDA2 
#define PIN25 1 << DDA3
#define PIN26 1 << DDA4 
#define PIN27 1 << DDA5 
#define PIN28 1 << DDA6 
#define PIN29 1 << DDA7
#define DS1 0b1110 << 4
#define DS2 0b1101 << 4
#define DS3 0b1011 << 4
#define DS4 0b0111 << 4
byte seven_seg_couting[10][7] = { { 1,1,1,1,1,1,0 },  
                                  { 0,1,1,0,0,0,0 },
                                  { 1,1,0,1,1,0,1 },  
                                  { 1,1,1,1,0,0,1 }, 
                                  { 0,1,1,0,0,1,1 },  
                                  { 1,0,1,1,0,1,1 },  
                                  { 1,0,1,1,1,1,1 },  
                                  { 1,1,1,0,0,0,0 },  
                                  { 1,1,1,1,1,1,1 },  
                                  { 1,1,1,0,0,1,1 }   
                                  };

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

/**
 * @brief Struct to hold all of a Tasks running state
 * @author Nakseung Choi 1572578
 */
typedef struct TCB {
  int id;
  int n_times;
  int sleepytime;
  int time;
  char name[20];
  void (*arg_ptr)();
  STATE state;
} tcb;

/* 
 * Function prototypes
 * Detail explanations for all of the functions are in the main Demo5.ino.
*/
void sleep_474(long t);
void schedule_sync();
void copy_tcb(tcb * running, tcb * dead);
void task_self_quit();
void start_task(tcb * task);
void load_up_task(void (*arg_ptr)(), const char *);
tcb * find_dead_task(const char * name);
void setOC4AFreq(uint32_t freq);
void Task2();
void Task4();
void Task4_1();
void Task4_2();
void interruptSetup();
void speakerSetup();
void displaySetup();
void ledSetup();
void DDSSetup();