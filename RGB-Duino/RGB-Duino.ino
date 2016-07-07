#include "RGBLED.h"
#include <SoftwareSerial.h>
#include <stdlib.h>

#define PIN_RED   3
#define PIN_GREEN 5
#define PIN_BLUE  6
#define PIN_RX    10
#define PIN_TX    11

#define MSG_TIMEOUT 1000
#define MAX_MSG_LENGTH 16
#define MAX_ARGS 5
#define MAX_ARG_LENGTH 32
#define MSG_LENGTH_IDX 1
#define MSG_TYPE_IDX  2
#define MSG_DATA_START_IDX 3

#define MSG_START '~'
#define MSG_POWER 0

RGBLED strip(PIN_RED, PIN_GREEN, PIN_BLUE);
SoftwareSerial softSerial(PIN_RX, PIN_TX);

char message[MAX_MSG_LENGTH];
char message_args[MAX_ARGS][MAX_ARG_LENGTH];
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
    char c = softSerial.read();

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

    // Parse args
    strcpy(message_args[0], strtok(message+MSG_DATA_START_IDX, ","));
    for (int i = 1; i < MAX_ARGS; i++) {
      strcpy(message_args[i], strtok(NULL, ","));
      if (message_args[i] == NULL) {
        break;
      }
    }

    switch (message[MSG_TYPE_IDX]) {
      case MSG_POWER:
        powered_on = atoi(message_args[0]);
        break;
      default: break;
    }

    Serial.print("MSG:");
    for (int i = 0; i < message_length; i++) {
      Serial.print(message[i]);
      //Serial.print(',');
    }
    Serial.println();

    Serial.print("ARG:");
    for (int i = 0; i < MAX_ARGS; i++) {
      Serial.print(message_args[i]);
      Serial.print(',');
    }
    Serial.println();
  }

  // Control light power
  if (powered_on) {
    strip.setRGB(200, 150, 100);
  } else {
    strip.setRGB(0, 0, 0);
  }
}
