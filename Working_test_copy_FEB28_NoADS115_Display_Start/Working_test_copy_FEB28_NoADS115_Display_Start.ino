//
//  24 keys with presets and chases Tasks are handled with TaskManager software #include <TaskManagerIO.h> 
//  https://www.thecoderscorner.com/products/arduino-libraries/taskmanager-io/task-manager-scheduling-guide/
//
//  Keys are responsive with little latency Events are working smoothly presets
//  and chases are working but will be improved overtime (My OCD)
//
//  Next step is to incorporate four faders 4 controlling RGB and chase speed.
//
//  initialize
#include <Wire.h>
#include <IoAbstraction.h>
#include <IoAbstractionWire.h>
#include<TaskManagerIO.h>
#include <KeyboardManager2023.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd1(0x27, 16, 2); // << Address 1
LiquidCrystal_I2C lcd2(0x26, 16, 2); // << Address 2

//#include <FastLEDPainter.h>

#include "FastLED.h"
#include "colorutils.h"


#define DATA_PIN 2
#define DATA_PIN 3
#define LED_TYPE WS2813
#define COLOR_ORDER GRB
#define NUM_LEDS 62

#define UPDATES_PER_SECOND  100
#define FRAMES_PER_SECOND  120

//   *********   Set  Brightness & Speed    **********

#define MASTER_BRIGHTNESS 255  // Set the master brigtness value [should be greater then min_brightness value].
#define MASTER_Speed 500       //   Adjusted for +0 Offset

#define MASTER_RED 255
#define MASTER_GREEN 255
#define MASTER_BLUE 255
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
//   *********   End  Brightness & Speed    **********

//   *********         KeyPad           **********
//
// We need to make a keyboard layout that the manager can use. choose one of the below.
// The parameter in brackets is the variable name.
//
//MAKE_KEYBOARD_LAYOUT_3X4(keyLayout)
//MAKE_KEYBOARD_LAYOUT_4X4(keyLayout)
MAKE_KEYBOARD_LAYOUT_3X8(keyLayout)
//
// We need a keyboard manager class too
//
    MatrixKeyboardManager keyboard;

// this examples connects the pins directly to an arduino but you could use
// IoExpanders or shift registers instead.

 MCP23017IoAbstraction io23017(0x20, ACTIVE_LOW_OPEN, 10);



int KeyFG = 0;





//   *********   Start define Pallets   *********
CRGB leds[NUM_LEDS];


CRGBPalette16 currentPalette;
CRGBPalette32 currentPalette32;
TBlendType    currentBlending;

extern CRGBPalette16 myRedBluePalette;
extern const TProgmemPalette16 myRedBluePalette_p PROGMEM;
extern CRGBPalette16 RWBPalette;
extern const TProgmemPalette16 RWB_p PROGMEM;
extern CRGBPalette16 Irish;
extern const TProgmemPalette16 Irish_p PROGMEM;

extern CRGBPalette32 Ocean;
extern const TProgmemPalette32 Ocean_p PROGMEM;
extern CRGBPalette32 FireFG;
extern const TProgmemPalette32 FireFG_p PROGMEM;


//  Functions for LEDS  Fastlight and Neopixel

void FillLEDsFromPaletteColors( uint8_t colorIndex) //  Load 16 Color palette
{
//     uint8_t brightness = 255;
//        Loads color palette    
      for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, MASTER_BRIGHTNESS, currentBlending);
        colorIndex += 3;
  }
}
void FillLEDsFromPaletteColors32( uint8_t colorIndex)  //  Load 32 Color palette
{
//     uint8_t brightness = 255;
//        Loads color palette(?)    
      for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette32, colorIndex, MASTER_BRIGHTNESS, currentBlending);
        colorIndex += 1;
  }
}

// ********* Start key press funcions   *********

