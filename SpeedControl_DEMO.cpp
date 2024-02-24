// *********************		SPEED CONTROL		 *********************
// *********************		Incomplete	Code	 *********************
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <FastLED.h>
//  #define FASTLED_ALLOW_INTERRUPTS 0  //  Necessary ??
  #define brightnessDownPin 5     //   Pull_Up
  #define brightnessUpPin   17
  #define speedUpPin        4
  #define speedDownPin      16
#define MASTER_BRIGHTNESS 100  // Set the master brigtness value [should be greater then min_brightness value].
#define MASTER_Speed 500       //   Adjusted for +0 Offset
bool SpeedFlag = true;        // set true for speed 100% in slow chases
bool SpeedFlag2 = true;        // set true for speed 95% in slow chases

#define MASTER_RED 255
#define MASTER_GREEN 255
#define MASTER_BLUE 255
#define DATA_PIN 2
#define CLOCK_PIN 0
#define NUM_LEDS    150
#define BRIGHTNESS  100
#define LED_TYPE    WS2812b  //  Best Led Type for 2815 ??
#define COLOR_ORDER GBR
//   #define FASTLED_ESP32_SPI_BUS HSP   //  ??????????
#define UPDATES_PER_SECOND  1000
#define FRAMES_PER_SECOND  400

int RBright, BBright, GBright, GreenVal, RedVal, BlueVal;
int SpeedRate = 256;    //  Set SpeedRate 0-512  256=50%
int brightnessDownButton, brightnessUpButton, speedUpButton, speedDownButton;
int brightness = 128, brightnessRate, Speed;

void setup(){
  Serial.begin(115200);   // IMPORTANT

  //   initialize FastLED Settings
        FastLED.addLeds<WS2812b, DATA_PIN, GRB >(leds, NUM_LEDS).setCorrection( CRGB( 255, 165, 245)) ;       //Above set to WS2812b ??????? 
  //    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);  OLD  //SMD5050=0xFFA0F0 /* 255, 160, 240 */,
 
    pinMode(brightnessDownPin, INPUT_PULLUP);
    pinMode(brightnessUpPin, INPUT_PULLUP);
    pinMode(speedUpPin, INPUT_PULLUP);
    pinMode(speedDownPin, INPUT_PULLUP);

  //  delay(10);
  FastLED.setBrightness(BRIGHTNESS);
  speedAndBrightnessControl();
}		//    END SETUP
void Speed100() {
  if (SpeedFlag == true) 
    {
      SpeedRate = (SpeedRate - SpeedRate);
      SpeedFlag = false;
    }
  else {
    SpeedRate = SpeedRate;
  }  
}
//    *********************************   set Speed to 95%  Once  *********************************
void Speed95() {
  if (SpeedFlag2 == true) 
    {
      SpeedRate = ( (SpeedRate + 30 ) - SpeedRate);     //   five Present = thirty
      SpeedFlag2 = false;
    }
  else {
    SpeedRate = SpeedRate;
  }  
}
void speedAndBrightnessControl() {
  //  **********  Brightness Control      **********
  brightnessDownButton = digitalRead (brightnessDownPin);
  brightnessUpButton = digitalRead (brightnessUpPin);
  if (brightnessDownButton == LOW) {
    brightness = brightness - 6;
    if (brightness < 0) {
      brightness = 0;
    }
  }
  else if (brightnessUpButton == LOW) {
    brightness = brightness + 6;
    if (brightness >= 255) {
      brightness = 255;
    }
  }
  brightnessRate = map(brightness, 0, 255, 0, 100);
  // delay(10);
  FastLED.setBrightness(brightness);  // Set master brightness based on button pressed
   // *********            SPEED          ***********
  speedUpButton = digitalRead (speedUpPin);
  speedDownButton = digitalRead (speedDownPin);
  if (speedUpButton == LOW) {
    SpeedRate = SpeedRate + 6;
    if (SpeedRate >= 500) {
      SpeedRate = 0;
    }
  }
  else if (speedDownButton == LOW) {
    SpeedRate = SpeedRate - 6;
    if (SpeedRate < 0 ) {
      SpeedRate = 0;
    }
  }
  Speed = map(SpeedRate, 0, 512, 100, 0);
  FastLED.delay(SpeedRate);
   }