int delay_time = 333; //delay time = 333ms.

void setup() {
  // setup without using pinMode. DDL2 is the bit 2 of Data Direction Register L.
  DDRL |= 1 << DDL2; // Bit 2 of DDRL = Pin 47
  DDRL |= 1 << DDL1; // Bit 1 of DDRL = Pin 48
  DDRL |= 1 << DDL0; // Bit 0 of DDRL = Pin 49
}

void loop() {
  // put your main code here, to run repeatedly:
  PORTL |= 1 << DDL2; // Port L is the output of L, set DDL2 as 1 (HIGH)
  delay(delay_time);  // HIGH for 333ms.
  PORTL &= !(1 << DDL2); // Port L is the output of L, set DDL2 as 0 (LOW)
  delay(delay_time); // LOW for 333ms.
  PORTL |= 1 << DDL1; // set DDL1 as 1 (HIGH)
  delay(delay_time); // HIGH for 333ms
  PORTL &= !(1 << DDL1); // set DDL1 as 0 (LOW)
  delay(delay_time); // LOW for 333ms
  PORTL |= 1 << DDL0; // set DDL0 as 1 (HIGH)
  delay(delay_time); // HIGH for 333ms.
  PORTL &= !(1 << DDL0); // set DDL0 as 0 (HIGH)
  delay(delay_time); // LOW for 333ms.
}
