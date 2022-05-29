/**
 *   @file   DDS6.h
 *   @author Nakseung Choi 1572578 Kejin Lin 1978130
 *   @date   23-May-2022
 *   @brief  Header file for Demo6.ino
 */

// 16-bit timer/counter set up for speaker
#define SYSTEM_REG_3A TCCR3A
#define SYSTEM_REG_3B TCCR3B
#define TCNT4_COUNTER TCNT4H
#define PRE_SCALE OCR3A
#define SPEAKER_DDRE DDRE
#define SPEAKER_PORTE PORTE
#define PIN5 1 << DDE3

// set up for LEDs
#define LED_DDR DDRL
#define LED_PORT PORTL
#define PIN47 1 << DDL2
#define LED_ON 250
#define LED_OFF 750
#define LED_TOTAL 1000

// durations tones, sleep, and time counting.
#define LED_DURATION 250
#define TONE_DURATION 200
#define SLEEP_DURATION 4000 
#define TASK2_SLEEP_DURATION 3000
#define SMILE_DURATION 2000
#define TASK2_DURATION (2 * (NTONE * TONE_DURATION)) + SLEEP_DURATION
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
byte seven_seg_counting[10][7] = { { 1,1,1,1,1,1,0 },  
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
                                  
byte smile[4][7] = { { 0,0,0,0,1,0,0 },  
                     { 1,1,0,1,0,1,0 },  
                     { 1,1,0,1,0,1,0 },  
                     { 0,0,1,0,0,0,0 },  
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
 * @brief Struct holding all of a Tasks running state
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
void set_OC3A_freq(uint32_t freq);
void display_segment(byte temp[7]);
void convert(int * digits, int temp);
void halt_me(int reset1);
void Task1();
void Task2();
void Task4_2();
void Task5();
void Task5_1();
void LED_setup();
void interruptSetup();
void speakerSetup();
void displaySetup();
void ledSetup();
void DDSSetup();