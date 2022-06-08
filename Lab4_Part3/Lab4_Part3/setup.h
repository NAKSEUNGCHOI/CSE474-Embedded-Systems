/**
 * @file setup.h
 * @author Nakseung Choi 1572578
 * @brief header file for Lab4_Part1.ino
 * @date May-28-2022
 */

// LED setup
#define LED_DDRL DDRL
#define LED_PORTL PORTL
#define PIN47 1 << DDL2

//speaker setup
#define SYSTEM_REG_3A TCCR3A
#define SYSTEM_REG_3B TCCR3B
#define TCNT4_COUNTER TCNT4H
#define PRE_SCALE OCR3A
#define SPEAKER_DDRE DDRE
#define SPEAKER_PORTE PORTE
#define PIN5 1 << DDE3

// durations
#define TONE_DURATION 200
#define PAUSE_DURATION 1500

// Frequencies for notes
#define NOTE_D4 293
#define NOTE_E4 329
#define NOTE_C4 261
#define NOTE_C3 130
#define NOTE_G3 196
#define NOTE_R 0
int melody[] = { NOTE_D4, NOTE_R, NOTE_E4, NOTE_R, NOTE_C4, NOTE_R, NOTE_C3,
                 NOTE_R, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_G3, NOTE_R };

#define NPLAY 3
#define NTONE 13
#define RANDOM 512
#define NFFTS 5



/**
 * Function Pointers
 * Detailed descriptions are written in the main file.
 */
void set_OC3A_freq(uint32_t freq);
void TaskExternalLED( void *pvParameters );
void TaskSpeaker( void *pvParameters );
void LED_setup();
void speaker_setup();
void RT3p0();
void RT3p1( void *pvParameters );
void RT4p0( void *pvParameters );

