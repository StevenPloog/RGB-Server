#ifndef RGBLED_H
#define RGBLED_H

#define POWER_OFF       0
#define POWERING_OFF    1
#define POWERING_ON     2
#define SOLID_COLOR     3
#define FADE            4
#define FADE_IN         5
#define FADE_OUT        6

class RGBLED {
    public:
        //RGBLED();
        RGBLED(float redPin, float greenPin, float bluePin);

        void tick();

        void setState(int state);

        void setSolidColor(float r, float g, float b);
        void setRGB(float r, float g, float b);
        void setRed(float r);
        void setGreen(float g);
        void setBlue(float b);

        void setFadeColor(int fadeColor, float r, float g, float b);
        void setFadeTime(unsigned long millis);

        void stepToRGB(float r, float g, float b, float stepSize);
        void stepToRGB(float r, float g, float b);

        void powerOn();
        void powerOff();

        float getRed();
        float getGreen();
        float getBlue();

    private:
        inline float boundcheck(float x);
        void poweringOff();
        void poweringOn();
        void rgbOut();
        void rgbOff();
        void setFadeSteps(int fadeColor);

        int current_state;
        int next_state;
        int powered_on_state;
        unsigned long last_tick_millis;

        bool power_off_after_fade;
        bool fade_init;
        int fade_current_color;
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
