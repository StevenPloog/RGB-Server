#include "Arduino.h"
#include "RGBLED.h"

//RGBLED::RGBLED() {}

// Constructor for the led
// Pins passed to this constructor must implement Arduino's analogWrite method
RGBLED::RGBLED(float _redPin, float _greenPin, float _bluePin) {
    redPin = _redPin;
    greenPin = _greenPin;
    bluePin = _bluePin;

    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    current_state = POWER_OFF;
    next_state = POWER_OFF;
    powered_on_state = FADE_IN;

    current_fade_color = 0;
    setFadeColor(0, 0.0, 0.0, 128.0);
    setFadeColor(1, 128.0, 0.0, 116.0);
    setFadeTime(3000);

    setRGB(0.0, 0.0, 0.0);

    last_tick_millis = millis();
}

void RGBLED::tick() {
    // Handle current state, power states are handled after switch
    if (current_state != POWER_OFF) {
        switch (current_state) {
            case POWERING_OFF:  fadeOut(current_fade_color, true); break;
            case POWERING_ON:   fadeIn(current_fade_color,  true); break;
            case SOLID_COLOR:   break;
            case FADE:          break;
            case FADE_IN:       fadeIn(current_fade_color,  false); break;
            case FADE_OUT:      fadeOut(current_fade_color, false); break;
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
        current_state = SOLID_COLOR;
        setRGB(pRed, pGreen, pBlue);
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

void RGBLED::nextFadeColor() {
    switch (current_fade_color) {
        case 0:     current_fade_color = 1; break;
        case 1:     current_fade_color = 0; break;
        default:    current_fade_color = 0; break;
    }
}

void RGBLED::fadeIn(int fadeColor, bool to_solid) {
    if (to_solid) {
        fade(fadeColor, DIRECTION_UP, SOLID_COLOR);
    } else {
        fade(fadeColor, DIRECTION_UP, FADE_OUT);
    }
}

void RGBLED::fadeOut(int fadeColor, bool to_off) {
    if (to_off) {
        fade(fadeColor, DIRECTION_DOWN, POWER_OFF);
    } else {
        fade(fadeColor, DIRECTION_DOWN, FADE_IN);
    }
}

void RGBLED::fade(int fadeColor, int direction, int to_state) {
    long num_millis = (long)(millis() - fade_last_millis);
    fade_last_millis = millis();

    next_state = current_state;

    if (num_millis > 0) {
        setRGB( red  +((float)direction * num_millis * fade_step[fadeColor][0]),
                green+((float)direction * num_millis * fade_step[fadeColor][1]),
                blue +((float)direction * num_millis * fade_step[fadeColor][2])
        );
    }

    if (direction == DIRECTION_UP) {
        red   = upperbound(red,   fade_rgb[fadeColor][0]);
        green = upperbound(green, fade_rgb[fadeColor][1]);
        blue  = upperbound(blue,  fade_rgb[fadeColor][2]);
        if (red == fade_rgb[fadeColor][0] && green == fade_rgb[fadeColor][1] && blue == fade_rgb[fadeColor][2]) {
            next_state = to_state;
            current_state = to_state;
        }
    } else {
        red   = lowerbound(red,   0.0);
        green = lowerbound(green, 0.0);
        blue  = lowerbound(blue,  0.0);
        if (red == 0.0 && green == 0.0 && blue == 0.0) {
            nextFadeColor();
            next_state = to_state;
            current_state = to_state;
        }
    }
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

int RGBLED::getState() {
    return current_state;
}

void RGBLED::setState(int state) {
    current_state = state;
}

void RGBLED::powerOn() {
    // Don't POWER_ON again if we are already on
    if (current_state != POWER_OFF && current_state != POWERING_OFF) return;

    // Fade the light on
    if (powered_on_state == SOLID_COLOR) {
        current_fade_color = 0;
        setRGB(0, 0, 0);
        setFadeColor(current_fade_color, pRed, pGreen, pBlue);
        setFadeTime(1500);
        next_state = POWERING_ON;
        current_state = POWERING_ON;
    } else {
        next_state = FADE_IN;
        current_state = FADE_IN;
    }

    fade_last_millis = millis();
}

void RGBLED::powerOff() {
    // Don't POWER_OFF again if we are already off
    if (current_state == POWER_OFF || current_state == POWERING_OFF) return;

    if (current_state == SOLID_COLOR) {
        pRed   = red;
        pGreen = green;
        pBlue  = blue;
        current_fade_color = 0;
        setFadeColor(current_fade_color, red, green, blue);
        setFadeTime(1500);
        next_state = POWERING_OFF;
        current_state = POWERING_OFF;
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

inline float RGBLED::lowerbound(float x, float bound) {
    if (x <= bound) return bound;
    return x;
}

inline float RGBLED::upperbound(float x, float bound) {
    if (x >= bound) return bound;
    return x;
}
