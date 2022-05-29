/*
 * University of Washington
 * ECE 474 prof. Vikram Iyer
 * Nakseung Choi 1572578
 * Kejin Li 1978130
 * Lab 2: Digital I/O and Timing of Outputs
 */
#include <avr/io.h>

// Part 1 ///////////////////////////////////////////////
#define PIN47 47
#define PIN48 48
#define PIN49 49
/////////////////////////////////////////////////////////

// part 3.3 ////////////////////////////////////////////
#define NOTE_c 3830 // 261 Hz
#define NOTE_d 3400 // 294 Hz
#define NOTE_e 3038 // 329 Hz
#define NOTE_f 2864 // 349 Hz
#define NOTE_g 2550 // 392 Hz
#define NOTE_a 2272 // 440 Hz
#define NOTE_b 2028 // 493 Hz
#define NOTE_C 1912 // 523 Hz
#define NOTE_R 0

int melody[] = {NOTE_e, NOTE_R, NOTE_d, NOTE_R, NOTE_c, NOTE_R, NOTE_d, NOTE_R, NOTE_e, NOTE_R, NOTE_e, NOTE_R, NOTE_e, NOTE_R, NOTE_d, NOTE_R, 
NOTE_d, NOTE_R, NOTE_d, NOTE_R, NOTE_e, NOTE_R, NOTE_g, NOTE_R, NOTE_g, NOTE_R, NOTE_e, NOTE_R, NOTE_d, NOTE_R, NOTE_c, NOTE_R, NOTE_d, NOTE_R, NOTE_e,
NOTE_R, NOTE_e, NOTE_R, NOTE_e, NOTE_R, NOTE_e, NOTE_R, NOTE_d, NOTE_R, NOTE_d, NOTE_R, NOTE_e, NOTE_R, NOTE_d, NOTE_R, NOTE_c, NOTE_R, NOTE_c};
int note_count = sizeof(melody)/sizeof(melody[0]);
// part 3.3 end /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Part 4 /////////////////////////////////////////////////////////////////////////////////
#define JoyX DDF0
#define JoyY DDF1
#define OP_DECODEMODE  8
#define OP_SCANLIMIT   10
#define OP_SHUTDOWN    11
#define OP_DISPLAYTEST 14
#define OP_INTENSITY   10 

void spiTransfer(volatile byte row, volatile byte data);

byte spidata[2];
int x_map, y_map, x_value, y_value;

int DIN = 12;
int CS =  11;
int CLK = 10;
// part 4 end /////////////////////////////////////////////////////////////////////////////

/// General time Setting ////
int delay_time_part1 = 333; // LEDs delay
int delay_time_part2 = 1000; //Speaker delay
int delay_time_part3 = 16000; // part 3.2 total time
int delay_time_part3_3 = 12000+(note_count*150); // part 3.3 total time
/////////////////////////////

