/*
  WS2812FX.h - Library for WS2812 LED effects.
  Harm Aldick - 2016
  www.aldick.org
  LICENSE
  The MIT License (MIT)
  Copyright (c) 2016  Harm Aldick
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

  Modified for WLED
*/

#ifndef WS2812FX_h
#define WS2812FX_h

#include "const.h"

#define FASTLED_INTERNAL //remove annoying pragma messages
#define USE_GET_MILLISECOND_TIMER
#include "FastLED.h"

#define DEFAULT_BRIGHTNESS (uint8_t)127
#define DEFAULT_MODE       (uint8_t)0
#define DEFAULT_SPEED      (uint8_t)128
#define DEFAULT_INTENSITY  (uint8_t)128
#define DEFAULT_COLOR      (uint32_t)0xFFAA00

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

//color mangling macros
#ifndef RGBW32
#define RGBW32(r,g,b,w) (uint32_t((byte(w) << 24) | (byte(r) << 16) | (byte(g) << 8) | (byte(b))))
#endif

/* Not used in all effects yet */
#define WLED_FPS         42
#define FRAMETIME_FIXED  (1000/WLED_FPS)
#define FRAMETIME        _frametime

/* each segment uses 52 bytes of SRAM memory, so if you're application fails because of
  insufficient memory, decreasing MAX_NUM_SEGMENTS may help */
#ifdef ESP8266
  #define MAX_NUM_SEGMENTS    16
  /* How many color transitions can run at once */
  #define MAX_NUM_TRANSITIONS  8
  /* How much data bytes all segments combined may allocate */
  #define MAX_SEGMENT_DATA  4096
#else
  #ifndef MAX_NUM_SEGMENTS
    #define MAX_NUM_SEGMENTS  32
  #endif
  #define MAX_NUM_TRANSITIONS 24
  #define MAX_SEGMENT_DATA  20480
#endif

/* How much data bytes each segment should max allocate to leave enough space for other segments,
  assuming each segment uses the same amount of data. 256 for ESP8266, 640 for ESP32. */
#define FAIR_DATA_PER_SEG (MAX_SEGMENT_DATA / MAX_NUM_SEGMENTS)

#define MIN_SHOW_DELAY   (_frametime < 16 ? 8 : 15)

#define NUM_COLORS       3 /* number of colors per segment */
#define SEGMENT          _segments[_segment_index]
#define SEGCOLOR(x)      _colors_t[x]
#define SEGENV           _segment_runtimes[_segment_index]
#define SEGLEN           _virtualSegmentLength
#define SEGACT           SEGMENT.stop
#define SPEED_FORMULA_L  5U + (50U*(255U - SEGMENT.speed))/SEGLEN

// some common colors
#define RED        (uint32_t)0xFF0000
#define GREEN      (uint32_t)0x00FF00
#define BLUE       (uint32_t)0x0000FF
#define WHITE      (uint32_t)0xFFFFFF
#define BLACK      (uint32_t)0x000000
#define YELLOW     (uint32_t)0xFFFF00
#define CYAN       (uint32_t)0x00FFFF
#define MAGENTA    (uint32_t)0xFF00FF
#define PURPLE     (uint32_t)0x400080
#define ORANGE     (uint32_t)0xFF3000
#define PINK       (uint32_t)0xFF1493
#define ULTRAWHITE (uint32_t)0xFFFFFFFF

// options
// bit    7: segment is in transition mode
// bits 4-6: TBD
// bit    3: mirror effect within segment
// bit    2: segment is on
// bit    1: reverse segment
// bit    0: segment is selected
#define NO_OPTIONS   (uint8_t)0x00
#define TRANSITIONAL (uint8_t)0x80
#define MIRROR       (uint8_t)0x08
#define SEGMENT_ON   (uint8_t)0x04
#define REVERSE      (uint8_t)0x02
#define SELECTED     (uint8_t)0x01
#define IS_TRANSITIONAL ((SEGMENT.options & TRANSITIONAL) == TRANSITIONAL)
#define IS_MIRROR       ((SEGMENT.options & MIRROR      ) == MIRROR      )
#define IS_SEGMENT_ON   ((SEGMENT.options & SEGMENT_ON  ) == SEGMENT_ON  )
#define IS_REVERSE      ((SEGMENT.options & REVERSE     ) == REVERSE     )
#define IS_SELECTED     ((SEGMENT.options & SELECTED    ) == SELECTED    )