void RedFunctionFor1() {
 // take action when 1 typed on keypad
  KeyFG = 1;
    Serial.println(KeyFG);
    Serial.print("   ");
}   
void OrangeFunctionFor2() {
 // take action when 2 typed on keypad
    KeyFG = 2;
      Serial.println(KeyFG);
      Serial.print("   ");
}
void YellowFunctionFor3() {
 // take action when 3 typed on keypad
    KeyFG = 3;
      Serial.println(KeyFG);
      Serial.print("   ");
}
void GreenFunctionFor4() {
 // take action when 4 typed on keypad
    KeyFG = 4;
      Serial.println(KeyFG);
      Serial.print("   "); 
}
void CyanFunctionFor5() {
 // take action when 5 typed on keypad
    KeyFG = 5;
      Serial.println(KeyFG);
      Serial.print("   ");  
}
void BlueFunctionFor6() {
 // take action when 6 typed on keypad
    KeyFG = 6;
      Serial.println(KeyFG);
      Serial.print("   ");      
}
void PurpleFunctionFor7() {
 // take action when 7 typed on keypad
    KeyFG = 7;
      Serial.println(KeyFG);
      Serial.print("   "); 
}
void MagentaFunctionFor8() {
 // take action when 8 typed on keypad
    KeyFG = 8;
      Serial.println(KeyFG);
      Serial.print("   ");  
}
void WhiteFunctionFor9() {
 // take action when 9 typed on keypad
    KeyFG = 9;
      Serial.println(KeyFG);
      Serial.print("   ");    
}
void Pre1FunctionForX() {
 // take action when X typed on keypad
    KeyFG = 10;
      Serial.println(KeyFG);
      Serial.print("   "); 
}
void Pre2FunctionForY() {
 // take action when 0 typed on keypad
    KeyFG = 11;
      Serial.println(KeyFG);
      Serial.print("   ");    
}
void Pre3FunctionForZ() {
 // take action when Y typed on keypad
    KeyFG = 12;
      Serial.println(KeyFG);
      Serial.print("   ");    
}

// *******************   KeyPad  #2  *********************************
//Chases

void  ChaseForA() {
 // take action when A typed on keypad
    KeyFG = 13;
      Serial.println(KeyFG);
      Serial.print("   ");   
}
void ChaseForB() {
 // take action when B typed on keypad
  KeyFG = 14;
     Serial.println(KeyFG);
     Serial.print("   ");    
}
void ChaseForC() {
 // take action when  C typed on keypad
  KeyFG = 15;
     Serial.println(KeyFG);
     Serial.print("   "); 
}
void ChaseForD() {
 // take action when  D typed on keypad
  KeyFG = 16;
     Serial.println(KeyFG);
     Serial.print("   ");       
}
void  ChaseForE() {
 // take action when E typed on keypad
    KeyFG = 17;
     Serial.println(KeyFG);
     Serial.print("   ");       
}
void ChaseForF() {
 // take action when F typed on keypad
  KeyFG = 18;
     Serial.println(KeyFG);
     Serial.print("   ");     
}
void ChaseForG() {
 // take action when  G typed on keypad
  KeyFG = 19;
     Serial.println(KeyFG);
     Serial.print("   ");      
}
void ChaseForH() {
 // take action when  H typed on keypad
  KeyFG = 20;
     Serial.println(KeyFG);
     Serial.print("   ");      
}
void  ChaseForI() {
 // take action when I typed on keypad
  KeyFG = 21;
     Serial.println(KeyFG);
     Serial.print("   ");      
}
void ChaseForJ() {
 // take action when J typed on keypad
  KeyFG = 22;
     Serial.println(KeyFG);
     Serial.print("   ");      
}
void ChaseForK() {
 // take action when  K typed on keypad
  KeyFG = 23;
     Serial.println(KeyFG);
     Serial.print("   ");         
}
void ChaseForL() {
 // take action when  K typed on keypad
  KeyFG = 24;
     Serial.println(KeyFG);
     Serial.print("   ");       
}

