#if GARLAND_SUPPORT

#include <list>

#include "anims.h"
#include "color.h"
#include "palette.h"
#include "scene.h"

struct Comet {
    float head = secureRandom(0, LEDS / 2);
    int len = secureRandom(10, 20);
    float speed = ((float)secureRandom(4, 10)) / 10;
    Color color;
    int dir = 1;
    Comet(Palette* pal) : color(pal->getRndInterpColor()) {
        // DEBUG_MSG_P(PSTR("[GARLAND] Comet created head = %d len = %d speed = %g cr = %d cg = %d cb = %d\n"), head, len, speed, color.r, color.g, color.b);
        if (secureRandom(10) > 5) {
            head = LEDS - head;
            dir = -1;
        }
    }
};

std::list<Comet> comets;

AnimComets::AnimComets() : Scene::Anim("Comets") {
}

void AnimComets::SetupImpl() {
    comets.clear();
    for (int i = 0; i < 4; ++i)
        comets.emplace_back(palette);
}

void AnimComets::Run() {
    for (int i = 0; i < LEDS; i++) leds[i] = 0;

    for (auto& c : comets) {
        int tail = c.head + c.len * -c.dir;
        // Check if Comet out of range and generate it again
        if ((c.head < 0 && tail < 0) || (c.head >= LEDS && tail >= LEDS)) {
            Comet new_comet(palette);
            std::swap(c, new_comet);
        }

        for (int l = 0; l < c.len; ++l) {
            int p = c.head + l * -c.dir;
            if (p >= 0 && p < LEDS) {
                Color bpc = Color((byte)(c.color.r * (c.len - l) / c.len), (byte)(c.color.g * (c.len - l) / c.len), (byte)(c.color.b * (c.len - l) / c.len));
                if (leds[p].empty()) {
                    leds[p] = bpc;
                } else {
                    leds[p] = leds[p].interpolate(bpc, 0.5);
                }
            }
        }
        c.head = c.head + c.speed * c.dir;
    }
}

#endif  // GARLAND_SUPPORT