#define MODE_COUNT  41

enum FXS {
FX_MODE_STATIC,         //"Solid"
FX_MODE_BLINK,          //"Blink"
FX_MODE_BREATH,         //"Breathe"
FX_MODE_RANDOM_COLOR,         //"Random Colors"
FX_MODE_RAINBOW,          //"Colorloop"
FX_MODE_RAINBOW_CYCLE,          //"Rainbow"
FX_MODE_FADE,         //"Fade"
FX_MODE_RUNNING_LIGHTS,         //"Running"
FX_MODE_SAW,          //"Saw"
FX_MODE_DISSOLVE,         //"Dissolve"
FX_MODE_DISSOLVE_RANDOM,          //"Dissolve Rnd"
FX_MODE_HYPER_SPARKLE,          //"Sparkle+"
FX_MODE_STROBE,         //"Strobe"
FX_MODE_STROBE_RAINBOW,         //"Strobe Rainbow"
FX_MODE_MULTI_STROBE,         //"Strobe Mega"
FX_MODE_BLINK_RAINBOW,          //"Blink Rainbow"
FX_MODE_LARSON_SCANNER,         //"Scanner"
FX_MODE_FIREWORKS,          //"Fireworks"
FX_MODE_FIRE_FLICKER,         //"Fire Flicker"
FX_MODE_FAIRY,          //"Fairy"
FX_MODE_FAIRYTWINKLE,         //"Fairytwinkle"
FX_MODE_TRICOLOR_WIPE,          //"Tri Wipe"
FX_MODE_TRICOLOR_FADE,          //"Tri Fade"
FX_MODE_LIGHTNING,          //"Lightning"
FX_MODE_DUAL_LARSON_SCANNER,          //"Scanner Dual"
FX_MODE_PRIDE_2015,         //"Pride 2015"
FX_MODE_FIRE_2012,          //"Fire 2012"
FX_MODE_COLORWAVES,         //"Colorwaves"
FX_MODE_FILLNOISE8,         //"Fill Noise"
FX_MODE_COLORTWINKLE,         //"Colortwinkles"
FX_MODE_LAKE,         //"Lake"
FX_MODE_TWINKLEFOX,         //"Twinklefox"
FX_MODE_TWINKLECAT,         //"Twinklecat"
FX_MODE_CANDLE,         //"Candle"
FX_MODE_HEARTBEAT,          //"Heartbeat"
FX_MODE_PACIFICA,         //"Pacifica"
FX_MODE_SUNRISE,          //"Sunrise"
FX_MODE_NOISEPAL,         //"Noise Pal"
FX_MODE_FLOW,         //"Flow"
FX_MODE_CANDY_CANE,         //"Candy Cane"
FX_MODE_DYNAMIC_SMOOTH          //"Dynamic Smooth"
};

class WS2812FX {
  typedef uint16_t (WS2812FX::*mode_ptr)(void);

  // pre show callback
  typedef void (*show_callback) (void);

  static WS2812FX* instance;
  
