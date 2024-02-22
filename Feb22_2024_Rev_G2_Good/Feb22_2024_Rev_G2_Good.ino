//  February 22, 2024
//  TO-DO  LIST 
//  Test all keys
//
//  Fix Fireworks-1 #49, Ater 5th 'FireWork' ESP Locks-up,  Possibly sending data to LEDs Past end (150)  \Done
//   Fireworks-1 code:  https://www.anirama.com/1000leds/1d-fireworks/ /Done
//   
//  Fix 2nd Fireworks-2  chase #66  Program too slow      \Done
//    Fireworks-2  Code:  https://pastebin.com/auNiA6Zd
//
//  Find a Better Way to Control SPEED, add stop at 100%, prevent rollover  \Done
//
//  Fixed Lines 1362 to 1382  Dim & Speed   \Done
//   Test Frames per seconds  max 400       \Done

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

//  #define FASTLED_ALLOW_INTERRUPTS 0

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
#define LED_TYPE    WS2812b  
#define COLOR_ORDER GBR
//   #define FASTLED_ESP32_SPI_BUS HSP   //  ??????????
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

//  *********************************             Juggle_Pal   int          ****************   
uint8_t    numdots =   4;                                     // Number of dots in use.
uint8_t   thisfade =   2;                                     // How long should the trails be. Very low value = longer trails.
uint8_t   thisdiff =  16;                                     // Incremental change in hue between each dot.
uint8_t    thishue =   0;                                     // Starting hue.
uint8_t     curhue =   0;                                     // The current hue
uint8_t    thissat = 255;                                     // Saturation of the colour.
uint8_t thisbright = 255;                                     // How bright should the LED/display be.
uint8_t   thisbeat =   5;                                     // Higher = faster movement.


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
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.  Start at 192
int thisdelay = 30;                                           // You can change the delay. Also you can change the allspeed variable above.   Start at 30
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

//  *********************************             Key #76  Tripin Kitt          ****************  

uint8_t colorPick;
// Custom color array
CRGB colorArray[] = {
  CRGB::Red,
  CRGB(0x2E8B57),     //  Sea Green
  CRGB::Blue,
  CRGB::Cyan,
  CRGB::LimeGreen,
  CRGB(0xFF3000),     //  Dark Orange
  CRGB(0xFFFF20),     //  Lemon
  CRGB(0xFF1040),     //  Magenta
  CRGB(0,255,0),      //  Green
  CHSV(195,255,255),  //  Pale Cyan
};
//  *********************************    END      Key #76  Tripin Kitt          ****************  

//  *********************************             Key #80  Soft Color Change        ****************  

uint8_t hue80;

//  *********************************    END      Key #80 Soft Color Change           ****************  

//  *********************************             Key #36  Blend Rainbow        ****************  
// https://github.com/marmilicious/FastLED_examples/blob/master/blend_into_rainbow.ino

uint8_t pos36;  //stores a position for color being blended in
uint8_t hue36, hue37;  //stores a color


//  *********************************    END      Key #36  Blend Rainbow        ****************  

//  *********************************             Key #92  EKG        ****************  
uint8_t bloodHue = 96;  // Blood color [hue from 0-255]  Red0  96 Green
uint8_t bloodSat = 255;  // Blood staturation [0-255]
int flowDirection = 1;   // Use either 1 or -1 to set flow direction
uint16_t cycleLength = 1500;  // Lover values = continuous flow, higher values = distinct pulses.
uint16_t pulseLength = 150;  // How long the pulse takes to fade out.  Higher value is longer.
uint16_t pulseOffset = 200;  // Delay before second pulse.  Higher value is more delay.
uint8_t baseBrightness = 20;  // Brightness of LEDs when not pulsing. Set to 0 for off.   Start = 10

//  *********************************    END      Key #92  EKG        ****************  

//  *********************************             Key #94  Cylon        **************** 
// Custom colors to choose from
CRGB colorArray94[] = {
  CRGB(0,255,0),    //green
  CRGB(180,180,180),  //a dim white
  CHSV(0,255,255),  //red
  CRGB::Blue,
  CRGB::Purple,
  CRGB::Aqua,
  CRGB(0xFF1040),  //Magenta
  CRGB(0xFFFF20),  //Lemon
  CRGB::DarkCyan,
  CRGB::HotPink,
  CRGB::LimeGreen
};

uint8_t numberOfColors = sizeof(colorArray94) / sizeof(colorArray94[0]);
uint8_t colorChoice;  //the current color choice
uint8_t pos;  //dot position
//  *********************************    END      Key #94  Cylon        **************** 

//  *********************************             Key #95  End-Fire        **************** 

bool gReverseDirection = false;
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  100     //  Start = 90

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 50     //  Start = 50
//  *********************************    END      Key #95  End-Fire        ****************

//  *********************************             Key #96  Pacifica        ****************
CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

//  *********************************    END      Key #96  Pacifica        ****************

//    *********************                       Key 97  LightHouse        ********************* 
uint8_t hue97 = 42;  // Lighthouse beacon color
uint8_t sat97 = 190;  // Saturation
uint8_t val = BRIGHTNESS;  // Max brightness of light

int delta97 = 1; // Number of 16ths of a pixel to move.  (Use negative value for reverse.)
int width97  = 20; // width of light in pixels
uint16_t holdTime97 = 1;  // Milliseconds to hold between microsteps. (Bigger moves slower.)   Start = 80
uint8_t fadeRate97 = 5;  // Fade out end a bit.

int pixelPos = 0; // position of the "fraction-based bar" [don't edit]
//    *********************              END      Key 97  LightHouse        ********************* 

//    *********************                       Key 99  Scan & Wipe        ********************* 
CRGB bgColor = CHSV(50,120,30);
CRGB scanColor = CHSV(128,240,255);
CRGB wipeColor = CHSV(95,200,90);
CRGB fgColor = wipeColor;

// Specify the total time for dot to scan down LED strip in MILLISECONDS
uint32_t scanTotalTime = 500;      //  Start = 4096
// Calculated scan step time (in milliseconds) per pixel
uint32_t scanStepTime = (scanTotalTime / NUM_LEDS);
// Make the wipe run equal to or faster then the scan
// Divide scanStepTime by 1 or more. Try values like 1,2,4,etc.
uint16_t wipeStepTime = (scanStepTime / 4 );  // [milliseconds]
uint8_t wipeStart;     // wipe start pixel position
uint8_t wipeEnd;       // wipe end pixel position
uint8_t scanPosition;  // keeps track of position

//    *********************              END      Key 99  Scan & Wipe        ********************* 

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
  CRGB pattern6A[PATTERN_LEN6A ] = { CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Gold, CRGB::Gold, CRGB::Gold };  //  Key_58 and 59     UKraine
#define PATTERN_LEN6B 6
  CRGB pattern6B[PATTERN_LEN6B ] = { CRGB::Green, CRGB::Green, CRGB::White, CRGB::White, CRGB::Red, CRGB::Red };  //  Key_85
#define PATTERN_LEN6C 6
  CRGB pattern6C[PATTERN_LEN6C ] = { CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Red, CRGB::Red, CRGB::Red };  //  Key_86   China
#define PATTERN_LEN6D 6
  CRGB pattern6D[PATTERN_LEN6D ] = { CRGB::Orange, CRGB::Orange, CRGB::DarkRed, CRGB::DarkRed, CRGB::White, CRGB::White };  //  Key_87 Candy Corn

#define PATTERN_LEN7A 7
  CRGB pattern7A[PATTERN_LEN7A] = { CRGB::Purple, CRGB::Blue, CRGB::Aqua, CRGB::Green, CRGB::Yellow, CRGB::DarkOrange, CRGB::Red };  //  Key_54
#define PATTERN_LEN7B 7
  CRGB pattern7B[PATTERN_LEN7B ] = { CRGB::Green, CRGB::Green, CRGB::Gold, CRGB::Gold, CRGB::Red, CRGB::Red, CRGB::Black };   //  Key_55 and 82  Bob Marley
