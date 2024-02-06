
//  Jan 29 2024
//  TO-DO  LIST Revision M
//  Test all keys
//  Add sences or chases to empty Keys,,, Empty keys cause errors when entered
//
//  Remove:   void chasisToInsert() to simplify code.  the bracketts{} are making additions and fixes difficult / Done
//
//  Fix Fireworks-1 #49, Ater 5th 'FireWork' ESP Locks-up,  Possibly sending data to LEDs Past end (150)  Have to hit restbuttons to restart
//   Fireworks-1 code:  https://www.anirama.com/1000leds/1d-fireworks/ /Done
//   
//  Fix 2nd Fireworks-2  chase #66  Program too slow
//    Fireworks-2  Code:  https://pastebin.com/auNiA6Zd
//
//   Fix  # 87   Need a good chase for Halloween
//   
//   Fix  # 90  coffetti/New Year's Day   too slow and looks sparse  not enough dots
//  Find a Better Way to Control SPEED, add stop at 100%, prevent rollover
//
//
//
//  Step 10 color Rainbow chase   \Done
//  SET  Color correction         \Done    Start point:  setCorrection( CRGB( 255, 160, 240))
//  Fix Scences & Chases   1 to 59  Good\  Done
//  
//  Fixed Lines 1362 to 1382  Dim & Speed \Done
//   Test Frames per seconds  max 400  / Done

String keyP = "";
int KeyFG = 0, x = 0;
char key2, key;
int RBright, BBright, GBright, GreenVal, RedVal, BlueVal;
int convKeyP = 11;
int SpeedRate = 256;    //  Set SpeedRate 0-512  256=50%
int brightnessDownButton, brightnessUpButton, speedUpButton, speedDownButton;
int brightness = 128, brightnessRate, Speed;

#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <FastLED.h>

  #define brightnessDownPin 5     //   Pull_Up
  #define brightnessUpPin   17
  #define speedUpPin        4
  #define speedDownPin      16

#define MASTER_BRIGHTNESS 100  // Set the master brigtness value [should be greater then min_brightness value].
#define MASTER_Speed 500       //   Adjusted for +0 Offset

#define MASTER_RED 255
#define MASTER_GREEN 255
#define MASTER_BLUE 255
#define DATA_PIN 2
#define CLOCK_PIN 0
#define NUM_LEDS    150
#define BRIGHTNESS  100
#define LED_TYPE    WS2813  
#define COLOR_ORDER GBR
 //  #define FASTLED_ESP32_SPI_BUS HSP   ??????????
#define UPDATES_PER_SECOND  1000
#define FRAMES_PER_SECOND  400

//  *********************************             Fireworks #1   int          ****************     
#define NUM_SPARKS 75 // max number (could be NUM_LEDS / 2);
float sparkPos[NUM_SPARKS];
float sparkVel[NUM_SPARKS];
float sparkCol[NUM_SPARKS];
float flarePos;
float gravity = -.002; // m/s/s

CRGB leds[NUM_LEDS];

//  *********************************             Fireworks #2   int          ****************     

const int F2NUM_LEDS=150;  
//  CRGB leds[F2NUM_LEDS]; // sets up block of memory   Refence ln.61
const int F2NUM_SPARKS = 75; // max number (could be NUM_LEDS / 2);
const int F2maxPos = (F2NUM_LEDS-1) * 128 ;
 
int F2sparkPos[3][F2NUM_SPARKS] ;
int F2sparkVel[3][F2NUM_SPARKS] ;
int F2sparkHeat[3][F2NUM_SPARKS];
int F2flarePos[3];
bool F2flareShot[3] = {false,false,false} ;
byte F2nSparks[3];
 
CRGBPalette16 gPal1;
CRGBPalette16 gPal2;
CRGBPalette16 gPal3;
 
DEFINE_GRADIENT_PALETTE( testp ) {
0, 0, 0,0, 
32, 8, 0, 0, 
64, 16,0, 0, 
96, 64,32, 0, 
128,128,80, 0, 
160, 160,100,0, 
192, 192, 192,0, 
224, 255, 255, 255, 
255, 200, 200, 255};

//  *********************************         END  Fireworks #2   int          ****************   

//  *********************************             Key #70  Sine  int          ****************   
//  https://github.com/atuline/FastLED-Demos/blob/master/one_sine_pal/one_sine_pal.ino

// Use qsuba for smooth pixel colouring and qsubd for non-smooth pixel colouring
#define qsubd(x, b)  ((x>b)?b:0)                   // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                          // Analog Unsigned subtraction macro. if result <0, then => 0

// Initialize changeable global variables. Play around with these!!!
int8_t thisspeed = 8;                                         // You can change the speed of the wave, and use negative values.
uint8_t allfreq = 32;                                         // You can change the frequency, thus distance between bars.
int thisphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
int thisdelay = 30;                                           // You can change the delay. Also you can change the allspeed variable above. 
uint8_t bgclr = 0;                                            // A rotating background colour.
uint8_t bgbright = 10;                                        // Brightness of background colour


//  *********************************        END  Key #70  Sine  int          ****************  

//  *********************************             Key #71  Lightning  int          ****************  
//  https://github.com/atuline/FastLED-Demos/blob/master/lightnings/lightnings.ino

uint8_t frequency71 = 50;                                       // controls the interval between strikes
uint8_t flashes71 = 8;                                          //the upper limit of flashes per strike
unsigned int dimmer71 = 1;

uint8_t ledstart71;                                             // Starting location of a flash
uint8_t ledlen71;                                               // Length of a flash

//  *********************************    END      Key #71  Lightning  int          ****************  


//    *********************          Pixel Chases          **********************

#define PATTERN_LEN3 3
  CRGB pattern3[PATTERN_LEN3] = { CRGB::White, CRGB::Blue, CRGB::Green };   //  Key_51
#define PATTERN_LEN3A 3
  CRGB pattern3A[PATTERN_LEN3A] = { CRGB::Red, CRGB::Blue, CRGB::Green };   //  Key_60

#define PATTERN_LEN 4
  CRGB pattern[PATTERN_LEN] = { CRGB::White, CRGB::Red, CRGB::Blue, CRGB::Green };  //  Key_50
#define PATTERN_LEN4A 4
  CRGB pattern4A[PATTERN_LEN4A] = { CRGB::White, CRGB::Blue, CRGB::Green, CRGB::Black };    //  Key_52
#define PATTERN_LEN4B 4
  CRGB pattern4B[PATTERN_LEN4B] = { CRGB::White, CRGB::Red, CRGB::Green, CRGB::Black };     //  Key_53
#define PATTERN_LEN4C 4
  CRGB pattern4C[PATTERN_LEN4C] = { CRGB::Blue, CRGB::Red, CRGB::Green, CRGB::Black };      //  Key_61
#define PATTERN_LEN4D 4
  CRGB pattern4D[PATTERN_LEN4D] = { CRGB::Green, CRGB::White, CRGB::Red, CRGB::Black };      //  Key_x

#define PATTERN_LEN6A 6
  CRGB pattern6A[PATTERN_LEN6A ] = { CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Gold, CRGB::Gold, CRGB::Gold };  //  Key_58 and 59
#define PATTERN_LEN6B 6
  CRGB pattern6B[PATTERN_LEN6B ] = { CRGB::Green, CRGB::Green, CRGB::White, CRGB::White, CRGB::Red, CRGB::Red };  //  Key_85
#define PATTERN_LEN6C 6
  CRGB pattern6C[PATTERN_LEN6C ] = { CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Red, CRGB::Red, CRGB::Red };  //  Key_86

#define PATTERN_LEN7A 7
  CRGB pattern7A[PATTERN_LEN7A] = { CRGB::Purple, CRGB::Blue, CRGB::Aqua, CRGB::Green, CRGB::Yellow, CRGB::DarkOrange, CRGB::Red };  //  Key_54
#define PATTERN_LEN7B 7
  CRGB pattern7B[PATTERN_LEN7B ] = { CRGB::Red, CRGB::Red, CRGB::Gold, CRGB::Gold, CRGB::Green, CRGB::Green, CRGB::Black };   //  Key_55 and 82

#define PATTERN_LEN9A 9
  CRGB pattern9A[PATTERN_LEN9A ] = { CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Green, CRGB::Green, CRGB::Green };  //  Key_56 and 57
#define PATTERN_LEN9B 9
  CRGB pattern9B[PATTERN_LEN9B ] = { CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red, CRGB::FloralWhite, CRGB::HotPink, CRGB::HotPink, CRGB::HotPink, CRGB::HotPink };  //  Key_81

#define PATTERN_LEN10A 10
  CRGB pattern10A[PATTERN_LEN10A ] = { CRGB::Red, CRGB::DarkOrange, CRGB::Yellow, CRGB::LimeGreen, CRGB::Green,
                                       CRGB::Aqua, CRGB::Blue, CRGB::DarkViolet, CRGB::DeepPink, CRGB::Crimson };  //  Key_63   Step-Rainbow

#define PATTERN_LEN11A 11
  CRGB pattern11A[PATTERN_LEN11A ] = { CRGB::PowderBlue, CRGB::PowderBlue, CRGB::LimeGreen, CRGB::LimeGreen, CRGB::HotPink,
                                       CRGB::HotPink, CRGB::HotPink, CRGB::Yellow, CRGB::Yellow, CRGB::Purple, CRGB::Purple };  //  Key_84


