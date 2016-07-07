#ifndef RGBLED_H
#define RGBLED_H

#ifndef RGBLED_FADE_DELAY
#define RGBLED_FADE_DELAY 15
#endif

class RGBLED {
    public:
        //RGBLED();
        RGBLED(float redPin, float greenPin, float bluePin);

        void fadeRGB(float r, float g, float b);
        void fadeRed(float r);
        void fadeGreen(float g);
        void fadeBlue(float b);

        void setRGB(float r, float g, float b);
        void setRed(float r);
        void setGreen(float g);
        void setBlue(float b);

        void stepToRGB(float r, float g, float b, float stepSize);
        void stepToRGB(float r, float g, float b);

        float red();
        float green();
        float blue();

    private:
        float _redPin;
        float _greenPin;
        float _bluePin;
        float _red;
        float _green;
        float _blue;
};

#endif