#define PATTERN_LEN7C 7
  CRGB pattern7C[PATTERN_LEN7C ] = { CRGB::Orange, CRGB::Orange, CRGB::DarkRed, CRGB::DarkRed, CRGB::White, CRGB::White, CRGB::Black };  //  Key_87 Candy Corn  

#define PATTERN_LEN9A 9
  CRGB pattern9A[PATTERN_LEN9A ] = { CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Green, CRGB::Green, CRGB::Green };  //  Key_56 and 57
#define PATTERN_LEN9B 9
  CRGB pattern9B[PATTERN_LEN9B ] = { CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red, CRGB::FloralWhite, CRGB::HotPink, CRGB::HotPink, CRGB::HotPink, CRGB::HotPink };  //  Key_81   Valentine's Day

#define PATTERN_LEN10A 10
  CRGB pattern10A[PATTERN_LEN10A ] = { CRGB::Red, CRGB::DarkOrange, CRGB::Yellow, CRGB::LimeGreen, CRGB::Green,
                                       CRGB::Aqua, CRGB::Blue, CRGB::DarkViolet, CRGB::DeepPink, CRGB::Crimson };  //  Key_63   Step-Rainbow

#define PATTERN_LEN11A 11
  CRGB pattern11A[PATTERN_LEN11A ] = { CRGB::PowderBlue, CRGB::PowderBlue, CRGB::LimeGreen, CRGB::LimeGreen, CRGB::HotPink,
                                       CRGB::HotPink, CRGB::HotPink, CRGB::Yellow, CRGB::Yellow, CRGB::Purple, CRGB::Purple };  //  Key_84  

#define PATTERN_LEN13A 13
  CRGB pattern13A[PATTERN_LEN13A ] = { CRGB::Red, CRGB::Red,  CRGB::DarkOrange, CRGB::DarkOrange, CRGB::Yellow, CRGB::Yellow,
                                       CRGB::White, CRGB::Yellow, CRGB::Yellow, CRGB::DarkOrange, CRGB::DarkOrange, CRGB::Red, CRGB::Red };  //  Key_77 Halloween
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

extern CRGBPalette16 FourTwenty;
extern const TProgmemPalette16 FourTwenty_p PROGMEM;

extern CRGBPalette16 myRedGreenXmassPalette;
extern const TProgmemPalette16 myRedGreenXmassPalette_p PROGMEM;

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

 //   Key #?? Void Setup  ***********************

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
    displayPatternScene(convKeyP, "Yellow Lemon");
     FastLED.show();
  }

  else if (  convKeyP == 5)    // Orange 
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB(0xFF3500));   //Adjusted
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
    fill_solid( leds, NUM_LEDS, CRGB(0xFF00FF));
    displayPatternScene(convKeyP, "Purple");
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

//  ***********************  Keys 10 to 20    More Solid Colors   ******************

  else if (convKeyP == 10)  // Dark Orange
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB(0xFF3000));   //Adjusted
    displayPatternScene(convKeyP, "Dark Orange");
    FastLED.show();
  }

  else if (convKeyP == 11)  // Sky Blue
  {
      key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::SkyBlue);
    displayPatternScene(convKeyP, "Sky Blue");
    FastLED.show();
  }

  else if (convKeyP == 12)  // Pink
  {
      key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::HotPink);
    displayPatternScene(convKeyP, "Pink");
    FastLED.show();
  }

  else if (convKeyP == 13)  // Dark Cyan
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::DarkCyan);
    displayPatternScene(convKeyP, "Dark Cyan");
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

  else if (convKeyP == 15)  // Chartreuse
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB(0x70FF00));   //Adjusted
    displayPatternScene(convKeyP, "Chartreuse");
    FastLED.show();
  }

  else if (convKeyP == 16)  //  Light Blue
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB(0x80B0FF));   //Adjusted
    displayPatternScene(convKeyP, "Light Blue");
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

  else if (  convKeyP == 19) //  Salmon
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::Salmon);
    displayPatternScene(convKeyP, "Salmon");
    FastLED.show();
  }

  else if (  convKeyP == 20)   //  Yellow   
  {  
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    fill_solid( leds, NUM_LEDS, CRGB::Yellow);
   displayPatternScene(convKeyP, "Yellow");
      FastLED.show();
  }

//  ******************************  Keys 21 to 30    Static Multi-Color Scenes   ********************************

  else if (  convKeyP == 21)        //    Fire
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

  else if (  convKeyP == 22)          //    Irish
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

  else if (  convKeyP == 24) //     Jammin’ Jamaica
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
       currentPalette = FourTwenty_p; currentBlending = LINEARBLEND;
static uint8_t startIndex = 0;
// startIndex = startIndex + 1; / motion speed / //Remove Chase
FillLEDsFromPaletteColors( startIndex);
displayPatternScene(convKeyP,"Jammin' Jamaica");
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

  else if (  convKeyP == 27) //     Cloudy Day
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

  else if (  convKeyP == 30) //        Faded Red White & Blue
  {  
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    currentPalette = RWB_p; currentBlending = LINEARBLEND;
    static uint8_t startIndex = 0;
    FillLEDsFromPaletteColors( startIndex);
   displayPatternScene(convKeyP, "Red White Blue Fade");
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
  startIndex = startIndex - 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);
    displayPatternChase(convKeyP,  "Red White & Blue");
       FastLED.show();
  }

  else if (  convKeyP == 34) //         Glitter Rainbow
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
    displayPatternChase(convKeyP,  "Glitter Rainbow");
    }
  
  else if (  convKeyP == 35) //         Popcorn       
  {
   key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
       static uint8_t gHue = 0;
       gHue = gHue + 1;                           //* motion speed */

 // Call the current pattern function once, updating the 'leds' array
          fadeToBlackBy( leds, NUM_LEDS, 10);  //(good fade #)
            int pos = random8(NUM_LEDS);
            leds[pos] += CHSV( gHue + random8(255), 255, 255); 
                        leds[pos] += CHSV( (gHue + 1) + random8(255), 255, 255); 

            int pos2 = random8(NUM_LEDS);
           leds[pos2] += CHSV( gHue + random8(255), 255, 255); // Good 2*Times the dots
           leds[pos2] += CHSV( (gHue + 1) + random8(255), 255, 255); // Good 2*Times the dots

       FastLED.show();
        displayPatternChase(convKeyP,  "Popcorn");
  }

  else if (  convKeyP == 36) //         Blend Rainbow
  {
   key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }

  EVERY_N_MILLISECONDS(35){          // start at 100
    hue36++;  //used to cycle through the rainbow
  }
  fill_rainbow(leds, NUM_LEDS, hue36, 255/NUM_LEDS/4);  //draw part of the rainbow into the strip
  fadeToBlackBy(leds, NUM_LEDS, 70);  //fade the whole strip down some

  EVERY_N_MILLISECONDS(10) {                  //   Start at 130
    pos36++;
    if (pos36 == NUM_LEDS) { pos36 = 0; }  //reset to begining
  }

  EVERY_N_MILLISECONDS(5){
    hue37 = hue37 - 1;  //used to change the moving color
  }
  CRGB blendThisIn  = CHSV(hue37, 170, 255);  //color to blend into the background
  CRGB blendThisIn1 = CHSV(hue37, 185, 225);  //color to blend into the background
  CRGB blendThisIn2 = CHSV(hue37, 195, 225);  //color to blend into the background
  CRGB blendThisIn3 = CHSV(hue37, 210, 200);  //color to blend into the background
  CRGB blendThisIn4 = CHSV(hue37, 220, 200);  //color to blend into the background
  CRGB blendThisIn5 = CHSV(hue37, 230, 190);  //color to blend into the background
  CRGB blendThisIn6 = CHSV(hue37, 245, 180);  //color to blend into the background
  CRGB blendThisIn7 = CHSV(hue37, 255, 180);  //color to blend into the background

  static uint8_t blendAmount = 250;  //amount of blend  [range: 0-255] start at 240  more more color difference from base

  nblend(leds[pos36],                  blendThisIn4, blendAmount/4);
  nblend(leds[mod8(pos36+1,NUM_LEDS)], blendThisIn4, blendAmount/3);
  nblend(leds[mod8(pos36+2,NUM_LEDS)], blendThisIn3, blendAmount/3);
  nblend(leds[mod8(pos36+3,NUM_LEDS)], blendThisIn3,  blendAmount/2);
  nblend(leds[mod8(pos36+4,NUM_LEDS)], blendThisIn2, blendAmount/2);
  nblend(leds[mod8(pos36+5,NUM_LEDS)], blendThisIn2, blendAmount);
  nblend(leds[mod8(pos36+6,NUM_LEDS)], blendThisIn, blendAmount);
  
  nblend(leds[mod8(pos36+7,NUM_LEDS)], blendThisIn, blendAmount);
  nblend(leds[mod8(pos36+8,NUM_LEDS)], blendThisIn2, blendAmount);
  nblend(leds[mod8(pos36+9,NUM_LEDS)], blendThisIn2, blendAmount/2);
  nblend(leds[mod8(pos36+10,NUM_LEDS)], blendThisIn3,  blendAmount/2);
  nblend(leds[mod8(pos36+11,NUM_LEDS)], blendThisIn3, blendAmount/3);
  nblend(leds[mod8(pos36+12,NUM_LEDS)], blendThisIn4, blendAmount/3);
  nblend(leds[mod8(pos36+13,NUM_LEDS)], blendThisIn4, blendAmount/4);

  FastLED.show();  //update the display