//   *********   Start define Pallets   *********

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
CRGBPalette32 currentPalette32;
// TBlendType    currentBlending;
TBlendType    currentBlending = LINEARBLEND;


extern CRGBPalette16 myRedBluePalette;
extern const TProgmemPalette16 myRedBluePalette_p PROGMEM;
extern CRGBPalette16 RWBPalette;
extern const TProgmemPalette16 RWB_p PROGMEM;
extern CRGBPalette16 Irish;
extern const TProgmemPalette16 Irish_p PROGMEM;
extern CRGBPalette16 myRedGreenXmassPalette;
extern const TProgmemPalette16 myRedGreenXmassPalette_p PROGMEM;
extern CRGBPalette16 FourTwentyPalette;                 //  Unknown Error when used in Key Code ???
extern const TProgmemPalette16 FourTwenty_p PROGMEM;    //  Unknown Error when used in Key Code ???

extern CRGBPalette32 Ocean;
extern const TProgmemPalette32 Ocean_p PROGMEM;
extern CRGBPalette32 FireFG;
extern const TProgmemPalette32 FireFG_p PROGMEM;




// ************************    Begin KeyPad  Setup_Int.  *********************************
#define ROWS  4
#define COLS  3
 
char keyMap[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
 
uint8_t rowPins[ROWS] = {14, 27, 26, 25}; // GIOP14, GIOP27, GIOP26, GIOP25
uint8_t colPins[COLS] = {33, 32, 18};     // GIOP33, GIOP32, GIOP18
 
Keypad keypad = Keypad(makeKeymap(keyMap), rowPins, colPins, ROWS, COLS );

// ************************    End KeyPad  Setup_Int.  *********************************

// ************************    Begin Display  Setup_Int.  *********************************

// set the LCD number of columns and rows
int lcdColumns = 20;
int lcdRows = 4;
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 



// ************************    Begin Void  Setup  *********************************
void setup(){

  Serial.begin(115200);   // IMPORTANT

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  //  lcd.begin(20,4);
  lcd.init(); 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.clear();
  welcomeMsg();

  //   initialize FastLED Settings
        FastLED.addLeds<WS2813, DATA_PIN, GRB >(leds, NUM_LEDS).setCorrection( CRGB( 255, 165, 245)) ;       //Above set to WS2813 ??????? 
  //    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);  OLD  //SMD5050=0xFFA0F0 /* 255, 160, 240 */,
 
    pinMode(brightnessDownPin, INPUT_PULLUP);
    pinMode(brightnessUpPin, INPUT_PULLUP);
    pinMode(speedUpPin, INPUT_PULLUP);
    pinMode(speedDownPin, INPUT_PULLUP);

//   FireWorks 2 Void Setup  ***********************

      gPal1 = testp;
      gPal2 = CRGBPalette16( CRGB::Black, CRGB::DarkBlue, CRGB::Aqua,  CRGB::White);
      gPal3 = CRGBPalette16( CRGB::Black, CRGB::DarkGreen, CRGB::Yellow, CRGB::White);



  //  delay(10);
  FastLED.setBrightness(BRIGHTNESS);
  speedAndBrightnessControl();

}

//    END SETUP
 
void loop(){
  
      lcd.setCursor(0,0);
      lcd.print((String)"Please enter a   ");
      lcd.setCursor(0,1);
      lcd.print((String)"number from: 1 - 99");
      lcd.setCursor(0,2);
      lcd.print((String)"and then press '#' ");
  while (true) {
    x = 0;
    if ((key) && (key != '#')&& (key != '*')) {
      keyP = keyP + key; lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print((String)"Number Entered: " + keyP + "        ");
    }
    else if (key == '#') {
      convKeyP = keyP.toInt();
      keyP = ""; lcd.clear();
      break;
    }
    else if (key == '*') {
      lcd.clear();
      keyP = "";
      lcd.setCursor(0,0);
      lcd.print((String)"Please enter a   ");
      lcd.setCursor(0,1);
      lcd.print((String)"number from: 1 - 99");
      lcd.setCursor(0,2);
      lcd.print((String)"and then press #");
    }
    key = keypad.getKey();
  }

  while (true) {
    key = keypad.getKey(); // wait for you to input a pattern number,
    LoopFLed();
    speedAndBrightnessControl();
    if ((key) && (key != '#')&& (key != '*')) {
      //////DO NOTHING,
    }
    else if (key == '*') {////////BREAK OUT ONLY WHEN * IS PRESSED
      lcd.clear();
      keyP = "";
       lcd.setCursor(0,0);
      lcd.print((String)"Please enter a   ");
      lcd.setCursor(0,1);
      lcd.print((String)"number from: 1 - 99");
      lcd.setCursor(0,2);
      lcd.print((String)"and then press #");
      break;
    }
  }

}

//     Begin  LOOP FEED   *****************************************

void LoopFLed() {
  //   Preset Fills

//  ******************************  Keys 1 to 20    Single Colors   ********************************

  if (  convKeyP == 1)    // Red
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::Red);
    displayPatternScene(convKeyP, "Red");
    FastLED.show();
  }

  else if (  convKeyP == 2)  // Blue
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::Blue);
     key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    displayPatternScene(convKeyP, "Blue");
     FastLED.show();
  }

  else if (  convKeyP == 3)   // Green
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::Green);
    displayPatternScene(convKeyP, "Green");
     FastLED.show();
  }

  else if (  convKeyP == 4)   // Lemon
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB(0xFFFF20));
    displayPatternScene(convKeyP, "Lemon");
     FastLED.show();
  }

  else if (  convKeyP == 5)    // Orange 
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::Orange);
    displayPatternScene(convKeyP, "Orange");
     FastLED.show();
  }

  else if (  convKeyP == 6)   // Cyan 
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::Cyan);
    displayPatternScene(convKeyP, "Cyan");
     FastLED.show();
  }

  else if (  convKeyP == 7)   // Purple 
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    displayPatternScene(convKeyP, "Purple");
    fill_solid( leds, NUM_LEDS, CRGB::Purple);
     FastLED.show();
  }

  else if (  convKeyP == 8)   // Magenta
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB(0xFF1040));
    displayPatternScene(convKeyP, "Magenta");
     FastLED.show();
  }

  else if (convKeyP == 9)   // White
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        fill_solid( leds, NUM_LEDS, CRGB::White);
        displayPatternChase(convKeyP, "White");      
    FastLED.show();
  }

//  Keys 10 to 19    More Scences

  else if (convKeyP == 10)  // Dark Orange
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::DarkOrange);
    displayPatternScene(convKeyP, "Dark Orange");
    FastLED.show();
  }

  else if (convKeyP == 11)  // Sky Blue
  {
      key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::DeepSkyBlue);
    displayPatternScene(convKeyP, "Sky Blue");
    FastLED.show();
  }

  else if (convKeyP == 12)  // Pink
  {
      key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::Pink);
    displayPatternScene(convKeyP, "Pink");
    FastLED.show();
  }

  else if (convKeyP == 13)  // Hot Pink
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::HotPink);
    displayPatternScene(convKeyP, "Hot Pink");
    FastLED.show();
  }

  else if (convKeyP == 14)  // Gold
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::Gold);
    displayPatternScene(convKeyP, "Gold");
    FastLED.show();
  }

  else if (convKeyP == 15)  // Sea Green
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::SeaGreen);
    displayPatternScene(convKeyP, "Sea Green");
    FastLED.show();
  }

  else if (convKeyP == 16)  //  Tomato
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::Tomato);
    displayPatternScene(convKeyP, "Tomato");
    FastLED.show();
  }

  else if (  convKeyP == 17)  // Coral
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::Coral);
    displayPatternScene(convKeyP, "Coral");
    FastLED.show();
  }

  else if (  convKeyP == 18)  // Lime Green
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::LimeGreen);
    displayPatternScene(convKeyP, "Lime Green");
    FastLED.show();
  }

  else if (  convKeyP == 19) //  Powder Blue
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::PowderBlue);
    displayPatternScene(convKeyP, "Powder Blue");
    FastLED.show();
  }

//  Keys # 20 to 29  Static Scenes


  else if (  convKeyP == 20)                    //    Needs changed to single Color
  {  
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    currentPalette = RWB_p; currentBlending = NOBLEND;
    static uint8_t startIndex = 0;
    FillLEDsFromPaletteColors( startIndex);
   displayPatternScene(convKeyP, "Red White & Blue");
      FastLED.show();
  }


//  ******************************  Keys 21 to 30    Static Multi-Color Scenes   ********************************


  else if (  convKeyP == 21)
  { 
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    currentPalette32 = FireFG_p; currentBlending = LINEARBLEND;
static uint16_t startIndex = 0;
// startIndex = startIndex + 1; / motion speed / //Remove Chase
FillLEDsFromPaletteColors32 ( startIndex);
displayPatternScene(convKeyP,"Fire");
FastLED.show();
  }

  else if (  convKeyP == 22)
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    currentPalette = Irish_p; currentBlending = NOBLEND;
static uint8_t startIndex = 0;
// startIndex = startIndex + 1; / motion speed / //Remove Chase
FillLEDsFromPaletteColors( startIndex);
displayPatternScene(convKeyP,"Irish");
FastLED.show();
  }

  else if (  convKeyP == 23) //E         Rainbow
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    currentPalette = RainbowColors_p; currentBlending = LINEARBLEND;
static uint8_t startIndex = 0;
// startIndex = startIndex + 1; / motion speed / //Remove Chase
FillLEDsFromPaletteColors( startIndex);
displayPatternScene(convKeyP,"Rainbow");
FastLED.show();
  }

  else if (  convKeyP == 24) //     Police
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    currentPalette = myRedBluePalette_p; currentBlending = NOBLEND;
static uint8_t startIndex = 0;
// startIndex = startIndex + 1; / motion speed / //Remove Chase
FillLEDsFromPaletteColors( startIndex);
displayPatternScene(convKeyP,"Police");
FastLED.show();
  }

  else if (  convKeyP == 25)   //    Forest
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
   currentPalette = ForestColors_p; currentBlending = LINEARBLEND;
