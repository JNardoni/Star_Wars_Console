
void moveLights() {

   for (int i = 0; i < NUM_STRIPS; i++) {
        //Turn off current light
      leds[lightBoard[i].ledOn] = CRGB::Black;

      //Advance light
      lightBoard[i].ledOn++;
      
      if (lightBoard[i].ledOn > lightBoard[i].lastLight) {
        lightBoard[i].ledOn = lightBoard[i].firstLight;
      }
      
      //Turn on new light
      leds[lightBoard[i].ledOn] = CRGB::Red;
   }

   FastLED.show();
  
}

void turnOffLights() {
  for (int i = 0; i < NUM_STRIPS; i++) {
        //Turn off current light
      leds[lightBoard[i].ledOn] = CRGB::Black;
  }

   FastLED.show();
  
}


/*
class lightStrips {

  int numLights=0;
  int firstLight=0;
  int lastLight=0;

  lightStrips(int numLights, int firstLight) {
    this.numLights = numLights;
    this.firstLight = firstLight;
    this.lastLight = firstLight + numLights - 1;  
  }


  
}
*/