//  EVERY_N_SECONDS(random8(5,21)) {
//    delay(3000);  //randomly stop the animation to observe the blending
//  }
    displayPatternChase(convKeyP,  "Blend Rainbow");
 }

  else if (  convKeyP == 37) //         Ticker Tape
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        static uint8_t gHue = 0;
           gHue = gHue - 1;                    //* motion speed */       Start at 30
              const uint16_t spewSpeed = 10;  // rate of advance         Start at 200
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

  else if (  convKeyP == 39) //         Fire With Sparks
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
       currentPalette32 = FireFG_p;
       currentBlending = LINEARBLEND;
  static uint16_t startIndex = 0;
  startIndex = startIndex - 1; /* motion speed */
  FillLEDsFromPaletteColors32( startIndex);     

     { if( random8() < 120)    //Good Amount of sparkles       
  leds[ random16(NUM_LEDS) ] += CRGB::White;}
  
      displayPatternChase(convKeyP,  "Fire With Sparks");
       FastLED.show();
  }

  else if (  convKeyP == 40) //                 peppermint candy
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        candyCane2();

  FastLED.show();
    displayPatternChase(convKeyP,  "Peppermint Candy");
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
          startIndex = startIndex - 1; /* motion speed */
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
        startIndex = startIndex - 1; /* motion speed */
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
        startIndex = startIndex - 1; /* motion speed */
        FillLEDsFromPaletteColors( startIndex);
                    { if( random8() < 120)                          //Good Amount of sparkles       
                      leds[ random16(NUM_LEDS) ] += CRGB::White; 
                      leds[ random16(NUM_LEDS) + 1] += CRGB::White;} 
    
    displayPatternChase(convKeyP,  "Drunk Xmass");
       FastLED.show();
  }
 
  else if (  convKeyP == 46) //         Juggle
                             // eight colored dots, weaving in and out of sync with each other
  {key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
      //    juggle();
            juggle2();
      //    FastLED.delay(1000.f / 120 - (NUM_LEDS * 0.03f + 0.59f));
  
  //   EVERY_N_MILLISECONDS(100)
  //   {                                 // AWESOME palette blending capability provided as required.
  //  uint8_t maxChanges = 24; 
  //  nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   
  //    }
 
//  currentPalette  = CRGBPalette16(CRGB::Black);
//  targetPalette   = RainbowColors_p;
//  currentBlending = LINEARBLEND;  

//  ChangeMe46();
//  juggle_pal();

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
    displayPatternChase(convKeyP,  "Many Rainbows");
  }

//    *****************************************************************************************************

  else if (  convKeyP == 49) //         Fireworks
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
          displayPatternChase(convKeyP,  "Fireworks");   //  Move to top of Key Func   
  // send up flare
  flare();
  
  // explode
  explodeLoop();

  // wait before sending up another
  delay(random16(1000, 4000));

//    displayPatternChase(convKeyP,  "Fireworks");   //  Move to top of Key Func   
 //   FastLED.show(); 
       FastLED.clear();
 // FastLED.show(); 
  }

  else if (  convKeyP == 50)           //4 color LED Chase
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
    } else {
        startIndex = (startIndex + 1) % 4;    //  Divide LED Array Number
    }
    displayPatternChase(convKeyP,  "4-Color Chase #1");
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
        } 
          else {
          // moving pattern 
           startIndex = (startIndex + 1) % 3;    //  Divide LED Array Number
          // Apply the colors to the LED strip     
    FastLED.show();
           }
    displayPatternChase(convKeyP,  "3-Color Chase #1");
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
        }
        else {
        // moving pattern 
        startIndex = (startIndex + 1) % 4;    //  Divide LED Array Number
      }
      displayPatternChase(convKeyP,  "4-Color Chase #2");
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
    } 
    else {
        // moving pattern 
        startIndex = (startIndex + 1) % 4;    //  Divide LED Array Number
    }
    displayPatternChase(convKeyP,  "4-Color Chase #3");
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
        if (UPDATES_PER_SECOND == 0) {
         }
         else {
        startIndex = (startIndex + 1) % 7;    //  Divide LED Array Number
    }
        FastLED.show();
        displayPatternChase(convKeyP,  "Rainbow Step Chase");
  }
 
  else if (  convKeyP == 55) //   Bob Marley
{
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
     static uint8_t startIndex = 0;
    //  CRGB pattern7B[PATTERN_LEN7B ] = { CRGB::Green, CRGB::Green, CRGB::Gold, CRGB::Glod, CRGB::Red, CRGB::Red, CRGB::Black }
      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern7B[(PATTERN_LEN7B - startIndex + i) % PATTERN_LEN7B];      
          }
    // Apply the colors to the LED strip     
          if (UPDATES_PER_SECOND == 0) {
      } 
        else {
        startIndex = (startIndex + 1) % 7;    //  Divide LED Array Number
      }
        FastLED.show();
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
        if (UPDATES_PER_SECOND == 0) {
      } 
        else {
        startIndex = (startIndex + 0) % 9;    //  Divide LED Array Number
      }
          FastLED.show();                          //  (startIndex + 0) for Static Scene
        displayPatternChase(convKeyP,  "Caribbean Scene");
  }

  else if (  convKeyP == 57) //              Caribbean Chase
{ 
     static uint8_t startIndex = 0;
    //  CRGB pattern9A[PATTERN_LEN9A ] = { CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Gold, CRGB::Gold, CRGB::Gold, CRGB::Green, CRGB::Green, CRGB::Green };
      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern9A[(PATTERN_LEN9A - startIndex + i) % PATTERN_LEN9A];      
          }
    // Apply the colors to the LED strip     
          if (UPDATES_PER_SECOND == 0) {
      } 
        else {
        startIndex = (startIndex + 1) % 9;    //  Divide LED Array Number
      }
        FastLED.show();
      displayPatternChase(convKeyP,  "Caribbean Chase");
  }

    else if (  convKeyP == 58) //                    UKraine Scene
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
        if (UPDATES_PER_SECOND == 0) {
      } 
        else {
        startIndex = (startIndex + 0) % 6;    //  Divide LED Array Number
    }  
         FastLED.show();                          //  (startIndex + 0) for Static Scene
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
        if (UPDATES_PER_SECOND == 0) {
      } 
        else {
        startIndex = (startIndex + 1) % 6;    //  Divide LED Array Number
      }
        FastLED.show();
      displayPatternChase(convKeyP,  "Ukraine Chase");
  }

  else if (  convKeyP == 60) //L         RGB
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
      } 
        else {
        startIndex = (startIndex + 1) % 3;    //  Divide LED Array Number
         // Apply the colors to the LED strip     
    FastLED.show();
           }
    displayPatternChase(convKeyP,  "3-Color RGB Chase");
  }