//  End Key Functions
//
// We need a class that extends from KeyboardListener. this gets notified when
// there are changes in the keyboard state.
//
class MyKeyboardListener : public KeyboardListener {
public:
  void keyPressed(char key, bool held) override {
//  Testing key presses       
      Serial.print("Key ");
      Serial.print(key);
      Serial.print(" is pressed, held = ");
      Serial.println(held);

//Start switch case for key press
//
  switch(key) {
      case '1':
          RedFunctionFor1();
      Serial.println("Red ");      
      Serial.println(KeyFG);
          break;

      case '2':
          OrangeFunctionFor2();
      Serial.println("Orange ");
      Serial.println(KeyFG);
           break;

      case '3':
           YellowFunctionFor3();
      Serial.println("Yellow ");
      Serial.println(KeyFG);    
           break;       

      case '4':
           GreenFunctionFor4();
      Serial.println("Green ");
      Serial.println(KeyFG);     
           break;
      case '5':
           CyanFunctionFor5();
      Serial.println("Cyan ");
      Serial.println(KeyFG);    
           break;
      case '6':
           BlueFunctionFor6();
      Serial.println("Blue ");
      Serial.println(KeyFG);     
           break;
      case '7':
           PurpleFunctionFor7();
      Serial.println("Purple ");
      Serial.println(KeyFG);     
           break;
      case '8':
           MagentaFunctionFor8();
      Serial.println("Magenta ");
      Serial.println(KeyFG);     
           break;
      case '9':
           WhiteFunctionFor9();
      Serial.println("Set RGB ");
      Serial.println(KeyFG); 

           break;
      case 'X':
           Pre1FunctionForX();
      Serial.println("Red White Blue with Sparkles ");
      Serial.println(KeyFG);     
           break;
      case 'Y':
           Pre2FunctionForY();
      Serial.println("Fire ");
      Serial.println(KeyFG);     
           break;
      case 'Z':
           Pre3FunctionForZ();
      Serial.println("Irish ");
      Serial.println(KeyFG);     
           break;

// *******************   KeyPad  #2  *********************************

      case 'A':
           ChaseForA();
      Serial.println("Rainbow ");
      Serial.println(KeyFG);      
           break;             
      case 'B':
           ChaseForB();
      Serial.println("Police ");
      Serial.println(KeyFG);     
           break;             

      case 'C':
           ChaseForC();
      Serial.println("Forest ");
      Serial.println(KeyFG);    
           break; 

      case 'D':
           ChaseForD();
      Serial.println("Patriot ");
      Serial.println(KeyFG);    
           break; 

      case 'E':
           ChaseForE();
      Serial.println("Gliter Rainbow ");
      Serial.println(KeyFG);    
           break;

       case 'F':
           ChaseForF();
      Serial.println("Popcorn");
      Serial.println(KeyFG);      
           break;             

      case 'G':
           ChaseForG();
      Serial.println("Back and Forth ");
      Serial.println(KeyFG);     
           break;             

      case 'H':
           ChaseForH();
      Serial.println("Tape Ticker");
      Serial.println(KeyFG);    
           break; 

      case 'I':
           ChaseForI();
      Serial.println("Right");
      Serial.println(KeyFG);    
           break; 

      case 'J':
           ChaseForJ();
      Serial.println("Left ");
      Serial.println(KeyFG);     
           break;             

      case 'K':
           ChaseForK();
      Serial.println("Clouds ");
      Serial.println(KeyFG);    
           break; 

      case 'L':
           ChaseForL();
      Serial.println("Ocean ");
      Serial.println(KeyFG);    
           break;      
      }

  }

void keyReleased(char key) override {   } 
} myListener;


