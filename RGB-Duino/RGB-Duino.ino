#include <SoftwareSerial.h>

#define PIN_RED   3
#define PIN_GREEN 5
#define PIN_BLUE  6
#define PIN_RX    10
#define PIN_TX    11

SoftwareSerial softSerial(PIN_RX, PIN_TX);
char last_char;

void setup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  
  pinMode(PIN_RX, INPUT);
  pinMode(PIN_TX, OUTPUT);

  softSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  if (softSerial.available() > 0) {
    last_char = softSerial.read();
    Serial.write(last_char);
    if (last_char == 'X') {
      analogWrite(PIN_RED, 0);
      analogWrite(PIN_GREEN, 0);
      analogWrite(PIN_BLUE, 255);
    } else {
      analogWrite(PIN_RED, 0);
      analogWrite(PIN_GREEN, 0);
      analogWrite(PIN_BLUE, 100);
    }
  }
}
