#pragma once

#if GARLAND_SUPPORT

#include "../config/general.h"
#include "palette.h"

#include <vector>

constexpr int LEDS = GARLAND_LEDS;
#define DEFAULT_BRIGHTNESS 12  // brightness adjustment, up to 255

#define TRANSITION_MS 1000  // transition time between animations, ms

// brigthness animation amplitude shift. true BrA amplitude is calculated as (0..127) value shifted right by this amount
#define BRA_AMP_SHIFT 1
// brigthness animation amplitude offset
#define BRA_OFFSET 127  //(222-64)

//probability of spark when in idle plase
#define SPARK_PROB 3

class Adafruit_NeoPixel;

class Scene {
public:
    Scene(Adafruit_NeoPixel* pixels);

    class Anim {
    public:
        String getName() { return _name; }
        Anim(String name);
        void Setup(Palette* palette, uint16_t numLeds, Color* leds, Color* ledstmp, byte* seq);
        virtual void Run() = 0;

    protected:
        uint16_t numLeds;
        int phase;
        int pos;
        int inc;
        Palette* palette;
        Color* leds = nullptr;
        Color* ledstmp = nullptr;
        byte* seq = nullptr;

        //brigthness animation (BrA) current initial phase
        byte braPhase;
        //braPhase change speed
        byte braPhaseSpd = 5;
        //BrA frequency (spatial)
        byte braFreq = 150;

        Color curColor = Color(0);
        Color prevColor = Color(0);
        const Color sparkleColor = Color(0xFFFFFF);

        virtual void SetupImpl(){};

        //glow animation setup
        void glowSetUp();

        //glow animation - must be called for each LED after it's BASIC color is set
        //note this overwrites the LED color, so the glow assumes that color will be stored elsewhere (not in leds[])
        //or computed each time regardless previous leds[] value
        void glowForEachLed(int i);

        //glow animation - must be called at the end of each animaton run
        void glowRun();

    private:
        const String _name;
    };

    void setPalette(Palette* pal);
    void setBrightness(byte brightness);
    byte getBrightness();
    void setAnim(Anim* anim) { _anim = anim; }
    bool run();  //returns true if actual change has completed, or false if it's dummy call (previous call was too recent in time)
    void setup();
    unsigned long getAvgCalcTime();
    unsigned long getAvgPixlTime();
    unsigned long getAvgShowTime();

private:
    Adafruit_NeoPixel* _pixels;
    uint16_t numLeds;
    //Color arrays - two for making transition
    std::vector<Color> leds1;
    std::vector<Color> leds2;
    //auxiliary colors array
    std::vector<Color> ledstmp;
    std::vector<byte> seq;

    // array of Color to work with
    Color* leds;
    int paletteInd = 0;
    Palette* palette;

    // millis to transition end
    unsigned long transms;

    int phase;
    int pos;
    int inc;

    byte brightness = DEFAULT_BRIGHTNESS;

    //whether to call SetUp on palette change
    //(some animations require full transition with fade, otherwise the colors would change in a step, some not)
    bool setUpOnPalChange = true;

    Color curColor = Color(0);
    Color prevColor = Color(0);

    Color sparkleColor = Color(0xFFFFFF);

    //brigthness animation (BrA) current initial phase
    byte braPhase;
    //braPhase change speed
    byte braPhaseSpd = 5;
    //BrA frequency (spatial)
    byte braFreq = 150;

    unsigned long sum_calc_time = 0;
    unsigned long sum_pixl_time = 0;
    unsigned long sum_show_time = 0;
    unsigned int calc_num = 0;
    unsigned int show_num = 0;
    unsigned int pixl_num = 0;

    Anim* _anim = nullptr;

    //glow animation setup
    void glowSetUp();

    //glow animation - must be called for each LED after it's BASIC color is set
    //note this overwrites the LED color, so the glow assumes that color will be stored elsewhere (not in leds[])
    //or computed each time regardless previous leds[] value
    void glowForEachLed(int i);

    //glow animation - must be called at the end of each animaton run
    void glowRun();

    void setupImpl();
};

unsigned int rng();

byte rngb();

#endif  // GARLAND_SUPPORT