//  ******************************  Keys 61 to 69    More Chases   ********************************

  else if (  convKeyP == 61) //                4-Color RGB+Black Chase
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
      } 
        else {
          startIndex = (startIndex + 1) % 4;    //  Divide LED Array Number
         // Apply the colors to the LED strip     
      FastLED.show();
           }
      displayPatternChase(convKeyP,  "4-Color RGB+Black Chase");
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
      } 
        else {
        startIndex = (startIndex + 1) % 10;    //  Divide LED Array Number
      }
      // Apply the colors to the LED strip     
          FastLED.show();
        displayPatternChase(convKeyP,  "Step-Rainbow"); 
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
      } 
        else {
        startIndex = (startIndex - 1) % 10;    //  Divide LED Array Number
    }
      // Apply the colors to the LED strip     
        FastLED.show();
          displayPatternChase(convKeyP,  "Step-Rainbow Left"); 
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
    FastLED.show();
    displayPatternChase(convKeyP,  "Rainbow");
   }  

  else if (  convKeyP == 66) //           FireWorks 2 Mult-Color
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        byte nrv;
      }
          displayPatternChase(convKeyP,  "FireWorks 2");    //  Move to top of Key Func  
       Speed95();        
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
 
//          displayPatternChase(convKeyP,  "FireWorks 2");    //  Move to top of Key Func
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
       FastLED.show();
        displayPatternChase(convKeyP,  "Stormy Day");
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
  
   } // loop()
      FastLED.show();
         displayPatternChase(convKeyP,  "Blur");
  }

  else if (  convKeyP == 69) //                          Color Blocks
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        // Palette definitions      ~~ at Line 150
           currentPalette = RainbowColors_p;
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

  else if (  convKeyP == 72) //                          Shift Rainbow
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

  else if (  convKeyP == 74) //         Kinght Rider
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    displayPatternChase(convKeyP,  "Knight Rider");     // Moved to top of Key func.     

      int wsize = 6;
      { for(int dot=(NUM_LEDS - wsize - 2); dot >=0; dot--)
            for(int j = 1; j <= wsize; j++ )
          {
            leds[dot] = CRGB::Red;
            leds[dot - 1] = CRGB::Red;
            leds[dot - 2] = CRGB::Red;
            leds[dot - 3] = CRGB::Red;
            leds[dot - 4] = CRGB::Red;
            leds[dot - 5] = CRGB::Red;

            leds[dot] = CRGB::Black;
            leds[dot + 1] = CRGB::Black;
            leds[dot + 2] = CRGB::Black;
            leds[dot + 3] = CRGB::Black;
            leds[dot + 4] = CRGB::Black;
            leds[dot + 5] = CRGB::Black;

        delay(7);   //   Start delay= 300 needed   Seven is good
        FastLED.show();
          }
 
          for(int dot = 0; dot < (NUM_LEDS - wsize - 2); dot++)
          for(int j = 1; j <= wsize; j++ )
    { 
            leds[dot] = CRGB::Red;
            leds[dot + 1] = CRGB::Red;
            leds[dot + 2] = CRGB::Red;
            leds[dot + 3] = CRGB::Red;
            leds[dot + 4] = CRGB::Red;
            leds[dot + 5] = CRGB::Red;            

            leds[dot] = CRGB::Black;
            leds[dot - 1] = CRGB::Black;
            leds[dot - 2] = CRGB::Black;
            leds[dot - 3] = CRGB::Black;
            leds[dot - 4] = CRGB::Black;
            leds[dot - 5] = CRGB::Black;

        delay(7);   //   Start delay= 300 needed   Seven is good
        FastLED.show();
  }
      }
//    displayPatternChase(convKeyP,  "Knight Rider");     // Moved to top of Key func.      
  }

  else if (  convKeyP == 75) //                   Evil Eye
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        displayPatternChase(convKeyP,  "Evil EYE");  // Moved to top of Key func.      
     int wsize = 23;
    { for(int dot=(NUM_LEDS); dot >=0; dot--)
          for(int j = 1; j <= wsize; j++ )
      {
        leds[dot] = CRGB::Coral;
        leds[dot - 1] = CRGB::White;
        leds[dot - 2] = CRGB::White;
        leds[dot - 3] = CRGB::White;
        leds[dot - 4] = CRGB::White;
        leds[dot - 5] = CRGB::White;
        leds[dot - 6] = CRGB::White;        
        leds[dot - 7] = CRGB::White;
        leds[dot - 8] = CRGB::Red;
        leds[dot - 9] = CRGB::Red;
        leds[dot - 10] = CRGB::Black;
        leds[dot - 11] = CRGB::Cyan;
        leds[dot - 12] = CRGB::Black;
        leds[dot - 13] = CRGB::Red;
        leds[dot - 14] = CRGB::Red;
        leds[dot - 15] = CRGB::White;
        leds[dot - 16] = CRGB::White;        
        leds[dot - 17] = CRGB::White;
        leds[dot - 18] = CRGB::White;
        leds[dot - 19] = CRGB::White;
        leds[dot - 20] = CRGB::White;
        leds[dot - 21] = CRGB::White;
        leds[dot - 22] = CRGB::Coral;

       FastLED.show();

        leds[dot] = CRGB::Black;
        leds[dot - 1] = CRGB::Black;
        leds[dot - 2] = CRGB::Black;
        leds[dot - 3] = CRGB::Black;
        leds[dot - 4] = CRGB::Black;
        leds[dot - 5] = CRGB::Black;
        leds[dot - 6] = CRGB::Black;
        leds[dot - 7] = CRGB::Black;
        leds[dot - 8] = CRGB::Black;
        leds[dot - 9] = CRGB::Black;
        leds[dot - 10] = CRGB::Black;
        leds[dot - 11] = CRGB::Black;
        leds[dot - 12] = CRGB::Black;
        leds[dot - 13] = CRGB::Black;
        leds[dot - 14] = CRGB::Black;
        leds[dot - 15] = CRGB::Black;
        leds[dot - 16] = CRGB::Black;
        leds[dot - 17] = CRGB::Black;
        leds[dot - 18] = CRGB::Black;
        leds[dot - 19] = CRGB::Black;
        leds[dot - 20] = CRGB::Black;
        leds[dot - 21] = CRGB::Black;
        leds[dot - 22] = CRGB::Black;
//      delay(5);   //   Start delay= 300  Maybe not needed
      }
        for(int dot = 0; dot < (NUM_LEDS); dot++)
          for(int j = 1; j <= wsize; j++ )
   { 
        leds[dot] = CRGB::Coral;
        leds[dot - 1] = CRGB::White;
        leds[dot - 2] = CRGB::White;
        leds[dot - 3] = CRGB::White;
        leds[dot - 4] = CRGB::White;
        leds[dot - 5] = CRGB::White;
        leds[dot - 6] = CRGB::White;        
        leds[dot - 7] = CRGB::White;
        leds[dot - 8] = CRGB::Red;
        leds[dot - 9] = CRGB::Red;
        leds[dot - 10] = CRGB::Black;
        leds[dot - 11] = CRGB::Cyan;
        leds[dot - 12] = CRGB::Black;
        leds[dot - 13] = CRGB::Red;
        leds[dot - 14] = CRGB::Red;
        leds[dot - 15] = CRGB::White;
        leds[dot - 16] = CRGB::White;        
        leds[dot - 17] = CRGB::White;
        leds[dot - 18] = CRGB::White;
        leds[dot - 19] = CRGB::White;
        leds[dot - 20] = CRGB::White;
        leds[dot - 21] = CRGB::White;
        leds[dot - 22] = CRGB::Coral;

FastLED.show();

        leds[dot] = CRGB::Black;
        leds[dot - 1] = CRGB::Black;
        leds[dot - 2] = CRGB::Black;
        leds[dot - 3] = CRGB::Black;
        leds[dot - 4] = CRGB::Black;
        leds[dot - 5] = CRGB::Black;
        leds[dot - 6] = CRGB::Black;
        leds[dot - 7] = CRGB::Black;
        leds[dot - 8] = CRGB::Black;
        leds[dot - 9] = CRGB::Black;
        leds[dot - 10] = CRGB::Black;
        leds[dot - 11] = CRGB::Black;
        leds[dot - 12] = CRGB::Black;
        leds[dot - 13] = CRGB::Black;
        leds[dot - 14] = CRGB::Black;
        leds[dot - 15] = CRGB::Black;
        leds[dot - 16] = CRGB::Black;
        leds[dot - 17] = CRGB::Black;
        leds[dot - 18] = CRGB::Black;
        leds[dot - 19] = CRGB::Black;
        leds[dot - 20] = CRGB::Black;
        leds[dot - 21] = CRGB::Black;
        leds[dot - 22] = CRGB::Black;
  //      delay(5);         //   Start delay= 300  Maybe not needed
      }
    }