  // segment parameters
  public:
    typedef struct Segment { // 31 (32 in memory) bytes
      uint16_t start;
      uint16_t stop; //segment invalid if stop == 0
      uint16_t offset;
      uint8_t  speed;
      uint8_t  intensity;
      uint8_t  palette;
      uint8_t  mode;
      uint8_t  options; //bit pattern: msb first: transitional needspixelstate tbd tbd (paused) on reverse selected
      uint8_t  grouping, spacing;
      uint8_t  opacity;
      uint32_t colors[NUM_COLORS];
      uint8_t  cct; //0==1900K, 255==10091K
      uint8_t  _capabilities;
      char *name;
      bool setColor(uint8_t slot, uint32_t c, uint8_t segn) { //returns true if changed
        if (slot >= NUM_COLORS || segn >= MAX_NUM_SEGMENTS) return false;
        if (c == colors[slot]) return false;
        uint8_t b = (slot == 1) ? cct : opacity;
        ColorTransition::startTransition(b, colors[slot], instance->_transitionDur, segn, slot);
        colors[slot] = c; return true;
      }
      void setCCT(uint16_t k, uint8_t segn) {
        if (segn >= MAX_NUM_SEGMENTS) return;
        if (k > 255) { //kelvin value, convert to 0-255
          if (k < 1900)  k = 1900;
          if (k > 10091) k = 10091;
          k = (k - 1900) >> 5;
        }
        if (cct == k) return;
        ColorTransition::startTransition(cct, colors[1], instance->_transitionDur, segn, 1);
        cct = k;
      }
      void setOpacity(uint8_t o, uint8_t segn) {
        if (segn >= MAX_NUM_SEGMENTS) return;
        if (opacity == o) return;
        ColorTransition::startTransition(opacity, colors[0], instance->_transitionDur, segn, 0);
        opacity = o;
      }
      void setOption(uint8_t n, bool val, uint8_t segn = 255)
      {
        bool prevOn = false;
        if (n == SEG_OPTION_ON) {
          prevOn = getOption(SEG_OPTION_ON);
          if (!val && prevOn) { //fade off
            ColorTransition::startTransition(opacity, colors[0], instance->_transitionDur, segn, 0);
          }
        }

        if (val) {
          options |= 0x01 << n;
        } else
        {
          options &= ~(0x01 << n);
        }

        if (n == SEG_OPTION_ON && val && !prevOn) { //fade on
          ColorTransition::startTransition(0, colors[0], instance->_transitionDur, segn, 0);
        }
      }
      bool getOption(uint8_t n)
      {
        return ((options >> n) & 0x01);
      }
      inline bool isSelected()
      {
        return getOption(0);
      }
      inline bool isActive()
      {
        return stop > start;
      }
      inline uint16_t length()
      {
        return stop - start;
      }
      inline uint16_t groupLength()
      {
        return grouping + spacing;
      }
      uint16_t virtualLength()
      {
        uint16_t groupLen = groupLength();
        uint16_t vLength = (length() + groupLen - 1) / groupLen;
        if (options & MIRROR)
          vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a single LED
        return vLength;
      }
      uint8_t differs(Segment& b);
      inline uint8_t getLightCapabilities() {return _capabilities;}
      void refreshLightCapabilities();
    } segment;

  // segment runtime parameters
    typedef struct Segment_runtime { // 28 bytes
      unsigned long next_time;  // millis() of next update
      uint32_t step;  // custom "step" var
      uint32_t call;  // call counter
      uint16_t aux0;  // custom var
      uint16_t aux1;  // custom var
      byte* data = nullptr;
      bool allocateData(uint16_t len){
        if (data && _dataLen == len) return true; //already allocated
        deallocateData();
        if (WS2812FX::instance->_usedSegmentData + len > MAX_SEGMENT_DATA) return false; //not enough memory
        // if possible use SPI RAM on ESP32
        #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_PSRAM)
        if (psramFound())
          data = (byte*) ps_malloc(len);
        else
        #endif
          data = (byte*) malloc(len);
        if (!data) return false; //allocation failed
        WS2812FX::instance->_usedSegmentData += len;
        _dataLen = len;
        memset(data, 0, len);
        return true;
      }
      void deallocateData(){
        free(data);
        data = nullptr;
        WS2812FX::instance->_usedSegmentData -= _dataLen;
        _dataLen = 0;
      }

      /** 
       * If reset of this segment was request, clears runtime
       * settings of this segment.
       * Must not be called while an effect mode function is running
       * because it could access the data buffer and this method 
       * may free that data buffer.
       */
      void resetIfRequired() {
        if (_requiresReset) {
          next_time = 0; step = 0; call = 0; aux0 = 0; aux1 = 0; 
          deallocateData();
          _requiresReset = false;
        }
      }

      /** 
       * Flags that before the next effect is calculated,
       * the internal segment state should be reset. 
       * Call resetIfRequired before calling the next effect function.
       * Safe to call from interrupts and network requests.
       */
      inline void markForReset() { _requiresReset = true; }
      private:
        uint16_t _dataLen = 0;
        bool _requiresReset = false;
    } segment_runtime;

