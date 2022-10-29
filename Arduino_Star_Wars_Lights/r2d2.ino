void runR2() {

  if(stepper1.stepsToGo() > 0) {
    stepper1.run();
  }
  
  else {  
    if (rotateCD == 0) {
      startR2Movement();

    }
  }
  else {
    rotateCD--;
    FastLED.delay(20);
  }  
}

void startR2Movement() {
     int stepsToMove = random(-5000,5000);

     curStepperPosition += stepsToMove;

    //Checks to see if R2 has moved too much from looking towards home. If so, resets him instead
    if (curStepperPosition > 20000 || curStepperPosition < -20000) {
      stepsToMove = -curStepperPosition;
      curStepperPosition=0;
    }
    
    stepper1.move(stepsToMove);
    rotateCD = random(800, 2000);; //Set a new timeout for later
  
}
