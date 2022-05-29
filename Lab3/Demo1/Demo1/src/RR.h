// 16-bit timer/counter set up
#define SYSTEM_REG_A TCCR3A
#define SYSTEM_REG_B TCCR3B
#define TCNT4_COUNTER TCNT4
#define PRE_SCALE OCR3A
#define LED_DDR DDRL
#define LED_PORT PORTL
#define SPEAKER_DDRE DDRE
#define SPEAKER_PORTE PORTE

// durations for LED, tones, sleep, and time counting.
#define LED_DURATION 250
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

/* Function prototypes
 * set_OC3A_freq : takes a frequency and convert it into N to play the tone.
 * Task1() turns on a LED for 250ms, turns off for 750ms and repeat.
 * Task2() plays 13 different tones for 250ms and stops for 4 seconds and plays agin. 
*/
void set_OC3A_freq(uint32_t freq);
void Task1();
void Task2();