static uint8_t startIndex = 0;
// startIndex = startIndex + 1; / motion speed / //Remove Chase
FillLEDsFromPaletteColors( startIndex);
displayPatternScene(convKeyP,"Forest");
FastLED.show();
  }

  else if (  convKeyP == 26) //          Red White Blue
    {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    currentPalette = RWB_p; currentBlending = NOBLEND;
static uint8_t startIndex = 0;
// startIndex = startIndex + 1; / motion speed / //Remove Chase
FillLEDsFromPaletteColors( startIndex);
displayPatternScene(convKeyP,"Red White Blue");
FastLED.show();
  }

  else if (  convKeyP == 27) //     Clouldy Day
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    currentPalette = CloudColors_p; currentBlending = LINEARBLEND;
static uint8_t startIndex = 0;
// startIndex = startIndex + 1; / motion speed / //Remove Chase
FillLEDsFromPaletteColors( startIndex);
displayPatternScene(convKeyP,"Cloudy Day");
FastLED.show();
      }

  else if (  convKeyP == 28) //  Ocean Vacation
  {
key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
   currentPalette32 = Ocean_p; currentBlending = LINEARBLEND;
static uint16_t startIndex = 0;
// startIndex = startIndex + 1; / motion speed / //Remove Chase
FillLEDsFromPaletteColors32( startIndex);
displayPatternScene(convKeyP,"Ocean Vacation");
FastLED.show();
  }

  else if (  convKeyP == 29) //   Christmas
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    currentPalette = myRedGreenXmassPalette_p; currentBlending = NOBLEND;
static uint8_t startIndex = 0;
// startIndex = startIndex + 1; / motion speed */ //Remove Chase
FillLEDsFromPaletteColors( startIndex);
displayPatternScene(convKeyP,"Christmas");
FastLED.show();
  }
  if (  convKeyP == 30) //        Red White & Blue
  {  
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    currentPalette = RWB_p; currentBlending = NOBLEND;
    static uint8_t startIndex = 0;
    FillLEDsFromPaletteColors( startIndex);
   displayPatternScene(convKeyP, "Red White & Blue");
      FastLED.show();
  }


//**********************************  Keys # 31 to 40      Start Chases  **********************************************


  else if (  convKeyP == 31) //         Police
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
       currentPalette = myRedBluePalette_p; currentBlending = NOBLEND;    //Police
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);
    displayPatternChase(convKeyP,  "Police");
       FastLED.show();
  }

  else if (  convKeyP == 32) //         Forest
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
      currentPalette = ForestColors_p; currentBlending = LINEARBLEND;
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);
  FastLED.show();
    displayPatternChase(convKeyP,  "Forest");
    
  }

  else if (  convKeyP == 33) //         Red White Blue
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
       currentPalette = RWB_p; currentBlending = NOBLEND;
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);
    displayPatternChase(convKeyP,  "Red White & Blue");
       FastLED.show();
  }

  else if (  convKeyP == 34) //         Gilter Rainbow
  {
   key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
static uint8_t gHue = 0;
   gHue = gHue + 2; /* motion speed */
 // Call the current pattern function once, updating the 'leds' array
 fill_rainbow( leds, NUM_LEDS, gHue, 4); //4 is a good Size(LEDs+=0)
     { if( random8() < 120)    //Good Amount of sparkles       
  leds[ random16(NUM_LEDS) ] += CRGB::White;} 
     { if( random8() < 120)
  leds[ random16(NUM_LEDS) ] += CRGB::White;}
  FastLED.show();
    displayPatternChase(convKeyP,  "Gilter Rainbow");
    
  }
  else if (  convKeyP == 35) //         Popcorn       
  {
   key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
       static uint8_t gHue = 0;
       gHue = gHue + 1; /* motion speed */
 // Call the current pattern function once, updating the 'leds' array
          fadeToBlackBy( leds, NUM_LEDS, 10);  //(good fade #)
            int pos = random8(NUM_LEDS);
            leds[pos] += CHSV( gHue + random8(255), 255, 255); 
            int pos2 = random8(NUM_LEDS);
           leds[pos2] += CHSV( gHue + random8(255), 255, 255); // Good 2*Times the dots
       FastLED.show();
        displayPatternChase(convKeyP,  "Popcorn");
  }

  else if (  convKeyP == 36) //         Meteor
  // a colored dot sweeping back and forth, with fading trails
  {
    static uint8_t gHue = 0;
       gHue = gHue + 1; /* motion speed */
 // Call the current pattern function once, updating the 'leds' array
       fadeToBlackBy( leds, NUM_LEDS, 20);
         int pos = beatsin16( 6, 0, NUM_LEDS-1 ); // 6 is Good
    leds[pos] += CHSV( gHue, random8(255), 255); 
  FastLED.show();
    displayPatternChase(convKeyP,  "Meteor");
    
  }

  else if (  convKeyP == 37) //         Ticker Tape
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        static uint8_t gHue = 0;
           gHue = gHue - 30; /* motion speed */
              const uint16_t spewSpeed = 200;  // rate of advance
              static boolean spewing = 0;  // pixels are On(1) or Off(0)
              static uint8_t count = 1;  // how many to light (or not light)
              static uint8_t temp = count;
              static uint8_t hue = random8();

EVERY_N_MILLISECONDS(spewSpeed){

  if (count == 0) {
    spewing = !spewing;
    if (spewing == 1) { count = random8(1,9); }  // random number for On pixels
    else { count = random8(2,4); }  // random number for Off pixels
    temp = count;
    hue = random8();
  }  
     for (uint8_t i = NUM_LEDS-1; i > 0; i--) {
      leds[i] = leds[i-1];  // shift data down the line by one pixel
  }
  if (spewing == 1) {  // new pixels are On
      if (temp == count) {
      leds[0] = CHSV(hue-5, 215, 255);  // for first dot
      leds[1] = CHSV(hue-5, 215, 255);  // for second dot
      leds[2] = CHSV(hue-5, 215, 255);  // for third dot        
      leds[3] = CHSV(hue-5, 215, 255);  // for forth dot        
      leds[4] = CHSV(hue-5, 215, 255);  // for fifth dot
    } else {
      leds[0] = CHSV(hue, 255, 255/(1+((temp-count)*2)) );  // for following dots

    }
  } else {  // new pixels are Off
    leds[0] = CHSV(0,0,0);  // set pixel 0 to black
  }

  count = count - 1;  // reduce count by one.
}
//end every_n
//  End Tape Ticker    ;

    displayPatternChase(convKeyP,  "Ticker Tape");
       FastLED.show();
  }

  else if (  convKeyP == 38) //  Fourth of July
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        currentPalette = RWB_p; currentBlending = NOBLEND;
           static uint8_t startIndex = 0;
        //   startIndex = startIndex + 1; /* motion speed */  //Remove Chase with comment
          FillLEDsFromPaletteColors( startIndex);
            { if( random8() < 120)    //Good Amount of sparkles       
        leds[ random16(NUM_LEDS) ] += CRGB::White;} 
          { if( random8() < 120)
          leds[ random16(NUM_LEDS) ] += CRGB::White;}  
  
    displayPatternChase(convKeyP,  "fourth of July");
    FastLED.show();
  }

  else if (  convKeyP == 39) //         Fire
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
       currentPalette32 = FireFG_p;
       currentBlending = LINEARBLEND;
  static uint16_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors32( startIndex);     

     { if( random8() < 120)    //Good Amount of sparkles       
  leds[ random16(NUM_LEDS) ] += CRGB::White;}
    displayPatternChase(convKeyP,  "Fire");
       FastLED.show();
  }


  else if (  convKeyP == 40) //         Irish
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        currentPalette = Irish_p; currentBlending = NOBLEND;
          static uint8_t startIndex = 0;
          startIndex = startIndex + 1; /* motion speed */ 
          FillLEDsFromPaletteColors( startIndex);;
  FastLED.show();
    displayPatternChase(convKeyP,  "Irish");
  }
  
