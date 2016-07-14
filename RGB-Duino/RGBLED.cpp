#include "Arduino.h"
#include "RGBLED.h"

//RGBLED::RGBLED() {}

// Constructor for the led
// Pins passed to this constructor must implement Arduino's analogWrite method
RGBLED::RGBLED(float _redPin, float _greenPin, float _bluePin) {
    redPin = _redPin;
    greenPin = _greenPin;
    bluePin = _bluePin;

    current_state = STATE_POWEROFF;
    power_state = STATE_POWEROFF;
    last_tick_millis = millis();

    fade_current_color = 0;
    fade_time = 1000;
    fade_init = true;

    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    setRGB(0.0, 0.0, 0.0);
}

void RGBLED::tick() {
    unsigned long current_millis = millis();
    long num_millis;

    // Handle current state, power states are handled after switch
    switch (current_state) {
        case STATE_RGB:
            break;
        case STATE_FADE:
            // Setup to begin fading
            if (fade_init) {
                setRGB(0, 0, 0);
                fade_last_millis = current_millis;
                fade_init = false;
            }

            num_millis = (long)(current_millis - fade_last_millis);
            //Serial.println(num_millis);
            if (num_millis > 0) {
                setRGB( red+(num_millis * fade_step[fade_current_color][0]),
                        green+(num_millis * fade_step[fade_current_color][1]),
                        blue+(num_millis * fade_step[fade_current_color][2])
                );

                if (red >= fade_rgb[fade_current_color][0] && green >= fade_rgb[fade_current_color][1] && blue >= fade_rgb[fade_current_color][2]) {
                    fade_step[fade_current_color][0] *= -1.0;
                    fade_step[fade_current_color][1] *= -1.0;
                    fade_step[fade_current_color][2] *= -1.0;
                } else if (red <= 0.0 && green <= 0.0 && blue <= 0.0) {
                    fade_step[fade_current_color][0] *= -1.0;
                    fade_step[fade_current_color][1] *= -1.0;
                    fade_step[fade_current_color][2] *= -1.0;
                    if (fade_current_color == 0) fade_current_color = 1;
                    else fade_current_color = 0;
                }
                fade_last_millis = current_millis;
            }
            break;
        default: break;
    }

    // Handle power states
    if (power_state == STATE_POWERON)
        rgbOut();
    else
        rgbOff();

    last_tick_millis = millis();
}

void RGBLED::setFadeTime(unsigned long millis) {
    fade_time = millis;
    setFadeSteps(0);
    setFadeSteps(1);
}

void RGBLED::setFadeSteps(int fadeColor) {
    float half_time = (float)fade_time / 2.0;
    fade_step[fadeColor][0] = fade_rgb[fadeColor][0] / half_time;
    fade_step[fadeColor][1] = fade_rgb[fadeColor][1] / half_time;
    fade_step[fadeColor][2] = fade_rgb[fadeColor][2] / half_time;

    fade_init = true;
}

void RGBLED::setFadeColor(int fadeColor, float r, float g, float b) {
    fade_rgb[fadeColor][0] = boundcheck(r);
    fade_rgb[fadeColor][1] = boundcheck(g);
    fade_rgb[fadeColor][2] = boundcheck(b);
    setFadeSteps(fadeColor);
}

// Step toward rgb value
void RGBLED::stepToRGB(float r, float g, float b, float stepSize) {
    if (red < r) {
        red += stepSize;
        if (red > r) { red = r; }
    } else if (red > r) {
        red -= stepSize;
        if (red < r) { red = r; }
    }
    setRed(red);

    if (green < g) {
        green += stepSize;
        if (green > g) { green = g; }
    } else if (green > g) {
        green -= stepSize;
        if (green < g) { green = g; }
    }
    setGreen(green);

    if (blue < b) {
        blue += stepSize;
        if (blue > b) { blue = b; }
    } else if (blue > b) {
        blue -= stepSize;
        if (blue < b) { blue = b; }
    }
    setBlue(blue);
}

// Step toward rgb value
void RGBLED::stepToRGB(float r, float g, float b) {
    stepToRGB(r, g, b, 1.0);
}

void RGBLED::setState(int state) {
    current_state = state;
}

void RGBLED::powerOn() {
    power_state = STATE_POWERON;
}

void RGBLED::powerOff() {
    power_state = STATE_POWEROFF;
}

void RGBLED::rgbOut() {
    analogWrite(redPin,  (int) red);
    analogWrite(greenPin,(int) green);
    analogWrite(bluePin, (int) blue);
}

void RGBLED::rgbOff() {
    analogWrite(redPin,   0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin,  0);
}

// Getters and setters for red, green, blue
float RGBLED::getRed   () { return red;   }
float RGBLED::getGreen () { return green; }
float RGBLED::getBlue  () { return blue;  }
void  RGBLED::setRed   (float r) { red   = boundcheck(r); }
void  RGBLED::setGreen (float g) { green = boundcheck(g); }
void  RGBLED::setBlue  (float b) { blue  = boundcheck(b); }

void  RGBLED::setRGB(float r, float g, float b) {
    red = boundcheck(r);
    green = boundcheck(g);
    blue = boundcheck(b);
}

inline float RGBLED::boundcheck(float x) {
    if (x > 255.0) return 255.0;
    if (x < 0.0) return 0.0;
    return x;
}
