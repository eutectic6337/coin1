/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://docs.arduino.cc/hardware/

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/basics/Blink/
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(PC5, OUTPUT);
  pinMode(PC3, OUTPUT);
  pinMode(PC2, OUTPUT);
  pinMode(PC0, OUTPUT);
  pinMode(PC1, OUTPUT);
  pinMode(PA1, OUTPUT);
  pinMode(PD0, OUTPUT);
  pinMode(PD7, OUTPUT);
  pinMode(PD5, OUTPUT);
  pinMode(PD4, OUTPUT);
  pinMode(PD1, OUTPUT);
  pinMode(PD3, OUTPUT);
  pinMode(PC7, OUTPUT);
  pinMode(PD2, OUTPUT);
  pinMode(PC6, OUTPUT);
  pinMode(PC4, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(PC5, LOW);
  delay(100);
  digitalWrite(PC3, LOW);
  delay(100);
  digitalWrite(PC2, LOW);
  delay(100);
  digitalWrite(PC0, LOW);
  delay(100);
  digitalWrite(PC1, LOW);
  delay(100);
  digitalWrite(PA1, LOW);
  delay(100);
  digitalWrite(PD0, LOW);
  delay(100);
  digitalWrite(PD7, LOW);
  delay(100);
  digitalWrite(PD5, LOW);
  delay(100);
  digitalWrite(PD4, LOW);
  delay(100);
  digitalWrite(PD1, LOW);
  delay(100);
  digitalWrite(PD3, LOW);
  delay(100);
  digitalWrite(PC7, LOW);
  delay(100);
  digitalWrite(PD2, LOW);
  delay(100);
  digitalWrite(PC6, LOW);
  delay(100);
  digitalWrite(PC4, LOW);
  delay(100);

  digitalWrite(PC5, HIGH);
  delay(100);
  digitalWrite(PC2, HIGH);
  delay(100);
  digitalWrite(PC1, HIGH);
  delay(100);
  digitalWrite(PD0, HIGH);
  delay(100);
  digitalWrite(PD5, HIGH);
  delay(100);
  digitalWrite(PD1, HIGH);
  delay(100);
  digitalWrite(PC7, HIGH);
  delay(100);
  digitalWrite(PC6, HIGH);
  delay(100);

  digitalWrite(PC5, LOW);
  delay(100);
  digitalWrite(PC3, LOW);
  delay(100);
  digitalWrite(PC2, LOW);
  delay(100);
  digitalWrite(PC0, LOW);
  delay(100);
  digitalWrite(PC1, LOW);
  delay(100);
  digitalWrite(PA1, LOW);
  delay(100);
  digitalWrite(PD0, LOW);
  delay(100);
  digitalWrite(PD7, LOW);
  delay(100);
  digitalWrite(PD5, LOW);
  delay(100);
  digitalWrite(PD4, LOW);
  delay(100);
  digitalWrite(PD1, LOW);
  delay(100);
  digitalWrite(PD3, LOW);
  delay(100);
  digitalWrite(PC7, LOW);
  delay(100);
  digitalWrite(PD2, LOW);
  delay(100);
  digitalWrite(PC6, LOW);
  delay(100);
  digitalWrite(PC4, LOW);
  delay(100);

  digitalWrite(PC3, HIGH);
  delay(100);
  digitalWrite(PC0, HIGH);
  delay(100);
  digitalWrite(PA1, HIGH);
  delay(100);
  digitalWrite(PD7, HIGH);
  delay(100);
  digitalWrite(PD4, HIGH);
  delay(100);
  digitalWrite(PD3, HIGH);
  delay(100);
  digitalWrite(PD2, HIGH);
  delay(100);
  digitalWrite(PC4, HIGH);
  delay(100);
}