//  ******************************  Keys 41 to 50    More Chases   ********************************

  else if (  convKeyP == 41) //  Rainbow Right
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
       currentPalette =  RainbowStripeColors_p; currentBlending = NOBLEND;
        static uint8_t startIndex = 0;
        startIndex = startIndex - 1; /* motion speed */
        FillLEDsFromPaletteColors( startIndex);;
    displayPatternChase(convKeyP,  "Rainbow Right");
       FastLED.show();
  }
  
  else if (  convKeyP == 42) //         Rainbow Left
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
      currentPalette =  RainbowStripeColors_p; currentBlending = NOBLEND;
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* motion speed */
        FillLEDsFromPaletteColors( startIndex);   
  FastLED.show();
    displayPatternChase(convKeyP,  "Rainbow Left");
  }
 
  else if (  convKeyP == 43) //         Cloudy Day
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
       currentPalette = CloudColors_p; currentBlending = LINEARBLEND;
          static uint8_t startIndex = 0;
          startIndex = startIndex + 1; /* motion speed */
          FillLEDsFromPaletteColors( startIndex); 
    displayPatternChase(convKeyP,  "Cloudy Day");
       FastLED.show();
  }
 
  else if (  convKeyP == 44) //         Ocean
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
      currentPalette32 = Ocean_p; currentBlending = LINEARBLEND;
        static uint16_t startIndex = 0;
        startIndex = startIndex + 1; /* motion speed */
        FillLEDsFromPaletteColors32( startIndex); 
  FastLED.show();
    displayPatternChase(convKeyP,  "Ocean");
  }
 
  else if (  convKeyP == 45) //         Drunk Xmass
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
       currentPalette = myRedGreenXmassPalette_p; currentBlending = NOBLEND;
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* motion speed */
        FillLEDsFromPaletteColors( startIndex);
    displayPatternChase(convKeyP,  "Drunk Xmass");
       FastLED.show();
  }
 
  else if (  convKeyP == 46) //         Juggle
                             // eight colored dots, weaving in and out of sync with each other
  {key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        fadeToBlackBy( leds, NUM_LEDS, 20);
        uint8_t dothue = 0;
        for( int i = 0; i < 8; i++) {
          leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
          dothue += 32;
  }
  FastLED.show();
    displayPatternChase(convKeyP,  "Juggle");
  }

  else if (  convKeyP == 47) //         Party Colors
                            // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        uint8_t gHue = 0; // rotating "base color" used by many of the patterns
        uint8_t BeatsPerMinute = 62;
         CRGBPalette16 palette = PartyColors_p;
           uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
             for( int i = 0; i < NUM_LEDS; i++)
            leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    displayPatternChase(convKeyP,  "Party Colors");
       FastLED.show();
  }
 
  else if (  convKeyP == 48) //         Multi_Rainbow
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        static uint16_t sPseudotime = 0;
        static uint16_t sLastMillis = 0;
        static uint16_t sHue16 = 0;

        uint8_t sat8 = beatsin88( 87, 220, 250);
        uint8_t brightdepth = beatsin88( 341, 96, 224);
        uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
        uint8_t msmultiplier = beatsin88(147, 23, 60);
        uint8_t gHue = 0; // rotating "base color" used by many of the patterns

        uint16_t hue16 = sHue16;//gHue * 256;
        uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
        uint16_t ms = millis();
        uint16_t deltams = ms - sLastMillis ;
        sLastMillis  = ms;
        sPseudotime += deltams * msmultiplier;
        sHue16 += deltams * beatsin88( 400, 5,9);
        uint16_t brightnesstheta16 = sPseudotime;

          for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
              hue16 += hueinc16;
              uint8_t hue8 = hue16 / 256;

              brightnesstheta16  += brightnessthetainc16;
              uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

              uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
              uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
              bri8 += (255 - brightdepth);
    
             CRGB newcolor = CHSV( hue8, sat8, bri8);
    
              uint16_t pixelnumber = i;
              pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
              nblend( leds[pixelnumber], newcolor, 64);
  }
  
  FastLED.show();
    displayPatternChase(convKeyP,  "Many Rainbow");
  }


//    *****************************************************************************************************

  else if (  convKeyP == 49) //         Fireworks
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }

  // send up flare
  flare();
  
  // explode
  explodeLoop();

  // wait before sending up another
  delay(random16(1000, 4000));


    displayPatternChase(convKeyP,  "Fireworks");
 //   FastLED.show(); 
       FastLED.clear();
 // FastLED.show(); 
  }


  else if (  convKeyP == 50) //4 color LED Chase
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
     static uint8_t startIndex = 0;
    //  CRGB pattern[PATTERN_LEN] = { CRGB::White, CRGB::Red, CRGB::Blue, CRGB::Green };   Top of code

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern[(PATTERN_LEN - startIndex + i) % PATTERN_LEN];      
          }

    // Apply the colors to the LED strip     
    FastLED.show();

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 4;    //  Divide LED Array Number
    }
    displayPatternChase(convKeyP,  "4-color Chase One");
  }
  

  //  *****************            Keys # 51 to 60       Chases           ********************************
  


  else if (  convKeyP == 51) //     3-color LED Chase One
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
     static uint8_t startIndex = 0;
    //  CRGB pattern3[PATTERN_LEN3] = { CRGB::White, CRGB::Blue, CRGB::Green };   Top of code

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern3[(PATTERN_LEN3 - startIndex + i) % PATTERN_LEN3];      
          }


    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 3;    //  Divide LED Array Number
    
       // Apply the colors to the LED strip     
    FastLED.show();
           }
    displayPatternChase(convKeyP,  "3-color Chase Two");
  }

  else if (  convKeyP == 52) //   4-color Chase Two
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
     static uint8_t startIndex = 0;
    //  CRGB pattern[PATTERN_LEN4A] = { CRGB::White, CRGB::Blue, CRGB::Green, CRGB::Black };   Top of code

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern4A[(PATTERN_LEN4A - startIndex + i) % PATTERN_LEN4A];      
          }

    // Apply the colors to the LED strip     
    FastLED.show();

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 4;    //  Divide LED Array Number
    }
    displayPatternChase(convKeyP,  "4-Color Chase Two");
  }

  
  else if (  convKeyP == 53) //   4-color Chase Three
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
     static uint8_t startIndex = 0;
    //  CRGB pattern[PATTERN_LEN4B] = { CRGB::White, CRGB::Red, CRGB::Green, CRGB::Black };   Top of code

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern4B[(PATTERN_LEN4B - startIndex + i) % PATTERN_LEN4B];      
          }

    // Apply the colors to the LED strip     
    FastLED.show();

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 4;    //  Divide LED Array Number
    }
    displayPatternChase(convKeyP,  "4-Color Chase Three");
  }


  else if (  convKeyP == 54) //   7-Color Rainbow Step Chase
{
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
     static uint8_t startIndex = 0;
    //  CRGB pattern7A[PATTERN_LEN7A] = { CRGB::Violet, CRGB::Blue, CRGB::Aqua, CRGB::Green, CRGB::Yellow, CRGB::DarkOrange, CRGB::Red };

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern7A[(PATTERN_LEN7A - startIndex + i) % PATTERN_LEN7A];      
          }

    // Apply the colors to the LED strip     
    FastLED.show();

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 7;    //  Divide LED Array Number
    }
    displayPatternChase(convKeyP,  "Rainbow Step Chase");
  }
 
  else if (  convKeyP == 55) //   Bob Marley
{
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
     static uint8_t startIndex = 0;
    //  CRGB pattern7B[PATTERN_LEN7B ] = { CRGB::Red, CRGB::Red, CRGB::Gold, CRGB::Glod, CRGB::Green, CRGB::Green, CRGB::Black }

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern7B[(PATTERN_LEN7B - startIndex + i) % PATTERN_LEN7B];      
          }

    // Apply the colors to the LED strip     
    FastLED.show();

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 7;    //  Divide LED Array Number
    }
    displayPatternChase(convKeyP,  "Bob Marely");
  }
  
  else if (  convKeyP == 56) //       Caribbean Scene
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
     static uint8_t startIndex = 0;
    //  CRGB pattern9A[PATTERN_LEN9A ] = { CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Green, CRGB::Green, CRGB::Green };

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern9A[(PATTERN_LEN9A - startIndex + i) % PATTERN_LEN9A];      
          }

    // Apply the colors to the LED strip     
    FastLED.show();

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 0) % 9;    //  Divide LED Array Number
    }                             //  (startIndex + 0) for Static Scene
    displayPatternChase(convKeyP,  "Caribbean Scene");
  }

  else if (  convKeyP == 57) //     Caribbean Chase
{ 
     static uint8_t startIndex = 0;
    //  CRGB pattern9A[PATTERN_LEN9A ] = { CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Green, CRGB::Green, CRGB::Green };

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern9A[(PATTERN_LEN9A - startIndex + i) % PATTERN_LEN9A];      
          }

    // Apply the colors to the LED strip     
    FastLED.show();

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 9;    //  Divide LED Array Number
    }
    displayPatternChase(convKeyP,  "Caribbean Chase");
  }


    else if (  convKeyP == 58) //       UKraine Scene
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
     static uint8_t startIndex = 0;
    //  CRGB pattern6A[PATTERN_LEN6A ] = { CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Gold, CRGB::Gold, CRGB::Gold };  //  Key_58 and 59

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern6A[(PATTERN_LEN6A - startIndex + i) % PATTERN_LEN6A];      
          }

    // Apply the colors to the LED strip     
    FastLED.show();

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 0) % 6;    //  Divide LED Array Number
    }                             //  (startIndex + 0) for Static Scene
    displayPatternChase(convKeyP,  "Ukraine Scene");
  }

  else if (  convKeyP == 59) //     Ukraine Chase
{ 
     static uint8_t startIndex = 0;
    //  CRGB pattern6A[PATTERN_LEN6A ] = { CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Gold, CRGB::Gold, CRGB::Gold };  //  Key_58 and 59

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern6A[(PATTERN_LEN6A - startIndex + i) % PATTERN_LEN6A];      
          }

    // Apply the colors to the LED strip     
    FastLED.show();

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 6;    //  Divide LED Array Number
    }
    displayPatternChase(convKeyP,  "Ukraine Chase");
  }


  else if (  convKeyP == 60) //L  RGB
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }

     static uint8_t startIndex = 0;
    //  CRGB pattern3A[PATTERN_LEN3A] = { CRGB::Red, CRGB::Blue, CRGB::Green };   Top of code
      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern3A[(PATTERN_LEN3A - startIndex + i) % PATTERN_LEN3A];      
          }
    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 3;    //  Divide LED Array Number
    
       // Apply the colors to the LED strip     
    FastLED.show();
           }
    displayPatternChase(convKeyP,  "3-color RGB Chase");
  }

