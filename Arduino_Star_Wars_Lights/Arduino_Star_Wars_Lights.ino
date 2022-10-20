//#include <FastLED.h>
#include <time.h>
#include <stdlib.h>
#include <Unistep2.h>

#define LED_PIN     13  //Pin
#define NUM_LEDS    4

#define INPUT_PIN  2

#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB
//CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 50 

#define STEPS_PER_REV   4096
#define STEP_DELAY      750


#define CLK             6
#define DT              7


Unistep2 stepper1 = Unistep2(8, 9, 10, 11, STEPS_PER_REV, STEP_DELAY);


//Lights defined by the pattern of the light strand seen below (line 110ish)

uint8_t ledColors[NUM_LEDS]; //Extra storage array used for a few functions

/*Color Pallettes
 * There are a few color palettes being loaded in at the start. Each is set up with
 * 16 colors, and each is used for a different mood. There is also a custom palette
 * which can be filled by custom colors used by a few modes which dont use distinct 
 * color patterns.
 * When the strands are being populated, the length determines how many LEDs each
 * color from the palette will last. When length = 16, each color will take up exactly
 * 1 light. Lower length means individual colors last longer. These can be blended
 * together to cause more seamless transitions between colors with the TBlendType.
 * EX - If one bulb is 0,0,255 and two later is 255,0,0, the inbetween may be 122,0,122
 * as an intermediary of the two
 */

//CRGBPalette16 currentPalette; 
//TBlendType    currentBlending;

uint8_t colorBase = 0; //only used by a removed function, SP of 2 waves
uint8_t length = 0;

static uint8_t mode = 6; 
static uint8_t lastMode = 2;
static uint8_t reset = 0;


uint8_t rotary_state = 0;
uint8_t last_rotary_state = 0;

int rotCount = 0;


/*:::::::: TO DO ::::::::: 
 *  1. Change 12 so patterns continue
 *  2. Give 6 some unique transitions. Doesnt fit well with the current ones
 *  3. De hardcode mode 29 to make less work on re-setup
 *    
 *:::::: Functionality :::::: 
 *  Follows a set pattern structure.
 *  1. Sits on a pattern (modes 0-9) for 39 seconds
 *      The pattern is a standard set of lights, something happy
 *      that you might expect to see from nice lights. Christmas patterns,
 *      bright twinkles, etc. Sets a pleasant mood
 *  2. Initiates a transition (modes 10-16) for 19 seconds
 *      Transitions are "darker", more grim, something you may expect for 
 *      Halloween. They mimic a being from the "Upsidedown" trying to communicate 
 *      with you. It could be the lights twinkling to be all red, shutting off 
 *      1 by 1, etc. All end with every light turned off.
 *  3. "Says" a word
 *      With all lights off, 1 by 1, a single light will flash a random color.
 *      Each light is positioned over a letter, spelling out some message that 
 *      the "Upsidedown" is trying to communicate with you. Classic examples from 
 *      the show are "RUN" and "BEHIND YOU", but other iconic lines from horror
 *      movies and more fun things are included 
 *  
 *  :::::::: MODES ::::::::
 *  
 *  //Patterns
 * :: 0, 4, 5, 8 Use random colors ::
 * 
 * //Transitions
 *     Transitions are meant to be extensions of the pattern, with a way for each 
 *     one to go subtly dark. It doesnt make sense for some transitions to go with 
 *     some patterns, so limitations are set
 * 10. Brightness drops -- 2/3 ONLY
 * 11. Blink until turn off -- Doesn't work with 6
 * 12. Lights Flash on and off
 * 13. Individually turn off
 * 14. Black - White light pattern
 * 15. Individual turn Red // Twinkle Only
 * 16. Speedup -- 2/3 ONLY
 * 
 * 19. Holdover Transition - Temporary waiting for the clock
 * 
 * 20-29. Misc text words 
 *
 * 30. Transition. Once the Word is done, the wall waits a few seconds before
 *      resetting.
 * 
 * 
 * ::::: GENERAL SETUP OF THE LIGHTS ::::::
 * 
 * UL x x x x x x x x x x x x x x x UR
 * x        a b c d e f g h i        x 
 * x                                 x
 * x    ML x x x x x x x x x x x x MR
 * x     x  j k l m n o p q 
 * x     x
 * x    BL x x x x x x x x x x x x BR
 * x       r s t u v w x y z       x
 * x                             x
 * S                           E
 *   
 *UL - Upper Left, UR - Upper Right
 *MTL - Middle Top Left, MTR - Middle Top Right
 *BL - Bottom Left, BR - Bottom Right
 *S - Start (light 0), E - End (light NUM_LEDS)
 *Letters - The general position of where letters are to spell words out
 *   
 *   The setup is a single strand of snakelike lights. Each pattern moves
 *   differently throughout the lights
 *   
 */



/*void PlayMode();
void FillLEDsFromPaletteColors(uint8_t);
void FillColumnsFromPaletteColors(uint8_t);
void FillRowsFromPaletteColors(uint8_t);
void displayColorsFromBinary();
void FillCircleFromPaletteColors(unit8_t);*/


