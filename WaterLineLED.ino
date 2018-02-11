#include <FastLED.h>

#define DATA_PIN   9 // SPI MOSI pin
#define CLOCK_PIN  8 //13 //SPI  SCK

#define COLOR_ORDER BGR  // most of the 10mm black APA102

#define CHIPSET     APA102
#define NUM_LEDS    300

#define FPS 100

//CRGB leds[NUM_LEDS];
CRGBArray<NUM_LEDS> leds;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
int brightness = 255;


uint8_t values[NUM_LEDS];

void setup() {
  delay(1000); // sanity delay

  FastLED.addLeds<CHIPSET, DATA_PIN, CLOCK_PIN, COLOR_ORDER, DATA_RATE_MHZ(8)>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( brightness );

currentPalette = PartyColors_p;
// currentPalette = OceanColors_p;
//  currentPalette = ForestColors_p;
  currentBlending = LINEARBLEND;
//  currentBlending = NOBLEND;
}

void loop()
{
  static uint8_t paletteIndex = 0; // the number of pixels that the pattern phase shifts each time
  // decouple the rate from the index by waiting for some time
  EVERY_N_MILLISECONDS( 80 ) { paletteIndex++; }  
  fill_mirror_from_palette(paletteIndex);
  FastLED.show();
  delayToSyncFrameRate(FPS);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        // the shift along color index changes the frequency of the pattern oscillations
        // higher numbers increase the frequency, shortening the period
        colorIndex += 1; // from PaletteTrace.ino
    }
}


void fill_mirror_from_palette(uint8_t colorIndex)
{
  for(int i=0; i < int(NUM_LEDS/2); i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
    leds[NUM_LEDS - i - 1] = leds[i];
    colorIndex += 1; // the color shift frequency
  }
}


// delayToSyncFrameRate - delay how many milliseconds are needed
//   to maintain a stable frame rate.
static void delayToSyncFrameRate( uint8_t framesPerSecond) {
  static uint32_t msprev = 0;
  uint32_t mscur = millis();
  uint16_t msdelta = mscur - msprev;
  uint16_t mstargetdelta = 1000 / framesPerSecond;
  // Serial.print("frame dt: ");
  // Serial.println(msdelta);
  if ( msdelta < mstargetdelta) {
    FastLED.delay( mstargetdelta - msdelta);
  }
  msprev = mscur;
}
