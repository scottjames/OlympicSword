/*
  OlympicSword: An Arduino library for Sarasota Pentathlon Light Show.
  Copyright © 2015 Scott James. Use, modification, and distribution are
  subject to the GPL license as described in the accompanying LICENSE file.

  See README for complete attributions.
  
  This work is based on the excellent NeoPixel library from AdaFruit
*/

#include <Adafruit_NeoPixel.h>


// INPUT A0 -- display "slow/low energy" program when A0 = +5V
#define PIN_SLOW   A0

// INPUT A1 -- display "fast/high energy" program when A1 = +5V
#define PIN_FAST   A1

// OUTPUT -- send WS2812 LED data on pin 13, can also see onboard LED flash to show it's working.
#define PIN_LED_STRIP 13

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// 3 Meters with 30 LED per meter.
Adafruit_NeoPixel strip = Adafruit_NeoPixel( (3*30) , PIN_LED_STRIP, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


void setup() {
  // use Serial output for testing/debugging.
  Serial.begin(115200);
  Serial.println(F("Begin"));

  pinMode(PIN_SLOW,INPUT);
  pinMode(PIN_FAST,INPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  randomSeed(analogRead(3));

}


int sequenceFast = 0;
int sequenceSlow = 0;
unsigned long lastFastTime = 0;
unsigned long lastSlowTime = 0;

// if longer idle than timeout seconds, then reset sequence to 0.
// this would allow pause and resume in the same sequence. but pause longer than 10 secs
// would initialize the sequence to start over again when resumed.
#define SEQ_TIMEOUT_SECS 10

void loop() {

  // don't show anything, unless either SLOW of FAST mode is selected.
  // this is done by closing a remote-control relay to bring pin A0 (or A1) to +5V.

  unsigned long currentTime = millis();

  // check SLOW
  if (digitalRead(A0) == HIGH) {
    if ( ((currentTime - lastSlowTime) / 1000) > SEQ_TIMEOUT_SECS ) {
      sequenceSlow = 0;
    }
    Serial.print(F("SLOW "));
    Serial.println(sequenceSlow, DEC);    
    slowSequence(sequenceSlow, strip);
    // run 1 quick loop in SLOW program
    // increment SLOW counter to know which sequence we're on
    lastSlowTime = currentTime;
    Serial.println(F("...done"));

    // if input off, then fade to black.
    if (digitalRead(A0) == LOW) {
      colorWipeReverse(strip, strip.Color(0, 0, 0), 5); // black out.
    }

  }

  // check FAST
  if (digitalRead(A1) == HIGH) {
    if ( ((currentTime - lastFastTime) / 1000) > SEQ_TIMEOUT_SECS ) {
      sequenceFast = 0;
    }
    Serial.print(F("FAST "));
    Serial.println(sequenceFast, DEC);    
    fastSequence(sequenceFast, strip);
    // run 1 quick loop in FAST program
    // increment FAST counter to know which sequence we're on
    lastFastTime = currentTime;
    Serial.println(F("...done"));

    // if input off, then fade to black.
    if (digitalRead(A1) == LOW) {
      colorWipeReverse(strip, strip.Color(0, 0, 0), 5); // black out.
    }

  }

  //delay(100);

} // loop()




// Slow sequence - for low engery display.
// run only one sequence then return (to check inputs)
void slowSequence(int &seq, Adafruit_NeoPixel &strip) {

  switch (seq) {
  case 1:
    colorWipe(strip, strip.Color(255, 255, 255), 10); // white
    break;
  case 2:
    colorWipe(strip, strip.Color(255, 0, 0), 5); // Red
    break;
  case 3:
    colorWipe(strip, strip.Color(0, 255, 0), 5); // Green
    break;
  case 4:
    colorWipe(strip, strip.Color(0, 0, 255), 5); // Blue
    break;

  case 10:
    //IdleRainbowCycleStrobe(strip, strip, 25);  
    rainbowCycleStrobe(strip, 25);
    break;

  case 11:
    //rainbow(strip, 20);
    rainbowCycle(strip, 30);
    break;
  case 12:
    break;
  case 13:
    theaterChaseRainbow(strip, 80);    
    break;

  case 20:
    // do parise eiffel tower, load all read then flash white strobes
    colorWipe(strip, strip.Color(255, 0, 0), 5); // Red

  case 50:
    colorWipeReverse(strip, strip.Color(0, 0, 0), 3); // black out.
    break;
  };

  seq++;
  if (seq > 100) {
    seq = 0;
  }


}

// Fast sequence - for high engery display.
// run only one sequence then return (to check inputs)
void fastSequence(int &seq, Adafruit_NeoPixel &strip) {

  switch (seq) {
  case 1:
    theaterChase(strip, strip.Color(127, 127, 127), 50); // White
    break;
  case 2:
    theaterChase(strip, strip.Color(127,   0,   0), 50); // Red
    break;
  case 3:
    theaterChase(strip, strip.Color(  0,   0, 127), 50); // Blue
    break;
  case 4:
    theaterChase(strip, strip.Color(  0,   127, 0), 50); // Green
    break;
  case 5:
    theaterChaseRainbow(strip, 30);
    break;

// TODO: add more sequences...
  case 10:
    //delay(3000);
    break;

  case 11:
    FireTip(strip, strip.Color(127, 0, 0), 3); // fire red 'bullet' toward tip.
    break;

  };

  seq++;
  if (seq > 100) {
    seq = 0;
  }

}

// ==== OLD STUFF ... test code used for development....

void ZZZtestLights() {
  // run through RGB elements
  colorWipe(strip, strip.Color(255, 0, 0), 2); // Red
  colorWipe(strip, strip.Color(0, 255, 0), 2); // Green
  colorWipe(strip, strip.Color(0, 0, 255), 2); // Blue
  colorWipe(strip, strip.Color(255, 255, 255), 5); // white and power test
  delay(1000); // hold for power test
  colorWipe(strip, strip.Color(0, 0, 0), 5); // black out. 
}



// if FINISH button, then do "big show" RainbowStrobe...
//  rainbowCycleStrobe(stripLeft, stripRight, 5);
//  colorWipe(stripLeft, stripLeft.Color(0, 0, 0), 3); // black out.
//  colorWipe(stripLeft, stripRight.Color(0, 0, 0), 3); // black out.
// TODO: rainbowCycleStrobe(stripRight, 5);
//  delay(3000);

// if LEFT SCORE then fire red toward tip
//  FireTip(stripLeft, stripLeft.Color(127, 0, 0), 5); // fire red 'bullet' toward tip.
// TODO: turn on Tip light, when bullet reaches tip.
//  delay(3000);

/***
 * // if IDLE then run subtle Rainbow Cycle with subtle strobes...
 * IdleRainbowCycleStrobe(strip, strip, 25);  
 * //colorWipe(stripLeft, stripLeft.Color(0, 0, 0), 3); // black out.
 * colorWipeReverse(strip, strip.Color(0, 0, 0), 3); // black out.
 * //colorWipeReverse(stripRight, stripRight.Color(0, 0, 0), 3); // black out.
 * delay(3000);
 * 
 * // flash White strobe, to Black background, time=50, count=200 strobes
 * strobeToBackground(strip, strip.Color(255, 255, 255), strip.Color(0, 0, 0), 50, 200);
 * delay(2000);
 * 
 * loadRainbow(strip, 50);
 * 
 * // flash White strobe, to Black background, time=250, count=75 strobes
 * strobeToBackground(strip, strip.Color(255, 255, 255), strip.Color(0, 0, 0), 250, 75);
 * delay(2000);
 * 
 * //Old_loop();
 ***/




/****
 * void Old_loop() {
 * 
 * 
 * // Some example procedures showing how to display to the pixels:
 * colorWipe(stripLeft, stripLeft.Color(255, 0, 0), 5); // Red
 * colorWipe(stripLeft, stripLeft.Color(0, 255, 0), 5); // Green
 * colorWipe(stripLeft, stripLeft.Color(0, 0, 255), 5); // Blue
 * colorWipe(stripLeft, stripLeft.Color(255, 255, 255), 10); // white
 * delay(5000);
 * 
 * // Send a theater pixel chase in...
 * theaterChase(stripLeft, stripLeft.Color(127, 127, 127), 50); // White
 * theaterChase(stripLeft, stripLeft.Color(127,   0,   0), 50); // Red
 * theaterChase(stripLeft, stripLeft.Color(  0,   0, 127), 50); // Blue
 * 
 * rainbow(stripLeft, 10);
 * rainbowCycle(stripLeft, 10);
 * theaterChaseRainbow(stripLeft, 20);
 * }
 * 
 * 
 * 
 * void OriginalDemoloop() {
 * 
 * 
 * // Some example procedures showing how to display to the pixels:
 * colorWipe(stripLeft, stripLeft.Color(255, 0, 0), 5); // Red
 * colorWipe(stripLeft, stripLeft.Color(0, 255, 0), 5); // Green
 * colorWipe(stripLeft, stripLeft.Color(0, 0, 255), 5); // Blue
 * colorWipe(stripLeft, stripLeft.Color(255, 255, 255), 10); // white
 * delay(5000);
 * 
 * // Send a theater pixel chase in...
 * theaterChase(stripLeft, stripLeft.Color(127, 127, 127), 50); // White
 * theaterChase(stripLeft, stripLeft.Color(127,   0,   0), 50); // Red
 * theaterChase(stripLeft, stripLeft.Color(  0,   0, 127), 50); // Blue
 * 
 * rainbow(stripLeft, 10);
 * rainbowCycle(stripLeft, 10);
 * theaterChaseRainbow(stripLeft, 20);
 * }
 * 
 ****/

// ==== Original sequences from AdaFruit, and new sequences from Scott James

// Fill the dots one after the other with a color
void colorWipe(Adafruit_NeoPixel &strip, uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


// Fill the dots one after the other with a color, fill from end, backwards to beginning.
void colorWipeReverse(Adafruit_NeoPixel &strip, uint32_t c, uint8_t wait) {

  for(uint16_t i=strip.numPixels()-1; i>=0 ; i--) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
    if (i==0) break; // handle for unsigned int which is always > 0.
  }
}


// FireTip - shoot 3 pixel sequence toward end of string.
void FireTip(Adafruit_NeoPixel &strip, uint32_t c, uint8_t wait) {

  uint32_t black = strip.Color(0, 0, 0);

  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.setPixelColor(i+1, c);
    strip.setPixelColor(i+2, c);
    strip.show();
    delay(wait);

    strip.setPixelColor(i, black);
    strip.setPixelColor(i+1, black);
    strip.setPixelColor(i+2, black);
    strip.show();

  }
}


// load rainbow slowly in foward direction
// WARNING: does not match rainbowCycle or chase - must adjust fill pattern for that.
void loadRainbow(Adafruit_NeoPixel &strip, uint8_t wait) {
  uint16_t i;

  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel(strip, (i) & 255));
    strip.show();
    delay(wait);
  }
}



