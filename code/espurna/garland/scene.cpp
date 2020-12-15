#if GARLAND_SUPPORT

#include "scene.h"

#include <Adafruit_NeoPixel.h>

#include "brightness.h"
#include "color.h"
#include "palette.h"

//=============================================================================
// Scene
//=============================================================================

Scene::Scene(Adafruit_NeoPixel* pixels)
    : _pixels(pixels),
      _numLeds(pixels->numPixels()),
      _leds1(_numLeds),
      _leds2(_numLeds),
      _ledstmp(_numLeds),
      _seq(_numLeds) {
}

void Scene::setPalette(Palette* palette) {
    _palette = palette;
    if (setUpOnPalChange) {
        setupImpl();
    }
}

void Scene::setBrightness(byte brightness) { this->brightness = brightness; }

byte Scene::getBrightness() { return brightness; }

bool Scene::run() {
    unsigned long iteration_start_time = micros();

    if (_anim) {
        _anim->Run();
    }

    sum_calc_time += (micros() - iteration_start_time);
    iteration_start_time = micros();
    ++calc_num;

    // transition coef, if within 0..1 - transition is active
    // changes from 1 to 0 during transition, so we interpolate from current
    // color to previous
    float transc = (float)((long)transms - (long)millis()) / TRANSITION_MS;
    Color* leds_prev = (_leds == &_leds1[0]) ? &_leds2[0] : &_leds1[0];

    if (transc > 0) {
        for (int i = 0; i < _numLeds; i++) {
            // transition is in progress
            Color c = _leds[i].interpolate(leds_prev[i], transc);
            byte r = (int)pgm_read_byte_near(BRI + c.r) * brightness / 256;
            byte g = (int)pgm_read_byte_near(BRI + c.g) * brightness / 256;
            byte b = (int)pgm_read_byte_near(BRI + c.b) * brightness / 256;
            _pixels->setPixelColor(i, _pixels->Color(r, g, b));
        }
    } else {
        for (int i = 0; i < _numLeds; i++) {
            // regular operation
            byte r =
                (int)pgm_read_byte_near(BRI + _leds[i].r) * brightness / 256;
            byte g =
                (int)pgm_read_byte_near(BRI + _leds[i].g) * brightness / 256;
            byte b =
                (int)pgm_read_byte_near(BRI + _leds[i].b) * brightness / 256;
            _pixels->setPixelColor(i, _pixels->Color(r, g, b));
        }
    }

    sum_pixl_time += (micros() - iteration_start_time);
    iteration_start_time = micros();
    ++pixl_num;
    

    _pixels->show();
    sum_show_time += (micros() - iteration_start_time);
    ++show_num;

    return true;
}

void Scene::setupImpl() {
    transms = millis() + TRANSITION_MS;

    // switch operation buffers (for transition to operate)
    if (_leds == &_leds1[0]) {
        _leds = &_leds2[0];
    } else {
        _leds = &_leds1[0];
    }

    if (_anim) {
        _anim->Setup(_palette, _numLeds, _leds, &_ledstmp[0], &_seq[0]);
    }
}

void Scene::setup() {
    sum_calc_time = 0;
    sum_pixl_time = 0;
    sum_show_time = 0;
    calc_num = 0;
    pixl_num = 0;
    show_num = 0;

    if (!setUpOnPalChange) {
        setupImpl();
    }
}

unsigned long Scene::getAvgCalcTime() { return sum_calc_time / calc_num; }
unsigned long Scene::getAvgPixlTime() { return sum_pixl_time / pixl_num; }
unsigned long Scene::getAvgShowTime() { return sum_show_time / show_num; }

unsigned int rng() {
    static unsigned int y = 0;
    y += micros();  // seeded with changing number
    y ^= y << 2;
    y ^= y >> 7;
    y ^= y << 7;
    return (y);
}

byte rngb() { return (byte)rng(); }

//=============================================================================
// Scene::Anim
//=============================================================================
Scene::Anim::Anim(String name) : _name(name) {}

void Scene::Anim::Setup(Palette* palette, uint16_t numLeds, Color* leds, Color* ledstmp, byte* seq) {
    this->palette = palette;
    this->numLeds = numLeds;
    this->leds = leds;
    this->ledstmp = ledstmp;
    this->seq = seq;
    SetupImpl();
}

void Scene::Anim::glowSetUp() {
    braPhaseSpd = secureRandom(4, 13);
    if (braPhaseSpd > 8) {
        braPhaseSpd = braPhaseSpd - 17;
    }
    braFreq = secureRandom(20, 60);
}

void Scene::Anim::glowForEachLed(int i) {
    int8 bra = braPhase + i * braFreq;
    bra = BRA_OFFSET + (abs(bra) >> BRA_AMP_SHIFT);
    leds[i] = leds[i].brightness(bra);
}

void Scene::Anim::glowRun() { braPhase += braPhaseSpd; }

#endif  // GARLAND_SUPPORT
