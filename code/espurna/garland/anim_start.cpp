#include "anim.h"

void Anim::animStart_SetUp() {
    phase = 0;
}

void Anim::animStart_Run() {
    if (phase < LEDS) {
        leds[phase].r = 255;
        leds[phase].g = 255;
        leds[phase].b = 255;
        for(int i=0; i<LEDS; i++) {
            leds[i].fade(50);
        }        
    } else if (phase >= LEDS) 
    {
        for(int i=0; i<LEDS; i++) {
            short r = LEDS + 255 - phase + rngb();
            r = min(r,(short)255); leds[i].r = (byte)max(r,(short)0);
            short g = LEDS + 255 - phase + rngb();
            g = min(g,(short)255); leds[i].g = (byte)max(g,(short)0);
            short b = LEDS + 255 - phase + rngb();
            b = min(b,(short)255); leds[i].b = (byte)max(b,(short)0);
        }
        phase++;
    }

    phase++;
}