//        displayPatternChase(convKeyP,  "Evil EYE");  // Moved to top of Key func.
  }                      
     //    ************************   End Key 75    *******************************

  else if (  convKeyP == 76) //                   Tripin' Kitt
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
      displayPatternChase(convKeyP,  "Tripin' Kitt");             // Moved to top of Key func.
             //pick a random color from the custom color array      
             colorPick = random8( sizeof(colorArray) / sizeof(colorArray[0]) );
                for (uint8_t a76 = 0; a76 < NUM_LEDS; a76++) 
                 {   leds[a76] = colorArray[colorPick];  }
     int wsize = 6;       //   Start at 4

    { for(int dot=(NUM_LEDS - wsize - 2); dot >=0; dot--)
          for(int j = 1; j <= wsize; j++ )
      {
        fill_solid( leds, NUM_LEDS, CRGB::Black);

        leds[dot] = colorArray[colorPick];
        leds[dot - 1] = colorArray[colorPick];
        leds[dot - 2] = colorArray[colorPick];
        leds[dot - 3] = colorArray[colorPick];
        leds[dot - 4] = colorArray[colorPick];
        leds[dot - 5] = colorArray[colorPick];

        leds[dot] = CRGB::Black;
        leds[dot + 1] = CRGB::Black;
        leds[dot + 2] = CRGB::Black;
        leds[dot + 3] = CRGB::Black;
        leds[dot + 4] = CRGB::Black;
        leds[dot + 5] = CRGB::Black;

        delay(7);   //   Start delay= 300 needed   Seven is good
        FastLED.show();
      }
         for(int dot = 0; dot < (NUM_LEDS - wsize - 2); dot++)
          for(int j = 1; j <= wsize; j++ )
      { 
        leds[dot] = colorArray[colorPick];
        leds[dot + 1] = colorArray[colorPick];
        leds[dot + 2] = colorArray[colorPick];
        leds[dot + 3] = colorArray[colorPick];
        leds[dot + 4] = colorArray[colorPick];
        leds[dot + 5] = colorArray[colorPick];

        leds[dot] = CRGB::Black;
        leds[dot - 1] = CRGB::Black;
        leds[dot - 2] = CRGB::Black;
        leds[dot - 3] = CRGB::Black;
        leds[dot - 4] = CRGB::Black;
        leds[dot - 5] = CRGB::Black;

        delay(7);   //   Start delay= 300 needed   Seven is good
      FastLED.show();   }
    }
//    displayPatternChase(convKeyP,  "Tripin' Kitt");     // Moved to top of Key func.
  }

  else if (  convKeyP == 77)      //    Halloween #1
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
       static uint8_t startIndex = 0;
//  CRGB pattern11B[PATTERN_LEN11B ] = { CRGB::Red, CRGB::Red,  CRGB::DarkOrange, CRGB::DarkOrange, CRGB::Yellow,
//                                     CRGB::Yellow, CRGB::White, CRGB::Yellow, CRGB::Yellow, CRGB::DarkOrange, CRGB::Red };  //  Key_77
      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern13A[(PATTERN_LEN13A - startIndex + i) % PATTERN_LEN13A];      
          }
    if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
        // again
       //  FastLED.delay(1000);
    } else {
        startIndex = (startIndex + 1) % 13;    //  Divide LED Array Number
    }
    displayPatternChase(convKeyP,  "Halloween #1");
    FastLED.show();
  }

  else if (  convKeyP == 78)                //        4th of July Chase #2
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        currentPalette = RWB_p; currentBlending = LINEARBLEND;
            static uint8_t startIndex = 0;
                startIndex = startIndex - 1;      // motion speed 
                  FillLEDsFromPaletteColors( startIndex);

  displayPatternScene(convKeyP,"4th of July #2");
  FastLED.show();
  }

  else if (  convKeyP == 79)               //        4th of July Chase #3
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
      currentPalette = RWB_p; currentBlending = LINEARBLEND;
            static uint8_t startIndex = 0;
                startIndex = startIndex - 1;                        // motion speed 
                  FillLEDsFromPaletteColors( startIndex);

                    { if( random8() < 120)                          //Good Amount of sparkles       
                      leds[ random16(NUM_LEDS) ] += CRGB::White; 
                      leds[ random16(NUM_LEDS) + 1] += CRGB::White;} 
                    { if( random8() < 120)
                      leds[ random16(NUM_LEDS) ] += CRGB::White;
                      leds[ random16(NUM_LEDS) + 1] += CRGB::White;} 
  displayPatternScene(convKeyP,"4th of July #3");
  FastLED.show();
  }

  else if (  convKeyP == 80)                //    Soft Color Change
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
 EVERY_N_MILLISECONDS(20) {       // Start=20
    hue80++;  // slowly cycle around the color wheel
  }
  EVERY_N_MILLISECONDS(40) {
    fill_solid(leds, NUM_LEDS, CHSV(hue80, 175, 140) );
    // A few different things to try out. 
    // Un-comment one of these at a time.
    //addGlitter(99);  // white, the same as in DemoReel100 example
    addMatchingGlitter(99);
    addMatchingGlitter_2(99);
    addMatchingGlitter_3(99);
    addMatchingGlitter_4(99);
  }
   FastLED.show();
   displayPatternChase(convKeyP,  "Soft Color Fade");
  }