void setup() {
  Serial.begin(9600);
  // Part 1.2 ////////////////////////////////
  pinMode(PIN47, OUTPUT);
  pinMode(PIN48, OUTPUT);
  pinMode(PIN49, OUTPUT);
  ////////////////////////////////////////////

  // Part 1.4.2 //////////////////////////////
  DDRL |= 1 << DDL2; // Bit 2 of DDRL = Pin 47
  DDRL |= 1 << DDL1; // Bit 1 of DDRL = Pin 48
  DDRL |= 1 << DDL0; // Bit 0 of DDRL = Pin 49
  ////////////////////////////////////////////

  // Part 2.4 ////////////////////////////////
  // Initialize them to zero. (IMPORTANT)
  TCCR4A = 0; 
  TCCR4B = 0;
  TCCR4C = 0;
  ////////////////////////////////////////////
  TCCR4A |= (1 << COM4A0); // Enable compare output mode with TCCR4A because TCCRnB does not have compare output mode. 0b01000000
  TCCR4B |= (1 << CS41); //| (1 << CS40); // set the precale to 8. 
  TCCR4B |= (1 << WGM42); // Enable Waveform Generation Mode (set WGM42 to 1)
  TCNT4 = 0;            // counts from 0
  DDRH |= 1 << DDH3; // Enable PIN 6
  PORTH |= 1 << DDH3; // Set portH as an output.
  // Part 2.4 end //////////////////////////////////////////////////////////////////////////////////////////////////////

  // Part 4.1 start ////////////////////////////////////////////////////////////////////////////////////////////////////
  DDRB |= 1 << DDB6 | 1 << DDB5 | 1 << DDB4; // enable pin 10(CLK), pin 11(CS), pin 12(DIN).
  PORTB |= 1 << DDB5; //set CS to HIGH.
  DDRF |= 0 << DDF0 | 0 << DDF1; //Analog Input Setting. (A0 and A1)

  spiTransfer(OP_DISPLAYTEST,0);
  spiTransfer(OP_SCANLIMIT,7);
  spiTransfer(OP_DECODEMODE,0);
  spiTransfer(OP_SHUTDOWN,1);
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
void loop() {
  ////**** Uncomment to run. ****////
  //part_1_2();
  //part_1_4();
  //part_2_4();
  //part_3_1();
  //scheduler_part_3_2();
  //scheduler_part_3_3();
  //scheduler_part_4();
  ///////////////////////////////////
}
void part_1_2(){
  //Each LED flashes for 333ms, total of 1 second.
  digitalWrite(PIN49, LOW); 
  digitalWrite(PIN47, HIGH);
  delay(delay_time_part1);
  digitalWrite(PIN47, LOW);
  digitalWrite(PIN48, HIGH);
  delay(delay_time_part1);
  digitalWrite(PIN48, LOW);
  digitalWrite(PIN49, HIGH);
  delay(delay_time_part1);
}
void part_1_4(){
  //LEDs run in the same pattern of 1.2, 
  //but using direction registers and port output.
  PORTL |= 1 << DDL2; 
  delay(delay_time_part1);  
  PORTL &= !(1 << DDL2); 
  PORTL |= 1 << DDL1; 
  delay(delay_time_part1); 
  PORTL &= !(1 << DDL1); 
  PORTL |= 1 << DDL0; 
  delay(delay_time_part1);
  PORTL &= !(1 << DDL0);
}
void part_2_4(){
  // System runs at 16MHz, which means 62.5ns per one tick for digitalized signal. 
  // scaling down to 200kHz with a pre-scale of 8. (5 micro seconds per tick)
  // digitalized signal reads every 5 micro seconds.
  // N = 200,000Hz / 400kHz = 500. 500 / 2 = 250 
  DDRH |= 1 << DDH3; // Enable PIN 6
  OCR4A = 2500;// 400Hz
  delay(delay_time_part2); // run it at 400Hz for one second.
  OCR4A = 4000;// 250Hz 
  delay(delay_time_part2); // run it at 250Hz for one second.
  OCR4A = 1250;// 800Hz
  delay(delay_time_part2); // run it at 800Hz for one second.
  DDRH &= 0 << DDH3; // Disable PIN 6
  delay(delay_time_part2);
}
void part_3_1(){
  // This is for part 3.1.
  // This is a kind of scheduler that calls each function.
  part_1_4();
  part_1_4();
  part_2_4();
  sleep();
}
void LED_part_3_2(){
  // task A and part of task C for part 3_2
  // Each LED flashes for 333ms.
  static int time = 0;
  if((time % 1000) == 0){
    PORTL |= 1 << DDL2;  
  }else if((time % 1000) == 333){
    PORTL &= !(1 << DDL2);
    PORTL |= 1 << DDL1;
  }else if((time % 1000) == 666){
    PORTL &= !(1 << DDL1);
    PORTL |= 1 << DDL0;
  }else if((time % 1000) == 999){
    PORTL &= !(1 << DDL0);
  }
  time++;
  // when time = one second, time is set to zero.
  if(time == 1000){
    time = 0;
  }
}
void speaker_part_3_2(int total_time){
  // Speaker with a prescale of 8 (same as the previous one.)
  // Using %, the speaker runs at a frequency of 400, 250, 800, and 0 Hz for one second each, a total of 4 seconds.
  static int time = 0;
  if((time % 4000) == 0){
    DDRH |= 1 << DDH3; // Enable PIN 6
    OCR4A = 2500; // 400Hz  
  }else if((time % 4000) == 1000){
    OCR4A = 4000;// 250Hz
  }else if((time % 4000) == 2000){
    OCR4A = 1250; // 800Hz
  }else if((time % 4000) == 3000){
    DDRH &= 0 << DDH3; // turn off.
  }
  time++;
  // when either time = 4000 or total time = 15999, time set to zero.
  if(time == 4000 || total_time == 15999){
    time = 0;
  }
}
void sleep(){
  // sleep for one second.
  DDRH &= 0 << DDH3; // speaker off
  PORTL &= !(1 << DDL2); // LED off
  PORTL &= !(1 << DDL1);
  PORTL &= !(1 << DDL0);
  delay(delay_time_part2); // 1 second delay.
}
void scheduler_part_3_2(){
  //scheduler for part 3_2
  // delay_time_part3 == 16 seconds = total time 
  static unsigned int time;
  if((time % delay_time_part3) == 0 || (time % delay_time_part3) < 1000){
    LED_part_3_2();
  }else if((time % delay_time_part3) == 1000 || (time % delay_time_part3) < 2000){
    LED_part_3_2();
  }else if((time % delay_time_part3) == 2000 || (time % delay_time_part3) < 6000){
    speaker_part_3_2(time);  
  }else if((time % delay_time_part3) == 6000 || (time % delay_time_part3) < delay_time_part3){
    LED_part_3_2();
    speaker_part_3_2(time);
  }
  time++;
  if(time == delay_time_part3){
    sleep();
    time = 0;
  }
  delay(1);
}
void part_3_3(int total_time){
  // This plays "Mary Has a Little Lamb."
  static int time = 0;
  DDRH |= 1 << DDH3; // Enable PIN 6
  OCR4A = melody[time/150];
  if(time / 150 == note_count){
    time = 0;
  }
  time++;
  if(total_time == delay_time_part3_3-1){
    time = 0;
  }
}
void scheduler_part_3_3(){
  /*  RR scheduler for part 3_2
      LEDs for 2s -> speaker for 7.5s -> both LEDs and speaker for 10 seconds -> sleep for one second
      delay_time_part3 == 19500 micro seconds = total time
  */ 
  static unsigned int time;
  if((time % delay_time_part3_3) == 0 || (time % delay_time_part3_3) < 2000){
    LED_part_3_2();
  }else if((time % delay_time_part3_3) == 2000 || (time % delay_time_part3_3) < 2000+(note_count*150)){
    part_3_3(time);  
  }else if((time % delay_time_part3_3) == 2000+(note_count*150) || (time % delay_time_part3_3) < 12000+(note_count*150)){
    LED_part_3_2();
    part_3_3(time);
  }
  time++;
  if(time == delay_time_part3_3){
    sleep();
    time = 0;
  }
  delay(1);
}
void scheduler_part_4() {
  // Scheduler for part 4.
  // A0 and A1 for alalog inputs.
  static unsigned time;
  x_value = analogRead(JoyX);
  y_value = analogRead(JoyY);
  // Serial.print("X: ");
  // Serial.println(x_value);
  // Serial.print("Y: ");
  // Serial.println(y_value);
  part_4(x_value, y_value);
  part_3_3(time);
  delay(1);
}
void part_4(int x_value, int y_value){
  // This function takes x and y values and divided by 128 to express into 0 to 7 to generate output through 8x8 LEDs.
  int x_map = x_value / 128;
  int y_map = y_value / 128;
  // Serial.print("X: ");
  // Serial.println(x_map);
  // Serial.print("Y: ");
  // Serial.println(y_map);
  //The loop interates through and if i == x_map(0 to 7) then, set the column as 1 to turn the light on.
  for(int i = 0; i < 8; i++){
    if(i == x_map){
      spiTransfer(i, 1 << y_map);
    }else{
      spiTransfer(i, 0b00000000); // if no value is read, put to rest. (at center)
    }
  }
}
void spiTransfer(volatile byte opcode, volatile byte data){
  int offset = 0; //only 1 device
  int maxbytes = 2; //16 bits per SPI command
  
  for(int i = 0; i < maxbytes; i++) { //zero out spi data
    spidata[i] = (byte)0;
  }
  //load in spi data
  spidata[offset+1] = opcode+1;
  spidata[offset] = data;
  PORTB &= 0 << DDB5; //
  for(int i=maxbytes;i>0;i--)
    shiftOut(DIN,CLK,MSBFIRST,spidata[i-1]); //shift out 1 byte of data starting with leftmost bit
  PORTB |= 1 << DDB5;
}