    typedef struct ColorTransition { // 12 bytes
      uint32_t colorOld = 0;
      uint32_t transitionStart;
      uint16_t transitionDur;
      uint8_t segment = 0xFF; //lower 6 bits: the segment this transition is for (255 indicates transition not in use/available) upper 2 bits: color channel
      uint8_t briOld = 0;
      static void startTransition(uint8_t oldBri, uint32_t oldCol, uint16_t dur, uint8_t segn, uint8_t slot) {
        if (segn >= MAX_NUM_SEGMENTS || slot >= NUM_COLORS || dur == 0) return;
        if (instance->_brightness == 0) return; //do not need transitions if master bri is off
        if (!instance->_segments[segn].getOption(SEG_OPTION_ON)) return; //not if segment is off either
        uint8_t tIndex = 0xFF; //none found
        uint16_t tProgression = 0;
        uint8_t s = segn + (slot << 6); //merge slot and segment into one byte

        for (uint8_t i = 0; i < MAX_NUM_TRANSITIONS; i++) {
          uint8_t tSeg = instance->transitions[i].segment;
          //see if this segment + color already has a running transition
          if (tSeg == s) {
            tIndex = i; break;
          }
          if (tSeg == 0xFF) { //free transition
            tIndex = i; tProgression = 0xFFFF;
          }
        }

        if (tIndex == 0xFF) { //no slot found yet
          for (uint8_t i = 0; i < MAX_NUM_TRANSITIONS; i++) {
            //find most progressed transition to overwrite
            uint16_t prog = instance->transitions[i].progress();
            if (prog > tProgression) {
              tIndex = i; tProgression = prog;
            }
          }
        }

        ColorTransition& t = instance->transitions[tIndex];
        if (t.segment == s) //this is an active transition on the same segment+color
        {
          bool wasTurningOff = (oldBri == 0);
          t.briOld = t.currentBri(wasTurningOff, slot);
          t.colorOld = t.currentColor(oldCol);
        } else {
          t.briOld = oldBri;
          t.colorOld = oldCol;
          uint8_t prevSeg = t.segment & 0x3F;
          if (prevSeg < MAX_NUM_SEGMENTS) instance->_segments[prevSeg].setOption(SEG_OPTION_TRANSITIONAL, false);
        }
        t.transitionDur = dur;
        t.transitionStart = millis();
        t.segment = s;
        instance->_segments[segn].setOption(SEG_OPTION_TRANSITIONAL, true);
        //refresh immediately, required for Solid mode
        if (instance->_segment_runtimes[segn].next_time > t.transitionStart + 22) instance->_segment_runtimes[segn].next_time = t.transitionStart;
      }
      uint16_t progress(bool allowEnd = false) { //transition progression between 0-65535
        uint32_t timeNow = millis();
        if (timeNow - transitionStart > transitionDur) {
          if (allowEnd) {
            uint8_t segn = segment & 0x3F;
            if (segn < MAX_NUM_SEGMENTS) instance->_segments[segn].setOption(SEG_OPTION_TRANSITIONAL, false);
            segment = 0xFF;
          }
          return 0xFFFF;
        }
        uint32_t elapsed = timeNow - transitionStart;
        uint32_t prog = elapsed * 0xFFFF / transitionDur;
        return (prog > 0xFFFF) ? 0xFFFF : prog;
      }
      uint32_t currentColor(uint32_t colorNew) {
        return instance->color_blend(colorOld, colorNew, progress(true), true);
      }
      uint8_t currentBri(bool turningOff = false, uint8_t slot = 0) {
        uint8_t segn = segment & 0x3F;
        if (segn >= MAX_NUM_SEGMENTS) return 0;
        uint8_t briNew = instance->_segments[segn].opacity;
        if (slot == 0) {
          if (!instance->_segments[segn].getOption(SEG_OPTION_ON) || turningOff) briNew = 0;
        } else { //transition slot 1 brightness for CCT transition
          briNew = instance->_segments[segn].cct;
        }
        uint32_t prog = progress() + 1;
        return ((briNew * prog) + (briOld * (0x10000 - prog))) >> 16;
      }
    } color_transition;