//  ******************************  Keys 81 to 89    Holiday Scenes   ********************************

  else if (  convKeyP == 81)                //    Valentines Day
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
          } else {
            startIndex = (startIndex + 1) % 9;    //  Divide LED Array Number
          }
      displayPatternChase(convKeyP,  "Valentines Day");
      FastLED.show();
  }

  else if (  convKeyP == 82) //   420
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
        currentPalette = FourTwenty_p; currentBlending = LINEARBLEND;
          static uint8_t startIndex = 0;
          startIndex = startIndex - 1;              // + Right to Left / 
          FillLEDsFromPaletteColors( startIndex);;
                    { if( random8() < 120)                          //Good Amount of sparkles       
                      leds[ random16(NUM_LEDS) ] += CRGB::White; 
                      leds[ random16(NUM_LEDS) + 1] += CRGB::White;} 
      FastLED.show();
      displayPatternChase(convKeyP,  "420");
  }

  else if (  convKeyP == 83) //   St.Patrick's Day
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
    {   
        currentPalette = Irish_p; currentBlending = NOBLEND;
          static uint8_t startIndex = 0;
          startIndex = startIndex - 1;              // + Right to Left / 
          FillLEDsFromPaletteColors( startIndex);;
                    { if( random8() < 120)                          //Good Amount of sparkles       
                      leds[ random16(NUM_LEDS) ] += CRGB::White; 
                      leds[ random16(NUM_LEDS) + 1] += CRGB::White;} 
    }
    // Apply the colors to the LED strip     
    FastLED.show();
    displayPatternChase(convKeyP,  "St. Patrick's Day");
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
          } else {
            startIndex = (startIndex + 1) % 11;    //  Divide LED Array Number
      }
        displayPatternChase(convKeyP,  "Easter"); 
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
        }   else {
            startIndex = (startIndex + 1) % 6;    //  Divide LED Array Number
      }
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
        }   else {
         startIndex = (startIndex + 1) % 6;    //  Divide LED Array Number
      }
        FastLED.show();
        displayPatternChase(convKeyP,  "Chinese New Year");
  }

  else if (  convKeyP == 87)          //                 Candy Corn
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
        }
     static uint8_t startIndex = 0;
    //  CRGB pattern7C[PATTERN_LEN7C ] = { CRGB::Orange, CRGB::Orange, CRGB::DarkRed, CRGB::DarkRed, CRGB::White, CRGB::White, CRGB::Black };    Top of code
      // Apply the pattern repeating across all LEDs
          for( int i =0; i < NUM_LEDS; i++) {
          leds[i] = pattern7C[(PATTERN_LEN7C - startIndex + i) % PATTERN_LEN7C];   
          currentBlending = LINEARBLEND;     
          }
            if (UPDATES_PER_SECOND == 0) {
          } else {
            startIndex = (startIndex + 1) % 7;    //  Divide LED Array Number
      }
      FastLED.show();
    displayPatternChase(convKeyP,  "Candy Corn");
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
            if (UPDATES_PER_SECOND == 0) {
        // static display, sleep one second then
        // essentially apply the same LED settings
         } else {
        startIndex = (startIndex + 1) % 3;    //  Divide LED Array Number
    }
      FastLED.show();
      displayPatternChase(convKeyP,  "3-Color Chase #2");
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
    } else {
        startIndex = (startIndex + 1) % 4;    //  Divide LED Array Number
    }
      FastLED.show();
      displayPatternChase(convKeyP,  "4-Color Chase #1");
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
 //     uint8_t thisdelay = 1;                    // We don't need much delay (if any)   changed Feb6,2024
      CRGBPalette16 currentPalette;
      CRGBPalette16 targetPalette;
      TBlendType    currentBlending; 
      
          ChangeMe();                         // Check the demo loop for changes to the variables.

  EVERY_N_MILLISECONDS(25) {                 //   Start at 100
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   // AWESOME palette blending capability.
  }

  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    confetti();
  }
    FastLED.show();
        displayPatternChase(convKeyP,  "New Year's Day");
  }

//  ******************************  Keys 91 to 99    More Chases   ********************************

  else if (  convKeyP == 91)                  //  Candy Cane
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        candyCane();

//      FastLED.show();
    displayPatternChase(convKeyP,  "Candy Cane");
  }

  else if (  convKeyP == 92)                  //    Heart Pulse  EKG
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
          heartBeat();  // Heart beat function
              FastLED.show();
    displayPatternChase(convKeyP,  "EKG     ");
  }

  else if (  convKeyP == 93)                //      Sparkles
  {  
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
  // sparkles(duration, amount, spread)
  // duration: how long a sparkel lasts [max 255]   Start (80,200)
  // amount: how often a new random pixel lights up [0-100] Start = 100
  // spread: how spread out a sparkle becomes [0-172] Start = 30
      sparkles(random8(20,220), 100, 150);  // sparkle duration, amount, and spread
      FastLED.show();
    displayPatternChase(convKeyP,  "Sparkles");
  }

  else if (  convKeyP == 94)        //    Cylon
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
    Speed100();
  //Comment or un-comment one of these examples to try each out.
  //cylon_cc1();
  cylon_cc2();
 
    FastLED.show();
    displayPatternChase(convKeyP,  "Cylon");
  }


  else if (  convKeyP == 95)        //          End Fire
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
  // Add entropy to random number generator; we use a lot of it.
         random16_add_entropy( random());

    Fire2012(); // run simulation frame
  
      mirror2ndHalf();  // copy and mirror first half of strip to second half
        FastLED.show(); // display this frame
        displayPatternChase(convKeyP,  "End Fire");
  }

  else if (  convKeyP == 96)          //     Pacifica 
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        EVERY_N_MILLISECONDS( 20) {
          pacifica_loop();
          FastLED.show();
  }
    displayPatternChase(convKeyP,  "Pacifica");
  }

  else if (  convKeyP == 97)          //    LightHouse
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
        EVERY_N_MILLISECONDS(holdTime97) {  // wait a bit before micro advancing
          if (delta97 > 0) {
          pixelPos += delta97;
        } else {
            pixelPos = (pixelPos + delta97 + (NUM_LEDS * 16)) % (NUM_LEDS * 16);
        }
            if( pixelPos >= (NUM_LEDS * 16)) {
              pixelPos -= (NUM_LEDS * 16);
              }
          static byte countdown = 0;
            if( countdown == 0) { countdown = 16; } // reset countdown
                countdown -= 1;  // decrement once each time through

            memset8( leds, 0, NUM_LEDS * sizeof(CRGB));  // Clear the pixel buffer
            drawFractionalBar( pixelPos, width97, hue97);  // Draw the pixels
          }
            FastLED.show();  // Show the pixels
            displayPatternChase(convKeyP,  "LightHouse");
  }

  else if (  convKeyP == 98)          //      Twinkle
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }
  static byte offset98 = 0;
  EVERY_N_MILLISECONDS(200) {offset98++;}
  random16_set_seed(23579);
  double a = millis();
  for (int i = 0; i < NUM_LEDS; i++) {
    byte hue = sin8((random16() / 256) + inoise8(random16() / 256 + a / 32) + i * 4);
    byte bright = sin8(random16() / 256 + inoise8(random16() / 256 + a / 32) + a / 8);
    nblend(leds[i], CHSV(hue + offset98, 255, bright), 128);
  }
  //delay(20);
    FastLED.show();
    displayPatternChase(convKeyP,  "Twinkle");
  }

  else if (  convKeyP == 99)          //          Scan & Wipe
  {
    key = keypad.getKey(); speedAndBrightnessControl();
      if ((key) && (key == '*')) {
        lcd.clear(); loop();
      }

      Speed100();

    EVERY_N_MILLISECONDS(wipeStepTime) {
    fill_solid(leds, NUM_LEDS, bgColor);

    if (wipeEnd < NUM_LEDS-1) {
      wipeEnd++;
      fgColor = wipeColor;
    }
    else if (wipeStart < NUM_LEDS-1) {
      wipeStart++;
    }
    else if (fgColor == bgColor) {
      wipeStart = 0;  // reset wipe
      wipeEnd = 0;
      fgColor = wipeColor;
    }
    else if (wipeStart == NUM_LEDS-1 && wipeEnd == NUM_LEDS-1) {
      fgColor = bgColor;
    }
    
    fill_gradient_RGB (leds, wipeStart, fgColor, wipeEnd, fgColor );
    leds[scanPosition] = scanColor;
    blur1d(leds, NUM_LEDS, 50);  // (optional)
  }

  EVERY_N_MILLISECONDS(scanStepTime) {
    scanPosition++;
    if (scanPosition == NUM_LEDS) {
      scanPosition = 0;  // reset scan back to start
    }
  }
  FastLED.show();
    displayPatternChase(convKeyP,  "Scan & Wipe");
  }
}     //   Last Bracket for LOOP !!

