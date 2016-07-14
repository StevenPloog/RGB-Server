#ifndef RGBLED_H
#define RGBLED_H

#define STATE_POWEROFF  0
#define STATE_POWERON   1
#define STATE_RGB       2
#define STATE_FADE      4

class RGBLED {
    public:
        //RGBLED();
        RGBLED(float redPin, float greenPin, float bluePin);

        void tick();

        void setState(int state);

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
        void rgbOut();
        void rgbOff();
        void setFadeSteps(int fadeColor);

        int current_state;
        int power_state;
        unsigned long last_tick_millis;

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
};

#endif
