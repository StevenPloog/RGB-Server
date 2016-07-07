#include "Arduino.h"
#include "RGBLED.h"

//RGBLED::RGBLED() {}

// Constructor for the led
// Pins passed to this constructor must implement Arduino's analogWrite method
RGBLED::RGBLED(float redPin, float greenPin, float bluePin) {
    _redPin = redPin;
    _greenPin = greenPin;
    _bluePin = bluePin;

    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);

    setRGB(0.0, 0.0, 0.0);
}

// Fade the red, green and blue values
void RGBLED::fadeRGB(float r, float g, float b) {
    while (_red != r || _green != g || _blue != b) {
        if (_red < r)        { setRed(_red+1.0); }
        else if (_red > r)   { setRed(_red-1.0); }

        if (_green < g)      { setGreen(_green+1.0); }
        else if (_green > g) { setGreen(_green-1.0); }

        if (_blue < b)       { setBlue(_blue+1.0); }
        else if (_blue > b)  { setBlue(_blue-1.0); }

        delay(RGBLED_FADE_DELAY);
    }
}

// Fade the red value
void RGBLED::fadeRed(float r) {
    while (_red != r) {
        if (_red < r)   { setRed(_red+1.0); }
        else            { setRed(_red-1.0); }

        delay(RGBLED_FADE_DELAY);
    }
}

// Fade the green value
void RGBLED::fadeGreen(float g) {
    while (_green != g) {
        if (_green < g) { setGreen(_green+1.0); }
        else            { setGreen(_green-1.0); }

        delay(RGBLED_FADE_DELAY);
    }
}

// Fade the blue value
void RGBLED::fadeBlue(float b) {
    while (_blue != b) {
        if (_blue < b)  { setBlue(_blue+1.0); }
        else            { setBlue(_blue-1.0); }

        delay(RGBLED_FADE_DELAY);
    }
}

// Set the red, green and blue values
void RGBLED::setRGB(float r, float g, float b) {
    _red = r;
    _green = g;
    _blue = b;

    analogWrite(_redPin,  (int) _red);
    analogWrite(_greenPin,(int) _green);
    analogWrite(_bluePin, (int) _blue);
}

// Set the red value
void RGBLED::setRed(float r) {
    _red = r;
    analogWrite(_redPin, (int) _red);
}

// Set the green value
void RGBLED::setGreen(float g) {
    _green = g;
    analogWrite(_greenPin, (int) _green);
}

// Set the blue value
void RGBLED::setBlue(float b) {
    _blue = b;
    analogWrite(_bluePin, (int) _blue);
}

// Step toward rgb value
void RGBLED::stepToRGB(float r, float g, float b, float stepSize) {
  if (_red < r) {
    _red += stepSize;
    if (_red > r) { _red = r; }
  } else if (_red > r) {
    _red -= stepSize;
    if (_red < r) { _red = r; }
  }
  setRed(_red);
  
  if (_green < g) {
    _green += stepSize;
    if (_green > g) { _green = g; }
  } else if (_green > g) {
    _green -= stepSize;
    if (_green < g) { _green = g; }
  }
  setGreen(_green);
  
  if (_blue < b) {
    _blue += stepSize;
    if (_blue > b) { _blue = b; }
  } else if (_blue > b) {
    _blue -= stepSize;
    if (_blue < b) { _blue = b; }
  }
  setBlue(_blue);
}

// Step toward rgb value
void RGBLED::stepToRGB(float r, float g, float b) {
  stepToRGB(r, g, b, 1.0);
}

// Getter methods for rgb values
float RGBLED::red()   { return _red;   }
float RGBLED::green() { return _green; }
float RGBLED::blue()  { return _blue;  }

