#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#define MSG_START '~'
#define MSG_MAX_LENGTH 16

using namespace std;

int main(int argc, char * argv[]) {
    int uart;
    
    if (argc != 3) {
        cout << "Too few arguments supplied.\n";
        return 1;
    }
    
    wiringPiSetupSys();
    
    if ((uart = serialOpen("/dev/serial0", 9600)) < 0) {
        cout << "Unable to open UART: " << strerror (errno) << endl;
        return 1;
    }

    serialPutchar(uart, MSG_START);
    serialPutchar(uart, atoi(argv[1]));
    //serialPutchar(uart, (const char)atoi((const char*)argv[2]));
    serialPuts(uart, argv[2]);

    return 0;
}