// Fastled show loop
void LoopFLed(){
//   Preset Fills   
  if(  KeyFG == 1)  { fill_solid( leds, NUM_LEDS, CRGB::Red);           goto RunSHOW; }
  if(  KeyFG == 2)  { fill_solid( leds, NUM_LEDS, CRGB::OrangeRed);     goto RunSHOW; }    
  if(  KeyFG == 3)  { fill_solid( leds, NUM_LEDS, CRGB(0xFFFF10));      goto RunSHOW; } 
  if(  KeyFG == 4)  { fill_solid( leds, NUM_LEDS, CRGB::Green);         goto RunSHOW; }  
  if(  KeyFG == 5)  { fill_solid( leds, NUM_LEDS, CRGB::Cyan);          goto RunSHOW; } 
  if(  KeyFG == 6)  { fill_solid( leds, NUM_LEDS, CRGB::Blue);          goto RunSHOW; } 
  if(  KeyFG == 7)  { fill_solid( leds, NUM_LEDS, CRGB::Purple);        goto RunSHOW; } 
  if(  KeyFG == 8)  { fill_solid( leds, NUM_LEDS, CRGB(0xFF1040));      goto RunSHOW; } 


  if(  KeyFG == 9)
 {   
    uint8_t min_red = 1;   // Set a minimum red level.
    uint8_t RedVal;            // Mapped Red
    int potPinR = A1;   // Pin A1 for potentiometer RED 
    int potValR;        // Variable to store potentiometer Red value 

      pinMode(potPinR, INPUT);  // Set pin as an input. 
        potValR = analogRead(potPinR);  // Read potentiometer R (for Red).
        RedVal = map(potValR, 1, 1023, min_red, MASTER_RED);

    uint8_t min_green = 1;   // Set a minimum green level.
    uint8_t GreenVal;            // Mapped Green
    int potPinG = A2;   // Pin A2 for potentiometer GREEN 
    int potValG;        // Variable to store potentiometer Green value 

      pinMode(potPinG, INPUT);  // Set pin as an input. 
        potValG = analogRead(potPinG);  // Read potentiometer G (for Green).
        GreenVal = map(potValG, 1, 1023, min_green, MASTER_GREEN);

    uint8_t min_blue = 1;   // Set a minimum brightness level.
    uint8_t BlueVal;            // Mapped Blue
    int potPinB = A3;   // Pin A3 for potentiometer BLUE 
    int potValB;        // Variable to store potentiometer Red value 

      pinMode(potPinB, INPUT);  // Set pin as an input. 
        potValB = analogRead(potPinB);  // Read potentiometer B (for brightness).
        BlueVal = map(potValB, 1, 1023, min_blue, MASTER_BLUE);


        for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(RedVal, GreenVal, BlueVal);  // Out to FastLED
  }   
       Serial.print("    R ");
       Serial.print(RedVal); 
       Serial.print("    G ");
       Serial.print(GreenVal); 
        Serial.print("    B "); 
        Serial.print(BlueVal); 


 goto RunSHOW;  

} 

//Presets
  if(  KeyFG == 10)  
{  
currentPalette = RWB_p; currentBlending = NOBLEND;
  static uint8_t startIndex = 0;
//   startIndex = startIndex + 1; /* motion speed */  //Remove Chase
  FillLEDsFromPaletteColors( startIndex);
     { if( random8() < 120)    //Good Amount of sparkles       
  leds[ random16(NUM_LEDS) ] += CRGB::White;} 
     { if( random8() < 120)
  leds[ random16(NUM_LEDS) ] += CRGB::White;}  
  goto RunSHOW;    

}    
  if(  KeyFG == 11)  
 { currentPalette32 = FireFG_p; currentBlending = LINEARBLEND;
  static uint16_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors32( startIndex);     

     { if( random8() < 120)    //Good Amount of sparkles       
  leds[ random16(NUM_LEDS) ] += CRGB::White;}
  goto RunSHOW;     

}
  if(  KeyFG == 12)   
{  
currentPalette = Irish_p; currentBlending = NOBLEND;
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */ 
  FillLEDsFromPaletteColors( startIndex);
  goto RunSHOW;
}  

// *******************   KeyPad  #2  *********************************

//  Chase Functions
 if(  KeyFG == 13)  //A
 { currentPalette = RainbowColors_p; currentBlending = LINEARBLEND;
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);  
  goto RunSHOW;   
}
 if(  KeyFG == 14)  //B
 { currentPalette = myRedBluePalette_p; currentBlending = NOBLEND;    //Police
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);     
  goto RunSHOW;
 } 
   if(  KeyFG == 15)  //C
 { currentPalette = ForestColors_p; currentBlending = LINEARBLEND;
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);
  goto RunSHOW;     
 }   
  if(  KeyFG == 16)  //D
 { currentPalette = RWB_p; currentBlending = NOBLEND;
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);
  goto RunSHOW;     
 }  
//   Different Functions   
 if(  KeyFG == 17)  //E  Gilter Rainbow
{ static uint8_t gHue = 0;
   gHue = gHue + 2; /* motion speed */
 // Call the current pattern function once, updating the 'leds' array
 fill_rainbow( leds, NUM_LEDS, gHue, 4); //4 is a good Size(LEDs+=0)
     { if( random8() < 120)    //Good Amount of sparkles       
  leds[ random16(NUM_LEDS) ] += CRGB::White;} 
     { if( random8() < 120)
  leds[ random16(NUM_LEDS) ] += CRGB::White;}
  goto RunSHOW;
 // send the 'leds' array out to the actual LED strip
 }
 if(  KeyFG == 18)  //F