    WS2812FX() {
      WS2812FX::instance = this;
      //assign each member of the _mode[] array to its respective function reference 
      _mode[FX_MODE_STATIC]                  = &WS2812FX::mode_static;
      _mode[FX_MODE_BLINK]                   = &WS2812FX::mode_blink;
      _mode[FX_MODE_BREATH]                  = &WS2812FX::mode_breath;
      _mode[FX_MODE_RANDOM_COLOR]            = &WS2812FX::mode_random_color;
      _mode[FX_MODE_RAINBOW]                 = &WS2812FX::mode_rainbow;
      _mode[FX_MODE_RAINBOW_CYCLE]           = &WS2812FX::mode_rainbow_cycle;
      _mode[FX_MODE_FADE]                    = &WS2812FX::mode_fade;
      _mode[FX_MODE_RUNNING_LIGHTS]          = &WS2812FX::mode_running_lights;
      _mode[FX_MODE_SAW]                     = &WS2812FX::mode_saw;
      _mode[FX_MODE_DISSOLVE]                = &WS2812FX::mode_dissolve;
      _mode[FX_MODE_DISSOLVE_RANDOM]         = &WS2812FX::mode_dissolve_random;
      _mode[FX_MODE_HYPER_SPARKLE]           = &WS2812FX::mode_hyper_sparkle;
      _mode[FX_MODE_STROBE]                  = &WS2812FX::mode_strobe;
      _mode[FX_MODE_STROBE_RAINBOW]          = &WS2812FX::mode_strobe_rainbow;
      _mode[FX_MODE_MULTI_STROBE]            = &WS2812FX::mode_multi_strobe;
      _mode[FX_MODE_BLINK_RAINBOW]           = &WS2812FX::mode_blink_rainbow;
      _mode[FX_MODE_LARSON_SCANNER]          = &WS2812FX::mode_larson_scanner;
      _mode[FX_MODE_FIREWORKS]               = &WS2812FX::mode_fireworks;
      _mode[FX_MODE_FIRE_FLICKER]            = &WS2812FX::mode_fire_flicker;
      _mode[FX_MODE_FAIRY]                   = &WS2812FX::mode_fairy;
      _mode[FX_MODE_FAIRYTWINKLE]            = &WS2812FX::mode_fairytwinkle;
      _mode[FX_MODE_TRICOLOR_WIPE]           = &WS2812FX::mode_tricolor_wipe;
      _mode[FX_MODE_TRICOLOR_FADE]           = &WS2812FX::mode_tricolor_fade;
      _mode[FX_MODE_LIGHTNING]               = &WS2812FX::mode_lightning;
      _mode[FX_MODE_DUAL_LARSON_SCANNER]     = &WS2812FX::mode_dual_larson_scanner;
      _mode[FX_MODE_PRIDE_2015]              = &WS2812FX::mode_pride_2015;
      _mode[FX_MODE_FIRE_2012]               = &WS2812FX::mode_fire_2012;
      _mode[FX_MODE_COLORWAVES]              = &WS2812FX::mode_colorwaves;
      _mode[FX_MODE_FILLNOISE8]              = &WS2812FX::mode_fillnoise8;
      _mode[FX_MODE_COLORTWINKLE]            = &WS2812FX::mode_colortwinkle;
      _mode[FX_MODE_LAKE]                    = &WS2812FX::mode_lake;
      _mode[FX_MODE_TWINKLEFOX]              = &WS2812FX::mode_twinklefox;
      _mode[FX_MODE_TWINKLECAT]              = &WS2812FX::mode_twinklecat;
      _mode[FX_MODE_CANDLE]                  = &WS2812FX::mode_candle;
      _mode[FX_MODE_HEARTBEAT]               = &WS2812FX::mode_heartbeat;
      _mode[FX_MODE_PACIFICA]                = &WS2812FX::mode_pacifica;
      _mode[FX_MODE_SUNRISE]                 = &WS2812FX::mode_sunrise;
      _mode[FX_MODE_NOISEPAL]                = &WS2812FX::mode_noisepal;
      _mode[FX_MODE_FLOW]                    = &WS2812FX::mode_flow;
      _mode[FX_MODE_CANDY_CANE]              = &WS2812FX::mode_candy_cane;
      _mode[FX_MODE_DYNAMIC_SMOOTH]          = &WS2812FX::mode_dynamic_smooth;

      _brightness = DEFAULT_BRIGHTNESS;
      currentPalette = CRGBPalette16(CRGB::Black);
      targetPalette = CloudColors_p;
      ablMilliampsMax = ABL_MILLIAMPS_DEFAULT;
      currentMilliamps = 0;
      timebase = 0;
      resetSegments();
    }