//    ***********************************   END KEY FUNCTIONS   ****************************

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
//    *********************************   set Speed to 100%  Once  *********************************
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

//    *********************************   Code for Key fuctions   *********************************

//    *********************             Key 46  Juggle               ********************* 

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void juggle2() {
  fadeToBlackBy(leds, NUM_LEDS, 20);
  byte dothue = 0;
  uint32_t const ms = millis();
  for (byte i = 0; i < 8; i++) {
    // at this moment in time, where along the cosine wave is this dot?
    uint16_t cospos = ms * (i + 7);
    //  if it's in the 2nd half of the wave, skip back to the 1st half 
    //  if (cospos >= 32768)
    //  cospos -= 32768;
    //  to reverse direction use this instead:
         if (cospos < 32768)
         cospos += 32768;
    // get the cosine and shift it to be always positive
    uint16_t cosine = cos16(cospos) + 32767;
    // scale down from 0-65535 to 0-(NUM_LEDS - 1)
    uint16_t ledpos = (uint32_t)cosine * NUM_LEDS / 65536;
    leds[ledpos] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
void juggle_pal() {                                           // Several colored dots, weaving in and out of sync with each other
  
  currentPalette  = CRGBPalette16(CRGB::Black);
  targetPalette   = RainbowColors_p;
  currentBlending = LINEARBLEND;  
  
  EVERY_N_MILLISECONDS(100) 
    {                                 // AWESOME palette blending capability provided as required.
    uint8_t maxChanges = 24; 
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);   
    }
  curhue = thishue;                                           // Reset the hue values.

  fadeToBlackBy(leds, NUM_LEDS, thisfade);
  
  for( int i = 0; i < numdots; i++) {
    leds[beatsin16(thisbeat+i+numdots,0,NUM_LEDS)] += ColorFromPalette(currentPalette, curhue , thisbright, currentBlending);    // Munge the values and pick a colour from the palette
    curhue += thisdiff;
  }
  
    FastLED.show();   
}                         // juggle_pal()
void ChangeMe46() {                                             // A time (rather than loop) based demo sequencer. This gives us full control over the length of each sequence.
  
  uint8_t secondHand = (millis() / 1000) % 30;                // IMPORTANT!!! Change '30' to a different value to change duration of the loop.
  static uint8_t lastSecond = 99;                             // Static variable, means it's only defined once. This is our 'debounce' variable.
  
  if (lastSecond != secondHand) {                             // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch(secondHand) {
      case  0: numdots = 1; thisbeat = 20; thisdiff = 16; thisfade = 2; thishue = 0; break;                  // You can change values here, one at a time , or altogether.
      case 10: numdots = 4; thisbeat = 10; thisdiff = 16; thisfade = 8; thishue = 128; break;
      case 20: numdots = 8; thisbeat =  3; thisdiff =  0; thisfade = 8; thishue=random8(); break;           // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 30: break;
          FastLED.show();   
    }
  }
  
} // ChangeMe46()

//    *********************      END    Key 46  Juggle               ********************* 

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

//    *********************   Key 90  Confetti (New Year's Day)   ********************* 
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
//    *********************  END Key 90  Confetti (New Year's Day)  ********************* 

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

//    *********************   Key 80  Glitter  ********************* 

void addMatchingGlitter( fract8 chanceOfGlitter) 
// If you know the current hue, it's super easy to simply
// do something like this:
//            uint8_t hue;
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CHSV(hue80,255,255);
  }
}

void addMatchingGlitter_2( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ].maximizeBrightness(255);
  }
}

void addMatchingGlitter_3( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    uint16_t i = random16(NUM_LEDS);
    CRGB color = leds[i];
    leds[i] += color.nscale8_video(255);
    // Can apply a second or third time for even brighter effect
    leds[i] += color.nscale8_video(255);
    leds[i] += color.nscale8_video(255);
  }
}

void addMatchingGlitter_4( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    uint16_t i = random16(NUM_LEDS);
    leds[i].r = brighten8_video(leds[i].r);
    leds[i].g = brighten8_video(leds[i].g);
    leds[i].b = brighten8_video(leds[i].b);
    // Can apply a second time for even brigter effect
    leds[i].r = brighten8_video(leds[i].r);
    leds[i].g = brighten8_video(leds[i].g);
    leds[i].b = brighten8_video(leds[i].b);
  }
}

//    *********************   END  Key 80  Glitter   ********************* 

//    *********************        Key 36  Meteor         ********************* 


//    *********************        END    Key 36  Meteor         ********************* 

//    *********************               Key 91  Candy Cane         ********************* 
void candyCane(){
  CRGB color1 = CRGB::White;  // color used between color 2 (and 3 if used)
  CRGB color2 = CRGB::Red;
  CRGB color3 = CHSV(0,170,255);  //optional 3rd color Pink
  //CRGB color3 = CRGB(0,255,0);  //optional 3rd color Green
  const uint16_t travelSpeed = 5;
  int shiftBy = -1;  //shiftBy can be positive or negative (to change direction)
  static uint8_t numColors = 2;  // Can be either 2 or 3
  static uint8_t stripeLength = 5;  //number of pixels per color
  static int offset;

  EVERY_N_SECONDS(5) {
    numColors = random8(2,4);  //picks either 2 or 3
    stripeLength = random8(3,6);  //picks random length
  }

  EVERY_N_MILLISECONDS(travelSpeed) {
    if (numColors==2) {
      for (uint8_t i=0; i<NUM_LEDS; i++){
        if ( (i + offset)%((numColors)*stripeLength)<stripeLength ) {
          leds[i] = color2;
        } else {
          leds[i] = color1;
        }
      }
    }

    if (numColors==3) {
      for (uint8_t i=0; i<NUM_LEDS; i++){
        if ( (i + offset)%((numColors+1)*stripeLength)<stripeLength ) {
          leds[i] = color2;
        }
        else if ( (i + offset+(2*stripeLength))%((numColors+1)*stripeLength)<stripeLength ) {
          leds[i] = color3;
        } else {
          leds[i] = color1;
        }
      }
    }

    FastLED.show();

    offset = offset + shiftBy;
    if (shiftBy>0) {  //for positive shiftBy
      if (offset>=NUM_LEDS) offset = 0;
    } else {  //for negitive shiftBy
      if (offset<0) offset = NUM_LEDS;
    }

  }//end EVERY_N
}
//    *********************        END    Key 91  Candy Cane         ********************* 

//    *********************               Key 40  Peppermint Candy         ********************* 
void candyCane2(){
  CRGB color1 = CRGB::White;  // color used between color 2 (and 3 if used)
  CRGB color2 = CRGB::Red;
  //  CRGB color3 = CHSV(0,170,255);  //optional 3rd color Pink
  CRGB color3 = CRGB(0,255,0);  //optional 3rd color Green
  const uint16_t travelSpeed40 = 10;   //  Start at 150
  int shiftBy40 = -1;  //shiftBy can be positive or negative (to change direction)
  static uint8_t numColors40 = 3;  // Can be either 2 or 3
  static uint8_t stripeLength40 = 5;  //number of pixels per color
  static int offset40;

  EVERY_N_SECONDS(5) {
    numColors40 = random8(2,4);  //picks either 2 or 3
    stripeLength40 = random8(3,6);  //picks random length
  }

  EVERY_N_MILLISECONDS(travelSpeed40) {
    if (numColors40==2) {
      for (uint8_t i=0; i<NUM_LEDS; i++){
        if ( (i + offset40)%((numColors40)*stripeLength40)<stripeLength40 ) {
          leds[i] = color2;
        } else {
          leds[i] = color1;
        }
      }
    }

    if (numColors40==3) {
      for (uint8_t i=0; i<NUM_LEDS; i++){
        if ( (i + offset40)%((numColors40+1)*stripeLength40)<stripeLength40 ) {
          leds[i] = color2;
        }
        else if ( (i + offset40+(2*stripeLength40))%((numColors40+1)*stripeLength40)<stripeLength40 ) {
          leds[i] = color3;
        } else {
          leds[i] = color1;
        }
      }
    }

    FastLED.show();

    offset40 = offset40 + shiftBy40;
    if (shiftBy40>0) {  //for positive shiftBy
      if (offset40>=NUM_LEDS) offset40 = 0;
    } else {  //for negitive shiftBy
      if (offset40<0) offset40 = NUM_LEDS;
    }

  }//end EVERY_N
}
//    *********************        END    Key 40      Peppermint Candy     ********************* 

