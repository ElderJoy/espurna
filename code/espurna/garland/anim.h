#pragma once

#include "color.h"

#define BRA_AMP_SHIFT          1    // brigthness animation amplitude shift. true BrA amplitude is calculated 
                                    // as (0..127) value shifted right by this amount
#define BRA_OFFSET           127    //(222-64) // brigthness animation amplitude offset

class Palette;

class Anim {
public:
    Anim(const char* name);
    const char* name() { return _name; }
    void Setup(Palette* palette, uint16_t numLeds, Color* leds, Color* _ledstmp, byte* seq);
    virtual bool finishedycle() const { return true; };
    virtual void Run() = 0;

protected:
    uint16_t    numLeds     = 0;
    Palette*    palette     = nullptr;
    Color*      leds        = nullptr;
    Color*      ledstmp     = nullptr;
    byte*       seq         = nullptr;

    int         phase;
    int         pos;
    int         inc;

    //brigthness animation (BrA) current initial phase
    byte        braPhase;
    //braPhase change speed
    byte        braPhaseSpd  = 5;
    //BrA frequency (spatial)
    byte        braFreq      = 150;

    Color       curColor     = Color(0);
    Color       prevColor    = Color(0);
    const Color sparkleColor = Color(0xFFFFFF);

    virtual void SetupImpl() = 0;

    //glow animation setup
    void glowSetUp();

    //glow animation - must be called for each LED after it's BASIC color is set
    //note this overwrites the LED color, so the glow assumes that color will be stored elsewhere (not in leds[])
    //or computed each time regardless previous leds[] value
    void glowForEachLed(int i);

    //glow animation - must be called at the end of each animaton run
    void glowRun();

private:
    const char* _name;
};

unsigned int rng();
byte         rngb();