    void
      finalizeInit(),
      service(void),
      blur(uint8_t),
      fill(uint32_t),
      fade_out(uint8_t r),
      setMode(uint8_t segid, uint8_t m),
      setColor(uint8_t slot, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0),
      setColor(uint8_t slot, uint32_t c),
      setCCT(uint16_t k),
      setBrightness(uint8_t b, bool direct = false),
      setRange(uint16_t i, uint16_t i2, uint32_t col),
      setShowCallback(show_callback cb),
      setTransition(uint16_t t),
      setTransitionMode(bool t),
      calcGammaTable(float),
      trigger(void),
      setSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t grouping = 0, uint8_t spacing = 0, uint16_t offset = UINT16_MAX),
      setMainSegmentId(uint8_t n),
      restartRuntime(),
      resetSegments(),
      makeAutoSegments(bool forceReset = false),
      fixInvalidSegments(),
      setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0),
      show(void),
			setTargetFps(uint8_t fps),
      deserializeMap(uint8_t n=0);

    inline void setPixelColor(uint16_t n, uint32_t c) {setPixelColor(n, byte(c>>16), byte(c>>8), byte(c), byte(c>>24));}

    bool
      gammaCorrectBri = false,
      gammaCorrectCol = true,
      checkSegmentAlignment(void),
      hasRGBWBus(void),
      hasCCTBus(void),
      // return true if the strip is being sent pixel updates
      isUpdating(void);

    uint8_t
      paletteFade = 0,
      paletteBlend = 0,
      milliampsPerLed = 55,
      autoWhiteMode = RGBW_MODE_DUAL,
      cctBlending = 0,
      getBrightness(void),
      getModeCount(void),
      getPaletteCount(void),
      getMaxSegments(void),
      getActiveSegmentsNum(void),
      getFirstSelectedSegId(void),
      getMainSegmentId(void),
      getLastActiveSegmentId(void),
      getTargetFps(void),
      setPixelSegment(uint8_t n),
      gamma8(uint8_t),
      gamma8_cal(uint8_t, float),
      get_random_wheel_index(uint8_t);

    inline uint8_t sin_gap(uint16_t in) {
      if (in & 0x100) return 0;
      return sin8(in + 192); // correct phase shift of sine so that it starts and stops at 0
    }

    int8_t
      tristate_square8(uint8_t x, uint8_t pulsewidth, uint8_t attdec);

    uint16_t
      ablMilliampsMax,
      currentMilliamps,
      triwave16(uint16_t),
      getLengthTotal(void),
      getLengthPhysical(void),
      getFps();

    uint32_t
      now,
      timebase,
      color_wheel(uint8_t),
      color_from_palette(uint16_t, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri = 255),
      color_blend(uint32_t,uint32_t,uint16_t,bool b16=false),
      currentColor(uint32_t colorNew, uint8_t tNr),
      gamma32(uint32_t),
      getLastShow(void),
      getPixelColor(uint16_t);

    WS2812FX::Segment
      &getSegment(uint8_t n),
      &getFirstSelectedSeg(void),
      &getMainSegment(void);

    WS2812FX::Segment*
      getSegments(void);

    // builtin modes
    uint16_t
      mode_static(void),
      mode_blink(void),
      mode_blink_rainbow(void),
      mode_strobe(void),
      mode_strobe_rainbow(void),
      mode_color_wipe(void),
      mode_color_sweep(void),
      mode_color_wipe_random(void),
      mode_color_sweep_random(void),
      mode_random_color(void),
      mode_dynamic(void),
      mode_breath(void),
      mode_fade(void),
      mode_scan(void),
      mode_dual_scan(void),
      mode_theater_chase(void),
      mode_theater_chase_rainbow(void),
      mode_rainbow(void),
      mode_rainbow_cycle(void),
      mode_running_lights(void),
      mode_saw(void),
      mode_twinkle(void),
      mode_my_dissolve(void),
      mode_dissolve(void),
      mode_dissolve_random(void),
      mode_sparkle(void),
      mode_flash_sparkle(void),
      mode_hyper_sparkle(void),
      mode_multi_strobe(void),
      mode_android(void),
      mode_chase_color(void),
      mode_chase_random(void),
      mode_chase_rainbow(void),
      mode_chase_flash(void),
      mode_chase_flash_random(void),
      mode_chase_rainbow_white(void),
      mode_colorful(void),
      mode_traffic_light(void),
      mode_running_color(void),
      mode_aurora(void),
      mode_running_random(void),
      mode_larson_scanner(void),
      mode_comet(void),
      mode_fireworks(void),
      mode_rain(void),
      mode_tetrix(void),
      mode_halloween(void),
      mode_fire_flicker(void),
      mode_gradient(void),
      mode_loading(void),
      mode_police(void),
      mode_fairy(void),
      mode_two_dots(void),
      mode_fairytwinkle(void),
      mode_running_dual(void),
      mode_bicolor_chase(void),
      mode_tricolor_chase(void),
      mode_tricolor_wipe(void),
      mode_tricolor_fade(void),
      mode_lightning(void),
      mode_icu(void),
      mode_multi_comet(void),
      mode_dual_larson_scanner(void),
      mode_random_chase(void),
      mode_oscillate(void),
      mode_fire_2012(void),
      mode_pride_2015(void),
      mode_bpm(void),
      mode_juggle(void),
      mode_palette(void),
      mode_colorwaves(void),
      mode_fillnoise8(void),
      mode_noise16_1(void),
      mode_noise16_2(void),
      mode_noise16_3(void),
      mode_noise16_4(void),
      mode_colortwinkle(void),
      mode_lake(void),
      mode_meteor(void),
      mode_meteor_smooth(void),
      mode_railway(void),
      mode_ripple(void),
      mode_twinklefox(void),
      mode_twinklecat(void),
      mode_halloween_eyes(void),
      mode_static_pattern(void),
      mode_tri_static_pattern(void),
      mode_spots(void),
      mode_spots_fade(void),
      mode_glitter(void),
      mode_candle(void),
      mode_starburst(void),
      mode_exploding_fireworks(void),
      mode_bouncing_balls(void),
      mode_sinelon(void),
      mode_sinelon_dual(void),
      mode_sinelon_rainbow(void),
      mode_popcorn(void),
      mode_drip(void),
      mode_plasma(void),
      mode_percent(void),
      mode_ripple_rainbow(void),
      mode_heartbeat(void),
      mode_pacifica(void),
      mode_candle_multi(void),
      mode_solid_glitter(void),
      mode_sunrise(void),
      mode_phased(void),
      mode_twinkleup(void),
      mode_noisepal(void),
      mode_sinewave(void),
      mode_phased_noise(void),
      mode_flow(void),
      mode_chunchun(void),
      mode_dancing_shadows(void),
      mode_washing_machine(void),
      mode_candy_cane(void),
      mode_blends(void),
      mode_tv_simulator(void),
      mode_dynamic_smooth(void);

  private:
    uint32_t crgb_to_col(CRGB fastled);
    CRGB col_to_crgb(uint32_t);
    CRGBPalette16 currentPalette;
    CRGBPalette16 targetPalette;

    uint16_t _length, _virtualSegmentLength;
    uint16_t _rand16seed;
    uint8_t _brightness;
    uint16_t _usedSegmentData = 0;
    uint16_t _transitionDur = 750;

		uint8_t _targetFps = 42;
		uint16_t _frametime = (1000/42);
    uint16_t _cumulativeFps = 2;

    bool
      _isOffRefreshRequired = false, //periodic refresh is required for the strip to remain off.
      _hasWhiteChannel = false,
      _triggered;

    mode_ptr _mode[MODE_COUNT]; // SRAM footprint: 4 bytes per element

    show_callback _callback = nullptr;

    // mode helper functions
    uint16_t
      blink(uint32_t, uint32_t, bool strobe, bool),
      candle(bool),
      color_wipe(bool, bool),
      dynamic(bool),
      scan(bool),
      running_base(bool,bool),
      larson_scanner(bool),
      sinelon_base(bool,bool),
      my_dissolve(uint32_t),
      dissolve(uint32_t),
      chase(uint32_t, uint32_t, uint32_t, bool),
      gradient_base(bool),
      ripple_base(bool),
      police_base(uint32_t, uint32_t),
      running(uint32_t, uint32_t, bool theatre=false),
      tricolor_chase(uint32_t, uint32_t),
      twinklefox_base(bool),
      spots_base(uint16_t),
      phased_base(uint8_t);

    CRGB twinklefox_one_twinkle(uint32_t ms, uint8_t salt, bool cat);
    CRGB pacifica_one_layer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);

    void
      blendPixelColor(uint16_t n, uint32_t color, uint8_t blend),
      startTransition(uint8_t oldBri, uint32_t oldCol, uint16_t dur, uint8_t segn, uint8_t slot),
      estimateCurrentAndLimitBri(void),
      load_gradient_palette(uint8_t),
      handle_palette(void);

    uint16_t* customMappingTable = nullptr;
    uint16_t  customMappingSize  = 0;
    
    uint32_t _lastPaletteChange = 0;
    uint32_t _lastShow = 0;

    uint32_t _colors_t[3];
    uint8_t _bri_t;
    bool _no_rgb = false;
    
    uint8_t _segment_index = 0;
    uint8_t _segment_index_palette_last = 99;
    uint8_t _mainSegment;

    segment _segments[MAX_NUM_SEGMENTS] = { // SRAM footprint: 24 bytes per element
      // start, stop, offset, speed, intensity, palette, mode, options, grouping, spacing, opacity (unused), color[], capabilities
      {0, 7, 0, DEFAULT_SPEED, 128, 0, DEFAULT_MODE, NO_OPTIONS, 1, 0, 255, {DEFAULT_COLOR}, 0}
    };
    segment_runtime _segment_runtimes[MAX_NUM_SEGMENTS]; // SRAM footprint: 28 bytes per element
    friend class Segment_runtime;

    ColorTransition transitions[MAX_NUM_TRANSITIONS]; //12 bytes per element
    friend class ColorTransition;

    uint16_t
      transitionProgress(uint8_t tNr);
  
  public:
    inline bool hasWhiteChannel(void) {return _hasWhiteChannel;}
    inline bool isOffRefreshRequired(void) {return _isOffRefreshRequired;}
};