//  ******************************  Keys 61 to 69    More Chases   ********************************


  else if (  convKeyP == 61) //   3-color BGB Chase
{
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }

     static uint8_t startIndex = 0;
     //  CRGB pattern4C[PATTERN_LEN4C] = { CRGB::Blue, CRGB::Red, CRGB::Green, CRGB::Black };  Top of code
      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern4C[(PATTERN_LEN4C - startIndex + i) % PATTERN_LEN4C];      
          }
    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 4;    //  Divide LED Array Number
    
       // Apply the colors to the LED strip     
    FastLED.show();
           }
    displayPatternChase(convKeyP,  "4-color RGB+Black Chase");
  }

  else if (  convKeyP == 62)  //     Heat Pallet
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
      currentPalette = HeatColors_p; currentBlending = LINEARBLEND;
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* motion speed */
        FillLEDsFromPaletteColors( startIndex); 
  FastLED.show();
    displayPatternChase(convKeyP,  "Hot in Here");
  }

  else if (  convKeyP == 63)  //  Step Rainbow
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }

       static uint8_t startIndex = 0;
    // #define PATTERN_LEN10A 10
    // CRGB pattern10A[PATTERN_LEN10A ] = { CRGB::Red, CRGB::DarkOrange, CRGB::Yellow, CRGB::LimeGreen, CRGB::Green,
    //                                      CRGB::Aqua, CRGB::Blue, CRGB::DarkViolet, CRGB::DeepPink, CRGB::Crimson };  //  Key_63   Step-Rainbow
      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern10A[(PATTERN_LEN10A - startIndex + i) % PATTERN_LEN10A];      
          }

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 10;    //  Divide LED Array Number
    }
    
    displayPatternChase(convKeyP,  "Step-Rainbow"); 
      // Apply the colors to the LED strip     
    FastLED.show();
  }


  else if (  convKeyP == 64)   //  Step-Rainbow Left
 {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
       static uint8_t startIndex = 0;
    // #define PATTERN_LEN10A 10
    // CRGB pattern10A[PATTERN_LEN10A ] = { CRGB::Red, CRGB::DarkOrange, CRGB::Yellow, CRGB::LimeGreen, CRGB::Green,
    //                                      CRGB::Aqua, CRGB::Blue, CRGB::DarkViolet, CRGB::DeepPink, CRGB::Crimson };  //  Key_63   Step-Rainbow
      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern10A[(PATTERN_LEN10A - startIndex + i) % PATTERN_LEN10A];      
          }

    if (UPDATES_PER_SECOND == 0) {

    
         // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex - 1) % 10;    //  Divide LED Array Number
    }


    displayPatternChase(convKeyP,  "Step-Rainbow Left"); 
      // Apply the colors to the LED strip     
    FastLED.show();
  }

  else if (  convKeyP == 65) //          Rainbow
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
      currentPalette = RainbowColors_p; currentBlending = LINEARBLEND;
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1; /* motion speed */
        FillLEDsFromPaletteColors( startIndex);
 
    displayPatternChase(convKeyP,  "Rainbow");
   FastLED.show();
   }  


  else if (  convKeyP == 66) //           FireWorks 2 Mult-Color
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        byte nrv;
      }
    {
          EVERY_N_MILLIS(3) { 
         // random16_add_entropy( random());
         if  (not F2flareShot[0])   F2shoot(0);
         if  (not F2flareShot[1])   F2shoot(1);
         if  (not F2flareShot[2])   F2shoot(2);
          }

          EVERY_N_MILLIS(3) {     
         if   (F2flareShot[0]) F2doSparks(0);
         if   (F2flareShot[1]) F2doSparks(1);
         if   (F2flareShot[2]) F2doSparks(2);
          } 
          
          EVERY_N_MILLIS(3) {  
 
          displayPatternChase(convKeyP,  "FireWorks 2");
           FastLED.show();
          // FastLED.clear();
         fadeToBlackBy(leds,NUM_LEDS,80);
          }
    }

    
  }


  else if (  convKeyP == 67) //  Stormy Day
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
       currentPalette = CloudColors_p; currentBlending = LINEARBLEND;
          static uint8_t startIndex = 0;
          startIndex = startIndex + 1; /* motion speed */
          FillLEDsFromPaletteColors( startIndex); 
              { if( random8() < 120)    //Good Amount of sparkles       
                leds[ random16(NUM_LEDS) ] += CRGB::White;} 
              { if( random8() < 120)
                leds[ random16(NUM_LEDS) ] += CRGB::White;} 
    displayPatternChase(convKeyP,  "Stormy Day");
       FastLED.show();
  }

  else if (  convKeyP == 68) //     Blur
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
  {
    uint8_t blurAmount = dim8_raw( beatsin8(3,64, 192) );       // A sinewave at 3 Hz with values ranging from 64 to 192.
    blur1d( leds, NUM_LEDS, blurAmount);                        // Apply some blurring to whatever's already on the strip, which will eventually go black.
  
    uint8_t  i = beatsin8(  9, 0, NUM_LEDS);
    uint8_t  j = beatsin8( 7, 0, NUM_LEDS);
    uint8_t  k = beatsin8(  5, 0, NUM_LEDS);
  
  // The color of each point shifts over time, each at a different speed.
    uint16_t ms = millis();  
    leds[(i+j)/2] = CHSV( ms / 29, 200, 255);
    leds[(j+k)/2] = CHSV( ms / 41, 200, 255);
    leds[(k+i)/2] = CHSV( ms / 73, 200, 255);
    leds[(k+i+j)/3] = CHSV( ms / 53, 200, 255);
  
      FastLED.show();
  } // loop()
         displayPatternChase(convKeyP,  "Blur");
           FastLED.show();
  }

  else if (  convKeyP == 69) //                          Color Blocks
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        // Palette definitions      ~~ at Line 150
//       currentPalette = RainbowColors_p;
//          CRGBPalette16 currentPalette;
//          CRGBPalette16 targetPalette;
//        	TBlendType 	blendType = LINEARBLEND; 
              
          beatwave();
    
                  EVERY_N_MILLISECONDS(100) {
                    uint8_t maxChanges = 24; 
                    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
        }

            EVERY_N_SECONDS(5)  {                                       // Change the target palette to a random one every 5 seconds.
            targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
            }
           FastLED.show();
         displayPatternChase(convKeyP,  "Color Blocks");
  }


  else if (  convKeyP == 70) //                     Sine_One
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }

//  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
      EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
      one_sine_pal(millis()>>4);
  }

       EVERY_N_MILLISECONDS(100) {
        uint8_t maxChanges = 24; 
          nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

              EVERY_N_SECONDS(5) {                                        // Change the target palette to a random one every 5 seconds.
                static uint8_t baseC = random8();                         // You can use this as a baseline colour if you want similar hues in the next line.
                targetPalette = CRGBPalette16(CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 255, random8(128,255)), CHSV(random8(), 192, random8(128,255)), CHSV(random8(), 255, random8(128,255)));
  }
  
  FastLED.show();

    displayPatternChase(convKeyP,  "Sine_One");
  }

//  ******************************  Keys 71 to 79    Complex Chases ????   ********************************

  else if (  convKeyP == 71) //             Lightning
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }

  ledstart71 = random8(NUM_LEDS);                               // Determine starting location of flash
  ledlen71 = random8(NUM_LEDS-ledstart71);                        // Determine length of flash (not to go beyond NUM_LEDS-1)
  
  for (int flashCounter = 0; flashCounter < random8(3,flashes71); flashCounter++) {
    if(flashCounter == 0) dimmer71 = 5;                         // the brightness of the leader is scaled down by a factor of 5
    else dimmer71 = random8(1,3);                               // return strokes are brighter than the leader
    
    fill_solid(leds+ledstart71,ledlen71,CHSV(255, 0, 255/dimmer71));
    FastLED.show();                       // Show a section of LED's
    delay(random8(4,10));                                     // each flash only lasts 4-10 milliseconds
    fill_solid(leds+ledstart71,ledlen71,CHSV(255,0,0));           // Clear the section of LED's
    FastLED.show();
    
    if (flashCounter == 0) delay (150);                       // longer delay until next flash after the leader
    
    delay(50+random8(100));                                   // shorter delay between strokes  
  } // for()
  
  delay(random8(frequency71)*10);                              // delay between strikes
  
      displayPatternChase(convKeyP,  "Lightning");
  }  

  else if (  convKeyP == 72) //     Shift Rainbow
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }

  uint8_t beatA72 = beatsin8(17, 0, 255);                        // Starting hue
  uint8_t beatB72 = beatsin8(13, 0, 255);
  fill_rainbow(leds, NUM_LEDS, (beatA72+beatB72)/2, 8);            // Use FastLED's fill_rainbow routine.

  FastLED.show();

    displayPatternChase(convKeyP,  "Shift Rainbow");
  }

  else if (  convKeyP == 73) //       Chem-Trails
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }

static uint8_t hue = 0;

 for(int i = 0; i < NUM_LEDS; i++) {
   leds[i] = CHSV(0,0,100);
   FastLED.show();
   fadeall();
   delay(120);
 }  
 for(int i = 0; i < NUM_LEDS; i++) {
   leds[i] = CHSV(0,0,138);
   FastLED.show(); 
   fadeall();
   delay(75);
 }
 for(int i = 0; i < NUM_LEDS; i++) {
   leds[i] = CHSV(0,0,177);
   FastLED.show(); 
   fadeall();
   delay(50);
 }
 for(int i = 0; i < NUM_LEDS; i++) {
   leds[i] = CHSV(0,0,216);
   FastLED.show(); 
   fadeall();
   delay(40);
 } 
 for(int i = 0; i < NUM_LEDS; i++) {
   leds[i] = CHSV(0,0,255);
   FastLED.show(); 
   fadeall();
   delay(30);
 } 
 for(int i = (NUM_LEDS)-1; i >= 0; i--) {
   leds[i] = CHSV(128, 255, 255);
   FastLED.show();
   fadeall();
   delay(40);
 }

 for(int i = 0; i < NUM_LEDS; i++) {
   leds[i] = CHSV(128, 255, 255);
   FastLED.show(); 
   fadeall();
   delay(115);
 }
for(int i = (NUM_LEDS)-1; i >= 0; i--) {
leds[i] = CHSV(128, 255, 255);
FastLED.show();
fadeall();
delay(115);
}

    displayPatternChase(convKeyP,  "Chem-Trails");
  }


//  else if (  convKeyP == 74) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }


//  else if (  convKeyP == 75) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }


//  else if (  convKeyP == 76) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }


//  else if (  convKeyP == 77) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }


//  else if (  convKeyP == 78) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }


//  else if (  convKeyP == 79) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }


//  else if (  convKeyP == 80) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }


//  ******************************  Keys 81 to 89    Holiday Scenes   ********************************

  else if (  convKeyP == 81) //    Valentines Day
{ 
        key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
    
     static uint8_t startIndex = 0;
    //  CRGB pattern9B[PATTERN_LEN9B ] = { CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red, CRGB::FloralWhite,
    //                                     CRGB::HotPink, CRGB::HotPink, CRGB::HotPink, CRGB::HotPink };  Top of Code 

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern9B[(PATTERN_LEN9B - startIndex + i) % PATTERN_LEN9B];      
          }

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 9;    //  Divide LED Array Number
    }
    displayPatternChase(convKeyP,  "Valentines Day");
     // Apply the colors to the LED strip     
    FastLED.show();
 }

  else if (  convKeyP == 82) //   420
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
     static uint8_t startIndex = 0;
    //  CRGB pattern7B[PATTERN_LEN7B ] = { CRGB::Red, CRGB::Red, CRGB::Gold, CRGB::Gold, CRGB::Green, CRGB::Green, CRGB::Black };   Top of code

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern7B[(PATTERN_LEN7B - startIndex + i) % PATTERN_LEN7B];      
          }
    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 7;    //  Divide LED Array Number
    }

    displayPatternChase(convKeyP,  "420");
        // Apply the colors to the LED strip     
    FastLED.show();
  }

  else if (  convKeyP == 83) //   St.Patrick's Day
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
    {   
      currentPalette = Irish_p; currentBlending = LINEARBLEND;
        static uint8_t startIndex = 0;
        startIndex = startIndex + 1;      // motion speed  Chase
        FillLEDsFromPaletteColors( startIndex);   
    }
    // Apply the colors to the LED strip     
    FastLED.show();

    displayPatternChase(convKeyP,  "St.Patrick's Day");
  }

  else if (  convKeyP == 84) //     Easter             
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }

       static uint8_t startIndex = 0;
    // CRGB pattern11A[PATTERN_LEN11A ] = { CRGB::PowderBlue, CRGB::PowderBlue, CRGB::LimeGreen, CRGB::LimeGreen, CRGB::HotPink, CRGB::HotPink, 
    //  CRGB::HotPink, CRGB::Yellow, CRGB::Yellow, CRGB::Purple, CRGB::Purple };  //  Key_84  Top of Code 
      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern11A[(PATTERN_LEN11A - startIndex + i) % PATTERN_LEN11A];      
          }

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 11;    //  Divide LED Array Number
    }
    
    displayPatternChase(convKeyP,  "Easter"); 
      // Apply the colors to the LED strip     
    FastLED.show();
  }

  else if (  convKeyP == 85) //     Cinco De Mayo 
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
     static uint8_t startIndex = 0;
     //  CRGB pattern6B[PATTERN_LEN6B ] = { CRGB::Green, CRGB::Green, CRGB::White, CRGB::White, CRGB::Red, CRGB::Red };  Top of code
      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
           leds[i] = pattern6B[(PATTERN_LEN6B - startIndex + i) % PATTERN_LEN6B];      
          }
    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 6;    //  Divide LED Array Number
    }
       // Apply the colors to the LED strip     
        FastLED.show();    
        displayPatternChase(convKeyP,  "Cinco De Mayo ");
  }

  else if (  convKeyP == 86) //     Chinese New Year
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
     static uint8_t startIndex = 0;
    //  CRGB pattern6C[PATTERN_LEN6C ] = { CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Red, CRGB::Red, CRGB::Red };   Top of code

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern6C[(PATTERN_LEN6C - startIndex + i) % PATTERN_LEN6C];   
          currentBlending = LINEARBLEND;     
          }

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        startIndex = (startIndex + 1) % 6;    //  Divide LED Array Number
    }
    // Apply the colors to the LED strip  
     
    FastLED.show();
    displayPatternChase(convKeyP,  "Chinese New Year");
  }

  else if (  convKeyP == 87) //                             Halloween
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }



    FastLED.show();
    displayPatternChase(convKeyP,  "Halloween");
  }

  else if (  convKeyP == 88) //  White Blue Green Chase  3-color Chase Two
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
     static uint8_t startIndex = 0;
    //  CRGB pattern3[PATTERN_LEN3] = { CRGB::White, CRGB::Blue, CRGB::Green };   Top of code

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern3[(PATTERN_LEN3 - startIndex + i) % PATTERN_LEN3];      
          }

    // Apply the colors to the LED strip     
    FastLED.show();

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 3;    //  Divide LED Array Number
    }
    displayPatternChase(convKeyP,  "3-color Chase Two");
  }


  else if (  convKeyP == 89) //   4-color Chase One
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
     static uint8_t startIndex = 0;
    //  CRGB pattern[PATTERN_LEN] = { CRGB::White, CRGB::Red, CRGB::Blue, CRGB::Green };   Top of code

      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern[(PATTERN_LEN - startIndex + i) % PATTERN_LEN];      
          }

    // Apply the colors to the LED strip     
    FastLED.show();

    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        // moving pattern 
        //  FastLED.delay(1000 / UPDATES_PER_SECOND);
        startIndex = (startIndex + 1) % 4;    //  Divide LED Array Number
    }
    displayPatternChase(convKeyP,  "4-color Chase One");
  }

  else if (  convKeyP == 90) //   New Year's Day
  {
//      unsigned long previousMillis;        // Store last time the strip was updated.
// Define variables used by the sequences.
//      uint8_t  thisfade = 8;                    // How quickly does it fade? Lower = slower fade rate.
//      int       thishue = 50;                   // Starting hue.
//      uint8_t   thisinc = 1;                    // Incremental value for rotating hues
//      uint8_t   thissat = 100;                  // The saturation, where 255 = brilliant colours.
//      uint8_t   thisbri = 255;                  // Brightness of a sequence. Remember, max_bright is the overall limiter.
//      int       huediff = 256;                  // Range of random #'s to use for hue
      uint8_t thisdelay = 1;                    // We don't need much delay (if any)

      CRGBPalette16 currentPalette;
      CRGBPalette16 targetPalette;
      TBlendType    currentBlending; 
      
          ChangeMe();                         // Check the demo loop for changes to the variables.

  EVERY_N_MILLISECONDS(100) {
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    confetti();
  }
  
  FastLED.show();
        displayPatternChase(convKeyP,  "New Year's Day");
  }


//  ******************************  Keys 91 to 99    Holiday Chases   ********************************


//  else if (  convKeyP == 91) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }

//  else if (  convKeyP == 92) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }
//  else if (  convKeyP == 93) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }
//  else if (  convKeyP == 94) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }
//  else if (  convKeyP == 95) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }
//  else if (  convKeyP == 96) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }
//  else if (  convKeyP == 97) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }
//  else if (  convKeyP == 98) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }
//  else if (  convKeyP == 99) //L
//  {
//    key = keypad.getKey(); speedAndBrightnessControl();
//      if ((key) && (key == '*')) {
//        lcd.clear(); loop();
//      }
//    //////////////****PASS THE CHASE STYLE AND PATTERN HERE******//////////////
//    displayPatternChase(convKeyP,  "description");
//  }
}