// random colored speckles that blink in and fade smoothly  Popcorn 
 { static uint8_t gHue = 0;
   gHue = gHue + 1; /* motion speed */
 // Call the current pattern function once, updating the 'leds' array
  fadeToBlackBy( leds, NUM_LEDS, 10);  //(good fade #)
     int pos = random8(NUM_LEDS);
     leds[pos] += CHSV( gHue + random8(255), 255, 255); 
     int pos2 = random8(NUM_LEDS);
     leds[pos2] += CHSV( gHue + random8(255), 255, 255); // Good 2*Times the dots
     goto RunSHOW;
 } 
  if(  KeyFG == 19)  //G  
// a colored dot sweeping back and forth, with fading trails
 { static uint8_t gHue = 0;
   gHue = gHue + 1; /* motion speed */
 // Call the current pattern function once, updating the 'leds' array
    fadeToBlackBy( leds, NUM_LEDS, 20);
    int pos = beatsin16( 6, 0, NUM_LEDS-1 ); // 6 is Good
    leds[pos] += CHSV( gHue, random8(255), 255); 
    goto RunSHOW;  
 } 

  if(  KeyFG == 20) //H
//  Spew  Tape Ticker   Good Tweaked all parameters!!
{   static uint8_t gHue = 0;
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
}//end every_n
    goto RunSHOW;
//  End Tape Ticker    
} 

  if(  KeyFG == 21)  //I   RIGHT
 { currentPalette =  RainbowStripeColors_p; currentBlending = NOBLEND;
  static uint8_t startIndex = 0;
  startIndex = startIndex - 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);  
  goto RunSHOW;   
 }   
 if(  KeyFG == 22)  //J   LEFT
 { currentPalette =  RainbowStripeColors_p; currentBlending = NOBLEND;
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);   
  goto RunSHOW;  
 }      
 if(  KeyFG == 23)  //K
 { currentPalette = CloudColors_p; currentBlending = LINEARBLEND;
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);  
  goto RunSHOW;   
 }      
 if(  KeyFG == 24)  //L
 { currentPalette32 = Ocean_p; currentBlending = LINEARBLEND;
  static uint16_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors32( startIndex); 
  goto RunSHOW;    

//   **************   Test FastLED.show();   **************


RunSHOW:
//  **********  Brightness Control      **********
    uint16_t min_brightness = 1;   // Set a minimum brightness level.
    uint16_t brightness;            // Mapped master brightness based on potentiometer reading.
    int potPinA = A0;   // Pin for potentiometer B (for brightness) 
    int potValA;        // Variable to store potentiometer B value (for brightness)

      pinMode(potPinA, INPUT);  // Set pin as an input. 
        potValA = analogRead(potPinA);  // Read potentiometer B (for brightness).
        brightness = map(potValA, 1, 1023, min_brightness, MASTER_BRIGHTNESS);
      FastLED.setBrightness(brightness);  // Set master brightness based on potentiometer position.



  lcd1.setCursor(0, 0);
  lcd1.print(brightness); 
  lcd1.setCursor(0, 1);
  lcd1.print("Dimmer");
//     Serial.print("  Bright ");  
//     Serial.print(brightness); 
//     Serial.println("    "); 

// *********    END Brightness Control ***********   

// *********            SPEED          ***********

    uint16_t min_RUNSpeed = 1;   // Set a minimum speed level.
    uint16_t RUNSpeed = 20;  // rate of advance
    uint16_t SpeedRate;
    int potPinS = A4;   // Pin for potentiometer S (Speed)
    int potValS;        // Variable to store potentiometer  value (Speed)
        pinMode(potPinS, INPUT);  // Set pin as an input.
        potValS = analogRead(potPinS);  // Read potentiometer S (for Speed).
       SpeedRate = map(potValS, 1, 1023, min_RUNSpeed, MASTER_Speed);
           SpeedRate = (SpeedRate + 1);  // Set minSpeed Advance

           if (0 <= SpeedRate && SpeedRate < 70)
          { 
            SpeedRate = ( SpeedRate / 5);
          }
           if (71 <= SpeedRate && SpeedRate < 150)
          { 
            SpeedRate = ( SpeedRate / 4);
          }
           if (151 <= SpeedRate && SpeedRate < 250)
          { 
            SpeedRate = ( SpeedRate / 3   );
          }           
           if (251 <= SpeedRate && SpeedRate < 350)
          { 
            SpeedRate = ( SpeedRate / 2  );
          }                    
           else 
             {  
              SpeedRate = SpeedRate;
             }

  lcd2.setCursor(0, 0);
  lcd2.print("Speed");
  lcd2.setCursor(0, 1);
  lcd2.print(SpeedRate);     
          
     FastLED.delay(SpeedRate);
//    FastLED.show();   Goto LoopFLed2  better responce
}  


}
//   End LoopFLed  !!

