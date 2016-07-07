#include <SoftwareSerial.h>
#include <stdlib.h>

#define PIN_RED   3
#define PIN_GREEN 5
#define PIN_BLUE  6
#define PIN_RX    10
#define PIN_TX    11

#define MSG_TIMEOUT 1000
#define MAX_MSG_LENGTH 16
#define MSG_LENGTH_IDX 1
#define MSG_TYPE_IDX  2
#define MSG_DATA_START_IDX 3

#define MSG_START 0xFF
#define MSG_POWER 0

SoftwareSerial softSerial(PIN_RX, PIN_TX);

unsigned char message[MAX_MSG_LENGTH];
int message_index;
int message_length;
unsigned long message_start_time;
bool in_message;
bool complete_message;

bool powered_on;

void setup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  
  pinMode(PIN_RX, INPUT);
  pinMode(PIN_TX, OUTPUT);

  softSerial.begin(9600);
  Serial.begin(9600);

  in_message = false;
  complete_message = false;
  message_length = 0;
  message_index = 0;
  message_start_time = 0;

  powered_on = false;
}

void loop() {
  // Timeout for getting a message
  if (millis() - message_start_time > MSG_TIMEOUT
      || millis() - message_start_time < 0) {
    in_message = false;
    complete_message = false;
    message_index = 0;
    message_length = 0;
  }
  
  // Get incoming data 
  if (softSerial.available() > 0) {
    unsigned char c = softSerial.read();
    //Serial.println(c, BIN);

    if (in_message) {
      // Save stream to current message
      message[message_index] = c;
      message_index++;

      // Store the length in message_length
      if (message_index == (MSG_LENGTH_IDX+1)) {
        message_length = message[MSG_LENGTH_IDX];
      }

      // We finished reading the message
      if (message_index >= message_length) {
        message[message_index] = '\0';
        in_message = false;
        complete_message = true;
      }
      
    } else if (c == MSG_START) {
      // Start receiving a new message
      message[0] = c;
      message_index = 1;
      in_message = true;
      message_start_time = millis();
    }
  }

  // Process messages if needed
  if (complete_message) {
    complete_message = false;

    switch (message[MSG_TYPE_IDX]) {
      case MSG_POWER:
        powered_on = atoi((const char *)(message+MSG_DATA_START_IDX));
        break;
      default: break;
    }

    for (int i = 0; i < message_length; i++) {
      Serial.println(message[i]);
    }
  }

  // Control light power
  if (powered_on) {
    analogWrite(PIN_RED, 200);
    analogWrite(PIN_GREEN, 150);
    analogWrite(PIN_BLUE, 100);
  } else {
    analogWrite(PIN_RED, 0);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE, 0);
  }
}
