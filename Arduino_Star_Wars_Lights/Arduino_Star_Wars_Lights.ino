#include <FastLED.h>
#include <time.h>
#include <stdlib.h>
#include <Unistep2.h>


//:::::::::::::::::::LED Definitions:::::::::::::::

#define LED_PIN     13  //Pin

#define NUM_LEDS    6
#define NUM_STRIPS  30


//#define INPUT_PIN  2

#define BRIGHTNESS  200
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB


class lightStrips {

  public:

    int numLights=0;
    int firstLight=0;
    int lastLight=0;

    void init(int numLights, int firstLight) {
      this->numLights = numLights;
      this->firstLight = firstLight;
      this->lastLight = firstLight + numLights - 1;  
    }
};



CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette; 
TBlendType    currentBlending;

lightStrips lightBoard[NUM_STRIPS]; //Holds the arrays of all light strips

int ledMode = 0;  
/*3 different LED modes
 * 0: Lights are temp on
 * 1. Hyperspace active, all lights on
 * 2. Lights are flashing up/down
 * 
 * 
 */



//:::::::::::::::::::Rotary Definitions:::::::::::::::::::::

#define CLK             6
#define DT              7


uint8_t rotary_state = 0;
uint8_t last_rotary_state = 0;

int rotCount = 0;

//::::::::::::::::Stepper Definitions:::::::::::::::::::

#define STEPS_PER_REV   4096
#define STEP_DELAY      750

Unistep2 stepper1 = Unistep2(8, 9, 10, 11, STEPS_PER_REV, STEP_DELAY);

int rotateCD = 1000;

int currentStepperPosition = 0;

//:::::::::::::::General Definitions:::::::::::::::::


//static uint8_t mode = 0; 


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
    
    lightBoard[0].init(4,0);
    lightBoard[1].init(4,0);
    lightBoard[2].init(4,0);
    lightBoard[3].init(4,0);
    lightBoard[4].init(4,0);
    lightBoard[5].init(4,0);
    lightBoard[6].init(4,0);
    lightBoard[7].init(4,0);
    lightBoard[8].init(4,0);
    lightBoard[9].init(4,0);
    lightBoard[10].init(4,0);
    lightBoard[11].init(4,0);
    lightBoard[12].init(4,0);
    lightBoard[13].init(4,0);
    lightBoard[14].init(4,0);
    lightBoard[15].init(4,0);
    lightBoard[16].init(4,0);
    lightBoard[17].init(4,0);
    lightBoard[18].init(4,0);
    lightBoard[19].init(4,0);
    lightBoard[20].init(4,0);
    lightBoard[21].init(4,0);
    lightBoard[22].init(4,0);
    lightBoard[23].init(4,0);
    lightBoard[24].init(4,0);
    lightBoard[25].init(4,0);
    lightBoard[26].init(4,0);
    lightBoard[27].init(4,0);
    lightBoard[28].init(4,0);

    //::::::::Setup Stepper Motor:::::::::

    //stepper1 = Unistep2(8, 9, 10, 11, STEPS_PER_REV, STEP_DELAY);


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
  runR2();

 }

   
}