//Setup, called at the start of the project
void setup() {
    delay( 3000 ); // power-up safety delay

    //Adds the strip of leds to be used
 //   FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );


    //Sets a standard color pallette and method of blending lights
  //  FastLED.setBrightness(  BRIGHTNESS );    
  //  currentPalette = TwinkleColors_p;
   // currentBlending = LINEARBLEND;

    //Random seeding based off pin 0 :)
    randomSeed(analogRead(0));
    Serial.begin(9600);



    //Setup Rotary Encoder
    pinMode (CLK, INPUT);
    pinMode (DT, INPUT);    

    //stepper1.setSpeed(RPM);
    
}

/*Main loop of the project
 * The function is called back anytime a mode loop ends
 * This typically means the mode needs to be changed, though not always
 *  * 
 * int reset - Determines whether or not a new mode is needed
 */

 void loop() {
/*
  if (Serial.available() > 0) {

    char in = Serial.read();

    if (in == 'H') {
   //   leds[0] = CRGB::Blue;
        Serial.println("High");
   //   FastLED.show();   
      }
    
    else if (in == 'L') {

  //    leds[0] = CRGB::Green;
        Serial.println("LOW");
   //   FastLED.show();   

      }

    else if (in == '\n') {
        Serial.println("Endline");
    }

    else {
   //   leds[0] = CRGB::Red;
        Serial.println("Error");
      }
  }

  if (stepper1.stepsToGo() == 0) {
     stepper1.move(STEPS_PER_REV);
  }

*/
  //Lever Encoder

  rotary_state = digitalRead(CLK);  

  if (rotary_state != last_rotary_state) {
    if (digitalRead(DT) != rotary_state) {
      rotCount++;
   //   Serial.println("+");
      stepper1.move(100);
    }
    else {
      rotCount--;
  //    Serial.println("-");
    }
   Serial.println(rotCount);
   last_rotary_state= rotary_state;    
  }



  //R2 Stepper

 
 //delay(1000);/*
 if(stepper1.stepsToGo() >0) {
  stepper1.run();
 // Serial.println(stepper1.stepsToGo());
 }

 //delay(1);
   
}

 /*
void loop()
{
  //If no reset is needed, calls PlayMode to play whichever mode is currently active
  if (reset == 0)
  {
    PlayMode();
  }
  //If a new mode is needed, determines which one should be called
  else {
    lastMode = mode;
    //Going from Pattern to Transition
    if (mode < 10 || mode > 49) { //> 49 is a holdover. Safe to remove
      if (mode == 2 || mode == 3) { // 50/50 chance to have each of mode 2/3s unique transitions
        mode = random(2);
        if (mode == 0) mode = 10; 
        else mode = 16;
      }
      if (mode == 9) {   // 50/50 chance to have mode 9s unique transition
        int j = random(2); 
        if (j == 0) mode == 15;
      }
      if (mode < 10) {
        if (mode == 6 || mode == 7) mode = random(12,15);
        else mode = random(11,15);        
      } 
      //if (mode == 12) mode = 13;
    }
    //Going from Transition to Text
    else if ((mode >= 10 && mode < 20)) {
      mode = 28;//random(20,30); 
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Black;
      }
    }
    //Going from Text to Pattern
    else if (mode >= 20 && mode < 49) {
      mode = random(10); 
      while (mode == 7) {mode = random(10);}  //Mode 7 was removed, not replaced yet
      currentPalette = RainbowColors_p;
    
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[i] = CRGB::Black;
      }
    }    
    //Certain modes can have any of a few colour pallettes. 5 palettes are preloaded,
    //  allowing for more variablitiy within the patterns
    if (mode == 0 || mode == 4 || mode == 5 || mode == 8) {
        int i = random(5);
        if (i == 0) {
             currentPalette = CustomColors_p; }
        else if (i == 1) {
             currentPalette = RainbowColors_p; }
        else if (i == 2) {
             currentPalette = CloudColors_p; }
        else if (i == 3) {
             currentPalette = PartyColors_p; }
        else if (i == 4) {
             currentPalette = TwinkleColors_p; }
        }
    
    //Resets the reset flag and calls the new mode!
    reset = 0;  
    Serial.print("Mode: ");
    Serial.println(mode);
                         
    PlayMode();  
  }  */
  
/* Used for the loop inside of each pattern and function
 * Every update calls the clock function to see if its time to change modes
 * Uses functionTime as a counter as opposed to individually using seconds
 * 
 *//*
int clockFunc()
{
  //Milliseconds/1000 will count up every second
  uint8_t secondHand = (millis() / 1000);// % 87;

    static uint8_t lastSecond = 0;
    static uint8_t functionTime = 0;

    //Each update checks to see if a second has passed since the last count
    if (lastSecond != secondHand) {
       functionTime++;      
       Serial.println(functionTime);
       lastSecond = secondHand;
       
        //Changes modes and triggers a reset in loop() if the time has passed
        if (mode >= 0 && mode < 10 && functionTime == 39) {
          reset = 1; 
          return 1;
        }
        else if (mode >=10 && mode < 20 && functionTime == 59) {
          reset = 1;
          return 1;
        }
        else if (mode == 30) { 
          functionTime = 0;
          reset = 1;
          return 1;
        }
    }
    return 0;
}

//Plays whichever mode is currently active
void PlayMode()
{
    switch(mode) {
      //Pattern Modes
      case 0: mode0();
        break;

}*/