void rainbow(Adafruit_NeoPixel &strip, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(strip, (i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(Adafruit_NeoPixel &strip, uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*1; j++) { // 1 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(strip, ((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


// Slightly different, this makes the rainbow equally distributed throughout
// with white strobe flashes for extra flare
void rainbowCycleStrobe(Adafruit_NeoPixel &strip, uint8_t wait) {
  uint16_t i, j;
  uint32_t col;

  for(j=0; j<256*1; j++) { // 1 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {

      // 5% chance of using white strobe instead of proper Wheel color.
      if ( random(100) < 5 ) {
        col = strip.Color(255, 255, 255);
      } 
      else {
        col = Wheel(strip, ((i * 256 / strip.numPixels()) + j) & 255);
      }

      strip.setPixelColor(i, col);

    }
    strip.show();
    delay(wait);
  }
}



// Slightly different, this makes the rainbow equally distributed throughout
// with white strobe flashes for extra flare
// run LEFT and RIGHT strips together
void IdleRainbowCycleStrobe(Adafruit_NeoPixel &strip, uint8_t wait) {
  uint16_t i, j;
  uint32_t col;

  strip.setBrightness(16); // 1=min, 255=just under max; 0=max.

  for(j=0; j<256*1; j++) { // 1 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {

      // 1% chance of using white strobe instead of proper Wheel color.
      if ( random(100) < 1 ) {
        strip.setBrightness(64); // 1=min, 255=just under max; 0=max.
        col = strip.Color(255, 255, 255);
        strip.setBrightness(16); // 1=min, 255=just under max; 0=max.
      } 
      else {
        col = Wheel(strip, ((i * 256 / strip.numPixels()) + j) & 255);
      }

      strip.setPixelColor(i, col);

    }
    strip.show();
    delay(wait);
  }
}




// strobe flare (fg color) with burn out to background (bg color)
void strobeToBackground(Adafruit_NeoPixel &strip, uint32_t fg, uint32_t bg, uint8_t wait, uint16_t num) {

  for (int j=0; j<num; j++) {  //do num cycles of strobes
    // use random location for next strobe
    int ledStrobe = random(strip.numPixels());
    strip.setPixelColor(ledStrobe, fg);    //turn on strobe color
    strip.show();
    delay(wait);
    strip.setPixelColor(ledStrobe, bg);    //turn on background / burnout color.
    strip.show();  
  }
}



// strobe flare (fg color) with NO burn out (save existing background)
void strobeSaveBackground(Adafruit_NeoPixel &strip, uint32_t fg, uint8_t wait, uint16_t num) {

  uint32_t savePixel; 

  for (int j=0; j<num; j++) {  //do num cycles of strobes

    // use random location for next strobe
    int ledStrobe = random(strip.numPixels());
    savePixel = strip.getPixelColor(ledStrobe);
    strip.setPixelColor(ledStrobe, fg);    //turn on strobe.
    strip.show();
    delay(wait);
    strip.setPixelColor(ledStrobe, savePixel);  // restore background
    strip.show();

  }
}




//Theatre-style crawling lights.
void theaterChase(Adafruit_NeoPixel &strip, uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
// NOTE: this sequence takes around 20 seconds (slow) So, someday check for input inside this loop.
void theaterChaseRainbow(Adafruit_NeoPixel &strip, uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel(strip,  (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(Adafruit_NeoPixel &strip, byte WheelPos) {
  if(WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}




