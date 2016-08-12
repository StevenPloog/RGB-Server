#include "Arduino.h"
#include "RGBLED.h"

//RGBLED::RGBLED() {}

// Constructor for the led
// Pins passed to this constructor must implement Arduino's analogWrite method
RGBLED::RGBLED(float _redPin, float _greenPin, float _bluePin) {
    redPin = _redPin;
    greenPin = _greenPin;
    bluePin = _bluePin;

    current_state = POWER_OFF;
    next_state = POWER_OFF;
    powered_on_state = FADE;
    last_tick_millis = millis();

    fade_current_color = 0;
    fade_time = 1000;
    fade_init = true;

    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    setRGB(0.0, 0.0, 0.0);
    setFadeColor(0, 0.0, 0.0, 128.0);
    setFadeColor(1, 128.0, 0.0, 116.0);
    setFadeTime(3000);
}

void RGBLED::tick() {
    unsigned long current_millis = millis();
    long num_millis;

    // Handle current state, power states are handled after switch
    if (current_state != POWER_OFF) {
        switch (current_state) {
            case POWERING_OFF:  poweringOff(); break;
            case POWERING_ON:   poweringOn();  break;
            case SOLID_COLOR:   break;
            case FADE:          break;
            case FADE_IN:       break;
            case FADE_OUT:      break;
            default: break;
        }
        current_state = next_state;
        rgbOut();
    } else {
        rgbOff();
    }

    last_tick_millis = millis();
}

void RGBLED::setSolidColor(float r, float g, float b) {
    powered_on_state = SOLID_COLOR;
    next_state = SOLID_COLOR;
    pRed    = boundcheck(r);
    pGreen  = boundcheck(g);
    pBlue   = boundcheck(b);

    if (current_state != POWER_OFF && current_state != POWERING_OFF) {
        setRGB(r, g, b);
    }
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
    if      (red   < r) { setRed  (red   + stepSize); }
    else if (red   > r) { setRed  (red   - stepSize); }
    if      (green < g) { setGreen(green + stepSize); }
    else if (green > g) { setGreen(green - stepSize); }
    if      (blue  < b) { setBlue (blue  + stepSize); }
    else if (blue  > b) { setBlue (blue  - stepSize); }
}

// Step toward rgb value
void RGBLED::stepToRGB(float r, float g, float b) {
    stepToRGB(r, g, b, 1.0);
}

void RGBLED::setState(int state) {
    current_state = state;
}

// Fade on to a solid color
void RGBLED::poweringOn() {
    long num_millis = (long)(millis() - fade_last_millis);
    fade_last_millis = millis();

    if (num_millis > 0) {
        setRGB( red  +(num_millis * fade_step[0][0]),
                green+(num_millis * fade_step[0][1]),
                blue +(num_millis * fade_step[0][2])
        );
    }

    if (red   > pRed  ) setRed(pRed);
    if (green > pGreen) setGreen(pGreen);
    if (blue  > pBlue ) setBlue(pBlue);

    if (red == pRed && green == pGreen && blue == pBlue) {
        next_state = SOLID_COLOR;
    }
}

void RGBLED::poweringOff() {
    long num_millis = (long)(millis() - fade_last_millis);
    fade_last_millis = millis();

    if (num_millis > 0) {
        setRGB( red  -(num_millis * fade_step[0][0]),
                green-(num_millis * fade_step[0][1]),
                blue -(num_millis * fade_step[0][2])
        );
    }
    
    if (red   <= 0.0) setRed(0.0);
    if (green <= 0.0) setGreen(0.0);
    if (blue  <= 0.0) setBlue(0.0);

    if (red == 0.0 && green == 0.0 && blue == 0.0) {
        next_state = POWER_OFF;
    }
}

void RGBLED::powerOn() {
    // Don't POWER_ON again if we are already on
    if (current_state != POWER_OFF && current_state != POWERING_OFF) return;

    // Fade the light on
    if (powered_on_state == SOLID_COLOR) {
        setRGB(0, 0, 0);
        setFadeColor(0, pRed, pGreen, pBlue);
        setFadeTime(1500);
        setFadeSteps(0);
        next_state = POWERING_ON;
        current_state = POWERING_ON;
    } else {
        next_state = FADE_IN;
        current_state = FADE_IN;
    }

    fade_last_millis = millis();
}

void RGBLED::powerOff() {
    // Don't POWER_ON again if we are already on
    if (current_state == POWER_OFF || current_state == POWERING_OFF) return;

    if (current_state == SOLID_COLOR) {
        pRed   = red;
        pGreen = green;
        pBlue  = blue;
        setFadeColor(0, red, green, blue);
        setFadeTime(1500);
        setFadeSteps(0);
        next_state = POWERING_OFF;
        powered_on_state = SOLID_COLOR;
    } else {
        power_off_after_fade = true;
        next_state = FADE_OUT;
        powered_on_state = FADE;
    }

    fade_last_millis = millis();
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
