#include <avr/io.h>
#define JoyX DDF0
#define JoyY DDF1
#define OP_DECODEMODE  8
#define OP_SCANLIMIT   10
#define OP_SHUTDOWN    11
#define OP_DISPLAYTEST 14
#define OP_INTENSITY   10 
void spiTransfer(volatile byte row, volatile byte data);

byte spidata[2];
int x_map, y_map, x_pos, y_pos;
int now_pos[2] ={0,0};
int push = 1;

int DIN = 12;
int CS =  11;
int CLK = 10;

byte now[8][8] = {
  {1, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};


int delay_time_part3 = 16000;

void setup() {
    // Part 1.4.2 //////////////////////////////
  
  DDRL |= 1 << DDL3 || 1 << DDL2 || 1 << DDL1 || 1 << DDL0; // Pin 46 Pin 47 Pin 48 Pin 49
  DDRB |= 1 << DDB5 || 1 << DDB4; // Pin 11 Pin 10
  DDRH |= 1 << DDH6 || 1 << DDH5; // Pin 9 Pin 8
  DDRC |= 1 << DDC7 || 1 << DDC6 || 1 << DDC5 || 1 << DDC4 || 1 << DDC1 || 1 << DDC0; // Pin 30 Pin 31 Pin 32 Pin 33 Pin 36 PIn 37
  DDRG |= 1 << DDG2 || 1 << DDG0; // Pin 39 Pin 41
  ////////////////////////////////////////////
  PORTL |= 1 << DDL3 || 1 << DDL2 || 1 << DDL1 || 1 << DDL0;
  PORTB |= 1 << DDB5 || 1 << DDB4; // Pin 11 Pin 10
  PORTH |= 1 << DDH6 || 1 << DDH5; // Pin 9 Pin 8
  PORTC |= 1 << DDC7 || 1 << DDC6 || 1 << DDC5 || 1 << DDC4 || 1 << DDC1 || 1 << DDC0; // Pin 30 Pin 31 Pin 32 Pin 33 Pin 36 PIn 37
  PORTG |= 1 << DDG2 || 1 << DDG0; // Pin 39 Pin 41

  //DDRB |= 1 << DDB6 | 1 << DDB5 | 1 << DDB4; // enable pin 10(CLK), pin 11(CS), pin 12(DIN).
  //PORTB |= 1 << DDB5; //set CS to HIGH.
  DDRF |= 0 << DDF0 | 0 << DDF1; //Analog Input Setting. (A0 and A1)

  spiTransfer(OP_DISPLAYTEST,0);
  spiTransfer(OP_SCANLIMIT,7);
  spiTransfer(OP_DECODEMODE,0);
  spiTransfer(OP_SHUTDOWN,1);


}

void loop() {
  read_joystick();
  delay(1);

  check_joy();
  delay(1);

  setDisplay();
  delay(200);

}
void display(){
  digitalWrite(cols[col], HIGH);
  col++;
  if(col == 0){
    col = 0;
  }
  for(int row = 0; row <= 7; row++){
    if(leds[col][7-row] == 1){
      digitalWrite(rows[row], HIGH);
    }else {
      digitalWrite(rows[row], LOW);
    }
  }
  digitalWrite(cols[col], LOW);
}
void clearLeds(){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j ++){
      leds[i][j] = 0;
    }
  }
}
void setDisplay(){
  digitalWrite(cols[col], HIGH);
  col++;
  if (col == 8){
    col = 0
  }
  for(int row = 0 row <= 7; row++){
    if(leds[col][7-row] == 1){
      digitalWrite(rows[row], HIGH);      
    }else{
      digitalWrite(rows[row], LOW);
    }
  }
}
void read_joystick(){
  x_pos = analogRead(JoyX);
  y_pos = analogRead(JoyY);
}
void check_joy(){
  if((x_pos < 510) && (x_pos < y_pos)){
    left();
  }else if((x_pos < 530) && (x_pos > y_pos)){
    right();
  }else if((y_pos < 510)){
    up();
  }else if((y_pos > 530)){
    down();
  }
}
void left(){
  if(now_pos[1] > 0){
    now[now_pos[0]][now_pos[1]] = 0;
    now_pos[1] = now_pos[1] - 1;
    now[now_pos[0]][now_pos[1]] = 1;
  }
}
void right(){
  if(now_pos[1] < 7){
    now[now_pos[0]][now_pos[1]] = 0;
    now_pos[1] = now_pos[1] + 1;
    now[now_pos[0]][now_pos[1]] = 1;
  }
}
void up(){
  if(now_pos[0] > 0){
    now[now_pos[0]][now_pos[1]] = 0;
    now_pos[0] = now_pos[0] - 1;
    now[now_pos[0]][now_pos[1]] = 1;
  }
}
void down(){
  if(now_pos[0] < 7){
    now[now_pos[0]][now_pos[1]] = 0;
    now_pos[0] = now_pos[0] + 1;
    now[now_pos[0]][now_pos[1]] = 1;
  }
}