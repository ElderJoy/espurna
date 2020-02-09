#include "anims.h"
#include "color.h"
#include "palette.h"
#include "scene.h"

#define SPREAD_MAX_WIDTH 20

AnimSpread::AnimSpread() : Scene::Anim("Spread") {
}

void AnimSpread::SetupImpl() {
    inc = random(2,8);
    memset(seq, 0, LEDS);
}

void AnimSpread::Run() {
    memset(_leds, 0, 3*LEDS);

    for (int i=0;i<LEDS;i++) {
        if (seq[i] > 0) {
            byte width = SPREAD_MAX_WIDTH - seq[i];
            for (int j=i-width;j<=(i+width);j++) {
                Color c = ledstmp[i];
                if (j>=0 && j<LEDS) {
                    _leds[j].r += c.r;
                    _leds[j].g += c.g;
                    _leds[j].b += c.b;
                }
            }
            ledstmp[i].fade(255/SPREAD_MAX_WIDTH);
            seq[i]--;
        }
    }

    if (random(inc) == 0) {
        byte pos = random(0,LEDS); 
        ledstmp[pos] = _palette->getPalColor((float)rngb()/256);
        seq[pos] = SPREAD_MAX_WIDTH;
    }        
}

AnimSpread anim_spread;