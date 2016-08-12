#ifndef RGBLED_H
#define RGBLED_H

#define POWER_OFF       0
#define POWERING_OFF    1
#define POWERING_ON     2
#define SOLID_COLOR     3
#define FADE            4
#define FADE_IN         5
#define FADE_OUT        6

#define DIRECTION_UP    1
#define DIRECTION_DOWN -1

class RGBLED {
    public:
        //RGBLED();
        RGBLED(float redPin, float greenPin, float bluePin);

        void tick();

        void setSolidColor(float r, float g, float b);

        void startFade();
        void setFadeColor(int fadeColor, float r, float g, float b);
        void setFadeTime(unsigned long millis);

        void powerOn();
        void powerOff();

        float getRed();
        float getGreen();
        float getBlue();

    private:
        inline float boundcheck(float x);
        inline float lowerbound(float x, float bound);
        inline float upperbound(float x, float bound);
        void fadeIn(int fadeColor, bool to_solid);
        void fadeOut(int fadeColor, bool to_off);
        void fade(int fadeColor, int direction, int to_state);
        void nextFadeColor();
        void rgbOut();
        void rgbOff();
        void setFadeSteps(int fadeColor);
        void setRGB(float r, float g, float b);
        void setRed(float r);
        void setGreen(float g);
        void setBlue(float b);
        void stepToRGB(float r, float g, float b, float stepSize);
        void stepToRGB(float r, float g, float b);

        int current_state;
        int powered_on_state;
        unsigned long last_tick_millis;

        bool power_off_after_fade;
        int current_fade_color;
        unsigned long fade_time;
        unsigned long fade_last_millis;
        float fade_rgb[2][3];
        float fade_step[2][3];
        float redPin;
        float greenPin;
        float bluePin;
        float red;
        float green;
        float blue;
        float pRed;
        float pGreen;
        float pBlue;
};

#endif
