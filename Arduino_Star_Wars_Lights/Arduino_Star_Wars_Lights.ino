#include <FastLED.h>
#include <time.h>
#include <stdlib.h>
//#include <Unistep2.h>


//:::::::::::::::::::LED Definitions:::::::::::::::

#define LED_PIN     13  //Pin

#define NUM_LEDS    279
#define NUM_STRIPS  29

#define BRIGHTNESS  250
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

int ledCount = 0;

class lightStrips {

  public:

    int numLights=0;
    int firstLight=0;
    int lastLight=0;
    int ledOn = 0;
    int dir= 0;

    void init(int numLights) {
      this->numLights = numLights;
      this->firstLight = ledCount;
      this->lastLight = firstLight + numLights - 1;  
      this->ledOn = firstLight;

      ledCount += numLights;
    }
};

//int test = 0;

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette; 
TBlendType    currentBlending;

lightStrips lightBoard[NUM_STRIPS]; //Holds the arrays of all light strips

int ledMode = 0;  
/*3 different LED mode
 * 0: Lights are temp on
 * 1. Hyperspace active, all lights on* 
 * 
 */



//:::::::::::::::::::Rotary Definitions::::::::::::::::::::
#define CLK             6
#define DT              7


uint8_t rotary_state = 0;
uint8_t last_rotary_state = 0;

int rotCount = 0;

/*::::::::::::::::Stepper R2 Definitions:::::::::::::::::::

#define STEPS_PER_REV   4096
#define STEP_DELAY      750

Unistep2 stepper1 = Unistep2(8, 9, 10, 11, STEPS_PER_REV, STEP_DELAY);

int rotateCD = 1000;

int currentStepperPosition = 0;

void runR2();
*/
//:::::::::::::::General Definitions:::::::::::::::::

int mode = 0;
//0: everything off
//1: lights moving


//static uint8_t mode = 0; 

int ledOn = 0;

/*
#define UPDATES_PER_SECOND 50 


static uint8_t lastMode = 2;
static uint8_t reset = 0;
/*:::::::: TO DO ::::::::: 
 */


//Setup, called at the start of the project
void setup() {

    ///:::::::General Setup::::::::
    
    delay( 3000 ); // power-up safety delay
    //Random seeding based off pin 0 :)
    randomSeed(analogRead(0));
    Serial.begin(9600);

    //:::::::::Setup FastLED::::::::::::
    //Adds the strip of leds to be used
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

    //Sets a standard color pallette and method of blending lights
    FastLED.setBrightness(  BRIGHTNESS );    

    //Setup individual LEDs
    
    lightBoard[0].init(16);
    lightBoard[1].init(11);
    lightBoard[2].init(11);
    lightBoard[3].init(11);
    lightBoard[4].init(11);
    lightBoard[5].init(6);
    lightBoard[6].init(11);
    lightBoard[7].init(11);
    lightBoard[8].init(11);
    lightBoard[9].init(11);
    lightBoard[10].init(11);

    lightBoard[11].init(4);
    lightBoard[12].init(11);
    lightBoard[13].init(11);
    lightBoard[14].init(11);
    lightBoard[15].init(4);
    lightBoard[16].init(11);
    lightBoard[18].init(11);
    lightBoard[19].init(11);
    lightBoard[20].init(11);
    
    lightBoard[21].init(11);
    lightBoard[22].init(11);
    lightBoard[23].init(11);
    lightBoard[24].init(4);
    lightBoard[25].init(11);
    lightBoard[26].init(6);
    lightBoard[27].init(4);
    lightBoard[28].init(11);
    lightBoard[29].init(4);

    //::::::::Setup Stepper Motor:::::::::

    //stepper1 = Unistep2(8, 9, 10, 11, STEPS_PER_REV, STEP_DELAY);

    rotCount = 0;
    //::::::::Setup Rotary Encoder::::::::::
    pinMode (CLK, INPUT);
    pinMode (DT, INPUT); 
    
}

/*Main loop of the project
 * The function is called back anytime a mode loop ends
 * This typically means the mode needs to be changed, though not always
 *  
 *  Each cycle, checks.. 
 *  First: For a serial message. This is done by the C# app to control the function, as a form of override
 *  Second: Was the encoder changed? This signifies that the status of the lights need to change
 *  Third: What is the motor doing? If motion is current, it moves, else counts down to see if it should start moving soon  
 *  
 */

 void loop() {

   //First: Serial Message 

  if (Serial.available() > 0) {

    char in = Serial.read();

    //Start the movement - auto turn on the stuff
    if (in == 'S') {
        mode = 1;
      }
    //Turns off the device , turn mode to 0
    if (in == 'O') {
        mode = 0;      
    }

    //R resets the Rotary Encoder
    else if (in == 'R') {
      rotCount = 0;
    }

    else if (in == '\n') {
    }

    else {
   //   leds[0] = CRGB::Red;
      }
  }

  //Lever Encoder

  rotary_state = digitalRead(CLK);  

  if (rotary_state != last_rotary_state) {
    if (digitalRead(DT) != rotary_state) {
      rotCount--;
     Serial.println("+");
    }
    else {
      rotCount++;
      if (rotCount < 0) {
        rotCount = 0;
      }
      Serial.println("-");
    }
   Serial.println(rotCount);
   last_rotary_state= rotary_state;   

    if (rotCount >= 4) {
      moveLights();
      mode = 1;   
      Serial.println(   rotCount);
    }
    if (mode == 1 && rotCount < 4)
    {
      mode = 0;
      turnOffLights();
    }
    
  }
  if (mode == 1) {
   moveLights();
  }
  
  //FastLED.delay(1);

  //R2 Stepper
  //runR2();

}

 