//    ***********************************   END KEY FUNCTIONS   *******************


void welcomeMsg(){
  lcd.print("      WELCOME     "); delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("  WS2815 LED Master  ");
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("      Designed      ");
  delay(100);
  lcd.setCursor(0, 2);
  lcd.print("         And        "); delay(100);
   lcd.setCursor(0, 3); 
  lcd.print("   Engineered by:   "); delay(1000);
   lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("     PITTSBURGH     "); delay(500);
  lcd.setCursor(0, 2);
  lcd.print("     EVENT       "); delay(500);
  lcd.setCursor(0, 3);
  lcd.print("     LIGHTING     "); delay(1000);
  Wire.begin(); // Initialize I2C communication
  delay(1000);
  lcd.clear();
}

void FillLEDsFromPaletteColors( uint8_t colorIndex) //  Load 16 Color palette
{
  //     uint8_t brightness = 255;
  //        Loads color palette
  for ( int i = 0; i < NUM_LEDS; i++) {
//            key = keypad.getKey(); speedAndBrightnessControl();
//            if ((key) && (key == '*')) {
//            lcd.clear(); loop();}
    leds[i] = ColorFromPalette( currentPalette, colorIndex, MASTER_BRIGHTNESS, currentBlending);
    colorIndex += 3;
  }
}


void FillLEDsFromPaletteColors32( uint8_t colorIndex)  //  Load 32 Color palette
{
  //     uint8_t brightness = 255;
  //        Loads color palette(?)
  for ( int i = 0; i < NUM_LEDS; i++) {
//            key = keypad.getKey(); speedAndBrightnessControl();
//            if ((key) && (key == '*')) {
//            lcd.clear(); loop();}
    leds[i] = ColorFromPalette( currentPalette32, colorIndex, MASTER_BRIGHTNESS, currentBlending);
    colorIndex += 1;
  }
}


// ***********************      Speed AND  DIMMING     **********************************************

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

  // *********    END Brightness Control ***********

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
 // delay(10);
  FastLED.delay(SpeedRate);
  // FastLED.show();   //   Is this nessary  ???
  }

// ********************   LCD Display Code    ************************************

void displayPatternChase(int numb, const char* description)
{
   key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
  lcd.setCursor(0, 0);
  lcd.print((String)"Chase: " + numb + "");
  lcd.setCursor(0, 1);
  lcd.print((String)description);
  lcd.setCursor(0, 2);
  lcd.print((String)"Dimmer"+ " " + brightnessRate + "%");
  lcd.setCursor(0, 3);
  lcd.print((String)"Speed" + " " + Speed + "%");
   key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
}


void displayPatternScene(int numb, const char* description)
{
   key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
  lcd.setCursor(0, 0);
  lcd.print((String)"Scene: " + numb + "");
  lcd.setCursor(0, 1);
  lcd.print((String) description);
  lcd.setCursor(0, 2);
  lcd.print((String)"Dimmer"+ " " + brightnessRate + "%");
  lcd.setCursor(0, 3);
  lcd.print((String)"Speed" + " " + Speed + "%");
   key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
}


//    *********************************   Code for Key fuctions   *********************************

//    *********************   Key 49  Fireworks 1   ********************* 

void flare() {
  
//#define NUM_SPARKS 32 // max number (could be NUM_LEDS / 2);
//float sparkPos[NUM_SPARKS];
//float sparkVel[NUM_SPARKS];
//float sparkCol[NUM_SPARKS];
//float flarePos;
//float gravity = -.004; // m/s/s

  flarePos = 0;
  float flareVel = float(random16(50, 90)) / 100; // trial and error to get reasonable range
  float brightness = 1;

  // initialize launch sparks
  for (int i = 0; i < 8; i++) {           //  Start = 5  8 seems to work longer
    sparkPos[i] = 0;
    sparkVel[i] = (float(random8()) / 255) * (flareVel / 5);
    // random around 20% of flare velocity
    sparkCol[i] = sparkVel[i] * 1000;
    sparkCol[i] = constrain(sparkCol[i], 0, 255);
  } 
  // launch 
  FastLED.clear();
  while (flareVel >= -.2) {
    // sparks
    for (int i = 0; i < 5; i++) {
      sparkPos[i] += sparkVel[i];
      sparkPos[i] = constrain(sparkPos[i], 0, 120);
      sparkVel[i] += gravity;
      sparkCol[i] += -.8;
      sparkCol[i] = constrain(sparkCol[i], 0, 255);
      leds[int(sparkPos[i])] = HeatColor(sparkCol[i]);
      leds[int(sparkPos[i])] %= 50; // reduce brightness to 50/255
    }
      // Debugging output
//      Serial.print("i: ");
//      Serial.print(i);
//      Serial.print(", sparkPos[i]: ");
//      Serial.print(sparkPos[i]);
//      Serial.print(", sparkVel[i]: ");
//      Serial.print(sparkVel[i]);
//      Serial.print(", sparkCol[i]: ");
//      Serial.println(sparkCol[i]);
    
    // flare
    leds[int(flarePos)] = CHSV(0, 0, int(brightness * 255));
    FastLED.show();
    FastLED.clear();
    flarePos += flareVel;
    flareVel += gravity;
    brightness *= .985;
  }
}

void explodeLoop() {
 
// #define NUM_SPARKS 32 // max number (could be NUM_LEDS / 2);
//float sparkPos[NUM_SPARKS];
//float sparkVel[NUM_SPARKS];
//float sparkCol[NUM_SPARKS];
//float flarePos;
//float gravity = -.004; // m/s/
  
  int nSparks = flarePos / 2; // works out to look about right
  
  // initialize sparks
  for (int i = 0; i < nSparks; i++) { 
    sparkPos[i] = flarePos; sparkVel[i] = (float(random16(0, 20000)) / 10000.0) - 1.0; // from -1 to 1 
    sparkCol[i] = abs(sparkVel[i]) * 500; // set colors before scaling velocity to keep them bright 
    sparkCol[i] = constrain(sparkCol[i], 0, 255); 
    sparkVel[i] *= flarePos / NUM_LEDS; // proportional to height 
  } 
  sparkCol[0] = 255; // this will be our known spark 
  float dying_gravity = gravity; 
  float c1 = 120; 
  float c2 = 50; 
  while(sparkCol[0] > c2/128) { // as long as our known spark is lit, work with all the sparks
    FastLED.clear();
    for (int i = 0; i < nSparks; i++) { 
      sparkPos[i] += sparkVel[i]; 
      sparkPos[i] = constrain(sparkPos[i], 0, NUM_LEDS); 
      sparkVel[i] += dying_gravity; 
      sparkCol[i] *= .99; 
      sparkCol[i] = constrain(sparkCol[i], 0, 255); // red cross dissolve 
      
      // Debugging output
//      Serial.print("i: ");
//      Serial.print(i);
//      Serial.print(", sparkPos[i]: ");
//      Serial.print(sparkPos[i]);
//      Serial.print(", sparkVel[i]: ");
//      Serial.print(sparkVel[i]);
//      Serial.print(", sparkCol[i]: ");
//      Serial.println(sparkCol[i]);
      
      if(sparkCol[i] > c1) { // fade white to yellow
        leds[int(sparkPos[i])] = CRGB(255, 255, (255 * (sparkCol[i] - c1)) / (255 - c1));
      }
      else if (sparkCol[i] < c2) { // fade from red to black
        leds[int(sparkPos[i])] = CRGB((255 * sparkCol[i]) / c2, 0, 0);
      }
      else { // fade from yellow to red
        leds[int(sparkPos[i])] = CRGB(255, (255 * (sparkCol[i] - c2)) / (c1 - c2), 0);
      }
    }
    dying_gravity *= .995; // as sparks burn out they fall slower
    FastLED.show();
  }
 FastLED.clear();
 FastLED.show();
}

//    *********************   Key 66  FireWorks 2   ********************* 

void F2shoot(byte nr) {
 
  if (random(1000) <10) {
    F2flareShot[nr]=true;
 
  F2flarePos[nr]=random(40,NUM_LEDS-40);
  F2nSparks[nr]=30;
  // initialize sparks
  for (int x = 0; x < F2nSparks[nr]; x++) { 
    F2sparkPos[nr][x] = F2flarePos[nr]<<7;
    F2sparkVel[nr][x] = random16(0, 5120)-2560;  // velocitie original -1 o 1 now -255 to + 255
    word sph = abs(F2sparkVel[nr][x])<<2;
    if (sph>2550) sph=2550;  // set heat before scaling velocity to keep them warm heat is 0-500 but then clamped to 255
    F2sparkHeat[nr][x]=sph ;
  } 
  F2sparkHeat[nr][0] = 5000; // this will be our known spark 
  }
}

 void F2doSparks(byte nr){

  const int maxPos = (NUM_LEDS-1) * 128 ;

    for (int x = 0; x < F2nSparks[nr]; x++) { 
      F2sparkPos[nr][x] = F2sparkPos[nr][x] + (F2sparkVel[nr][x]>>3);   // adjust speed of sparks here  Start = 5
      F2sparkPos[nr][x]=constrain(F2sparkPos[nr][x],0,maxPos);
      F2sparkHeat[nr][x]=scale16(F2sparkHeat[nr][x],64000);   // adjust speed of cooldown here   Start = 64000
 
      CRGB color;
      if (nr==0) color = ColorFromPalette( gPal1  , scale16(F2sparkHeat[nr][x],6600));
      if (nr==1) color = ColorFromPalette( gPal2  , scale16(F2sparkHeat[nr][x],6600));
      if (nr==2) color = ColorFromPalette( gPal3  , scale16(F2sparkHeat[nr][x],6600));
 
 
      leds[F2sparkPos[nr][x]>>7]+=color;
      if (F2sparkHeat[nr][0] < 1) {
         F2flareShot[nr]=false;  // this fireworks is done
        }
    }
  }



