/*
void runR2() {

  if(stepper1.stepsToGo() > 0) {
    stepper1.run();
  }
  
  else {  
    if (rotateCD == 0) {
      startR2Movement();

    }
    else {
      rotateCD--;
   //   Serial.println(rotateCD);
    }      
  }

}

void startR2Movement() {
     int stepsToMove = random(-8000,8000);

     currentStepperPosition += stepsToMove;

    //Checks to see if R2 has moved too much from looking towards home. If so, resets him instead
    if (currentStepperPosition > 20000 || currentStepperPosition < -20000) {
      stepsToMove = -currentStepperPosition;
      currentStepperPosition=0;
    }
    
    stepper1.move(stepsToMove);
    rotateCD = random(500, 1500); //Set a new timeout for later
  
}

*/