//    *********************               Key 92  EKG         ********************* 
void heartBeat(){
  for (int i = 0; i < NUM_LEDS ; i++) {
    uint8_t bloodVal = sumPulse( (5/NUM_LEDS/2) + (NUM_LEDS/2) * i * flowDirection );
    leds[i] = CHSV( bloodHue, bloodSat, bloodVal );
  }
}

int sumPulse(int time_shift) {
  //time_shift = 0;  //Uncomment to heart beat/pulse all LEDs together
  int pulse1 = pulseWave8( millis() + time_shift, cycleLength, pulseLength );
  int pulse2 = pulseWave8( millis() + time_shift + pulseOffset, cycleLength, pulseLength );
  return qadd8( pulse1, pulse2 );  // Add pulses together without overflow
}

uint8_t pulseWave8(uint32_t ms, uint16_t cycleLength, uint16_t pulseLength) {
  uint16_t T = ms % cycleLength;
  if ( T > pulseLength) return baseBrightness;
  uint16_t halfPulse = pulseLength / 2;
  if (T <= halfPulse ) {
    return (T * 255) / halfPulse;  //first half = going up
  } else {
    return((pulseLength - T) * 255) / halfPulse;  //second half = going down
  }
}
//    *********************        END    Key 92  EKG         ********************* 

//    *********************               Key 93  Sparkles         ********************* 
void sparkles(uint8_t sparkel_duration, uint8_t sparkel_amount, uint8_t sparkel_spread) {
  static uint8_t sparkle_pixel;
  EVERY_N_MILLISECONDS_I( timingObj, 1) {
    timingObj.setPeriod(sparkel_duration);
    leds[sparkle_pixel] = CRGB::Black;
    uint8_t previous_pixel = sparkle_pixel;
    while (previous_pixel == sparkle_pixel) {  // pixel can't repeat
      sparkle_pixel = random8(NUM_LEDS);
    }
    if (random8(100) < sparkel_amount) {
      //leds[sparkle_pixel] = CRGB(random8(), random8(), random8());
      leds[sparkle_pixel] = CHSV(random8(), random8(20,200), random8(50,255));
    }
  }
  EVERY_N_MILLISECONDS(10) {
    //fadeToBlackBy(leds, NUM_LEDS, 1);  // fade out a bit over time
    blur1d(leds, NUM_LEDS, sparkel_spread);  // spreads and fades out color over time
  }
}
//    *********************        END    Key 93  Sparkles         ********************* 

//    *********************               Key 94  Cylon         ********************* 
void colorChange(boolean r) {
  // Change the color.
  // If r = 0, the next color in the array is picked and then
  // it cycles back around to the first color in the array.
  // If r = 1 then a random color is picked from the array.

  if (r == 0) {
    colorChoice = colorChoice + 1;
    if (colorChoice >= numberOfColors) { colorChoice = 0; }
    //Serial.print("cycling: "); Serial.println(colorChoice);
  } else {
    colorChoice = random8(numberOfColors);
    //Serial.print("random: "); Serial.println(colorChoice);
  }
}
//---------------------------------------------------------------
void cylon_cc1() {
  //cylon effect with color change based on time
  
  static uint8_t delta = 1;  //direction of travel

  EVERY_N_MILLISECONDS(450) {
    colorChange(0);  //set next color
  }
  EVERY_N_MILLISECONDS(20) {          // Start = 60
    fadeToBlackBy(leds, NUM_LEDS, 15);
    leds[pos] = colorArray94[colorChoice];
    pos = pos + delta;
    if (pos == NUM_LEDS-1) { delta = -1; }  //change direction
    if (pos == 0) { delta = 1; }  //change direction
  }
}       //end_cylon_cc1
//---------------------------------------------------------------
void cylon_cc2() {
  //cylon effect with color change when end of strip is reached
  static uint8_t delta = 1;  //direction of travel

  EVERY_N_MILLISECONDS(10) {        // Start = 60
    fadeToBlackBy(leds, NUM_LEDS, 15);
    leds[pos] = colorArray94[colorChoice];
    pos = pos + delta;
    if (pos == NUM_LEDS-1) {
      delta = -1;  //change direction
      colorChange(1);  //random next color
    }
    if (pos == 0) {
      delta = 1;  //change direction
      colorChange(1);  //random next color
    }
  }
}  

   //end_cylon_cc2
//    *********************        END    Key 94  Cylon         ********************* 

//    *********************               Key 95  Mirror Fire2012         ********************* 
void Fire2012()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS/2];
  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS/2; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS/2) + 2));
    }
      // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS/2 - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }
    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS/2; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS/2-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
}
//---------------------------------------------------------------
void mirror2ndHalf() {
  // copy in reverse order first half of strip to second half
  for (uint8_t i = 0; i < NUM_LEDS/2; i++) {
    leds[NUM_LEDS-1-i] = leds[i];  
  }
}
//    *********************        END    Key 95  Mirror Fire2012         ********************* 

//    *********************               Key 96  Pacifica        ********************* 
void pacifica_loop()
{
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));
  // Clear out the LED array to a dim background blue-green
  fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));
  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer( pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));
  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps();
  // Deepen the blues and greens a bit
  pacifica_deepen_colors();
}
// Add one layer of waves into the led array
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}
// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps()
{
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}
// Deepen the blues and greens
void pacifica_deepen_colors()
{
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
  }
}
//    *********************        END    Key 96  Pacifica        ********************* 

//    *********************               Key 97  LightHouse        ********************* 
void drawFractionalBar( int pos16, int width97, uint8_t hue97)
{
  int i = pos16 / 16; // convert from pos to raw pixel number
  uint8_t frac = pos16 & 0x0F; // extract the 'factional' part of the position
  uint8_t firstpixelbrightness = 255 - (frac * 16);
  uint8_t lastpixelbrightness  = 255 - firstpixelbrightness;
  uint8_t bright97;
  for( uint8_t n = 0; n <= width97; n++) {
    if( n == 0) {
      bright97 = firstpixelbrightness;  // first pixel in the bar
      leds[i] += CHSV( hue97, sat97, bright97);
    }
    else if (n == width97) {
      bright97 = lastpixelbrightness;  // last pixel in the bar
      leds[i] += CHSV( hue97, sat97, bright97);
      fadeToBlackBy( leds, NUM_LEDS, fadeRate97 );  // creates outward fade
    }
    else {
      bright97 = 255;  // center pixel always max bright
      leds[i] += CHSV( hue97, sat97, bright97);
    }
    i++;
    if( i == NUM_LEDS) i = 0; // wrap around
  }
}
//    *********************        END    Key 97  LightHouse        ********************* 

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
//  *****          Palette   16   *****
const TProgmemRGBPalette16 FourTwenty_p PROGMEM =
{
  CRGB::Green,
  CRGB::Green,
  CRGB::Yellow,
  CRGB::Yellow,

  CRGB::Red,
  CRGB::Red,
  CRGB::Green,
  CRGB::Yellow,

  CRGB::Red,
  CRGB::Black,
  CRGB::Green,
  CRGB::Green,

  CRGB::Yellow,
  CRGB::Yellow,
  CRGB::Red,
  CRGB::Red,
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

//    *********************     END ALL      *********************  