//    *********************   Key 90  Confetti   ********************* 
void confetti() {

uint8_t   thisfade = 8;                                        // How quickly does it fade? Lower = slower fade rate.
int       thishue = 140;                                       // Starting hue.
uint8_t   thisinc = 1;                                        // Incremental value for rotating hues
uint8_t   thissat = 255;                                      // The saturation, where 255 = brilliant colours.
uint8_t   thisbri = 255;                                      // Brightness of a sequence. Remember, max_bright is the overall limiter.
int       huediff = 256;                                      // Range of random #'s to use for hue
uint8_t   thisdelay = 1;                                        // We don't need much delay (if any)

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;  

{                                             // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, thisfade);                    // Low values = slower fade.
  int pos = random16(NUM_LEDS);                               // Pick an LED at random.
   leds[pos] = ColorFromPalette(currentPalette, thishue + random16(huediff)/4 , thisbri, currentBlending);
   thishue = thishue + thisinc;                                // It increments here.
} 
}

void ChangeMe()     // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
 {  

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;
TBlendType    currentBlending;  

uint8_t   thisfade = 8;                                        // How quickly does it fade? Lower = slower fade rate.
int       thishue = 140;                                       // Starting hue.
uint8_t   thisinc = 1;                                        // Incremental value for rotating hues
uint8_t   thissat = 255;                                      // The saturation, where 255 = brilliant colours.
uint8_t   thisbri = 255;                                      // Brightness of a sequence. Remember, max_bright is the overall limiter.
int       huediff = 256;                                      // Range of random #'s to use for hue
uint8_t   thisdelay = 1; 

  uint8_t secondHand = (millis() / 1000) % 5;                // IMPORTANT!!! Change '15' to a different value to change duration of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: targetPalette = OceanColors_p; thisinc=1; thishue=192; thissat=255; thisfade=2; huediff=255; break;  // You can change values here, one at a time , or altogether.
      case  5: targetPalette = LavaColors_p; thisinc=2; thishue=128; thisfade=8; huediff=64; break;
      case 10: targetPalette = ForestColors_p; thisinc=1; thishue=random16(255); thisfade=1; huediff=16; break;   
                               // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 15: break;                                         // Here's the matching 15 for the other one.                                                               
    }
  }
} 
//    *********************  END Key 80  Confetti   ********************* 

//    *********************   Key 69  Color Blocks   ********************* 
void beatwave() {
  
  uint8_t wave1 = beatsin8(9, 0, 255);                        // That's the same as beatsin8(9);
  uint8_t wave2 = beatsin8(8, 0, 255);
  uint8_t wave3 = beatsin8(7, 0, 255);
  uint8_t wave4 = beatsin8(6, 0, 255);

  for (int i=0; i<NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, i+wave1+wave2+wave3+wave4, 255, currentBlending); 
  }
  
}

//    *********************   END  Key 69  Color Blocks   ********************* 

//    *********************   Key 70      Sine_One        ********************* 

void one_sine_pal(uint8_t colorIndex) {                                       // This is the heart of this program. Sure is short.
  
  thisphase += thisspeed;                                                     // You can change direction and speed individually.
  
  for (int k=0; k<NUM_LEDS-1; k++) {                                          // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int thisbright = qsubd(cubicwave8((k*allfreq)+thisphase), thiscutoff);    // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = CHSV(bgclr, 255, bgbright);                                     // First set a background colour, but fully saturated.
    leds[k] += ColorFromPalette( currentPalette, colorIndex, thisbright, currentBlending);    // Let's now add the foreground colour.
    colorIndex +=3;
  }
  
  bgclr++;
  
} 
//    *********************  END Key 70  Sine_One   ********************* 

//    *********************   Key 73  Chem-Trails  ********************* 

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

//    *********************   END  Key 73  Chem-Trails   ********************* 


//    *********************         Extrenal Paletts    *********************

//    *********************         Palette   16   *********************
const TProgmemRGBPalette16 myRedBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Blue,
  CRGB::Red,
  CRGB::Blue,

  CRGB::Red,
  CRGB::Blue,
  CRGB::Red,
  CRGB::Blue,

  CRGB::Red,
  CRGB::Blue,
  CRGB::Red,
  CRGB::Blue,

  CRGB::Red,
  CRGB::Blue,
  CRGB::Red,
  CRGB::Blue,
};

//  *****          Palette   16   *****
const TProgmemRGBPalette16 RWB_p  PROGMEM =
{
  CRGB::Blue,
  CRGB::Red, 
  CRGB::White,
  CRGB::Blue,

  CRGB::Red,
  CRGB::White,
  CRGB::Blue,
  CRGB::Red,

  CRGB::White,
  CRGB::Blue,
  CRGB::Red,
  CRGB::White,

  CRGB::Red,
  CRGB::Red,
  CRGB::White,
  CRGB::Blue,
};

//  *****          Palette   16   *****
const TProgmemRGBPalette16 Irish_p PROGMEM =
{
  CRGB::Green,
  CRGB::Green,
  CRGB::DarkOrange,
  CRGB::Green,

  CRGB::Green,
  CRGB::Green,
  CRGB::DarkOrange,
  CRGB::Green,

  CRGB::Green,
  CRGB::Green,
  CRGB::DarkOrange,
  CRGB::Green,

  CRGB::Green,
  CRGB::Green,
  CRGB::DarkOrange,
  CRGB::Green,
};

//  *****          Palette   32   *****
const TProgmemRGBPalette32 FireFG_p FL_PROGMEM =
{
 0xB22222, //FireBrick
 CRGB::Maroon,
 CRGB::Crimson,
 CRGB::OrangeRed,

 CRGB::DarkRed,
 0xFF0040, //  Carmine
 CRGB::DarkRed,
 CRGB::DarkOrange,

 CRGB::Tomato,
 CRGB::DarkRed,
 CRGB::Red,
 CRGB::Orange,

 CRGB::White,
 CRGB::Yellow,
 CRGB::Gold,
 CRGB::DarkRed,

 CRGB::OrangeRed,
 0xFF0040, //  Carmine
 CRGB::Black,
 CRGB::Red,

 CRGB::DarkRed,
 CRGB::Gold,
 0xB22222, //FireBrick
 CRGB::Red,

 CRGB::DarkRed,
 CRGB::DarkOrange,
 CRGB::OrangeRed,
 CRGB::Orange,

 CRGB::White,
 CRGB::Orange,
 CRGB::Red,
 CRGB::DarkRed,
};

//  *****          Palette   32   *****
const TProgmemRGBPalette32 Ocean_p PROGMEM =
{
  CRGB::MidnightBlue,
  CRGB::DarkBlue,
  CRGB::Teal,
  CRGB::Navy,

  CRGB::DarkBlue,
  CRGB::Blue,
  CRGB::SeaGreen,
  CRGB::DarkGreen,

  CRGB::CadetBlue,
  CRGB::Blue,
  CRGB::DarkCyan,
  CRGB::OliveDrab,

  CRGB::MediumSeaGreen,
  CRGB::Aqua, 
  CRGB::SeaGreen,
  CRGB::DarkSeaGreen,

  CRGB::LimeGreen,
  CRGB::DarkTurquoise,
  CRGB::Aqua,
  CRGB::Blue,

  CRGB::DarkBlue,
  CRGB::MediumBlue,
  CRGB::SeaGreen,
  CRGB::Teal,

  CRGB::CadetBlue,
  CRGB::Blue,
  CRGB::DarkCyan,
  CRGB::DarkSlateBlue,

  CRGB::Violet,
  CRGB::MediumPurple,
  CRGB::Indigo,
  CRGB::Purple,
};

//  *****          Palette   16   *****
const TProgmemPalette16 myRedGreenXmassPalette_p PROGMEM =
{
CRGB::Red,
CRGB::Green,
CRGB::Red,
CRGB::Green,

CRGB::Red,
CRGB::Green,
CRGB::Red,
CRGB::Green,

CRGB::Red,
CRGB::Green,
CRGB::Red,
CRGB::Green,

CRGB::Red,
CRGB::Green,
CRGB::Red,
CRGB::Green,
};

//  *****          Palette   16   *****   FIX
const TProgmemPalette16 FourTwentyPalette_p PROGMEM =
{
CRGB::Red,
CRGB::Gold,
CRGB::Green,
CRGB::Red,

CRGB::Gold,
CRGB::Green,
CRGB::Red,
CRGB::Gold,

CRGB::Green,
CRGB::Red,
CRGB::Gold,
CRGB::Green,

CRGB::Red,
CRGB::Gold,
CRGB::Green,
CRGB::Red,
};
//    *********************     END ALL      *********************  