//  *****  Isolate  FastLED.show()  Function  ********
void LoopFLed2(){

// *********            SPEED          ***********

    FastLED.show(); 
}
//  *****  Isolate  FastLED.show()  Function  ********


//  *****          SetUP    ********
void setup() {
  while(!Serial);
  Serial.begin(9600);

  startTaskManagerLogDelegate();

 // analogReference(EXTERNAL); // use AREF for reference voltage

  keyLayout.setRowPin(0, 29);
  keyLayout.setRowPin(1, 28);
  keyLayout.setRowPin(2, 27);
  keyLayout.setRowPin(3, 26);
  keyLayout.setRowPin(4, 25);
  keyLayout.setRowPin(5, 24);
  keyLayout.setRowPin(6, 23);
  keyLayout.setRowPin(7, 22);

  keyLayout.setColPin(0, 33);
  keyLayout.setColPin(1, 32);
  keyLayout.setColPin(2, 31);

  // create the keyboard mapped to arduino pins and with the layout chosen above.
  // it will callback our listener
  keyboard.initialise(asIoRef(internalDigitalDevice()), &keyLayout, &myListener);

  // start repeating at 850 millis then repeat every 350ms
  keyboard.setRepeatKeyMillis(850, 350);

  Serial.println("Keyboard is initialised!");


//          Display
  lcd1.init();
  lcd2.init();

  lcd1.backlight();
  lcd2.backlight();
//   Start UP FastLed  Set to Black
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( 60 );
  fill_solid( leds, NUM_LEDS, CRGB::Black); FastLED.show(); 


uint8_t taskId = taskManager.scheduleFixedRate(15, LoopFLed);;
uint8_t taskId2 = taskManager.scheduleFixedRate(15, LoopFLed2);;
// uint8_t taskId = taskManager.scheduleFixedRate(SpeedRate, LoopFLed);;
// uint8_t taskId2 = taskManager.scheduleFixedRate(SpeedRate, LoopFLed2);;
//  ****   Keep "SpeedRate" equal in both commands Range 20 to 1023   ****** 
}

//  *****     END  SetUP    ********

void loop() {
  // as this indirectly uses taskmanager, we must include this in loop.
  taskManager.runLoop();

}


//  *****          Extrenal Paletts    ********
//  *****          Palette   16   *****
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
  CRGB::OrangeRed,
  CRGB::Green,

  CRGB::Green,
  CRGB::Green,
  CRGB::OrangeRed,
  CRGB::Green,

  CRGB::Green,
  CRGB::Green,
  CRGB::OrangeRed,
  CRGB::Green,

  CRGB::Green,
  CRGB::Green,
  CRGB::OrangeRed,
  CRGB::Green,
};

//  *****          Palette   32   *****
const TProgmemRGBPalette32 FireFG_p FL_PROGMEM =
{
 0x990066, //   XXXXX
 CRGB::Maroon,
 CRGB::Crimson,
 CRGB::OrangeRed,

 CRGB::DarkRed,
 0xDF2800, //  Ferrari
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
 0xDF2800, //  Ferrari
 CRGB::Black,
 CRGB::Red,

 CRGB::DarkRed,
 CRGB::Gold,
 0xE0115F, //Ruby
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
//  *****  END ALL   *****