//10 names per line

  // FX_MODE_STATIC,
  // FX_MODE_BLINK_RAINBOW,
  // FX_MODE_BREATH,
  // FX_MODE_CANDLE,
  // FX_MODE_CANDLE_MULTI,

const char JSON_mode_names[] PROGMEM = "[\"STATIC\",\"BLINK\",\"BREATH\",\"RANDOM_COLOR\",\"RAINBOW\",\"RAINBOW_CYCLE\",\"FADE\",\"RUNNING_LIGHTS\",\"SAW\",\"DISSOLVE\",\"DISSOLVE_RANDOM\",\"HYPER_SPARKLE\",\"STROBE\",\"STROBE_RAINBOW\",\"MULTI_STROBE\",\"BLINK_RAINBOW\",\"LARSON_SCANNER\",\"FIREWORKS\",\"FIRE_FLICKER\",\"FAIRY\",\"FAIRYTWINKLE\",\"TRICOLOR_WIPE\",\"TRICOLOR_FADE\",\"LIGHTNING\",\"DUAL_LARSON_SCANNER\",\"PRIDE_2015\",\"FIRE_2012\",\"COLORWAVES\",\"FILLNOISE8\",\"COLORTWINKLE\",\"LAKE\",\"TWINKLEFOX\",\"TWINKLECAT\",\"CANDLE\",\"HEARTBEAT\",\"PACIFICA\",\"SUNRISE\",\"NOISEPAL\",\"FLOW\",\"CANDY_CANE\",\"DYNAMIC_SMOOTH\"]";


const char JSON_palette_names[] PROGMEM = R"=====([
"Default","* Random Cycle","* Color 1","* Colors 1&2","* Color Gradient","* Colors Only","Party","Cloud","Lava","Ocean",
"Forest","Rainbow","Rainbow Bands","Sunset","Rivendell","Breeze","Red & Blue","Yellowout","Analogous","Splash",
"Pastel","Sunset 2","Beech","Vintage","Departure","Landscape","Beach","Sherbet","Hult","Hult 64",
"Drywet","Jul","Grintage","Rewhi","Tertiary","Fire","Icefire","Cyane","Light Pink","Autumn",
"Magenta","Magred","Yelmag","Yelblu","Orange & Teal","Tiamat","April Night","Orangery","C9","Sakura",
"Aurora","Atlantica","C9 2","C9 New","Temperature","Aurora 2","Retro Clown","Candy","Toxy Reaf","Fairy Reaf",
"Semi Blue","Pink Candy","Red Reaf","Aqua Flash","Yelblu Hot","Lite Light","Red Flash","Blink Red","Red Shift","Red Tide",
"Candy2"
])=====";

#endif
