/**
 *   @file   SRRI4.h
 *   @author Nakseung Choi 1572578 Kejin Li 1978130
 *   @date   21-May-2022
 *   @brief  header file for Demo4.ino
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
#define PIN47 1 << DDL2
#define LED_ON 250
#define LED_OFF 750
#define LED_TOTAL 1000

//Speaker setup
#define PIN5 1 << DDE3

// durations for LED, tones, sleep, and time counting.
#define LED_DURATION 250
#define N_FLASH 3
#define TONE_DURATION 200
#define SLEEP_DURATION 4000
#define TIME_COUNTING (NTONE * TONE_DURATION) + SLEEP_DURATION

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
byte seven_seg_counting[10][7] = { { 1,1,1,1,1,1,0 },  // = 0
                                   { 0,1,1,0,0,0,0 },  // = 1
                                   { 1,1,0,1,1,0,1 },  // = 2
                                   { 1,1,1,1,0,0,1 },  // = 3
                                   { 0,1,1,0,0,1,1 },  // = 4
                                   { 1,0,1,1,0,1,1 },  // = 5
                                   { 1,0,1,1,1,1,1 },  // = 6
                                   { 1,1,1,0,0,0,0 },  // = 7
                                   { 1,1,1,1,1,1,1 },  // = 8
                                   { 1,1,1,0,0,1,1 }   // = 9
                                   };


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
#define T1 0
#define T2 1
#define T3 2

enum state{
  READY, 
  RUNNING, 
  SLEEPING
};
enum flag_state{
  PENDING, 
  DONE
};

/* 
 * Function prototypes
 * Detail explanations for all of the functions are in the main Demo4.ino
*/
void sleep_474(long t);
void schedule_sync();
void interrupt_setup();
void speaker_setup();
void seven_segment_setup();
void LED_setup();
void set_OC3A_freq(uint32_t freq);
void Task1();
void Task2();
void Task3();
void displaySetup();
void convert(int * digits, int count);
void display_segment(byte temp[7]);